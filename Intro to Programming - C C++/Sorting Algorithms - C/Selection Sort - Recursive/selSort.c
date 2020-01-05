//Returns index of smallest element from arr[mid] to arr[len -1]
int findMinpos(int arr[], int mid, int length) {

	int index = length -1;
	
	// Create a function stack until mid = length
	if (mid < length-1) {
		index = findMinpos(arr, mid + 1, length);
	}

	// Compare arr[index] with arr[mid] and return the smaller value
	if (arr[index] < arr[mid]) {
		return index;
	}
	else {
		return mid;
	}
}

//Swaps arr[mid] and arr[minpos]
void swapMinpos(int arr[], int mid, int length) {
	
	int smalldex = findMinpos(arr, mid, length);
	int swapval = arr[mid];
	arr[mid] = arr[smalldex];
	arr[smalldex] = swapval;
}

//Puts smallest mid elements in arr in sorted order in arr[0...mid-1];
void selSortR(int arr[], int mid, int length) {   

	if (mid > 0) {
		selSortR(arr, mid-1, length);
		swapMinpos(arr, mid-1, length);
	}
}