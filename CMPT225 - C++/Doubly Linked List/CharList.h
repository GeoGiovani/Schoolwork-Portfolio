#include <string>

class CharNode {
    public:
        CharNode(char c); // CharNode constructor. Initializes with c.
        ~CharNode();      // CharNode destructor.
    private:
        char data;
        CharNode* next;
        CharNode* prev;
    friend class CharList;
};

class CharList {
    public:      
        bool empty(); // True if the list is empty      
        int size();   // Return the number of elements in the list        
        void insert(char c); // Inserts the character c at the front of list
        void append(char c); // Inserts the character c at the back of list
        void insert(char c, char d); // Inserts c immediately before the first occurance of d, or
        // at the end of the list if d does not occur.
        void append(char c, char d); // Inserts c immediately after the first occurance of d, or
        // at the end of the list if d does not occur.
        void insert(char c, char d, int n); // Inserts c immediately before the nth occurance of d, or
        // at the end of the list if d does not occur n times.
        void append(char c, char d, int n); // Inserts c immediately after the nth occurance of d, or
        // at the end of the list if d does not occur n times.
        void remove(char c); // Delete the first occurance of character c. Do nothing if c does not occur.
        void remove(char c, int n); // Delete the nth occurance of c. Do nothing if c does not occur n times.
        std::string toString(); // Return the list as a string.
        CharList(std::string s); // Constructor. Initializes the list to the contents of the string.
        ~CharList(); // Destructor. Returns all memory in the list.

    private:
        CharNode* header;
        CharNode* trailer;
        int len;

        void insert_after(CharNode* ind, char d); // Inserts d immediately after ind
        void insert_before(CharNode* ind, char d); // Inserts d immediately before ind
        void remove_node(CharNode* ind); // Link the elements before and after ind together, then deletes ind
};