#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class Node {
    public:
        Node(double data):
            // COLOR(if not red), LEFT, RIGHT, and PARENT NEED TO BE ASSIGNED
            data(data), height(1), color('R'), left(NULL), right(NULL), parent(NULL){}
        int updateHeight(){
            int l = (left == NULL ? 0 : left->height);
            int r = (right == NULL ? 0 : right->height);
            height = 1+(l > r ? l : r);
            return height;
        }
        double data;
        int height;
        char color;
        Node *left, *right, *parent;
};

class RedBlack {
    public:
        RedBlack(): root(NULL) {};

        void insert(double data){
            //insert for root
            Node *newNode = new Node(data);
            if (root == NULL) {
                root = newNode;
                root->color = 'B';
            } else {
                //after insertion, update the color of the new node
                insert(root, newNode);
                updateColor(newNode);
            }
        };

        void insert(Node *parentNode, Node *newNode){
            //if the data is smaller than the parent node, go left
            if(newNode->data < parentNode->data){
                if(parentNode->left == NULL){
                    parentNode->left = newNode;
                    newNode->parent = parentNode;
                } else {
                    insert(parentNode->left, newNode);
                }
            } else {
                //if the data is larger or equal to the parent node, go right
                if(parentNode->right == NULL){
                    parentNode->right = newNode;
                    newNode->parent = parentNode;
                } else {
                    insert(parentNode->right, newNode);
                }
            }
            //update parent's height because it changed due to a child
            parentNode->updateHeight();
        }

        void updateColor(Node *node){
            //case 1 to check to make sure the root is always black
            if(node == root){
                node->color = 'B';
                return;
            }
            //checks if the parent is black, nothign to be done
            if(node->parent->color == 'B'){
                return;
            }

            Node *P = node->parent;
            Node *GP = P->parent;
            Node *uncle = (GP->left == P ? GP->right : GP->left);

            //case 2 to swap colors, if parent and uncle are both red. 
            if(uncle != NULL && uncle->color == 'R'){
                P->color = 'B';
                uncle->color = 'B';
                GP->color = 'R';
                //recurisvley check up the tree.
                updateColor(GP);
            } else {
                //rotation cases 
                //can use just RR and LL because red black doesnt care about height tracking so consecutive uses works
                if(GP->left == P){
                    //LR or LL case.
                    if(P->right == node){
                        RR(P);
                        node->color = 'B';
                    } else {
                        P->color = 'B';
                    }
                    LL(GP);
                    //swap colors after rotation
                    GP->color = 'R';
                } else {
                    //RL or RR case
                    if(P->left == node){
                        LL(P);
                        node->color = 'B';
                    
                    } else {
                        P->color = 'B';
                        
                    }
                    RR(GP);
                    //swap colors after rotation
                    GP->color = 'R';
                }
            }
        }

        //rotations


        //rr rotation 
        Node* RR(Node *node){
            Node *newT = node->right;
            node->right = newT->left;
            if(newT->left != NULL){
                newT->left->parent = node;
            }

            newT->parent = node->parent;
            if(node->parent == NULL){
                this->root = newT;
            } else if (node == node->parent->left){
                node->parent->left = newT;
            } else {
                node->parent->right = newT;
            }

            newT->left = node;
            node->parent = newT;

            node->updateHeight();
            newT->updateHeight();
            
            return newT;
        }

        //ll rotation
        Node* LL(Node *node){
            Node *newT = node->left;

            node->left = newT->right;
            if(newT->right != NULL){
                newT->right->parent = node;
            }

            newT->parent = node->parent;
            if(node->parent == NULL){
                this->root = newT;
            } else if(node == node->parent->right){
                node->parent->right = newT;
            } else {
                node->parent->left = newT;
            }

            newT->right = node;
            node->parent = newT;

            node->updateHeight();
            newT->updateHeight();

            return newT;
        }

        int size(){
            int tSize = size(root);
            //cout<<"size finished"<<endl;
            return tSize;
        };

        int size(Node *node){
            if(node == NULL){
                return 0;
            }
            return 1 + size(node->left) + size(node->right);

        };

        int height(){
            if(root == NULL){
                return 0;
            }
            return height(root);
        };

        int height(Node *node){
            if(node == NULL){
                return 0;
            }
            int lh = height(node->left), rh = height(node->right);
            return 1 + (lh > rh ? lh : rh);
        };


        double max(){
            if(root == NULL)
                return 0;
            return max(root)->data;
        };

        Node *max(Node *node){
            if(node == NULL) 
                return NULL;
            if(node->right == NULL)
                return node;
            return max(node->right);
        };

        double min(){
            if(root == NULL)
                return 0;
            return min(root)->data;
        };

        Node *min(Node *node){
            if(node == NULL)
                return NULL;
            if(node->left == NULL)
                return node;
            return min(node->left);
        };

        //Dr Li's Inorder
        double* inorder(){
            double *a;
            a = new double[size()];
            int i = 0;
            inorder(root, a, i);
            return a;
        };

        void inorder(Node *node, double*a, int &i){
            if(node == NULL)
                return;
            inorder(node->left,a,i);
            a[i++] = node->data;
            inorder(node->right,a,i);
        };

        ~RedBlack(){
            emptyTree(root);
        };

        void emptyTree(Node *&node){
            if(node != nullptr){
                emptyTree(node->left);
                emptyTree(node->right);
                delete node;
            }
            node = nullptr;
        };

        void dfsTraversal(){
            vector<Node*> v;
            dfsTraversal(root, v);
        }

        void dfsTraversal(Node* node, vector<Node*> &v){
            //add seen node to vector
            v.push_back(node);
            //if node is a leaf node
            if(node->left == NULL && node->right == NULL){
                //count the black nodes in the path
                int bCount = 0;
                for(int i = 0; i != v.size(); i++){
                    if(v.at(i)->color == 'B'){
                        bCount++;
                    }
                }
                cout<< "(bn:"<<bCount<<") ";

                //show the colors in the path
                for(int i = 0; i != v.size(); i++){
                    cout<< v.at(i)->color << " ";
                    if(i == v.size() - 1){
                        cout << "(" << v.at(i)->data << ")"<<endl;
                    }
                }
                //remove the leaf node
                v.pop_back();
            } else if(node->left != NULL && node->right == NULL){
                //if left exists and right doesnt, go left
                dfsTraversal(node->left, v);
                v.pop_back();
            } else if(node->left == NULL && node->right != NULL){
                //if right exists and left doesnt, go right
                dfsTraversal(node->right, v);
                v.pop_back();
            } else {
                //if they both exist
                dfsTraversal(node->left,v);
                dfsTraversal(node->right,v);
                v.pop_back();
            }


        }
    
    private:
        Node *root;
};

//Dr. Li's Print Array
void printArray(double a[], int n, int size) {
	cout << " [";
	for (int i=0; i< size && i < n; i++) {
			cout << *(a+i) << " ";
	}
	if (n >= size)
		cout << "]";
	else
		cout <<  ".........";
};

int main(int argc, char* argv[]){

    if(argv[1] != NULL){
        string fileName = argv[1];
        string dataToInsert = "";
        double data = 0.0;

        RedBlack* tree = new RedBlack();

        ifstream file(fileName);
        while(getline(file, dataToInsert)){
            stringstream ss(dataToInsert);
            ss >> data;
            tree->insert(data);
            //cout<<"insert successful"<<endl;
            
        }
        file.close();
        //cout<<"file closed, tree creation successful"<<endl;

        int treeSize = tree->size();
        int treeHeight = tree->height();

        cout<<"File Name: "<< fileName <<endl;
        cout<<"Red-Black Tree: Size ="<< treeSize << " Height=" << treeHeight <<endl;

        double* a = tree->inorder();
        printArray(a, treeSize, treeSize);
        delete[] a;
        a = nullptr;
    
        cout<<"\n";
        tree->dfsTraversal();

        delete tree;
        tree = nullptr;

    } else {
        cout<< "invalid file name"<<endl;
    }


    return 0;
}

