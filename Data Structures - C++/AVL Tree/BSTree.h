//=============================================================================
// Name        : BSTree.h
//                                                   George Giovanis, 301254694
//=============================================================================



#ifndef BSTREE_H_
#define BSTREE_H_

#include "Customer.h"
#include "Node.h"
#include <vector>
#include <string>
using namespace std;


/*       The BSTree class implemented below is an AVL Tree that maintains a  
 * height of log(n), where n is the number of Nodes in the tree. The AVL Tree 
 * is composed of Nodes that contain a pointer to a Customer object, as well as
 * pointers to their left and right children, and parent. External Nodes are
 * not used to store Customer data, have height 0, and have NULL pointers to 
 * their left and right children.
 * 
 *       The concatenation of a Node's Customer object's surname and 
 * first_initial values is used as a key. String comparisons are used to define
 * the total order relation amongst such keys. As a result, the AVL Tree does
 * not violate the properties of a Binary Search Tree.
 * 
 *       The BSTree class contains an implementation of an Iterator class,
 * of which stores a pointer to a Node within the BSTree. A default 
 * constructor and destructor, as well as a constructor that takes a Node 
 * pointer are supplied. 
 *   
 *       The core BSTree functions, insert(), remove(), and search() run in  
 * O(log(n)) time, and the primary utility functions, rebalance() and 
 * restructure() run in O(log(n)) and O(1) time respectively, where n is the 
 * number of Nodes in the BSTree. rangeSearch() and inOrderPrint() both run in 
 * O(n) time, where n is the number of Nodes whose customer_data is being 
 * inserted into a Customer vector, or being  printed, respectively. */


class BSTree {

// BSTree Data Members
private:

    Node* super_root;   // Sentinel used for balancing the BSTree
    Node* virtual_root; // The root of the BSTree
    int num_nodes;      // Number of nodes in the BSTree


// Iterator Class
private:

    /* Standard Iterator for traversing the tree in an inorder fashion.
     * Provides overloaded == and != operators for comparisons and * operator
     * for read/write access of the Iterator element. */
    class Iterator {
     
    private: 

        Node* node_element;  // The element of the Iterator

        Iterator(Node* n);   // Creates an Iterator with node_element == n
        ~Iterator();         // Destructor
    
        // Return node_element of the Iterator
        Node* operator * () const;

        // Compare two Iterators for equality
        bool operator == (const Iterator i) const;  

        // Compare two Iterators for inequality
        bool operator != (const Iterator i) const;  

        // Return Iterator's inorder successor 
        Iterator& operator ++ ();      

        friend class BSTree;       
    };


// BSTree Utility Functions
private:

    // Return the height of Node n
    int height(const Node* n) const;   
    
    // Adjust the height of a Node to one more than that of its tallest child.
    void adjust_height(Node* n) const; 
   
    // Return the tallest child of Node n
    Node* tallest_child(const Node* n) const; 

    // Return the tallest grandchild of Node n
    Node* tallest_grandchild(const Node* n) const;  

    // Return the balance factor of Node n
    int balance_factor(const Node* n) const;        
   
    /* Return true if Node n is balanced, i.e. when the height of n's children
     * differ by no more than 1 */
    bool balanced(const Node* n);                   

    /* Find the first Node in the BSTree with customer_data that has the same
     * surname and first_initial as Customer c. If no Node is found, return
     * an external Node where a Node with customer_data c would be inserted. */ 
    Node* find_node(const Customer c, Node* n) const;   

    /* Using Binary Search, find an external node that can be expanded with 
     * Node n. If a customer with identical surname and first_initial is found,
     * progress to the right child of it. */
    Node* find_external(const Customer c, Node* n) const; 

    // Expand the external Node n to make it an internal Node
    void expand_external(Node* n); 

    /* Replace Node n's parent with n's sibling, and then delete the old  
     * parent and n. */
    Node* remove_above_external(Node * n);          

    /* PRE:   A BSTree with an unbalanced Node (tall_grandchild's grandparent).
     *        tall_grandchild is either the left-left (case 1),
     *        left-right (case 2), right-left (case 3), or right-right (case 4)
     *        child of the grandparent.
     * POST:  The case causing tall_grandchild's grandparent to be unbalanced
     *        is identified. A trinode restructure is then performed to 
     *        balance tall_grandchild's grandparent.
     * PARAM: The Node that was tall_grandchild's grandparent is now balanced.
     *        The BSTree may remain unbalanced at a Node closer to the root. */
    Node* restructure(Node* tall_grandchild);


    /* PRE:   A BSTree that may or may not be balanced, 
     *        NOTE: a BSTree is balanced when all of its Node's are balanced.
     * POST:  Starting with Node n, move up the BSTree to the virtual_root and 
     *        check that each Node encountered is balanced. If an unbalanced 
     *        Node is encountered, make a call to restructure() to balance it.
     * PARAM: A Node n equal to or below the virtual_root in the BSTree. */
    void rebalance(Node* n);
 

// BSTree Public Functions
public:

	BSTree();                   // Default Constructor
	BSTree(const BSTree* bst);  // Copy Constructor
	~BSTree();                  // Destructor


    // Delete elements from the BSTree by traversing in a postorder manner
    void post_order_delete(Node* n);  

    Iterator begin() const;  // Return an Iterator to first entry in BSTree
    Iterator end() const;    // Return an Iterator to last entry in BSTree

    int node_count() const;  // Return the number of Nodes in the BSTree


	/* a New Node with Customer data specified by the arguments is created and
    inserted at the proper external Node. rebalance() is used to find any 
    Nodes that have become unbalanced from the insertion, which are rebalanced 
    using restructure(). */
	bool insert(const string surname, const char initial, const int bal);

    /* Customer data from the Iterator i's node_element is used to call
     * insert(). The result is a new Node with customer_data equal to i's 
     * element inserted into the BSTree */
    bool insert(Iterator i);

    /* Removes the first Node with matching customer_data from the BSTree, and
     * then rebalances the BSTree using rebalance() and restructure(). Returns
     * true if a matching Node was removed, and false otherwise. */
	bool remove(const string surname, const char first_initial);

    /* Returns true if a Node containing a Customer with matching surname and
     * first_initial is present in the BSTree. Returns false otherwise. */
	bool search(string, char);

    /* Returns a vector of Customers where the Customer's surname and 
     * first_initial are in the range specified by the parameters. */
	vector<Customer> rangeSearch(string name_1, char a, string name_2, char b);

	// Prints the contents of the BSTree in sorted order.
	void inOrderPrint();
};

#endif /* BSTREE_H_ */