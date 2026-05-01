/*
 * myArray.h
 *
 *  Created on: Jan 24, 2019
 *  Author: Chung-Chih Li
 */

#ifndef MYARRAY_H_
#define MYARRAY_H_


class myArray  {
public:
    myArray();                  // Default constructor;
    myArray(int s, int iv);  	// s:size; iv:initial values;
    myArray(const myArray & a); // Copy constructor;
    myArray & operator = (const myArray & a); // Overload =
    ~myArray();                 // Destructor;
    int & operator [](int i);	// Return the ith element in the array;
    double average();           // Return the average of the numbers in the array;
    int max();                  // Return the biggest number's index in the array;
    int min();                  // Return the smallest number's index in the array;
    int getSize();              // Return the size of the array;
private:
    int size;
    int *A;
};


#endif /* MYARRAY_H_ */
