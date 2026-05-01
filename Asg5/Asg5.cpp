//============================================================================
// Name        : Asg5.cpp
// Author      : Chung-Chih Li
// Version     :
// Copyright   : Your copyright notice
// Created date: 3/5/2026
//============================================================================
#include <iostream>
#include <cstdlib>
#include "Heap.cpp"
using namespace std;

/**
 * New an int array of size n with random values from 0 to n-1, no duplication.
 */
int * populateRandomKeys(int n) {
	int temp, k, *a = new int[n];
	for (int i=0;i<n;i++) a[i]=i*10;
	for (int i=0;i<n;i++) {
		k = rand()%n;
		temp = a[i];
		a[i]=a[k];
		a[k]=temp;
	}
	return a;
}

// Print the first n elements in the array
void printArray(int *a, int n, int size) {
	cout << " [";
	for (int i=0; i< size && i < n; i++) {
			cout << *(a+i) << " ";
	}
	if (n >= size)
		cout << "]";
	else
		cout <<  ".........";
}


/**
* Test adding n kets into an empty heap
*/
bool testAdding(int n) {
	Heap *h = new Heap(10);
	bool ok=true;
	for (int i=0;i<n;i++) h->add(rand()%n);
	int a=h->poll(), b;
	while (h->size() !=0) {
		b = h->poll();
		if (a>b) {
			ok=false;
			cout << "\nError!! A child is smaller than a parent!! ";
			break;
		}
		a=b;
	}
	return ok;
}

/**
 *  Test copy constructor. The  argument will be copied in first.
 */
void testCopyConstructor(int n){
	cout << "\n** Test copy constructor and toArrat, sorted Array\n";
	int *A = populateRandomKeys(n);
	Heap *H1 = new Heap(A, n);
    Heap *H2 = new Heap(*H1);
    cout << "H1:"; printArray(H1->toArray(),H1->size(),H1->size());  cout << endl;
    cout << "H2:"; printArray(H2->toArray(),H2->size(),H2->size());  cout << endl;
	H2->add(H1->poll());
	H2->add(H1->poll());
	cout << "After H2->add(H1->poll());  H2->add(H1->poll());" << endl;
	cout << "H1:"; printArray(H1->toArray(),H1->size(),H1->size());  cout << endl;
	cout << "H2:"; printArray(H2->toArray(),H2->size(),H2->size());  cout << endl;
	cout << "H2:"; printArray(H2->sortedArray(),H2->size(),H2->size()); cout << "Sorted" << endl;
	cout << "H2:"; printArray(H2->toArray(),H2->size(),H2->size());
}

/**
 *  Test Assignment operator.
 */
void testAssgOp(int n){
	cout << "\n\n** Test Assignment operator\n";
	int *A = populateRandomKeys(n);
	Heap H1 = Heap(A, n);
    Heap H2, H3;
    H3 = H2 = H1;
    cout << "H1:"; printArray(H1.toArray(),H1.size(),H1.size());  cout << endl;
    cout << "H2:"; printArray(H2.toArray(),H2.size(),H2.size());  cout << endl;
	H2.add(H1.poll());
	H2.add(H1.poll());
	cout << "After H2.add(H1.poll());  H2.add(H1.poll());" << endl;
	cout << "H1:"; printArray(H1.toArray(),H1.size(),H1.size());  cout << endl;
	cout << "H2:"; printArray(H2.toArray(),H2.size(),H2.size());  cout << endl;
	cout << "H3:"; printArray(H3.toArray(),H3.size(),H3.size()); cout << endl;
}

/**
*  Test removing n items from a heaps. 
*/
bool testRemoving(int n) {
	bool ok=true;
	int *A = populateRandomKeys(n);
	Heap *h = new Heap(A, n);
	int a=h->poll(), b;
	while (h->size() !=0) {
		b = h->poll();
		if (a>b) {
			ok=false;
			cout << "\nError!! A child is smaller than a parent!! ";
			break;
		}
		a=b;
	}
	return ok;
}

/**
 *  Test destructor
 */
void testDescructor(){
	int *firstArray=NULL;
	cout << "\n** Test destructor";
	int n=1000, *A = populateRandomKeys(n);
	for (int i=0;i<10;i++) {
		Heap H = Heap(A, n);
		cout << "\n" << "i=" << i << " inner array at " << H.heap;
		if (i==0) {
			firstArray= H.heap;
		} else {
			cout << " --> difference: " << (long) H.heap - (long) firstArray;
		}
	}
}

int main() {
	srand(2792026);

	bool ok;
	int n=100;

	cout << "\n *** Test adding:";
	for (int i=0;i<1000;i++) {
		ok = testAdding(n);
		if (!ok) {
			cout << " Some Error in adding!\n";
			break;
		}
	}
	if (ok) cout << "   No Error found.\n";
	cout << " *** Test removing:";
	for (int i=0;i<1000;i++) {
		ok=testRemoving(n);
		if (!ok) {
			cout << " Some Error in removing!\n";
			break;
		}
	}
	if (ok) cout << " No Error found.\n";

	testCopyConstructor(10);
	testAssgOp(10);
	testDescructor();
	cout << "\n\n** Done **\n";


	return 0;
}
