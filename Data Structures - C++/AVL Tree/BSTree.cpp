//=============================================================================
// Name        : BSTree.cpp
//                                                   George Giovanis, 301254694
//=============================================================================



#include "BSTree.h"
#include "Customer.h"
#include <vector>
using namespace std;
using std::vector;


//======================== Iterator Member Functions ==========================

// Create an Iterator from a Node.
BSTree::Iterator::Iterator(Node* n)

    : node_element(n) { }


// Destructor
BSTree::Iterator::~Iterator() { }      


//======================= Iterator Overloaded Operators =======================

// Return a pointer to the node_element.
Node* BSTree::Iterator::operator * () const
{
    return this->node_element;
}          


// Compare two Iterators for equality.
bool BSTree::Iterator::operator == (const Iterator i) const
{
    return (this->node_element == i.node_element);
}


// Compare two Iterators for inequality.
bool BSTree::Iterator::operator != (const Iterator i) const
{
    return (!(this->node_element == i.node_element));
}


// Return an Iterator to an Iterator's inorder successor.
BSTree::Iterator& BSTree::Iterator::operator ++ ()
{
    Node* successor = this->node_element->get_right();

    // If Iterator's Node has an internal right child, then the Node's inorder  
    // successor is the last internal Node on the left side of the subtree 
    if (successor->is_internal()) {

        // Move through left children
        while (successor->is_internal()) {
            successor = successor->get_left();
        }
        // Set Iterator element to its inorder successor
        this->node_element = successor->get_parent(); 
    }
    // If iterator's Node does not have a right child, then the inorder 
    // successor is the first parent up and to the right of the Node
    else {
        successor = this->node_element->get_parent();

        // Keep moving up until a parent to the right of successor is found
        while (this->node_element != successor->get_left()) {
            this->node_element = successor;
            successor = successor->get_parent();
        }
        // Set Iterator element to its inorder successor
        this->node_element = successor;
    }
    return *this;
}     


//========================= BSTree Utility Functions ==========================

// Return the height of Node n
int BSTree::height(const Node* n) const
{
    // If n is external, it's height will be 0
    if (n->is_external()) {
        return 0;
    }
    else {
        return n->get_height();
    }
}                  


// Adjust the height of a Node to one more than that of its tallest child.
void BSTree::adjust_height(Node* n) const
{
    // Get the height of n's tallest child
    Node* tall_child = tallest_child(n);
    int child_height = height(tall_child);

    // Set the height of n
    n->set_height(child_height + 1);
}  


// Return the tallest child of Node n.
Node* BSTree::tallest_child(const Node* n) const
{
    // Find and return the tallest child of n using n's balance factor
    if (balance_factor(n) >= 0) {
        return n->get_left();
    }
    else {
        return n->get_right();
    }
}


// Return the tallest grandchild of Node n.
Node* BSTree::tallest_grandchild(const Node* n) const
{
    // Locate the tallest child of n
    Node* tall_child = tallest_child(n);

    // Locate the tallest grandchild
    if (balance_factor(tall_child) >= 0) {
        return tall_child->get_left();
    }
    else {
        return tall_child->get_right();
    }
}


/* Return the balance factor of Node n.
 * If balance factor > 0, then n->left is taller than n->right.
 * If balance factor < 0, then n->right is taller than n->left. */
int BSTree::balance_factor(const Node* n) const
{
    // External Nodes have height 0
    if (n->is_external()) {
        return 0;
    }
    return height(n->get_left()) - height(n->get_right());
}


/* Return true if Node n is balanced, i.e. when the height of n's children
 * differ by no more than 1 */
bool BSTree::balanced(const Node* n)
{
    int balance = balance_factor(n);
    return (balance >= -1 && balance <= 1);
}


/* Find the first Node in the BSTree with customer_data that has the same
 * surname and first_initial as Customer c. If no Node is found, return
 * an external Node where a Node with customer_data c would be inserted. */
Node* BSTree::find_node(const Customer c, Node* n) const 
{
    // Base Case: external Node reached
    if (n->is_external()) {
        return n;
    }
    // Base Case: matching Node reached
    if (c == *n->get_data()) {
        return n;
    }

    // Recur: Customer c is less than Customer at current Node
    if (c < *n->get_data()) {
        return find_node(c, n->get_left());
    }
    // Recur: Customer c is greater than Customer at current Node
    else {
        return find_node(c, n->get_right());
    }
}


/* Using Binary Search, find an external node that can be expanded with Node n.
 * If a customer with identical surname and first_initial is found, progress
 * to the right child of it. */
Node* BSTree::find_external(const Customer c, Node* n) const
{
    // Base case: location has been found
    if (n->is_external()) {
        return n;
    }

    // Recur: Customer c is less than Customer at current Node
    if (c < *n->get_data()) {
        return find_external(c, n->get_left());
    }
    // Recur: Customer c is greater than or equal to Customer at current Node
    else {
        return find_external(c, n->get_right());
    }
}


// Expand the external Node n to make it an internal Node
void BSTree::expand_external(Node* n) 
{
    // Make left and right children external Nodes
    n->set_left(new Node(n, 0)); 
    n->set_right(new Node(n, 0));

    // Update Number of Nodes in the BSTree
    this->num_nodes += 2;
} 


/* Replace Node n's parent with n's sibling, and then delete the old parent
 * and n. */
Node* BSTree::remove_above_external(Node * n)   
{
    // Set Node pointers
    Node* parent = n->get_parent();
    Node* grand_parent = parent->get_parent();
    Node* sibling;

    // Set the sibling Node
    if (parent->get_right() == n) {
        sibling = parent->get_left();
    }
    else {
        sibling = parent->get_right();
    }

    // Set the sibling's parent
    sibling->set_parent(grand_parent);

    // Set the grandparent's child
    if (grand_parent->get_left() == parent) {
        grand_parent->set_left(sibling);
    }
    else {
        grand_parent->set_right(sibling);
    }

    // Special Case: Parent Node is the virtual_root
    if (parent == virtual_root) {
        this->virtual_root = sibling;
        this->super_root->left = sibling;
    }

    // Update Number of Nodes in the BSTree
    this->num_nodes -= 2; 

    // Delete removed Nodes and return the replaced Node
    delete parent;
    delete n;

    return sibling;
}


/* There are 4 possible cases that create an unbalanced BSTree. The case will 
 * be determined, and then a trinode restructure will be performed such that 
 * the balanced Node b becomes the parent of Nodes a and c, who will become the 
 * left and right children of b, respectively. */
Node* BSTree::restructure(Node* tall_grandchild)
{
    Node* parent = tall_grandchild->get_parent();
    Node* grand_parent = parent->get_parent();

    Node* a;
    Node* b;
    Node* c;
    
    int case_number = 0;

    // Step 1: Determine case and perform the appropriate trinode restructure
    if (grand_parent->get_left() == parent) {

        // Case 1 (left-left case)
        if (parent->get_left() == tall_grandchild) {
            a = tall_grandchild;
            b = parent;
            c = grand_parent;
            case_number = 1;
        }

        // Case 2 (left-right case)
        else if (parent->get_right() == tall_grandchild) {
            a = parent;
            b = tall_grandchild;
            c = grand_parent;
            case_number = 2;
        }
    }

    else if (grand_parent->get_right() == parent) {

        // Case 3 (right-left case)
        if (parent->get_left() == tall_grandchild) {
            a = grand_parent;
            b = tall_grandchild;
            c = parent;
            case_number = 3;
        }

        // Case 4 (right-right case)
        else if (parent->get_right() == tall_grandchild) {
            a = grand_parent;
            b = parent;
            c = tall_grandchild;
            case_number = 4;
        }
    }


    // Step 2: After restructuring, b's parent needs to be adjusted
    b->set_parent(grand_parent->get_parent());

    
    // Step 3: Adjust b->parent's child to b
    // if b's parent is the BSTree's super_root, then b is the virtual_root
    if (b->get_parent() == super_root) {
        super_root->set_left(b);
        virtual_root = b;
    }
    // if b's data is less than it's parent, so b is a left child
    else if (*b->get_data() < *(b->get_parent())->get_data()) {
        b->parent->set_left(b);
    }
    // if b's data is greater or equal to it's parent, so b is a right child
    else if (*b->get_data() >= *(b->get_parent())->get_data()) {
        b->parent->set_right(b);
    }


    // Step 4: Adjust subtrees based on case number
    if (case_number == 2 || case_number == 3 || case_number == 4) {
        a->set_right(b->get_left());
        b->left->set_parent(a);
    }
    if (case_number == 1 || case_number == 2 || case_number == 3) {
        c->set_left(b->get_right());
        b->right->set_parent(c);
    }


    // Step 5: Make a and c the children of b
    b->left = a;
    b->right = c;
    a->parent = b;
    c->parent = b;


    // Step 6: Return the balanced node
    return b;
}


/* Starting with Node n's parent, move up the BSTree to the virtual_root and 
 * check that each Node encountered is balanced. If an unbalanced Node is
 * encountered, make a call to restructure() to balance it. */
void BSTree::rebalance(Node* n)
{
    Node* index_node = n;

    // Use index_node to move up the tree towards to the virtual_root
    while (index_node != this->virtual_root) {

        // Adjust the height of the Node
        index_node = index_node->get_parent();
        adjust_height(index_node);

        // If the Node is unbalanced, balalance it using restructure()
        if (!balanced(index_node)) {

            // Find the unbalanced Node's tallest grandchild and restructure()
            Node* tgc = tallest_grandchild(index_node);
            index_node = restructure(tgc);

            // Adjust the heights of the restructured Nodes
            adjust_height(index_node->get_left());
            adjust_height(index_node->get_right());
            adjust_height(index_node);
        }
    }
}


//========================== BSTree Public Functions ==========================

// Default Constructor
BSTree::BSTree() 

    : super_root(new Node(NULL, 0)),
      virtual_root(new Node(super_root, 0)),
      num_nodes(1) 
{
    super_root->set_left(virtual_root);
}


// Copy Constructor
BSTree::BSTree(const BSTree* bst)

    : super_root(new Node(NULL, 0)),
      virtual_root(new Node(super_root, 0)),
      num_nodes(1) 
{
    super_root->set_left(virtual_root);

    Iterator start = bst->begin();
    Iterator finish = bst->end();

    while (start != finish) {
        this->insert(start);
        ++start;
    }
    this->num_nodes = bst->node_count();
}  


// Destructor
BSTree::~BSTree() 
{
    post_order_delete(this->virtual_root);
    delete this->super_root;  
}


// Delete elements from the BSTree by traversing in a postorder manner
void BSTree::post_order_delete(Node* n)
{
    if (n != NULL) {
        post_order_delete(n->left);
        post_order_delete(n->right);
        delete n;
    }
}


// Return an Iterator to first entry in the BSTree
BSTree::Iterator BSTree::begin() const
{
    /* Starting from the virtual root, move down the left of the BSTree 
     * until the first external Node is reached. This Node's parent is the 
     * first Node of an inorder traversal of the BSTree */
    Node* n = this->virtual_root;
    while(n->is_internal()) {
        n = n->get_left();
    }
    return Iterator(n->get_parent());
} 


// Return an Iterator to last entry in the BSTree (the super root)
BSTree::Iterator BSTree::end() const
{
    return Iterator(this->super_root);
}  


// Return the number of Nodes in the BSTree
int BSTree::node_count() const
{
    return this->num_nodes;
}   


/* A new Node with the supplied Customer data is constructed and inserted at
 * proper external Node. rebalance() is used to find any Nodes that have become
 * unbalanced from the insertion, which are rebalanced using restructure(). */
bool BSTree::insert(const string surname, const char initial, const int bal) 
{
    // Create a Customer to use as a search key
    Customer c(surname, initial, bal);  

    // Find an external Node for the new Node to be inserted
    Node* insert_node = find_external(c, this->virtual_root);

    expand_external(insert_node);            // Expand the new Node
    insert_node->set_data(new Customer(c));  // Update the data member
    adjust_height(insert_node);              // Adjust the height value
    rebalance(insert_node);                  // Rebalance the BSTree

	return true;
}


/* Customer data from the Iterator i's node_element is used to call
 * insert(). The result is a new Node with customer_data equal to i's element
 * inserted into the BSTree */
bool BSTree::insert(BSTree::Iterator i) 
{
    Node* customer_node = i.node_element;
    Customer* cust = customer_node->get_data();

    string surname = cust->get_surname();
    char initial = cust->get_first_initial();
    int balance = cust->get_account_balance();

    return insert(surname, initial, balance);
}


/* Removes the first Node with matching customer_data from the BSTree, and
 * then rebalances the BSTree using rebalance() and restructure(). Returns
 * true if a matching Node was removed, and false otherwise. */
bool BSTree::remove(const string surname, const char first_initial) 
{
    // Create a search key
    Customer c(surname, first_initial, 0); 

    // Starting from the virtual root, look for the first Node that matches c
    Node* del_node = find_node(c, this->virtual_root);

    // If no matching Customer was found, return false
    if (del_node->is_external()) {
        return false;
    }

    /* If the Node to be removed has two Internal children, exchange it's 
     * customer_data with a Node that has an external child and remove it. */
    if (del_node->left->is_internal() && del_node->right->is_internal()) {

        // Find del_node's inorder successor
        Iterator iter(del_node);
        ++iter;
        Node* has_ext = *iter;

        // Exchange customer_data
        Customer* copy_customer = has_ext->get_data();
        Customer* transfer_data = del_node->get_data();

        transfer_data->surname = copy_customer->get_surname();
        transfer_data->first_initial = copy_customer->get_first_initial();
        transfer_data->account_balance = copy_customer->get_account_balance();

        // Remove the Node with the external child
        if (has_ext->left->is_external()) {
            del_node = remove_above_external(has_ext->get_left());
        }
        else {
            del_node = remove_above_external(has_ext->get_right());
        }
    }
    // Otherwise remove del_node
    else if (del_node->left->is_external()) {
        del_node = remove_above_external(del_node->get_left());
    }
    else if (del_node->right->is_external()) {
        del_node = remove_above_external(del_node->get_right());
    }
    
    // Rebalance the BSTree
    rebalance(del_node);

    return true;
}


/* Returns true if a Node containing a Customer object with matching surname
 * and first_initial is present in the BSTree. Returns false otherwise. */
bool BSTree::search(string surname, char first_initial) 
{	
    // Create a search key
    Customer customer_key(surname, first_initial, 0); 

    // Search for the Node
    Node* ind = find_node(customer_key, this->virtual_root);

    // Customer was not found
    if (ind->is_external()) {
        return false;
    }

    // Customer was found
    return true;
}


/* Returns a vector of Customers where the Customer's surname and first_initial
 * are in the range specified by the parameters. */
vector<Customer> BSTree::rangeSearch(string nm1, char i1, string nm2, char i2) 
{
    vector<Customer> customer_vector;  // Create a Customer vector

    // Create Customer keys
    Customer left_key(nm1, i1, 0); 
    Customer right_key(nm2, i2, 0);

    // Return an empty Customer Vector if range is invalid
    if (left_key > right_key) {
        return customer_vector;
    }

    // Find a Node that matches the left_key, or an external Node if none match
    Node* index_node = find_node(left_key, this->virtual_root);    

    if (index_node->is_external()) {
        index_node = index_node->get_parent();
    }

    // Iterators for progressing through the BSTree in an inorder manner
    Iterator index_iter = Iterator(index_node);
    Iterator end_iter = end();

    // Traverse the BSTree and insert Nodes within the range into the vector
    while (index_iter != end_iter) {

        // Exit if outside of the right bound of the search range
        if (*index_iter.node_element->get_data() > right_key) {
            break;
        }

        // Increment left iterator until it is inside the search range
        if (*index_iter.node_element->get_data() < left_key) {
            ++index_iter;
            continue;
        }

        // Insert Customers within the search range into the Customer vector
        customer_vector.push_back(*index_iter.node_element->get_data());

        ++index_iter;
    }

    // Return the filled Customer vector
    return customer_vector;
}


// Prints the contents of the BSTree in sorted order.
void BSTree::inOrderPrint() 
{
    Iterator start = begin();
    Iterator finish = end();
    
    while (start != finish) {
        cout << *start.node_element->get_data() << endl;
        ++start;
    }
}