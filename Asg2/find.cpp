#include <iostream>
#include <list>
#include <fstream>
using namespace std;

class dirNode {
    public:
        dirNode(string path, string name);
        void addChild(dirNode *newChild);
        dirNode *findNode(string pathname);
        void print(int level);
        ~dirNode();

        string path;
        string name;
        dirNode *firstChild;
        dirNode *nextSibling;
};

dirNode::dirNode(string path = "NULL", string name = "NULL"){
    this->path = path;
    this->name = name;
    firstChild = nullptr;
    nextSibling = nullptr;
}

void dirNode::addChild(dirNode *newChild){
    //This will make sure that newChild has a value before we assign it
    //and it will check that we are not adding the newChild to itself. 
    if(newChild == this || newChild == NULL){
        return;
    }

    //We can also check whether or not the child is already in the list.
    // curr will be assigned to the firstChild and go through the list to check the siblings until NULL
    for(dirNode* curr = firstChild; curr !=NULL; curr = curr->nextSibling){
        if(curr == newChild){
            return;
        }
    }

    newChild->nextSibling = firstChild;
    this->firstChild = newChild;
}

void dirNode::print(int level){
    for(int i = 0; i < level; i++){
        cout<< "\t"<<endl;
    }
    cout << name <<endl;

    dirNode* child = firstChild;
    while(child != NULL){
        child->print(level + 1);
        child = child->nextSibling;
    }
};

class dirTokenizer {
    public:
        dirTokenizer(string path);
        list<string> tokens;
        list<string>:: iterator it;
        string next();
};

dirTokenizer::dirTokenizer(string path){
    //build a list based off of the words provided in the path string, delimited by /
    string p = path; 
    string currToken;

    
    //first i need to check if there is a / at the start 
    //if so i need to remove it.
    //if the first character is a /, then p will be same string with that removed.
    if(p[0] == '/'){
        tokens.push_back("/");
        p = p.substr(1);
    }

    //the loop will run until there are no /'s left in the string.
    while(p.find('/') != string::npos){
        //the current token will be a substring of the path until the /
        currToken = p.substr(0,p.find('/'));
        //p will then be the rest of the string from that / onwards.
        p = p.substr(p.find('/')+1);
        //add currToken to the end of the list.
        tokens.push_back(currToken);
    }
    //in the case of a directory, the line ends with a :
    if(p.back() == ':'){
        currToken = p.substr(0, p.length() - 1);
        tokens.push_back(currToken);
    } else {
        //if it doesnt end in :, push the rest of the string.
        if(!p.empty()){
            tokens.push_back(p);
        }
    }
    //create an iterator pointed to the first string in the list.
    it = tokens.begin();
}

string dirTokenizer::next(){
    //check if the iterator is at the end
    if(it == tokens.end()){
        return "";
    }
    //otherwise we will return the current token and increment
    //cout<< *it <<endl;
    return *it++;
}

//the non-recursive approach to findNode provided by Dr. Li
dirNode *dirNode::findNode(string pathname){
    dirTokenizer path = dirTokenizer(pathname);
    //cout<< "pathname: " << pathname <<endl;
    string dir = path.next();
    //cout<<"next called, new dir: " << dir<<endl;
    dirNode *cur = this;
    if(cur->name != dir) return NULL;
    dir = path.next();
    //cout<<"next called 2, new dir: " << dir<<endl;
    if(dir == "") return cur;
    cur = cur->firstChild;
    while (cur != NULL){
        if(cur->name != dir){
            cur = cur->nextSibling;
            continue;
        }
        dir = path.next();
        //cout<<"next called 3, new dir: " << dir<<endl;
        if(dir == "") return cur;
        else cur = cur->firstChild;
    }
    return cur;
}

dirNode::~dirNode(){
    dirNode* current = firstChild;
    while(current != NULL){
        dirNode* next = current->nextSibling;
        delete current;
        current = next;
    }
}

int main(int argc, char* argv[]){
    string desiredPath = argv[1];
    //what do i need to sucessfully build this tree. 
    //1. for each node i need the directory path to it and its name from that path.
    //     a. the path to any node can be built using the most recent parent directory path plus the current Word
    //2. i need a reference to the previous node created, two cases: 
    //     a. a previous child of the same directory
    //     b. or the parent directory
    ifstream file("LinuxDir.txt");
    //currWord will store each line from the text file as it is read in.
    string currWord;
    //currentParentDirectory will store each line that ends in : as it comes in.
    string currentParentDirectory;

    //a root node for the tree.
    dirNode rootNode = dirNode("/", "/");

    //What do i need to do for every line...
    //correct loop header while(file >> currWord )
    //testing loop

    for(int i = 0; i < 4476; i++){
        //takes a new line from the LinuxDir.txt file
        file >> currWord;

        //to create a directory/file from this line we need to get the path and the name.

        //in the case that the is a directory (ends in :)
        //example: '/home/:'
        //         '/home/ad.ilstu.edu:'
        //         '/home/ad.ilstu.edu/aahern2:'

        if(!currWord.empty() && currWord.back() == ':'){
            currentParentDirectory = currWord.substr(0, currWord.length() - 1);
            //in the case of '/home/:' , currentParentDirectory = "/home/"
            //in the case of '/home/ad.ilstu.edu:' , currentParentDirectory = "/home/ad.ilstu.edu"

            //the home case, check if there is an extra / at the end of the string
            if(currentParentDirectory.back() == '/'){
                //remove that slash
                currentParentDirectory = currentParentDirectory.substr(0, currentParentDirectory.length() - 1);
                //remove the front slash because we will build from rootNode.
                string currName = currentParentDirectory.substr(1);
                //Should just be 'home' now.

                //ideally we should not need to check in this case as /home should not exist before we create it.
                //creates the node
                dirNode* dN = new dirNode(currentParentDirectory, currName);
                //adds the node.
                rootNode.addChild(dN);
            }

            //cases other than home...
            if(rootNode.findNode(currentParentDirectory) == NULL){
                //find the directory name
                string currName = currentParentDirectory.substr(currentParentDirectory.find_last_of('/') + 1);
                dirNode* dN = new dirNode(currentParentDirectory, currName);
                string parentNodeDir = currentParentDirectory.substr(0, currentParentDirectory.find_last_of('/'));


                dirNode* parentNode = rootNode.findNode(parentNodeDir);
                parentNode->addChild(dN);
            }
        }else {
            //for ' files '
            string currDirectory = currentParentDirectory + "/" + currWord;
            //check if it already exists
            if(rootNode.findNode(currDirectory) == NULL){
                //creates the new 'file' node
                dirNode* dN = new dirNode(currDirectory, currWord);
                dirNode* parentNode = rootNode.findNode(currentParentDirectory);
                if(parentNode != NULL){
                    parentNode->addChild(dN);
                }
                
            }            
        }
    }

    dirNode* result = rootNode.findNode("/home/ad.ilstu.edu/" + desiredPath);
    if(result != NULL){
        cout << result->path << "/"<< endl;
        result->print(0);
    } else {
        cout<< "Directory not found"<<endl;
    }

}