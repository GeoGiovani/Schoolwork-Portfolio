#include "node.h"

// Linked list class, used in the Stack class
class LL {
    private:
        node * head; // pointer to first node
        node * tail; // pointer to last node
    
    public:
        LL(); // Constructor
        ~LL(); // Destructor
        void prepend(int value); //Add a node to the beginning of the LL
        int removeHead();        //Remove the first node of the LL
        void print();            //Print the elements of the LL
		node * get_head();       //Access pointer to the first node of the LL
};