#include "node.h"
#include <stdio.h>

//Constructor
node::node(int x, node * n) {
    next = n;
    data = x;
}

//Destructor
node::~node() { }

//Method for changing the data property of a node
void node::set_data(int x) {
    data = x;
}

//Method for changing the next property of a node
void node::set_next(node * n) {
	next = n;
}

//Method for obtaining the data property of a node
int node::get_data() {
	return data;
}

//Method for obtaining the next property of a node
node * node::get_next() {
	return next;
}