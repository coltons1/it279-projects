#include <iostream>
#include <vector>
#include "BinomialQueue.h"
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////
/*                              binomial queue implementations                             */
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
BinomialQueue<T>::BinomialQueue(){                                                  // default constructor
    // Initializes the inner roots array to a basic size of 5 and nullptrs all values.
    // Initializes all other necessary variables.
    roots = new Node<T>*[5]{nullptr};
    queueSize = 5;
    maxDegree = -1;
    sizeItems = 0;
}

template <typename T>
BinomialQueue<T>::BinomialQueue(int queueSize){                                     // parameterized constructor
    // Initializes the inner roots array to a basic size of queueSize and nullptrs all values.
    // Initializes all other necessary variables.
    roots = new Node<T>*[queueSize]{nullptr};
    this->queueSize = queueSize;
    maxDegree = -1;
    sizeItems = 0;
}

template <typename T>
BinomialQueue<T>::BinomialQueue(const BinomialQueue<T>& a){                         // copy constructor
    // Copies the necessary values to the new BQ's variables.
    // Initalizes a new roots array to the same size and copies all trees in their proper positions.
    queueSize = a.queueSize;
    maxDegree = a.maxDegree;
    sizeItems = a.sizeItems;

    roots = new Node<T>*[a.queueSize]{nullptr};
    //copy roots
    for(int i = 0; i < a.queueSize; i++){
        if(a.roots[i] == nullptr){
            this->roots[i] = nullptr;
        } else {
            this->roots[i] = copyTree(a.roots[i]);
        }
    }
}

template <typename T>
BinomialQueue<T>& BinomialQueue<T>::operator= (const BinomialQueue<T> &a){          // overloaded assignment operator
    // Checks for self assignment, then empties the queue, copies necessary variables, copies array.
    if(this == &a) return *this;
    emptyQueue();

    queueSize = a.queueSize;
    maxDegree = a.maxDegree;
    sizeItems = a.sizeItems;

    roots = new Node<T>*[a.queueSize]{nullptr};
    for(int i = 0; i < a.queueSize; i++){
        if(a.roots[i] == nullptr){
            this->roots[i] = nullptr;
        } else {
            this->roots[i] = copyTree(a.roots[i]);
        }
    }
    return *this;
}

template <typename T>
Node<T>* BinomialQueue<T>::copyTree(const Node<T> *t){                              // copy tree helper
    // Helper function to copy the trees, copies children and siblings
    if(t == nullptr) return nullptr;
    Node<T> *node = new Node<T>(t->key);
    if(t->firstChild != nullptr)
        node->firstChild = copyTree(t->firstChild);
    if(t->nextSibling != nullptr)
        node->nextSibling = copyTree(t->nextSibling);
    return node;
}

template <typename T>
void BinomialQueue<T>::add(T key){                                                  // add function, adds value to the queue
    // Add function to add a single new value to the queue
    Node<T>* newTree = new Node<T>(key);
    if(newTree != nullptr && roots[0] == nullptr){
        roots[0] = newTree;
    } else if(roots[0] != nullptr){
        merge(newTree);
    }
    sizeItems++;
}

template <typename T>
void BinomialQueue<T>::add(Node<T> *tempTree){                                      // add function, adds a passed in tree to the queue
    // Add function to add an existing tree to the queue. 
    if(tempTree != nullptr){
        merge(tempTree);
    }
}

template <typename T>
T BinomialQueue<T>::remove(){                                                       // remove function, removes value from the queue
    // Remove function, locates the minimum value and it's location in the queue.
    // Stores value and removes the value from the queue, then merges all children into the main queue.
    
    //using max possible integer value for starting.
    int minVal = INT32_MAX;
    int minLoc = 0;
    for(int i = 0; i < queueSize; i++){
        if(roots[i] != nullptr && roots[i]->key < minVal){
            minVal = roots[i]->key;
            minLoc = i;
        }
    }
    //get all the children of the tree at min loc and merges them into the main queue. 
    if(roots[minLoc]->firstChild != nullptr){
        //if the tree has a child (ie. not a singleton)
        Node<T> *toDelete = roots[minLoc];
        roots[minLoc] = nullptr;
        Node<T> *currTree = toDelete->firstChild;
        //loop to merge all child trees into main queue.
        while(currTree != nullptr){
            Node<T> *next = currTree->nextSibling;
            currTree->nextSibling = nullptr;
            merge(currTree); 
            currTree = next;
        }
        //remove the old tree.
        delete toDelete;

        //quick confirmation to recheck maxDegree
        for(int i = maxDegree; roots[i] == nullptr; i--){
            maxDegree = i - 1;
        }
    } else {
        //if the tree is a singleton
        delete roots[minLoc];
        roots[minLoc] = nullptr;
    }
    sizeItems--;
    return minVal;
}

template <typename T>
void BinomialQueue<T>::merge(Node<T>* toBeMerged){                                  // merge function, takes in one tree and merges it into the queue
    // Merge function, merges a tree into the queue.

    // tree to hold any possible carries from the previous operation. 
    Node<T> *carryTree = nullptr;
    // decide the max degree
    maxDegree = (toBeMerged->degree > maxDegree ? toBeMerged->degree : maxDegree);

    // merge from the degree of toBeMerged, until i is greater than max degree, carryTree is empty, or toBeMerged is empty. 
    for(int i = toBeMerged->degree; i <= maxDegree || carryTree != nullptr || toBeMerged != nullptr; i++){
        Node<T> *currTreeinRoots = roots[i];
        // how many trees are there currently...
        int count = (currTreeinRoots != nullptr) + (toBeMerged != nullptr) + (carryTree != nullptr);
        if (count == 0) {
            // no trees can be merged
            return;
        } else if (count == 1){
            // there is only one tree, fill the position, update maxDegree, and set carrytree to nullptr.
            if(currTreeinRoots != nullptr){
                roots[i] = currTreeinRoots;
                maxDegree = (currTreeinRoots->degree > maxDegree ? currTreeinRoots->degree : maxDegree);
            } else if (toBeMerged != nullptr && toBeMerged->degree == i){
                roots[i] = toBeMerged;
                maxDegree = (toBeMerged->degree > maxDegree ? toBeMerged->degree : maxDegree);
                toBeMerged = nullptr;
            } else if(carryTree != nullptr) {
                roots[i] = carryTree;
                maxDegree = (carryTree->degree > maxDegree ? carryTree->degree : maxDegree);
                carryTree = nullptr;
            }
        } else if (count == 2){
            // there are two trees present, 
            // no placements to be made, a merge has to be done. put it into the carryTree and nullptr toBeMerged if used.
            if(currTreeinRoots != nullptr && carryTree != nullptr){
                carryTree = currTreeinRoots->merge(carryTree);
            } else if (currTreeinRoots != nullptr && toBeMerged != nullptr){
                carryTree = currTreeinRoots->merge(toBeMerged);
                toBeMerged = nullptr;
            } else {
                carryTree = toBeMerged->merge(carryTree);
                toBeMerged = nullptr;
            }
            roots[i] = nullptr;
        }  else if(count == 3){
            // in this case, the carry would be the correct value for that index and the others would need to be merged.
            roots[i] = carryTree;
            maxDegree = (carryTree->degree > maxDegree ? carryTree->degree : maxDegree);
            carryTree = currTreeinRoots->merge(toBeMerged);
            toBeMerged = nullptr;
        }
    }
}

template <typename T>
int BinomialQueue<T>::size(){                                                       // returns the number of all items in the queue
    return sizeItems;
}

template <typename T>
void BinomialQueue<T>::print(){                                                     // print function to display the queue and all trees inside. 
    // simply prints the current degree/index and then calls a helper function to print the trees.
    for(int i = 0; i <= maxDegree; i++){
        cout <<"B["<<i<<"]:\n";
        printTree(roots[i],1);
        cout<<"\n";
    }
    cout<< "Size = " << sizeItems << "\n";
}

template <typename T>
void BinomialQueue<T>::printTree(Node<T> *node, int depth){                         // helper function to print the trees out
    // prints the trees recursively. 
    if (node == nullptr){
        return;
    }

    for(int i = 0; i < depth; i++){
        cout<< "    ";
    }
    cout<< node->key << "\n";

    printTree(node->firstChild, depth+1);
    printTree(node->nextSibling, depth);
}

template <typename T>
BinomialQueue<T>::~BinomialQueue(){                                                 // destructor
    emptyQueue();
}

template <typename T>   
void BinomialQueue<T>::emptyQueue(){                                                // helper that empties the queue
    // empties each index of the queue with emptyTree.
    for(int i = 0; i < queueSize; i++){
        emptyTree(roots[i]);
    }
    delete[] roots;
    roots = nullptr;
    queueSize = 0;
    maxDegree = 0;
    sizeItems = 0;
}

template <typename T>
void BinomialQueue<T>::emptyTree(Node<T> *&node){                                   // helper that empties a tree
    // empties a tree, its children, siblings, and then itself.
    if(node != nullptr){
        emptyTree(node->firstChild);
        emptyTree(node->nextSibling);
        delete node;
    }
    node = nullptr;
}