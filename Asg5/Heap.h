/*
 *  Heap.h
 *
 *  Created on: 3/5/2025
 *  Author: Chung-Chih Li
 */

#ifndef Heap_H_
#define Heap_H_

class Heap  {
public:
    Heap();                     // Default constructor;
    Heap(int c);                // Set the capacity to c;
    Heap(int* a, int n);        // Build a heap from array a[0]..a[n-1];
    void add(int key);          // add a new key into the heap
    int get() {return heap[0];} // Return the item with the highest priority
    int poll();                 // Remove and return the item with the highst priority
    int size();                 // Return the number of items in the heap
    int * toArray();            // Return a copy of the internal array;
    int * sortedArray();        // Return a sorted copy of the internal array, and
    Heap(const Heap & h);       // Copy constructor;
    Heap & operator = (const Heap & h); // Overload =
    ~Heap();                    // Destructor;
private:
    void sink(int x);           // Sink heap[x] down;
    void percolate();           // Percolate to top, i.e, prepare *heap to be a heap.
    void doubleCapacity();      // Double array capacity
    int n;                      // The highest index of the heap, n+1 will be the size;
    int capacity;               // The capacity of the internal array
    int *heap;                  // The internal array for heap

friend void testDescructor();
};


#endif /* Heap_H_ */
