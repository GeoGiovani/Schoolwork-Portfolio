#include <stdio.h>

void arrCpy ( int arr[], int newArr[], int len ) {

	int i = 0;
	for (int i = 0; i < len; i++) {
		arr[i] = newArr[i];	
	}
}

//Modified merge () to replace identical elements with -1
void merge ( int arr[], int first, int mid, int last ) {

	int len = last-first+1;
	int newArr[len]; //New array for holding sorted elements

	int left = first; //Used for tracking first position in new array
	int right = mid+1; //Used for tracking mid position in new array
	int newPos = 0;
	
	while (left <= mid && right <= last) {

		//Replace value of leftmost duplicate with -1
		if (arr[left] == arr[right]) {
			arr[left] = -1;
		}

		//Move through arr[] and sort elements into newArr[]
		if (arr[left] < arr[right]) {
			newArr[newPos++] = arr[left++];
		} else {
			newArr[newPos++] = arr[right++];
		}
	}

    //Flush non-empty piece
    arrCpy(newArr + newPos, arr + left, mid - left + 1);
    arrCpy(newArr + newPos, arr + right, last - right + 1);

	//Copy sorted elements from newArr[] back to arr[]
	arrCpy(arr + first, newArr, len);
}

void mergeSort ( int arr[], int first, int last ) {

	if (last <= first) return;
	
	//Find the middle index position to split array
	int mid = (first + last) / 2;

	//Recursively sort two halves
	mergeSort(arr, first, mid);
	mergeSort(arr, mid+1, last);

	//Join chunks together
	merge(arr, first, mid, last);
}

void main () {

	int array[10] = {1, 3, 3, 0, 100, 3, 4, 2, 0, 6};
	mergeSort(array, 0, 9);
	
	for (int j = 0; j < 10; j++) {
		printf("array[%d] = %d\n", j, array[j]);
	}
}