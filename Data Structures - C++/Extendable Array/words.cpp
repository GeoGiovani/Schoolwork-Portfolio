#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/* 
My strategy for answering Part 1 will be to implement an extendable array that has the function 
isInAlphabeticalOrder(string s) as a member function. Each position i of the extendable array holds the 
number of words of length i entered into isInAlphabeticalOrder(s), as well as the number of comparisons 
performed by isInAlphabeticalOrder(s) on words of length i.

An extension of the extendable array will occur whenever isInAlphabeticalOrder(s) is called with a 
string argument that is longer than any other string entered into the array. In this case, the extendable array
is expanded to hold up to and including the new word's length. Therefore, the average amortized running 
time and memory usage of an extension per word is approximately equal to (w/t), where w is the maximum 
word length entered into the extendable array, and t is the total number of words in the extendable array.

The extendable array has a member function makeFloatArray(), which returns a float array that holds the average 
number of comparisons made by isInAlphabeticalOrder(s) on words of length i at index i. 
To minimize rounding errors, a single calculation is used to to calculate the value of each index 
in the returned array. This process runs in Θ(w) time, where w is the number of different word lengths 
that have been entered into the extendable array.
*/


// A position in the extendable array. Each arrayPosition
// stores data for words entered into isInAlphabeticalOrder(s).
class ArrayPosition {

    private:
        int wordLength;                     // The wordlength associated with the position
        unsigned long int numComparisons;   // Number of comparisons for words of this length
        unsigned long int numWords;         // Number of words of this length encountered

    public:
        ArrayPosition();                  
        ~ArrayPosition() { }                

    friend class WordDataArray;            
};

ArrayPosition::ArrayPosition()

    : wordLength(0),
      numComparisons(0),
      numWords(0) { }


// An extendable array of ArrayPositions
class WordDataArray {

    private:
        int numPositions;                   // Number of ArrayPositions in the extendable array (excluding position 0)
        long unsigned int numComparisons;   // Total number of comparisons made by isInAlphabeticalOrder(string s)
        long unsigned int numCharacters;    // Total number of characters of strings read by isInAlphabeticalOrder(s)
        long unsigned int numWords;         // Total number of words
        ArrayPosition* positions;

    public:
        WordDataArray(unsigned long int n);                    // Creates a new WordDataArray with numWords set to n
        ~WordDataArray() { delete[] this->positions; }         // Destructor. Deletes all positions
        
        inline int size() const { return this->numPositions + 1; }                     // Return size of WordDataArray
        inline bool empty() const { return (this->numPositions == 0); }                // Return true if empty
        inline ArrayPosition& operator [](int i) const { return this->positions[i]; }  // Return a reference to index i

        bool isInAlphabeticalOrder(string s);                  // Returns true if string s is in alphabetical order
        void reserve(const int newNumPositions);               // Extends the size of the WordDataArray
        void insertWord(int wordLength, int numComparisons);   // Inserts new word data into the correct position
        
        // Return the average word length of all words
        inline float avgWordLength() const { return (float) this->numCharacters / (float) this->numWords; } 

        // Return the average number of comparisons made per word      
        inline float avgComparisons() const { return (float) this->numComparisons / (float) this->numWords; } 

        // Return the maximum word length 
        inline int maxWordLength() const { return this->numPositions; } 

        // Convert the extendable array to a float array
        float* makeFloatArray() const;  
};


// Creates a new WordDataArray with numWords set to n.
WordDataArray::WordDataArray(unsigned long int n = 0)

    : numPositions(0),
      numComparisons(0),
      numCharacters(0),
      numWords(n),
      positions(NULL) { }


// Updates the numWords and numComparisons members for the position at 
// positions[wordLength]. If the length of the word being inserted is geater
// than numPositions, the WordDataArray is resized to wordLength + 1.
void WordDataArray::insertWord(int wordLength, int numComparisons) 
{
    // Resize the WordDataArray if needed
    if (this->numPositions < wordLength) {
        reserve(wordLength);
    }

    // Update running total data members
    this->numCharacters += wordLength;
    this->numComparisons += numComparisons;

    // Update position data members
    this->positions[wordLength].numWords++;
    this->positions[wordLength].numComparisons += numComparisons;
}


// Increase the size of the WordDataArray. 
// Does nothing if the new size is smaller than the current size or
// if the new size is less than zero. 
void WordDataArray::reserve(const int newNumPositions) 
{
    // Check if the new size is valid
    if (newNumPositions < this->numPositions || numPositions < 0) {
        return;
    }

    // Create the new array with size equal to (newNumPositions + 1)
    // The extra position accounts for index 0, which is always empty
    ArrayPosition* newPositions = new ArrayPosition[newNumPositions + 1];

    // Copy the old positions into the new WordDataArray
    for (int i = 1; i <= this->numPositions; i++) {
        newPositions[i] = this->positions[i];
    }

    // Delete the old positions
    if (this->positions != NULL) {
        delete[] this->positions;
    }

    // Update numPositions and positions
    this->numPositions = newNumPositions;
    this->positions = newPositions;
}


// Returns true if the string s is in alphabetical ASCII order.
// Keeps track of the number of comparisons performed, 
// and updates the position corresponging to the length of s 
// in the WordDataArray by calling insertWord() once.
bool WordDataArray::isInAlphabeticalOrder(string s) 
{
    int length = s.size();
    int numComparisons = 0;

    // Check each letter of s one-by-one to see if the characters in s 
    // appear in alphabetical ASCII order.
    // Increments numComparisons each for each (s[i] > s[i+1]) comparison made.
    for (int i = 0; i < length - 1; ++i) {
        numComparisons++;
        if (s[i] > s[i+1]) {
            this->insertWord(length, numComparisons);
            return false;
        }
    }

    this->insertWord(length, numComparisons);
    return true;
}


// Convert the extendable array to a float array, with size (numPositions + 1).
// The ith index of the returned array will contain the average number of 
// comparisons made by isInAlphabeticalOrder(s) for words of length i.
float* WordDataArray::makeFloatArray() const 
{
    // Check if the WordDataArray is empty
    if (this->empty()) {
        return 0;
    }

    float* floatArray = new float[this->numPositions + 1];

    // Loop through and compute the average number of comparisons made for each word length.
    // If no words of length i were encountered, set the average number of comparisons to 0.
    for (int i = 0; i <= numPositions; i++) {
        if (this->positions[i].numWords == 0) {
            floatArray[i] = 0;
        }
        else {
            floatArray[i] = ((float) this->positions[i].numComparisons) / ((float) this->positions[i].numWords);
        }
    }
    return floatArray;
}


class FileOpeningException {
private:
	string errorMessage;
public:
	FileOpeningException(string message) {
		errorMessage = message;
	}
	string getMessage() {
		return errorMessage;
	}
};


string* readWords(string filename, int & arraySize);
void writeArray(string filename, float *arr, int arraySize) throw (FileOpeningException);

int main (void) 
{
	int numWords;
	string wordListFilename = "./wordlist";
	string outputFilename = "average_comps.txt";

	// This reads the words from wordListFilename.
	// numWords will be the number of words in the array words
	// words[i] will be the ith word
	string *words = readWords(wordListFilename, numWords);
   
    // Make an extendable word array
    WordDataArray* WordStats = new WordDataArray(numWords);

    // For each word in words[], call isInAlphatecialOrder 
	for (int i = 0; i < numWords; i++) {
        WordStats->isInAlphabeticalOrder(words[i]);
	}

    // Compute values
    float* avgComparisonsForLength = WordStats->makeFloatArray();
    int maxWordLength = WordStats->size();

	// Print average number of comparisons for each length to a file for plotting.
	writeArray(outputFilename, avgComparisonsForLength, maxWordLength);

	// Clean up memory and return.
	delete[] words;
    delete[] avgComparisonsForLength;
    delete WordStats;
	return 0;
}


// Writes the array arr in text format to a file named filename.
// each element of the array has its own line in the file, 
// which contains the element's index and value, separated by a space.
void writeArray(string filename, float *arr, int arraySize) throw (FileOpeningException) 
{
	ofstream outputStream(filename.c_str());

	if (outputStream.fail()) {
		throw new FileOpeningException("Error opening " + filename);
	}

	for (int i = 0; i<arraySize; i++) {
		outputStream << i << " " << arr[i] << endl;
	}
	outputStream.close();
}


int numLinesInStream(ifstream& stream) 
{
	int numLines = 0;
	string tempLine;

	// getline returns 0 at end of file.
	while (getline(stream, tempLine)) {
		numLines++;
	}
	return numLines;
}


// note how the following function is all about one thing:
// the handling of the ifstream called inputStream().
// the exact computation done using inputStream() is abstracted
// to a single call (to numLinesInStream).
// Here, I don't declare the thrown object with a "throw ()" on the
// first line.  It's optional, so I'm leaving it off this time.
int numLinesInFile(string filename) 
{
	ifstream inputStream(filename.c_str());

	if (inputStream.fail()) {
		throw new FileOpeningException(filename + "... file not found ...");
	}

	int numLines = numLinesInStream(inputStream);

	inputStream.close();
	return numLines;
}


// Reads lines from a file named filename.
// Returns an array of strings, one per line of the file filename, 
// and places the size of the array in the reference parameter (output parameter) arraySize
string* readWords(string filename, int & arraySize) 
{
	string* result = NULL;
	arraySize = numLinesInFile(filename);
	ifstream inputStream(filename.c_str());

	// Read file contents into result, now that size is known
	result = new string[arraySize];			//create results array
	for (int i=0; i < arraySize; i++){
		getline(inputStream, result[i]);
	}
	inputStream.close(); //don't forget to close file

    return result;
}