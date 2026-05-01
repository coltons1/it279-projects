//+---------------------+----------------------+//
// Colton Stanek                                //
// Binmoial Queue                               //
// Due Date: 1 April 2026                       //
//+---------------------+----------------------+//

#ifndef BINOMIALQUEUE_H_
#define BINOMIALQUEUE_H_

//////////////////////////////////////////////////
/*              Binomial Tree Node              */
//////////////////////////////////////////////////

template <typename T>
class Node {
    public: 
        Node(T key){                                                // node constructor 
            this->key = key;
            firstChild = nullptr;
            nextSibling = nullptr; 
            degree = 0;
        };
        Node<T>* merge(Node* tree){                                 // merge for two trees of the same degree.
            Node<T> *greaterTree, *lesserTree;  
            greaterTree = (this->key > tree->key ? this : tree);    // larger tree
            lesserTree = (this->key > tree->key ? tree : this);     // smaller tree

            if(lesserTree->firstChild == nullptr){                  // no child, good to add
                lesserTree->firstChild = greaterTree;
            } else {
                //add as first child, make sure to maintain sibling
                greaterTree->nextSibling = lesserTree->firstChild;
                lesserTree->firstChild = greaterTree;
            }
            
            //increment degree
            lesserTree->degree++;

            return lesserTree;
        }

        T key;                                                      // the key value
        Node* firstChild;                                           // the first child of node
        Node* nextSibling;                                          // the next sibling (next child of the parent node)
        int degree;                                                 // the current degree of the tree
};

//////////////////////////////////////////////////
/*                Binomial Queue                */
//////////////////////////////////////////////////

template <typename T>
class BinomialQueue {
    public: 
        BinomialQueue();                                            // default constructor
        BinomialQueue(int queueSize);                               // parameterized constructor where n is the size of the queue.
        BinomialQueue(const BinomialQueue<T>& a);                   // copy constructor
        void add(T key);                                            // adds item into the queue
        void add(Node<T> *tempTree);                                // adds tree into the queue
        T remove();                                                 // removes an item from the queue and returns the item
        void merge(Node<T> *tree);
        int size();                                                 // returns the number of items in the queue
        void print();                                               // print out with correct indentation, all trees in the queue up to the highest non null degree
        BinomialQueue & operator = (const BinomialQueue<T> & a);    // overloaded assignment operator
        ~BinomialQueue();                                           // destructor



    private:
        void printTree(Node<T> *node, int depth);                   // print the tree
        void emptyQueue();                                          // empties the queue of all binomial trees
        void emptyTree(Node<T> *&node);                             // empties a binomial tree of all nodes
        Node<T>* copyTree(const Node<T> *t);                        // copy tree helper function, copies the tree recursively
        Node<T> **roots;                                            // the array storing the binomial trees.
        int queueSize;                                              // the length of the queue
        int sizeItems;                                              // the number of items in the queue
        int maxDegree;                                              // the tree of the highest degree in the queue.
};
;

#endif