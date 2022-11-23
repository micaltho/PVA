#include <iostream>
#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/concurrent_priority_queue.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

#define FILE_NAME "../USA-road-d.NY.gr"
#define START 1
#define DESTINATION 264346

struct node;

struct street{
    street(node* _destination, int _length): destination(_destination), length(_length){}
    node* destination;
    int length;
};

struct queueElement;

class node{
public:
    node():distance(INT32_MAX),successor(nullptr),done(false){}
    int id;
    std::vector<street> streets;
    int distance;
    node* successor;
    bool done;
};

class cmp{
public:
    bool operator()(node* lhs, node* rhs) {
        return lhs->distance > rhs->distance;
    }
};

void handleStreet(node* minNode, oneapi::tbb::concurrent_priority_queue<node*,cmp>& queue, street s){
    node *destination = s.destination;
    int alternative = minNode->distance + s.length;
    if (alternative < destination->distance) {

        destination->distance = alternative;
        destination->successor = minNode;

        queue.push(destination);
    }
}

enum EXEC_TYPE{
    SEQUENTIAL,
    PARALLEL
};

/*
dijkstra -- dijkstra algo for EXEC_TYPES seq and par
*/
void dijkstra(EXEC_TYPE et, oneapi::tbb::concurrent_priority_queue<node*,cmp>& queue){

    do{
        node* minNode;
        do {
            queue.try_pop(minNode);
        }while(minNode->done);

        if(minNode->id == DESTINATION){
            //break;
        }

        std::vector<street> &streetsFromMin = minNode->streets;
        switch(et) {
            case EXEC_TYPE::SEQUENTIAL:
                for (int i = 0; i < streetsFromMin.size(); i++) {
                    handleStreet(minNode, queue, streetsFromMin[i]);
                }
                break;
            case EXEC_TYPE::PARALLEL:
                oneapi::tbb::parallel_for(0, (int) streetsFromMin.size(), [&](int i) {
                    handleStreet(minNode, queue, streetsFromMin[i]);
                });
                break;
        }


        minNode->done = true;
    }while(!queue.empty());
}

//void_reader -- Reading .gr data
void void_reader(std::vector<node> &graph, std::vector<node> &graphPar){
    std::ifstream fin(FILE_NAME);
    std::string cmd;
    int u,v,w;
    while(!fin.eof()){
        fin >> cmd;
        if(cmd == "a" && (fin >> u >> v >> w)){
            graph[u].streets.push_back(street(&(graph[v]),w));
            graph[u].id = u;

            graphPar[u].streets.push_back(street(&(graphPar[v]),w));
            graphPar[u].id = u;
        }
        if(cmd == "p" && (fin >> cmd >> u >> v)){
            graph = std::vector<node>(u+1);
            graphPar = std::vector<node>(u+1);
        }
        else{
            getline(fin,cmd);
        }
    }
}

//void_writer -- Printing the results for dijkstra
void void_writer(node* i){
    std::cout << START << "-->" << DESTINATION << ": " << i->distance << " {";
    std::vector<int> path;
    for(i = i->successor; i->id != START; i = i->successor){
        path.push_back(i->id);
    }
    for(int i = path.size()-1; i > 0; i--){
        std::cout << path[i] << ", ";
    }
    std::cout << path[0] << "}" << std::endl;
}

//double_dijkstra_par -- return value is duration in us for sequentiell dijkstra
double double_dijkstra_seq(std::vector<node> &graph){
    graph[START].distance = 0;
    oneapi::tbb::concurrent_priority_queue<node*,cmp> queue;
    queue.push(&graph[START]);
    auto start = std::chrono::high_resolution_clock::now();
    dijkstra(EXEC_TYPE::SEQUENTIAL,queue);
    auto seqTime  =  std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();

    return seqTime;
}

//double_dijkstra_par -- return value is duration in us for parallel dijkstra
double double_dijkstra_par(std::vector<node> &graphPar){
    node* dummy;
    graphPar[START].distance = 0;
    oneapi::tbb::concurrent_priority_queue<node*,cmp> queue;
    queue.push(&graphPar[START]);
    auto startPar = std::chrono::high_resolution_clock::now();
    dijkstra(EXEC_TYPE::PARALLEL,queue);
    auto parTime  = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startPar).count();

    return parTime;
}

int main() {

    std::cout << "Hello, Dijkstra`s World!\n" << std::endl;
    std::vector<node> graph;
    std::vector<node> graphPar;

    std::cout << "Reading graph data from " << FILE_NAME << "... ok\n" << std::endl; 
    void_reader(graph, graphPar);

    std::cout << "Running Dijkstra`s algorithm from 1... ok\n" << std::endl;
    double seqTime=double_dijkstra_seq(graph);
    double parTime=double_dijkstra_par(graphPar);
    std::cout << "--Elapsed time--" << std::endl;
    std::cout << "SeqTime: " << seqTime << " μs" << std::endl;
    std::cout << "ParTime: " << parTime << " μs" << std::endl;

    std::cout << "Speed Up: " << ((float)seqTime/parTime) << std::endl;

    void_writer(&(graphPar[DESTINATION]));
    return 0;
}

