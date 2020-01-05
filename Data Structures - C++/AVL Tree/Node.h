//=============================================================================
// Name        : Node.h
//                                                   George Giovanis, 301254694
//=============================================================================



#ifndef NODE_H_
#define NODE_H_

#include "Customer.h"
#include <string>
using namespace std;


/*       The Node class below represents a node of a Binary Search Tree. A Node
 * contains a pointer to a Customer object and a value for its height within
 * a Binary Search Tree. Each Node also contains pointers to the Node's parent
 * and children. All functions defined in the Customer class run in O(1) time.
 * 
 *       The class contains a default constructor and destructor, a constructor 
 * that takes as arguments values for Customer data members, and a specialized 
 * copy constructor that creates a new Node with a deep copy of another Node's 
 * customer_data. Getters and setters are supplied for each data member.
 * 
 *       The functions is_external() and is_internal() can be used to determine
 * if a Node is external or internal within a Binary Search Tree. */


class Node {

// Data Members
private:

    Customer* customer_data;  // Customer object serves as Node's key and value
    int height;               // Height of the Node (in a BSTree)

    Node* left;    // Pointer to Node's left child
    Node* right;   // Pointer to Node's right child
    Node* parent;  // Pointer to Node's parent


// Protected Utility Functions
protected:

    void set_data(Customer* c);     // Set Node's customer_data to Customer c
    void set_left(Node* n);         // Set Node's left child to Node n
    void set_right(Node* n);        // Set Node's right child to Node n
    void set_parent(Node* n);       // Set Node's parent to Node n
    void set_height(const int ht);  // Set Node's height to ht

    Customer* get_data() const;  // Return pointer to Node's customer_data
    Node* get_left() const;      // Return pointer to Node's left child
    Node* get_right() const;     // Return pointer to Node's right child
    Node* get_parent() const;    // Return pointer to Node's parent
    int get_height() const;      // Return the height of a Node

    bool is_external() const;  // Return true if the Node is an external Node
    bool is_internal() const;  // Return true if the Node is an internal Node


// Public Functions
public:

    Node();  // Default Constructor

    // Constructor with supplied Customer member values
    Node(const string surname, const char init, const int bal, const int ht); 

    Node(const Node& n);          // Copy Constructor (deep copy n's Customer)
    Node(Node* n, const int ht);  // Construct Node with parent n and height ht
    ~Node();                      // Destructor

    // Friend declarations
    friend class BSTree;
};

#endif /* NODE_H_ */