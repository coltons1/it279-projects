#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

#define INT32_MAX (2147483647)
using namespace std;

//---------------------X+X----------------------//
//             Dijkstra's Algorithm             //
//                Colton Stanek                 //
//                17 April 2026                 //
//---------------------\+/----------------------//

/* - Read / Import Graph files.
/  - Build Djikstra's Algorithm based off of Class 24 notes.*/
//-------------------------------------------------------------------------------------------//

struct Edge {
    Edge(int s = -1, int t = -1, double w = 0.0){                           // edge constructor, sets source, target, and weight to nothing.
        sourceIdx = s;
        targetIdx = t;
        weight = w;
    }
    int sourceIdx;                                                          // source vertex of the edge
    int targetIdx;                                                          // target vertex of the edge
    double weight;                                                          // weight of the edge
};

struct Vertex {
    Vertex(int id = -1, bool m = false, double pW = 0.0){   // vertex constructor, sets id, marked, and pathWeight to nothing.
        this->id = id;
        marked = m;
        pathWeight = pW;
    }
    int id;                                                                 // id to track the vertex
    bool marked;                                                            // bool to track which vertices will have been added to path
    double pathWeight;                                                      // total path weight.
    bool addedToQueue = false;                                              // bool to track whether vertex has been added to queue or not.
};

struct CompareVertex{
    bool operator()(Vertex *a, Vertex *b){
        return a->pathWeight > b->pathWeight;
    }
};

void dijkstra(string graph, vector<vector<Edge*>> &adjList, int V){
    // create two arrays cost and prev of size |V| with init values of int32 max and -1.
    int *cost = new int[V]{INT32_MAX}; // should be of size |V|
    int *prev = new int[V]{-1};        // should be of size |V|

    for(int i = 0; i < V; i++){        // initializng all values in both arrays to 'null' values. 
        cost[i] = INT32_MAX;
        prev[i] = -1;
    }

    // list to track existing vertices.
    Vertex **existingVertices = new Vertex*[V]();

    // also create a priority queue Q.
    priority_queue<Vertex*, vector<Vertex*>, CompareVertex> Q;

    // set cost[source vertex] = 0 and a vertex v = to source (s). Mark s. 
    //create a vertex v and set it equal to the source, vertex of index 0.
    Vertex *v = new Vertex(0, true);
    existingVertices[v->id] = v;

    cost[v->id] = 0;

    Vertex *vT = new Vertex(V-1);
    existingVertices[vT->id] = vT;

    while(existingVertices[vT->id]->marked != true){

        // step a
        // this loop is looking at all the vertices that our source connects to.
        for(int i = 0; i < adjList[v->id].size(); i++){
            // create a temp vertex object to store the newly discovered vertices.
            Vertex *tempVert;
            Edge *currEdge = adjList[v->id].at(i);

            if(adjList[v->id].at(i) == nullptr){
                continue;
            }

            if(currEdge != nullptr  && existingVertices[currEdge->targetIdx] == nullptr){
                // create a new vertex of the target vertex of the edge.  
                tempVert = new Vertex(currEdge->targetIdx, false, currEdge->weight);
                existingVertices[tempVert->id] = tempVert;

            } else if(currEdge != nullptr) {
                // get the previously created vertex.
                tempVert = existingVertices[currEdge->targetIdx];
            } else {
                continue;
            }

            if((cost[v->id] + currEdge->weight) < cost[tempVert->id] ){
                // set cost tempvert = that if'd cost
                cost[tempVert->id] = cost[v->id] + currEdge->weight;
                // set prev tempVert to v
                prev[tempVert->id] = v->id;

                //step b
                tempVert->pathWeight = cost[tempVert->id];
                Q.push(tempVert);
            }
        }
        
        //step c
        Vertex *nextV = nullptr;
        while (!Q.empty()){
            Vertex* candidate = Q.top();
            Q.pop();
            if(candidate->marked == false && candidate->pathWeight == cost[candidate->id]){
                nextV = candidate;
                break;
            }
            if(Q.empty()){
                nextV = nullptr;
                break;
            }
        }
        if(nextV == nullptr){
            break;
        }
        // mark the vertex and continue
        nextV->marked = true;
        v = nextV;
    }
    // POST DIJKSTRAS
    cout<<graph<<"'s shortest path from 0 to "<<V-1<<": "<<endl; 

    // want to trace back through the prev array from the target vertex.
    int prevVertex = prev[V-1];
    if(prevVertex == -1){
        cout<<"*** There is no path.\n"<<endl;
    } else {
        vector<int> paths;
        // last index
        paths.push_back(V-1);
        // if there is a path, we need to get the path from prev.
        while(prevVertex != 0){
            paths.push_back(prevVertex);
            prevVertex = prev[prevVertex];
        }
        paths.push_back(0);

        // paths now contains the full path backwards.
        float totalWeight = 0;

        for(int i = paths.size() - 1; i > 0; i--){
            // need to find the edge from paths[i] to paths[i-1]
            Edge *e;
            for(int j = 0; j < adjList[paths[i]].size(); j++){
                if(adjList[paths[i]].at(j)->targetIdx == paths[i-1]){
                    e = adjList[paths[i]].at(j);
                    break;
                } 
            }
            totalWeight += e->weight;
            cout<< "\t(" << e->sourceIdx << ", "<< e->targetIdx << ", " << e->weight << ") ----> " << totalWeight <<endl;
        }
        cout<<"\n"<<endl;
    }
    delete[] cost;
    cost = nullptr;
    delete[] prev;
    prev = nullptr;

    for(int i = 0; i < V; i++){
        delete existingVertices[i];
        existingVertices[i] = nullptr;

    }
    delete[] existingVertices;
    existingVertices = nullptr;

}


int main(int argc, char* argv[]){
    ifstream file(argv[1]);                                            
    string line;
    int V;                                                                  // amount of vertices in the graph;
    string graph;
    vector<vector<Edge*>> edges(10);                                        // to be resized when V is discovered.
    bool parsingEdges = false;

    cout<<"\nShortest Paths from vertex 0 to vertex n-1 in "<< argv[1] << ", |V| = n\n"<<endl;

    while(getline(file, line)){
        if(parsingEdges == false){                                          // branch to capture the amnt of vertices in the graph
            if(line.find("** G") != string::npos){
                graph = line.substr(line.find("G"),line.find("G"));
            }
            if(line.find("|V|=") != string::npos){
                V = stoi(line.substr(line.find('=') + 1));
                edges.resize(V);
                parsingEdges = true;                                        // set parsing bool to true
            }
        } else {
            string tempSource = "";                                         // creates the temporary variables that store info from stringstream
            string tempTarget = "";
            double tempWeight = 0.0;

            if(line.find("-") != string::npos){
                // call dijkstras here and reset variables
                dijkstra(graph, edges, V);
                tempSource = "";
                tempTarget = "";
                tempWeight = 0.0;

                for(int i = 0; i < edges.size(); i++){
                    for(int j = 0; j < edges[i].size(); j++){
                        delete edges[i].at(j);
                        edges[i].at(j) = nullptr;
                    }
                }
                // empty the vectors
                for(int i = 0; i < edges.size(); i++){
                    edges[i].clear();
                }
                parsingEdges = false;
                continue;

            } else if(line.find('(') == string::npos){                          // ignores lines that arent vertices.
                continue;
            }

            for(char& c: line){                                                 // prepares the values for the string stream
                if(c == '(' || c == ')' || c == ',')
                    c = ' ';
            }

            stringstream ss(line);
            if(ss >> tempSource >> tempTarget >> tempWeight){
                Edge* edge = new Edge(stoi(tempSource), stoi(tempTarget), tempWeight);
                edges[edge->sourceIdx].push_back(edge);
                
            }
        }
    }
    return 0;
}
