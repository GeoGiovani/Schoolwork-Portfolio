#include "Stack.h"
#include <stdio.h>
#include <assert.h>

//Constructor
Stack::Stack() { 
	myStack = new LL();
}

//Destructor
Stack::~Stack() {
	delete myStack;
}

//Push an item onto the stack
void Stack::push(int value) {
	myStack->prepend(value);
}

//Remove a node from top of stack and return it's value
int Stack::pop() {

	//If stack is empty, print an error and return 
	if (myStack->get_head() == NULL) {
		printf("Error: Stack is empty\n");
		return -1;	
	}
	//Use the removehead() function to pop from stack
	int value = myStack->removeHead();
	return value;
}
	
//Return 1 if empty, 0 otherwise
int Stack::isEmpty() {

	if (myStack->get_head() == NULL) {
		return 1;
	}
	return 0;
}

//Print the stack
void Stack::print() {

	//If stack is empty
	if (myStack->get_head() == NULL) {
		printf("Error: Stack is empty\n");
		return;	
	}
	printf("\nTop - Bottom\n");
	myStack->print();
	putchar('\n');
}