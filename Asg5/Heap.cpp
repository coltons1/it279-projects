#include "Heap.h"
#include <iostream>

using namespace std;

//constructor
Heap::Heap(){n = 0, capacity = 10;
    heap = new int[capacity];
};

//Set heap capacity to c
Heap::Heap(int c){
    n = 0;
    capacity = c;
    heap = new int[c];
}; 

//Build new heap from array
Heap::Heap(int* a, int n){
    //need to copy array. 
    this->capacity = n;
    this->n = n;
    this->heap = new int[n];
    for(int i = 0; i < n; i++){
        heap[i] = a[i];
    }
    this->percolate();
};

void swap(int* heap, int p, int i){
    //problem here i think.
    int temp;
    temp = heap[p];
    heap[p] = heap[i];
    heap[i] = temp;
};


//add new key to heap
void Heap::add(int key){
    n++;
    if(n == capacity) doubleCapacity();
    heap[n-1] = key;
    int p = n;
    int i = n - 1;
    while(i > 0) {
        p = (i-1)/2;
        if(heap[p] <= heap[i]) break;
        swap(heap,p,i);
        i=p;
    }
};

//Remove and return item with highest priority
int Heap::poll(){
    if(n<=0) return 0;
    int key = heap[0];
    heap[0] = heap[--n];
    sink(0);
    return key;
};

//return the number of items in the heap
int Heap::size(){
    return n;
};

//return a copy of the internal array
int *Heap::toArray(){
    int* arr = new int[n];
    for(int i = 0; i < n; i++){
        arr[i] = heap[i];
    }
    return arr;
};

//return sorted copy of array
int *Heap::sortedArray(){
    int* unsortArr = new int[n];
    for(int i = 0; i < n; i++){
        unsortArr[i] = heap[i];
    }
    //insertion sort
    for(int i = 0; i < n; i++){
        int key = unsortArr[i];
        int j = i-1;

        while(j>=0 && unsortArr[j] > key){
            unsortArr[j+1] = unsortArr[j];
            j = j-1;
        }
        unsortArr[j+1] = key;
    }
    return unsortArr;
};

//copy constructor
Heap::Heap(const Heap & h){
    heap = new int[h.capacity+1];
    for(int i = 0; i < h.n; i++){
        heap[i] = h.heap[i];
    }
    this->capacity = h.capacity + 1;
    this->n = h.n;
};

//overload =
Heap & Heap::operator = (const Heap & h){
  if(this != &h){
    if(this->n != h.n){
        //create a new array to store heap and delete old array
        delete[] heap;
        heap = new int[h.n+1];
        
    }
    //perform the copy
    for(int i = 0; i < h.n; i++){
        heap[i] = h.heap[i];
    }
    //if the internal arryas have the same capacity, there is still the chance that the original
    //array would have more elemnts or a greater size so it is likley safer to use h.n+1 to ensure there are no extra elements.
    this->capacity = h.n+1;
    this->n = h.n;

  }
  return *this; 
};

//destructor
Heap::~Heap(){
    delete[] heap;
    heap = nullptr;
};

// sink heap[x] down
void Heap::sink(int x){
    int l = 2*x+1, r = 2*x+2;
    if(l >= n) return;
    if(r >= n) {
        if (heap[x] > heap[l])
            swap(heap, x, l);
        return;
    }
    int i = (heap[l] < heap[r] ? l : r);
    if(heap[x] <= heap[i]) return;
    swap(heap, x, i);
    sink(i);
};

// percolate to top, prepare *heap to be a heap.
void Heap::percolate(){
    for (int i = (n-1)/2; i >= 0; i--){
        sink(i);
    }
};

//Double array capacity
void Heap::doubleCapacity(){
    int* newArr = new int[capacity*2];
    for(int i = 0; i < n; i++){
        newArr[i] = heap[i];
    }
    delete[] heap;
    this->heap = newArr;
    this->capacity = capacity*2;
};


