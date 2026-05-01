//============================================================================
// Name        : TestAsg1.cpp
// Author      : Chung-Chih Li
// Version     :
// Description : Test Program for IT279 Programming Assignment 1  
// Created     : 8/26/2025
//============================================================================

#include <iostream>
#include "myArray.h"
using namespace std;

/*
 * This function should be defined in myArray.cpp
 * This function will return a myArray that contains first n Fibonacci numbers.
 */
myArray fibonacci(int n);

/*
 * This function will take a pointer of myArray and replace every entry with a random
 * integer between 0 and the size of the myArray pointed by na in the argument.
 *
 */
void rand(myArray *ma);


void show(myArray a) {
	cout <<  endl;
	for (int i = 0; i< a.getSize(); i++) {
			cout <<  a[i] << " ";
	}
	cout << endl;
	cout << "size=" << a.getSize() << " ";
	cout << "max=a["<< a.max() << "]=" << a[a.max()] << " ";
	cout << "min=a["<< a.min() << "]=" << a[a.min()] << " ";
	cout << "average=" << a.average() << endl;

}

void test() {
	int n=25;
	myArray a = fibonacci(n);
	cout << "\n" << n << " Fibonacci numbers:";
	show(a);
	
	cout << "\n" << n << " random numbers:";
	rand(&a);
	show(a);
	
	myArray b,c;
	
	c = b = a;
		
	rand(&a);
	show(a);
	show(b);
	show(c);
}


int main() {
	test();
	return 0;
}
