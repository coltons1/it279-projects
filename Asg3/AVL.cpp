#include "AVL.h"
#include <iostream>
using namespace std;

//inserts a node with that key and data.
template <typename T> 
void AVL<T>::insert(int key, T data){
   //check if the root of the subtree does not exist.
   //should only ever run once at the creation of the tree.
   if(this->root == NULL){
    root = new Node<T>(key, data);
   } else {
    
    if(hasKey(key)){
        cout<<"Key already present in tree."<<endl;
        return;
    }

    Node<T> *node = new Node<T>(key, data);
    if(node->key >= root->key){
        //go to the right
        if(root->right == nullptr){
            root->right = node;
        } else {
            insertHelper(root->right, node);
        }
    } else if(node->key < root->key){
        //go to the left
        if(root->left == nullptr){
            root->left = node;
        } else {
            insertHelper(root->left, node);
        }
    }
   }
};

template <typename T> 
Node<T>* insertHelper(Node<T> *parentNode, Node<T> *newNode){
    if(newNode->key > parentNode->key){
        //go to the right
        if(parentNode->right == nullptr){
            parentNode->right = newNode;
            return newNode;
        } else {
            insertHelper(parentNode->right, newNode);
            parentNode->right = updateHeightCheckRotation(parentNode->right);
        }
    } else if(newNode->key < parentNode->key){
        //go to the left
        if(parentNode->left == nullptr){
            parentNode->left = newNode;
            return newNode;
        } else {
            insertHelper(parentNode->left, newNode);
            parentNode->left = updateHeightCheckRotation(parentNode->left);
        }
    } 
    return parentNode;
}

//removes the node with that key
template <typename T> 
void AVL<T>::remove(int key){
    if(hasKey(key) == false){
        cout<<"No matching key in the tree"<<endl;
        return;
    }
    this->root = removeHelper(root, key);
}

template <typename T>
Node<T>* removeHelper(Node<T> *node, int key){
    if(node == NULL) return NULL;
    if(node->key == key){
        if(node->left == nullptr)
            return node->right;
        if(node->right == nullptr)
            return node->left;

        Node<T> *leftmax = maxHelper(node->left);
        node->data = leftmax->data;
        node->key = leftmax->key;
        node->left = removeHelper(node->left, node->key);
        delete leftmax;
    } else {
        if(node->key < key)
            node->right = removeHelper(node->right, key);
        else 
            node->left = removeHelper(node->left, key);
    }
    return updateHeightCheckRotation(node);
}

template <typename T> 
Node<T> * updateHeightCheckRotation(Node<T> *node){
    node->updateHeight();
    int lh = (node->left == NULL ? 0 : node->left->height);
    int rh = (node->right == NULL ? 0 : node->right->height);

    if(rh-lh == 2){
        lh = (node->right->left == NULL ? 0 : node->right->left->height);
        rh = (node->right->right == NULL ? 0 : node->right->right->height);
        if (rh-lh >= 0)
            return RR(node);
        else
            return RL(node);
    }
    if(rh-lh == -2){
        lh = (node->left->left == NULL ? 0 : node->left->left->height);
        rh = (node->left->right == NULL ? 0 : node->left->right->height);
        if(rh-lh >=0)
            return LR(node);
        else
            return LL(node);
    }
    return node;
}

template <typename T>
Node<T>* RL(Node<T> *node){
    Node<T> *newT = node->right->left;
    Node<T> *newL = node;
    Node<T> *newR = node->right;

    newR->left = newT->right;
    newL->right = newT->left;
    newT->left = newL;
    newT->right = newR;

    newL->updateHeight();
    newR->updateHeight();
    newT->updateHeight();
    
    return newT;
}

template <typename T>
Node<T>* RR(Node<T> *node){
    Node<T> *newT = node->right;
    Node<T> *newL = node;
    Node<T> *newR = node->right->left;

    newT->left = node;
    newL->right = newR;

    newL->updateHeight();
    newT->updateHeight();
    
    return newT;
}

template <typename T>
Node<T>* LR(Node<T> *node){
    Node<T> *newT = node->left->right;
    Node<T> *newL = node->left;
    Node<T> *newR = node;

    newR->left = newT->right;
    newL->right = newT->left;
    newT->left = newL;
    newT->right = newR;

    newL->updateHeight();
    newR->updateHeight();
    newT->updateHeight();

    return newT;

}

template <typename T>
Node<T>* LL(Node<T> *node){
    Node<T> *newT = node->left;
    Node<T> *newR = node;
    Node<T> *newL = node->left->right;

    newR->left = newL;
    newT->right = newR;

    newR->updateHeight();
    newT->updateHeight();

    return newT;
}


//gets the node with that key
template <typename T> 
T AVL<T>::get(int key){
    Node<T> *node=this->root;
	while (node != NULL) {
		if (node->key == key)
			return node->data;
		if (node->key < key)  	// search right
			node = node->right;
		else 					// search left
			node = node->left;
	}
	// Error Here
	return NULL;
}

//checks tree for key
template <typename T> 
bool AVL<T>::hasKey(int key){
    Node<T> *node = this->root;
    while (node != NULL){
        if (node->key == key)
            return true;
        if (node->key < key) //right
            node = node->right;
        else //left
            node = node->left; 
    }   
    return false;
}

// Return the number of nodes of this tree
template <typename T> 
int AVL<T>::size(){
    return sizeHelper(root);
}   

template <typename T> 
int sizeHelper(Node<T> *node){
   
    if(node == NULL){
        return 0;
    } 
    return 1 + sizeHelper(node->left) + sizeHelper(node->right);
}

// Return the height of this tree
template <typename T> 
int AVL<T>::height(){
    if(root == NULL){
        cout<<"This tree is empty"<<endl;
        return 0;
    }
    return heightHelper(root);
}

template <typename T> 
int heightHelper(Node<T> *node){
    if(node == NULL){
        return 0;
    } 
    int lh = heightHelper(node->left), rh = heightHelper(node->right);

    return 1 + (lh > rh ? lh : rh);
}

// Return the maximum key
template <typename T> 
int AVL<T>::maxKey(){
    if (root == NULL) return(-1);;
    return maxHelper(root)->key;
}

// Return the minimum key
template <typename T> 
int AVL<T>::minKey(){
    if (root == NULL) return(-1);
    return minHelper(root)->key;
} 

// Return a pointer of data with max key
template <typename T> 
T AVL<T>::max(){
    if (root == NULL) return NULL;
    return maxHelper(root)->data;
}   

template <typename T> 
Node<T> *maxHelper(Node<T> *node){
    if (node == NULL) return NULL;
    if (node->right==NULL) return node;
    return maxHelper(node->right);
}

// Return a pointer of data with min key  
template <typename T> 
T AVL<T>::min(){
    if (root == NULL) return NULL;
    return minHelper(root)->data;
}

template <typename T> 
Node<T> *minHelper(Node<T> *node){
    if (node == NULL) return NULL;
    if (node->left==NULL) return node;
    return minHelper(node->left);
}

// Return an array of pointers of all data in inorder
// From Dr Li's BST.cpp
template <typename T> 
T* AVL<T>::inorder(){
    T *a;
    a = new T[AVL<T>::size()];
    int i = 0;
    inorderHelper(AVL<T>::root, a, i);
    return a;
} 

template <typename T> 
void inorderHelper(Node<T> *node, T*a, int &i){
    if (node == NULL) return;
    inorderHelper(node->left, a, i);
    a[i++] = node->data; 
    inorderHelper(node->right, a, i);
}

// Return an array of pointers of all data in preorder
// From Dr Li's BST.cpp
template <typename T> 
T* AVL<T>::preorder(){
    T *a;
	a = new T[AVL<T>::size()];
	int i=0;
    preorderHelper(AVL<T>::root, a, i);
	return a;
} 

template <typename T> 
void preorderHelper(Node<T> *node, T *a, int &i){
    if (node == NULL) return;
    a[i++] = node->data;
    preorderHelper(node->left, a, i);
    preorderHelper(node->right, a, i);
}

// Return an array of pointers of all data in postorder
// From Dr Li's BST.cpp
template <typename T> 
T* AVL<T>::postorder(){
    T *a;
	a = new T[AVL<T>::size()];
	int i=0;
    postorderHelper(AVL<T>::root, a, i);
	return a;
}

template <typename T> 
void postorderHelper(Node<T> *node, T*a, int &i){
    if (node == NULL) return;
    postorderHelper(node->left, a, i);
    postorderHelper(node->right, a, i);
    a[i++] = node->data;
}

template <typename T>
AVL<T>& AVL<T>::operator= (const AVL<T> & a){
    if(this->root == a.root) return *this;
    emptyTree(this->root);
    if(a.root == NULL) this->root = NULL;
    else {
        this->root = new Node<T>(a.root->key, a.root->data);
        this->root->left = copyTree(a.root->left);
        this->root->right = copyTree(a.root->right);
    }
    return *this;
}


template <typename T> 
AVL<T>::AVL(const AVL<T> & t){
    root = copyTree(t.root);
};

template <typename T>
Node<T>* copyTree(const Node<T> *t){
    if(t == NULL) return NULL;
    Node<T> *node = new Node<T>(t->key, t->data);
    if(t->left != NULL)
        node->left = copyTree(t->left);
    if(t->right != NULL)
        node->right = copyTree(t->right);
    return node;
}

//destructor
template <typename T> 
AVL<T>::~AVL(){
    emptyTree(root);
};

template <typename T>
void emptyTree(Node<T> *&node){
    if(node != nullptr){
        emptyTree(node->left);
        emptyTree(node->right);
        delete node;
    }
    node = nullptr;
}