#include <iostream>
#include "myArray.h"
using namespace std;

myArray::myArray(){size = 0;A = nullptr;}

myArray::myArray(int s=0, int iv=0){ //s = size, iv = initial values
    A = new int[s];
    size = s;
    for (int i = 0; i < s; i++){
        A[i] = iv;
    }
}

myArray::myArray(const myArray & a){ //copy constructor
    size = a.size;
    A = new int[size];
    for (int i = 0; i < size; i++){
        A[i] = a.A[i];
    }

}

myArray & myArray::operator = (const myArray & a){
    //how do i copy an object entirely.
    //ensure they are the same size, if not create a new array of the required size
    //and delete the old array. 

    //perform a deep copy (make new array, not just point to other array.)
    //if the size of the arrays is the same, then we can just copy it over.
    //if not...
    if(this != &a){
        if(size != a.size){

            //create new array, delete old array
            delete[] A;
            A = new int[a.size];
            
        }
        //set size and perform copy. 
        size = a.size;
        for(int i = 0; i < a.size; i++){
            A[i] = a.A[i];
        }
    }   
    return *this;

};

myArray::~myArray(){ //destructor
    delete[] A;
    A = nullptr;
    
}

int & myArray::operator [](int i){ //return the ith element in the array
    if (i < 0 || i >= size){
        cout << "out of range:"; 
        return A[0];
    }
    return A[i];
}

double myArray::average(){ //return the average of the numbers in the array
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += A[i];
    }
    return sum / size;
}
int myArray::max(){ //return the biggest number's index in the array
    int maxIndex = 0;
    for(int i = 0; i < size; i++){
        if(A[i] > A[maxIndex]){
            maxIndex = i;
        }
    }
    return maxIndex;
}
int myArray::min(){ //return the smallest number's index in the array
    int minIndex = 0;
    for(int i = 0; i < size; i++){
        if(A[i] < A[minIndex]){
            minIndex = i;
        }
    }
    return minIndex;
}
int myArray::getSize(){ //return the size of the array
    return size;
}

myArray fibonacci(int n){
    myArray fibArr = myArray(n, 0);
    if (n > 0){
        fibArr[0] = 0;
    }
    if (n > 1){
        fibArr[1] = 1;
    }  
    for (int i = 2; i < n; i++){
        fibArr[i] = fibArr[i-1] + fibArr[i-2];
    }
    return fibArr;
}


//set a predetermined separate array which has the numbers of the same lenght of the array.
//use a version of a selection sort or such to populate the object array. 
void rand(myArray *ma){ //O(n)
    static int randomSeed = 2;
    int arr [ma->getSize()];
    int arrLen = sizeof(arr) / sizeof(arr[0]);

    for(int i = 0; i < arrLen; i++){
        arr[i] = i;
    }

    //where the randomization happens
    srand (randomSeed);
    randomSeed++;
    for (int i = arrLen - 1; i > 0; i--){
        int j = rand() % (i + 1);
        //swap
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    for(int i = 0; i < ma->getSize(); i++){
        (*ma)[i] = arr[i];
    }
}





