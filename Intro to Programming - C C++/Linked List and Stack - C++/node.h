class node { //node class used in the LL (linked list) class
    private:
        node * next; //Pointer to next node of an LL
        int data;    //integer data stored in this node
        
    public:
        node(int x, node * n);    //Constructor
        ~node();                  //Destructor
        void set_data(int x);     //Change the data of this node
        void set_next(node * n);  //Change the next pointer of this node
        int get_data();           //Access the data of this node
        node * get_next();        //Access the next pointer of this node
};      