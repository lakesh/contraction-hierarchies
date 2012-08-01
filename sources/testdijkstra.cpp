//
//  testdijkstra.cpp
//  
//
//  Created by lakesh kansakar on 7/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <sys/time.h>


using namespace std;
using namespace boost;

inline double timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return double(tp.tv_sec) + tp.tv_usec / 1000000.;
}


int main() {
    // Read the source and target pairs for testing dijkstra
    string testDataFileName = "testDataFile.dat";
    int n=100;
    int source[n];
    int target[n];
    
    ifstream inTest(testDataFileName.c_str());
    if(inTest.is_open()) {
        int index = 0;
        string line;        
        while(getline(inTest,line)) {
            stringstream linestream(line);
            linestream >> source[index];
            linestream >> target[index];
            index++;
        }
        inTest.close();
    }
    
    // Read the graph
    string graphFileName = "minneapolis.ddsg";
    int numberOfNodes, numberOfEdges;
    int numberOfOneWays = 0;
    ifstream inGraph(graphFileName.c_str());
    
    
    //Using boost's Dijkstra algorithm (sample code obtained from http://www.boost.org/doc/libs/1_50_0/libs/graph/example/dijkstra-example.cpp
    typedef adjacency_list < listS, vecS, directedS,no_property, property < edge_weight_t, int > > graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
    typedef std::pair<int, int> Edge;

    
    if(inGraph.is_open()) {
        //Read the number of nodes and edges
        string firstLine;
        /* Execute the getline two times because the first line consisits of character 
         d followed by the number of nodes and edges at the second line
         */

        getline(inGraph,firstLine);
        getline(inGraph,firstLine);
        stringstream firstLineStream(firstLine);
        firstLineStream >> numberOfNodes;
        firstLineStream >> numberOfEdges;
        
        string line;  
        int index = 0;
        while(getline(inGraph,line)) {
            stringstream lineStream(line);
            int source,target,weight,isOneWay;
            lineStream >> source;
            lineStream >> target;
            lineStream >> weight;
            lineStream >> isOneWay;
            //count the number of one way edges
            if(isOneWay == 1) {
                numberOfOneWays++;
            } else {

            }
        }
        //inGraph.close();
    } else {
        cout << "Couldn't read the graph " << endl;
        exit(1);
    }
    
    cout << "Number of one ways " << numberOfOneWays << endl;
    
    const int num_nodes = numberOfNodes;
    Edge edge_array[numberOfEdges + numberOfEdges-numberOfOneWays];
    int weights[numberOfEdges + numberOfEdges-numberOfOneWays];
    
    inGraph.clear();
    inGraph.seekg(0, ios::beg);
    
    if(inGraph.is_open()) {
        //Read the number of nodes and edges
        string firstLine;
        /* Execute the getline two times because the first line consisits of character 
         d followed by the number of nodes and edges at the second line
         */
        
        getline(inGraph,firstLine);
        getline(inGraph,firstLine);
        stringstream firstLineStream(firstLine);
        firstLineStream >> numberOfNodes;
        firstLineStream >> numberOfEdges;
        
        string line;  
        int index = 0;
        while(getline(inGraph,line)) {
            stringstream lineStream(line);
            int source,target,weight,isOneWay;
            lineStream >> source;
            lineStream >> target;
            lineStream >> weight;
            lineStream >> isOneWay;
            cout << source << " " << target << " " << weight << " " << isOneWay << endl;
            edge_array[index] = Edge(source,target);
            weights[index] = weight;
            index++;
            //If both ways add the edge twice
            if(isOneWay == 0) {
                edge_array[index] = Edge(target,source);
                weights[index] = weight;
                index++;
            }
        }
        inGraph.close();
    } else {
        cout << "Couldn't read the graph " << endl;
        exit(1);
    }
    
    
    int num_arcs = sizeof(edge_array) / sizeof(Edge);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
    graph_t g(num_nodes);
    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
    for (std::size_t j = 0; j < num_arcs; ++j) {
        edge_descriptor e; bool inserted;
        boost::tie(e, inserted) = add_edge(edge_array[j].first, edge_array[j].second, g);
        weightmap[e] = weights[j];
    }
#else
    graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
#endif
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));
    
    double timeTaken;
    timeTaken = timestamp();
    
    for(int i=0; i<n; i++) {
        vertex_descriptor s = vertex(source[i], g);
        //cout << source[i] << endl;
    
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
        // VC++ has trouble with the named parameters mechanism
        property_map<graph_t, vertex_index_t>::type indexmap = get(vertex_index, g);
        dijkstra_shortest_paths(g, s, &p[0], &d[0], weightmap, indexmap, 
                            std::less<int>(), closed_plus<int>(), 
                            (std::numeric_limits<int>::max)(), 0,
                            default_dijkstra_visitor());
#else
        dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
#endif
        /*if(i == n-1) { 
        std::cout << "distances and parents:" << std::endl;
        graph_traits < graph_t >::vertex_iterator vi, vend;
        for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
            std::cout << "distance(" << *vi << ") = " << d[*vi] << ", ";
            std::cout << "parent(" << *vi << ") = " << p[*vi] << std::
            endl;
        }
        std::cout << std::endl;
        }*/
    }
    timeTaken = timestamp() - timeTaken;
    cout << "Total time taken " << timeTaken << endl;

}
