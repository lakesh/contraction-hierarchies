/* Copyright (c) 2012 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Dijkstra's_algorithm_(C_Plus_Plus)?action=history&offset=20120126104057

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Dijkstra's_algorithm_(C_Plus_Plus)?oldid=17819
*/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <limits> // for numeric_limits
#include <set>
#include <utility> // for pair

typedef int vertex_t;
typedef double weight_t;

struct edge {
    const vertex_t target;
    const weight_t weight;
    edge(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};

typedef std::map<vertex_t, std::list<edge> > adjacency_map_t;


void DijkstraComputePaths(vertex_t source,
                          const adjacency_map_t &adjacency_map,
                          std::map<vertex_t, weight_t> &min_distance,
                          std::map<vertex_t, vertex_t> &previous)
{
    for (adjacency_map_t::const_iterator vertex_iter = adjacency_map.begin();
         vertex_iter != adjacency_map.end();
         vertex_iter++)
    {
        vertex_t v = vertex_iter->first;
        min_distance[v] = std::numeric_limits< double >::infinity();
        for (std::list<edge>::const_iterator edge_iter = vertex_iter->second.begin();
             edge_iter != vertex_iter->second.end();
             edge_iter++)
        {
            vertex_t v2 = edge_iter->target;
            min_distance[v2] = std::numeric_limits< double >::infinity();
        }
    }
    min_distance[source] = 0;
    std::set< std::pair<weight_t, vertex_t> > vertex_queue;
    vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty()) 
    {
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());


        // Visit each edge exiting u
        const std::list<edge> &edges = adjacency_map.find(u)->second;
        for (std::list<edge>::const_iterator edge_iter = edges.begin();
             edge_iter != edges.end();
             edge_iter++)
        {
            vertex_t v = edge_iter->target;
            weight_t weight = edge_iter->weight;
            weight_t distance_through_u = min_distance[u] + weight;
	    if (distance_through_u < min_distance[v]) {
	        vertex_queue.erase(std::make_pair(min_distance[v], v));

	        min_distance[v] = distance_through_u;
	        previous[v] = u;
	        vertex_queue.insert(std::make_pair(min_distance[v], v));
	    }
        }
    }
}

std::list<vertex_t> DijkstraGetShortestPathTo(
    vertex_t target, const std::map<vertex_t, vertex_t> &previous)
{
    std::list<vertex_t> path;
    std::map<vertex_t, vertex_t>::const_iterator prev;
    vertex_t vertex = target;
    path.push_front(vertex);
    while((prev = previous.find(vertex)) != previous.end())
    {
        vertex = prev->second;
        path.push_front(vertex);
    }
    return path;
}

int main()
{
    adjacency_map_t adjacency_map;
    std::vector<std::string> vertex_names;

    vertex_names.push_back("Harrisburg");   // 0
    vertex_names.push_back("Baltimore");    // 1
    vertex_names.push_back("Washington");   // 2
    vertex_names.push_back("Philadelphia"); // 3
    vertex_names.push_back("Binghamton");   // 4
    vertex_names.push_back("Allentown");    // 5
    vertex_names.push_back("New York");     // 6
    adjacency_map[0].push_back(edge(1,  79.83));
    adjacency_map[0].push_back(edge(5,  81.15));
    adjacency_map[1].push_back(edge(0,  79.75));
    adjacency_map[1].push_back(edge(2,  39.42));
    adjacency_map[1].push_back(edge(3, 103.00));
    adjacency_map[2].push_back(edge(1,  38.65));
    adjacency_map[3].push_back(edge(1, 102.53));
    adjacency_map[3].push_back(edge(5,  61.44));
    adjacency_map[3].push_back(edge(6,  96.79));
    adjacency_map[4].push_back(edge(5, 133.04));
    adjacency_map[5].push_back(edge(0,  81.77));
    adjacency_map[5].push_back(edge(3,  62.05));
    adjacency_map[5].push_back(edge(4, 134.47));
    adjacency_map[5].push_back(edge(6,  91.63));
    adjacency_map[6].push_back(edge(3,  97.24));
    adjacency_map[6].push_back(edge(5,  87.94));

    std::map<vertex_t, weight_t> min_distance;
    std::map<vertex_t, vertex_t> previous;
    DijkstraComputePaths(0, adjacency_map, min_distance, previous);
    for (adjacency_map_t::const_iterator vertex_iter = adjacency_map.begin();
         vertex_iter != adjacency_map.end();
         vertex_iter++)
    {
        vertex_t v = vertex_iter->first;
        std::cout << "Distance to " << vertex_names[v] << ": " << min_distance[v] << std::endl;
        std::list<vertex_t> path =
            DijkstraGetShortestPathTo(v, previous);
        std::list<vertex_t>::const_iterator path_iter = path.begin();
        std::cout << "Path: ";
        for( ; path_iter != path.end(); path_iter++)
        {
            std::cout << vertex_names[*path_iter] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
