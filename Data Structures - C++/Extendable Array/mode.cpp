#include <iostream>
#include <fstream>
using namespace std;

/*
My strategy for finding the mode of an array of integers will be to first sort
the array using merge sort, which runs in O(nlogn) time, and then loop through
the sorted array and identify the element that occurs the most times. If the 
array is bimodal or multimodal, the smallest mode is returned. The second loop
takes O(n) time, so the time complexity of the entire process is O(nlogn).
*/


// Exception for handling an input array with 0 elements.
class emptyArrayException {
    private:
        std::string errorMsg;

    public:
        emptyArrayException(const std::string& err) { errorMsg = err; }
        std::string getMessage() const { return errorMsg; }
};


// Sort arr[first,..,mid,..,last] by (1) creating a new array and copying  
// elements from arr into the new array in ascending order, and then (2) copying 
// the sorted elements back to arr[first,..mid,..,last].
void merge (long* arr, int first, int mid, int last) 
{
	int len = last - first + 1; // Length of the temporary array
	long tempArr[len];          // Temporary array for storing sorted elements
    int tempInd = 0;            // Index of the temporary array
	int left = first;           // Index of the leftmost element being compared
	int right = mid + 1;        // Index of the rightmost element being compared
	
	// Move through arr and compare elements at arr[left] and arr[right]. 
    // Store the smaller of the two in the temporary array, and
    // then increment the left or right index (whichever was stored). 
	while (left <= mid && right <= last) {

		if (arr[left] < arr[right]) {
			tempArr[tempInd++] = arr[left++];
		} 
        else {
			tempArr[tempInd++] = arr[right++];
		}
	}

    // Transfer any remaining elements in the range arr[left] to arr[mid].
    while (left <= mid) {
        tempArr[tempInd++] = arr[left++];
    }
    // Transfer any remaining elements in the range arr[right] to arr[last].
    while (right <= last) {
        tempArr[tempInd++] = arr[right++];
    }
    // Copy the sorted elements back from the temporary array to the original.
    for (int i = 0; i < len; i++) {
        arr[first++] = tempArr[i];
    }   
}


// Using divide and conquer, recursively divide arr into pieces, and
// then merge them back together in sorted order using merge().
void mergeSort (long* arr, int first, int last) 
{
	// Base case, stop  dividing arr
	if (last <= first) {
        return;
    }

	// Find the middle index, it will be used to split the array
	int mid = (first + last) / 2;

	// Recursively divide each half of the array
	mergeSort(arr, first, mid);
	mergeSort(arr, mid + 1, last);

	// Merge the divided pieces together
	merge(arr, first, mid, last);
}


// Move through arr and find the mode. If the arr is bimodal or multimodal,  
// return the first element encountered that occurs the most times.
// Requires arr to be sorted in ascending order.
long findMode(long* arr, int len) 
{
    long mode = arr[0];
    int numEncounters = 0;

    long prevElem = arr[0];
    int prevCounter = 0;

    // Move through the entirety of arr
    for (int i = 0; i < len; i++) {

        // Whenever a new element is encountered, check if it occurred more
        // times than the current mode. If so, adjust mode and numEncounters.
        if (arr[i] != prevElem) {
            if (prevCounter > numEncounters) {
                mode = prevElem;
                numEncounters = prevCounter;
            }
            prevElem = arr[i];
            prevCounter = 0;
        }
        prevCounter++;
    }
    if (prevCounter > numEncounters) {
        mode = prevElem;
        numEncounters = prevCounter;
    }
    return mode;
}


// Sort arr using mergeSort(), and then find/return the mode using findMode().
// Throws an exception if an empty array is passed in as an argument.
long mode (long* arr, int n) 
{
    if (n <= 0) {
        throw emptyArrayException("Empty Array");
    }

    mergeSort(arr, 0, n-1);
    return findMode(arr, n);
}


// Input is the number of elements, followed by that many integers.
// Writes mode of elements to file specified as first command-line argument.
int main (int argc,  char* argv[]) 
{
	// ofstream for writing result.
	ofstream outputfile;

	// Parse command-line arguments.
	if (argc != 2) {
		// Note that the program name is the first argument, 
        // so argc==1 if there are no additional arguments.
		cerr << "Expected one argument." << endl;
		cerr << "  Usage: " << argv[0] << " output_filename" << endl;
		return 1;
	} else {
		// Open the filename specified for input.
		outputfile.open (argv[1]);
	}

	// Read a set of elements into an array.
	int n;
	long *arr;

	// Get the number of elements
	cout << "Enter the number of elements:" << endl;
	cin >> n;

	// Create array in dynamic memory.
	arr = new long[n];

	for (int i=0; i<n; i++) {
		cout << "Enter a number:" << endl;
		cin >> arr[i];
	}

	long m = mode(arr,n);

	cout << "The mode is " << m << endl;

	outputfile << m << endl;
	outputfile.close();

	// Free up memory.  Note use of delete [], since this is an array.
	delete [] arr;

	return 0;
}