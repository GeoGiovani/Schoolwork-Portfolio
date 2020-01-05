//=============================================================================
// Name        : Node.cpp 
//                                                   George Giovanis, 301254694
//=============================================================================



#include "Node.h"
#include <string>
using namespace std;


//======================= Protected Utility Functions =========================

// Set Node's customer_data to Customer c
void Node::set_data(Customer* c) 
{ 
    this->customer_data = c; 
}


// Set Node's left child to Node n
void Node::set_left(Node* n) 
{ 
    this->left = n; 
}


// Set Node's right child to Node n
void Node::set_right(Node* n) 
{ 
    this->right = n; 
} 


// Set Node's parent to Node n
void Node::set_parent(Node* n) 
{ 
    this->parent = n; 
}


// Set Node's height to ht
void Node::set_height(const int ht) 
{ 
    this->height = ht; 
}


// Return pointer to Node's customer_data
Customer* Node::get_data() const 
{ 
    return this->customer_data; 
}


// Return pointer to Node's left child
Node* Node::get_left() const 
{ 
    return this->left; 
} 


// Return pointer to Node's right child
Node* Node::get_right() const 
{ 
    return this->right; 
}


// Return pointer to Node's parent
Node* Node::get_parent() const 
{ 
    return this->parent; 
}


// Return the height of a Node
int Node::get_height() const 
{ 
    return this->height; 
} 


// Return true if the Node is an external Node
bool Node::is_external() const
{ 
    return (this->parent != NULL && this->left == NULL && this->right == NULL); 
}


// Return true if the Node is an internal Node
bool Node::is_internal() const 
{ 
    return (!is_external()); 
}


//============================ Public Functions ===============================

// Default Constructor
Node::Node()

    : customer_data(new Customer),
      height(0),
      left(NULL), right(NULL), parent(NULL) { }
      

// Constructor with supplied Customer member values
Node::Node(const string surname, const char init, const int bal, const int ht)

    : customer_data(new Customer(surname, init, bal)),
      height(ht),
      left(NULL), right(NULL), parent(NULL) { }


// Specialized Copy Constructor (copies Node n's Customer data and height only)
Node::Node(const Node& n)

    : customer_data(new Customer(*n.customer_data)),
      height(n.height),
      left(NULL), right(NULL), parent(NULL) { }


// Construct a Node with parent Node n and height ht
Node::Node(Node* n, const int ht)

    : customer_data(NULL),
      height(ht),
      left(NULL), right(NULL), parent(n) { }
      

// Destructor
Node::~Node() 
{ 
    if (this->customer_data != NULL) {
        delete this->customer_data;
    }
}