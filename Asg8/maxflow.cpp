#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <list>

using namespace std;

//---------------------X+X----------------------//
//          Ford-Fulkerson's Algorithm          //
//                Colton Stanek                 //
//                30 April 2026                 //
//---------------------\+/----------------------//

bool dfs(vector<vector<double>> &graph, vector<bool> &visited, vector<int> &parent, int source, int target){
    vector<int> s;
    s.push_back(source); // source vertex

    while(!s.empty()){
        int u = s.back();
        s.pop_back();
        if(u == target){
            return true;
            //stop
        } else if(!visited[u]){
            visited[u] = true;
            //mark u as visited
            for(int v = graph[u].size() - 1; v >= 0; v--){
                if(graph[u][v] != 0 && (!visited[v])){
                    s.push_back(v);
                    parent[v] = u;
                }
            }
            //iterate through uth row of matrix from v-1 to 0
            //for each v where matrix u v is not 0 and v is not visited push v onto stack, record parent v = u
        }

    }
    return false;

}

void printGraph(double graphMax, vector<pair<double,list<int>*>> &pathsTraveled){
    for(int i = 0; i < pathsTraveled.size(); i++){
        cout<<"Path "<< i+1 << " = " << pathsTraveled[i].first << " => [";

        while(!pathsTraveled[i].second->empty()){
            // need to check, on last element in list do not put a , after.
            cout<< pathsTraveled[i].second->front();
            pathsTraveled[i].second->pop_front();
            if(!pathsTraveled[i].second->empty()){
                cout<<", ";
            }
        }
        cout<<"] "<<endl;

    }
    cout<<"---------------"<<endl;
    cout<<"Total Flow = "<<graphMax<<"\n"<<endl; 
};

// return a pair of the max flow (double) and a vector of the path, but need to track all paths.
// a vector of pairs of max flow and vector path.

double FordFulkerson(vector<vector<double>> &graph, int s, int t, vector<pair<double,list<int>*>> &pathsTraveled){       // s and t should be the source and targets, 0 and v-1
    int V = graph.size();

    vector<vector<double>> augGraph;

    augGraph.resize(V);
    for(int i = 0; i < V; i++){
        augGraph[i].resize(V);
    }

    for(int i = 0; i < V; i++){
        for(int j = 0; j < V; j++){
            augGraph[i][j] = graph[i][j];
        }
    }
    
    double maxFlow = 0;
    vector<bool> visited(V, false);
    vector<int> parent(V, -1);

    
    while(dfs(augGraph, visited, parent, s, t)){
        double pathFlow = __DBL_MAX__;
        //list for constant time addition to the front.
        list<int> *path = new list<int>();
        pair<double,list<int>*> p = {0.0, path};

        // add path p into G max and update augGraph
        // path is recovered by traversing backwards through the parent array. 

        // need to get the path from parent, find bottleneck from that path.
        for(int v = t; v != s; v = parent[v]){
            int u = parent[v];
            pathFlow = (augGraph[u][v] < pathFlow ? augGraph[u][v] : pathFlow);

            // add the curr vertex to a list for later printing.
            path->push_front(v);
        }
        path->push_front(s);

        for(int v = t; v != s; v = parent[v]){
            int u = parent[v];
            augGraph[u][v] -= pathFlow;
            augGraph[v][u] += pathFlow;
        }

        p.first = pathFlow;
        maxFlow += pathFlow;

        pathsTraveled.push_back(p);

        // this will happen for the next path.
        visited.assign(V, false);
        parent.assign(V, -1);
    }
    // when loop breaks, no more possible paths, output g max.
    return maxFlow;
};

int main(int argc, char* argv[]){
    ifstream file(argv[1]);
    string line;
    string graphTitle;
    bool parsingGraph = false;
    int V;
    
    vector<vector<double>> adjMatrix;
    

    while(getline(file,line)){
        if(parsingGraph == false){
            if(line.find("** G") != string::npos){
                graphTitle = line.substr(line.find("*"), line.find(','));
                cout<<graphTitle<<endl;
            }
            if(line.find("|V|=") != string::npos){
                V = stoi(line.substr(line.find('=')+1));
                parsingGraph = true;

                adjMatrix.assign(V, vector<double>(10));
                for(int i = 0; i < V; i++){
                    adjMatrix[i].assign(V, 0);          // need them to be 0 initially, each edge in an empty flow network should have zero flow.
                }
            }
        } else {
            string tempSource = "";
            string tempTarget = "";
            double tempWeight = 0.0;

            if(line.find("-") != string::npos){
                // this is where a graph starts.
                vector<pair<double,list<int>*>> pathsTraveled;

                double graphMax = FordFulkerson(adjMatrix, 0, V-1, pathsTraveled);

                // print here
                printGraph(graphMax, pathsTraveled);

                parsingGraph = false;

                for(int i = 0; i < pathsTraveled.size(); i++){
                    delete pathsTraveled[i].second;
                    pathsTraveled[i].second = nullptr;
                }

                pathsTraveled.clear();
                continue;
            } else if(line.find('(') == string::npos){
                continue;
            }

            for(char& c: line){
                if(c == '(' || c == ')' || c == ',')
                    c = ' ';
            }

            stringstream ss(line);
            if(ss >> tempSource >> tempTarget >> tempWeight){
                // add that 'edge' to the matrix.
                adjMatrix[stoi(tempSource)][stoi(tempTarget)] = tempWeight;
            }
        }
    }
    return 0;
};