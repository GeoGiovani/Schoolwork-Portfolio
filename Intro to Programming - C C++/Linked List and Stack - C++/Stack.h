#include "LL.h"

class Stack {
	private:
		LL* myStack; //Pointer to a linked list;
		
	public:
		Stack();  //Constructor
		~Stack(); //Destructor
		
		//Other methods
		void push(int value);
		int pop();
		int isEmpty();
		void print();
};