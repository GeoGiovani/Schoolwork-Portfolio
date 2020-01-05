#include "LL.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//Constructor
LL::LL() {
	head = NULL;
	tail = NULL;
}

//Destructor
LL::~LL() {
	
	//Verify that the LL has contents
	if (head == NULL) {
		assert(tail == NULL); //Sanity check
		return;
	}		

	//Go through the list and delete all nodes
	node *target = head;

	while (target != NULL) {
		head = head->get_next(); 
		delete target;
		target = head;
	}
	head = NULL;
	tail = NULL;
}

//Add a node to the beginning of the LL
void LL::prepend(int value) {
	
	//Make a new node
	node *newN = new node(value, head); //Heap declaration

	//Update head
	head = newN;
	
	//Update tail if NULL
	if(tail == NULL) tail = newN;
}

// remove the first node of the LL
int LL::removeHead() {
	
	//Make a pointer to head
	node *temp = head;

	//If temp == NULL the list is considered empty
	if (temp == NULL) {
		printf("List is empty! Nothing was removed.\n");
		return -1;
	}

	//If there is no node after the head, we have to ensure the list is empty 
	//following the remove
	if (temp->get_next() == NULL) {	
		int val = temp->get_data();
	
		//head == tail in this case, so we delete both
		delete head;
		delete tail;
		delete temp;

		//Set head and tail to NULL before returning
		head = NULL;
		tail = NULL;
		return val;
	}

	//Typical case, remove head and return value
	else {
		int val = temp->get_data();
		temp = temp->get_next();
		delete head;
		head = temp;
		return val;
	}
}

// print the elements of the LL
void LL::print() {

	// If list is empty return
	if (head == NULL) {
		assert(tail == NULL); //Sanity check
		printf("Warning! List is empty\n");
		return;
	}
	
	//Node pointer to iterate through list
	node *temp = head;

	while (temp != NULL) {
		printf("%d ", temp->get_data());
		temp = temp->get_next();
	}
	putchar('\n');
	delete temp;
}          

//Access the pointer to the first node of the LL
node * LL::get_head() { 
	return head;
}