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
#include <boost/graph/astar_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/random.hpp>
#include <boost/graph/graphviz.hpp>
#include <sys/time.h>
#include <vector>
#include <list>
#include <math.h>  
#include <sys/time.h>


using namespace std;
using namespace boost;

inline double timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return double(tp.tv_sec) + tp.tv_usec / 1000000.;
}

// auxiliary types
struct location
{
    float y, x; // lat, long
};
typedef float cost;

template <class Name, class LocMap>
class city_writer {
public:
    city_writer(Name n, LocMap l, float _minx, float _maxx,
                float _miny, float _maxy,
                unsigned int _ptx, unsigned int _pty)
    : name(n), loc(l), minx(_minx), maxx(_maxx), miny(_miny),
    maxy(_maxy), ptx(_ptx), pty(_pty) {}
    template <class Vertex>
    void operator()(ostream& out, const Vertex& v) const {
        float px = 1 - (loc[v].x - minx) / (maxx - minx);
        float py = (loc[v].y - miny) / (maxy - miny);
        out << "[label=\"" << name[v] << "\", pos=\""
        << static_cast<unsigned int>(ptx * px) << ","
        << static_cast<unsigned int>(pty * py)
        << "\", fontsize=\"11\"]";
    }
private:
    Name name;
    LocMap loc;
    float minx, maxx, miny, maxy;
    unsigned int ptx, pty;
};

template <class WeightMap>
class time_writer {
public:
    time_writer(WeightMap w) : wm(w) {}
    template <class Edge>
    void operator()(ostream &out, const Edge& e) const {
        out << "[label=\"" << wm[e] << "\", fontsize=\"11\"]";
    }
private:
    WeightMap wm;
};


// euclidean distance heuristic
template <class Graph, class CostType, class LocMap>
class distance_heuristic : public astar_heuristic<Graph, CostType>
{
public:
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    distance_heuristic(LocMap l, Vertex goal)
    : m_location(l), m_goal(goal) {}
    CostType operator()(Vertex u)
    {
        CostType dx = m_location[m_goal].x - m_location[u].x;
        CostType dy = m_location[m_goal].y - m_location[u].y;
        return ::sqrt(dx * dx + dy * dy);
    }
private:
    LocMap m_location;
    Vertex m_goal;
};


struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
public:
    astar_goal_visitor(Vertex goal) : m_goal(goal) {}
    template <class Graph>
    void examine_vertex(Vertex u, Graph& g) {
        if(u == m_goal)
            throw found_goal();
    }
private:
    Vertex m_goal;
};


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
    
    
    //Using boost's A star algorithm (sample code obtained from http://www.boost.org/doc/libs/1_38_0/libs/graph/example/astar-cities.cpp
    typedef adjacency_list<listS, vecS, directedS, no_property,
    property<edge_weight_t, cost> > mygraph_t;
    typedef property_map<mygraph_t, edge_weight_t>::type WeightMap;
    typedef mygraph_t::vertex_descriptor vertex;
    typedef mygraph_t::edge_descriptor edge_descriptor;
    typedef mygraph_t::vertex_iterator vertex_iterator;
    typedef std::pair<int, int> edge;
    
    
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
    edge edge_array[numberOfEdges + numberOfEdges-numberOfOneWays];
    cost weights[numberOfEdges + numberOfEdges-numberOfOneWays];
    
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
            edge_array[index] = edge(source,target);
            weights[index] = weight;
            index++;
            //If both ways add the edge twice
            if(isOneWay == 0) {
                edge_array[index] = edge(target,source);
                weights[index] = weight;
                index++;
            }
        }
        inGraph.close();
    } else {
        cout << "Couldn't read the graph " << endl;
        exit(1);
    }
    
    
    //read the locations
    string locationFileName = "location_processed.dat";
    ifstream locationGraph(locationFileName.c_str());
    location locations[numberOfNodes];
    
    if(locationGraph.is_open()) {
        string line;
        int index = 0;
        while(getline(locationGraph,line)) {
            stringstream lineStream(line);
            double id,lat,lon;
            lineStream >> id;
            lineStream >> lat;
            lineStream >> lon;
            cout << id << " " << lat << " " << lon << endl;
            locations[index].x = lat;
            locations[index].y = lon;
            index++;
        }
    } else {
        cout << "Couldn't open the location file" << endl;
        exit(1);
    }
    
    // create graph
    
    unsigned int num_edges = sizeof(edge_array) / sizeof(edge);
    
    mygraph_t g(numberOfNodes);
    WeightMap weightmap = get(edge_weight, g);
    for(std::size_t j = 0; j < num_edges; ++j) {
        edge_descriptor e; bool inserted;
        tie(e, inserted) = add_edge(edge_array[j].first,
                                    edge_array[j].second, g);
        weightmap[e] = weights[j];
    }

    
    vector<mygraph_t::vertex_descriptor> p(num_vertices(g));
    vector<cost> d(num_vertices(g));
    double timeTaken;
    timeTaken = timestamp();
    
    for(int i=0; i<n; i++) {
        vertex start = source[i];
        vertex goal = target[i];
        
        //cout << source[i] << " " << target[i] << endl;
        
        //cout << "Start vertex: " << start << endl;
        //cout << "Goal vertex: " << goal << endl;

        try {
            // call astar named parameter interface
            astar_search
            (g, start,
             distance_heuristic<mygraph_t, cost, location*>
             (locations, goal),
             predecessor_map(&p[0]).distance_map(&d[0]).
             visitor(astar_goal_visitor<vertex>(goal)));
            
            
        } catch(found_goal fg) { // found a path to the goal
            list<vertex> shortest_path;
            for(vertex v = goal;; v = p[v]) {
                shortest_path.push_front(v);
                if(p[v] == v)
                    break;
            }
            cout << "Shortest path from " << start << " to "
            << goal << ": ";
            list<vertex>::iterator spi = shortest_path.begin();
            cout << start;
            for(++spi; spi != shortest_path.end(); ++spi)
                cout << " -> " << *spi;
            cout << endl << "Total travel time: " << d[goal] << endl;
             
        }
        
        //cout << "Didn't find a path from " << start << "to"
        //<< goal << "!" << endl;
    }
    timeTaken = timestamp() - timeTaken;
    cout << "Total time taken " << timeTaken << endl;
    
}
