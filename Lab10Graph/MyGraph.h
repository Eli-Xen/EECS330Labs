#ifndef _MYGRAPH_H_
#define _MYGRAPH_H_

#include <iostream>
#include <fstream>

#include "MyVector.h"
#include "MyLinkedList.h"
#include "MyQueue.h"
#include "MyStack.h"
#include "MyHashTable.h" 


//g++ --std=c++11 MainTest.cpp -o Lab10

// for i in {0..9}; do
//     ./Lab10 Inputs/input_vertex_$i.txt Inputs/input_edge_$i.txt > output_$i.txt
//     python3 GradingScript.py output_$i.txt Outputs/output_$i.txt
// done

// for i in {0..9}; do
//     printf $i" my output vs given output line by line:\n"
//     diff output_$i.txt Outputs/output_$i.txt
//     printf "\n\n"
// done

typedef long long VertexIDType;
typedef long long EdgeIDType;

template <typename VertexDataType, typename EdgeDataType>
class MyGraph
{ public:
    class Vertex
    { public: 
        Vertex() // default constructor
        {   return;   }

        explicit Vertex(const VertexDataType& vertex_data) : // vertex copy constructor
            data(vertex_data)
        {   return;    }

        explicit Vertex(VertexDataType && vertex_data) : // vertex move constructor
            data(std::move(vertex_data))
        {   return;    }

        Vertex& operator=(const Vertex& rhs) // copy assignment
        {   id = rhs.id;
            tag = rhs.tag;
            data = rhs.data;
            return *this;
        }

        Vertex& operator=(Vertex && rhs) // move assignment
        {   id = rhs.id;
            tag = rhs.tag;
            data = std::move(rhs.data);
            return *this;
        }

        VertexIDType id;        // the unique ID of the vertex
        int tag;                // a reserved tag variable
        VertexDataType data;    // the property associated with the vertex

        friend class MyGraph;

    };

    class Edge
    { public:
        Edge() // default constructor
        {   return;    }

        explicit Edge(const VertexIDType sid, const VertexIDType tid, const EdgeDataType& edge_data) : // copy constructor, sid: source vertex ID; vid: target vertex ID
            src(sid),
            tgt(tid),
            data(edge_data)
        {   return;    }

        explicit Edge(const VertexIDType sid, const VertexIDType tid, EdgeDataType && edge_data) : // move constructor, sid: source vertex ID; vid: target vertex ID
            src(sid),
            tgt(tid),
            data(std::move(edge_data))
        {   return;     }

        Edge& operator=(const Edge& rhs) // copy assignment
        {   id = rhs.id;
            src = rhs.src; //source 
            tgt = rhs.tgt; //target 
            tag = rhs.tag; 
            data = rhs.data;
            return *this;
        }

        Edge& operator=(Edge && rhs) // move assignment
        {   id = rhs.id;
            src = rhs.src;
            tgt = rhs.tgt;
            tag = rhs.tag;
            data = std::move(rhs.data);
            return *this;
        }
      
        EdgeIDType id;          // the unique ID of the edge
        VertexIDType src, tgt;  // the IDs for the source vertex and the target vertex of the edge
        int tag;                // the reserved tag varaible
        EdgeDataType data;      // the property associated with the edge

        friend class MyGraph;

    };

    MyGraph() : // default constructor of graph, initialize the graph with a size of 0
        current_vertex_ID(0),
        current_edge_ID(0),
        num_vertices(0),
        num_edges(0),
        vertex_set{0},
        edge_set{0},
        vertex_map{},
        edge_map{},
        adj_list{}
    {   return;   }

    
    ~MyGraph() // destructor
    {   for(size_t i = 0; i < vertex_set.size(); ++ i)
        {   delete vertex_set[i];    }
        for(size_t i = 0; i < edge_set.size(); ++ i)
        {   delete edge_set[i];    }
        for(size_t i = 0; i < adj_list.size(); ++ i)
        {   adj_list[i]->clear();
            delete adj_list[i];
        }
        return;
    }

    VertexIDType addVertex(const VertexDataType& v_data) // adds an vertex v to the graph (copy); assign ID according the to oder of vertex insertion, for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex; 
    {   Vertex* newV=new Vertex(v_data); //create new vertex 
        newV->id=current_vertex_ID++; //next number of vertex ID 
        newV->tag=0; 
        vertex_set.push_back(newV); //put new vertex onto list of verticies 
        vertex_map.insert(HashedObj<VertexIDType, size_t>(newV->id, vertex_set.size()-1)); //inserts new hash object into hash table using ints for vertex ID and last position in hash table 
        adj_list.push_back(new MyLinkedList<EdgeIDType>()); //new lined list for vertex to track edges 
        num_vertices++; //incriment number of verticies 
        return newV->id; //return id since thats the return type 
    } 

    VertexIDType addVertex(VertexDataType && v_data)// adds an vertex v to the graph (copy); assign ID according the to oder of vertex insertion, for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex; 
    {   Vertex* newV=new Vertex(std::move(v_data)); //create new vertex 
        newV->id=current_vertex_ID++; //next number of vertex ID 
        newV->tag=0; 
        vertex_set.push_back(newV); //put new vertex onto list of verticies 
        vertex_map.insert(HashedObj<VertexIDType, size_t>(newV->id, vertex_set.size()-1)); //inserts new hash object into hash table using ints for vertex ID and last position in hash table 
        adj_list.push_back(new MyLinkedList<EdgeIDType>()); //new lined list for vertex to track edges 
        num_vertices++; //incriment number of verticies 
        return newV->id; //return id since thats the return type 
    } 

    Vertex* getVertex(const VertexIDType vid) // retrieves the information of the vertex specified by the vid, returns the vertex
    {   HashedObj<VertexIDType, size_t> result;
        if (!vertex_map.retrieve(vid, result)) 
        {   std::cerr << "warning: attempt to access non-existent vertex ID: " << vid << std::endl;
            return nullptr;
        }
        return vertex_set[result.value]; //gets the vertex with that ID 
    } 
    
    EdgeIDType addEdge(const VertexIDType sid, const VertexIDType tid, const EdgeDataType& e_data) 
    {   Edge* nedge=new Edge(sid, tid, e_data); //make new edge 
        nedge->id=current_edge_ID++; //assign next largest num for edge ID 
        nedge->tag=0;
    
        edge_set.push_back(nedge); //put new edge onto array of edges 
        edge_map.insert(HashedObj<EdgeIDType, size_t>(nedge->id, edge_set.size()-1));
        size_t spos=vertexID2SetPos(sid); //mark that the vertecies have an edge connecting them 
        size_t tpos=vertexID2SetPos(tid); //mark that the vertecies have an edge connecting them 
        

        auto sbeg=adj_list[spos]->begin();
        while(sbeg!=adj_list[spos]->end() && *sbeg<nedge->id)
            ++sbeg;
        adj_list[spos]->insert(sbeg, nedge->id);
        
        auto tbeg=adj_list[tpos]->begin();
        while (tbeg!=adj_list[tpos]->end() && *tbeg<nedge->id) 
            ++tbeg;
        adj_list[tpos]->insert(tbeg, nedge->id);

        num_edges++;
        return nedge->id; // returns the assigned edge ID for e
    }

    EdgeIDType addEdge(const VertexIDType sid, const VertexIDType tid, EdgeDataType && e_data) 
    {   Edge* nedge = new Edge(std::move(sid), std::move(tid), std::move(e_data));
        nedge->id=current_edge_ID++; //assign next largest num for edge ID 
        nedge->tag=0;
    
        edge_set.push_back(nedge); //put new edge onto array of edges 
        edge_map.insert(HashedObj<EdgeIDType, size_t>(nedge->id, edge_set.size()-1));
        size_t spos=vertexID2SetPos(sid); //mark that the vertecies have an edge connecting them 
        size_t tpos=vertexID2SetPos(tid); //mark that the vertecies have an edge connecting them 
        

        auto sbeg=adj_list[spos]->begin();
        while(sbeg!=adj_list[spos]->end() && *sbeg<nedge->id)
            ++sbeg;
        adj_list[spos]->insert(sbeg, nedge->id);
        
        auto tbeg=adj_list[tpos]->begin();
        while (tbeg!=adj_list[tpos]->end() && *tbeg<nedge->id) 
            ++tbeg;
        adj_list[tpos]->insert(tbeg, nedge->id);

        num_edges++;
        return nedge->id; // returns the assigned edge ID for e
    }

    Edge* getEdge(const EdgeIDType eid) // retrieves the information of the edge specified by the eid, returns the edge
    {   HashedObj<EdgeIDType, size_t> result;
        if (!edge_map.retrieve(eid, result)) {
            std::cerr << "Warning: Attempt to access non-existent edge ID: " << eid << std::endl;
            return nullptr;
        }
        return edge_set[result.value]; //find the edge with given ID 
    } 

    bool probeEdge(const VertexIDType sid, const VertexIDType tid, EdgeIDType& eid) // checks whether an edge exists between src and tgt, if yes, return TRUE and record the corresponding edge ID in eid otherwise false 
    {   size_t spos=vertexID2SetPos(sid); //get where in adjList the vertex is 
        MyLinkedList<EdgeIDType>* list=adj_list[spos]; //LL that we have to search for the edge, use LL of source vertex (sid) 
        for (auto itr=list->begin(); itr!=list->end(); ++itr) //from LL beggining to end... 
        {   Edge* e=getEdge(*itr); //get the edge at that iterator index 
            if (!e) continue; //debug
            if ((e->src==sid && e->tgt==tid) || (e->src==tid && e->tgt==sid)) //if the edge found matches either end with source and target... 
            {   eid=e->id; //save the ID of that edge to eid 
                return true; //edge found 
            }
        }
        return false; //edge not found and doesnt exist 
    }

    size_t degree(const VertexIDType vid) // returns the degree of the vertex specified by vid
    {   return adj_list[vertexID2SetPos(vid)]->size(); } //gets size of linked list at the vertex (in array) since thats how many edges are connected 
        
    void deleteVertex(const VertexIDType vid) // deletes the vertex specified by vid from the graph and also delete all edges associated with it; expected time complexity: O(d^2), where d is the average degree of the graph
    {   if (!vertex_map.contains(vid)) 
        {   std::cout << "vertex ID not found" << std::endl;
            return;
        }
        
        size_t vpos=vertexID2SetPos(vid);
    
        MyVector<EdgeIDType> edgesToDelete; //collect edges to delete 
        for(auto it=adj_list[vpos]->begin(); it!=adj_list[vpos]->end(); ++it) //delete all connected edges (copy of edge IDs first)
        {   edgesToDelete.push_back(*it);    }
        for(EdgeIDType eid:edgesToDelete) 
        {   deleteEdge(eid);    }
    
        
        size_t lastPos=vertex_set.size()-1;
        if(vpos!=lastPos) //swap with last vertex if needed
        {   std::swap(vertex_set[vpos], vertex_set[lastPos]); //swap vertexes 
            std::swap(adj_list[vpos], adj_list[lastPos]);
    
            vertex_map.remove(vertex_set[vpos]->id); //update vertex_map for swapped vertexes
            vertex_map.insert(HashedObj<VertexIDType, size_t>(vertex_set[vpos]->id, vpos));
    
            
            for (Edge* edge : edge_set) //update all edges that connect to the swapped vertex
            {   if (!edge) continue; //if edge doesnt exist skip iteration 
                if (edge->src==vertex_set[lastPos]->id) edge->src=vertex_set[vpos]->id; 
                if (edge->tgt==vertex_set[lastPos]->id) edge->tgt=vertex_set[vpos]->id; 
            }
        }
    
        delete vertex_set.back();
        vertex_set.pop_back();
        delete adj_list.back();
        adj_list.pop_back();
        vertex_map.remove(vid);
        num_vertices--; 
    }

    void deleteEdge(const EdgeIDType eid) // deletes the edge specified by eid from the graph; expected time complexity: O(d), where d is the average degree of the graph
    {   if (!edge_map.contains(eid)) return; //if not in map do nothing 
        HashedObj<EdgeIDType, size_t> edgeInfo; //false edge to test if eid is alrdy in retrieve 
        if (!edge_map.retrieve(eid,edgeInfo)) return;
        
        Edge* edge=edge_set[edgeInfo.value]; //get pointer referance to edge we want to delete 
        if(!edge) return; //if pointer null do nothing 

        size_t spos=vertexID2SetPos(edge->src);
        size_t tpos=vertexID2SetPos(edge->tgt);

        MyLinkedList<EdgeIDType>* srcList=adj_list[spos]; //remove edge ID from source vertexs adjacency list
        for (auto it=srcList->begin(); it!=srcList->end(); ++it) //remove edge ID from sources adjacency list
        {   if (*it==eid) 
            {   srcList->erase(it);
                break;
            }
        }

        MyLinkedList<EdgeIDType>* tgtList = adj_list[tpos]; //remove edge ID from target vertexs adjacency list
        for (auto it=tgtList->begin(); it!=tgtList->end(); ++it) 
        {   if (*it==eid) 
            {   tgtList->erase(it);
                break;
            }
        }

        size_t lastPos=edge_set.size()-1; //swap with last edge if needed
        if (edgeInfo.value!=lastPos) //if edge to delete is at then end of the set... 
        {   Edge* lastEdge=edge_set[lastPos]; //move the last edge into the deleted spot
            edge_set[edgeInfo.value]=lastEdge;
            edge_map.remove(lastEdge->id); //update edge_map for swapped edge
            edge_map.insert(HashedObj<EdgeIDType, size_t>(lastEdge->id, edgeInfo.value)); // re-insert with updated index; replace debug 
        }

        edge_set.pop_back(); //remove from edge_set 
        edge_map.remove(eid); //remove the deleted edge from the map
        delete edge; //delete from memory
        num_edges--; 
    }

    void breadthFirstSearch(const VertexIDType v_src, MyVector<VertexIDType>& path) // performs BFS on the graph, starting from v_src; record the path in path; outgoing edge visit order should be determined based on their ID (smaller ID visited earlier) 
    {   MyQueue<VertexIDType> que; //queue to store verticies to visit next 
        MyHashTable<VertexIDType, bool> visited; //hash table to mark visited vertecies 

        que.enqueue(v_src); //start queue with source vertex 
        visited.insert(HashedObj<VertexIDType, bool>(v_src, true)); //mark it as visited 
        
        while(!que.empty()) //until the queue is empty... 
        {   VertexIDType current=que.front(); //...put the front of the queue on the path
            que.dequeue();
            path.push_back(current);

            size_t vpos=vertexID2SetPos(current);
            MyLinkedList<EdgeIDType>* adj=adj_list[vpos]; //LL of adjacent vertecies of current 
            
            MyVector<EdgeIDType> edges; //vector to store edges 
            for (auto itr=adj->begin(); itr!=adj->end(); ++itr)  //save all neighbors to array 
            {   edges.push_back(*itr);    } //collect and sort edge IDs in ascending order
            std::sort(edges.begin(), edges.end());  //sort by edge ID (ascending) 
            //std::reverse(edges.begin(), edges.end()); //added to reverse the list 
            
            // Process neighbors in order of edge IDs
            for(size_t i=0; i<edges.size(); ++i)  //for all neighbors...
            {   Edge* edge=getEdge(edges[i]);
                if (!edge) continue;
                
                VertexIDType neighbor=(edge->src==current) ? edge->tgt : edge->src; //if current edges source is itself then neighbor is its target, otherwise neighbor is the source depending on which way were coming from 
                if(!visited.contains(neighbor))  //if neighbor not yet visited...
                {   visited.insert(HashedObj<VertexIDType, bool>(neighbor, true)); //mark neighbor as visited 
                    que.enqueue(neighbor); //put it on the queue 
                }
            }
        }
    }


    void depthFirstSearch(const VertexIDType v_src, MyVector<VertexIDType>& path) 
    {   MyStack<VertexIDType> stk;
        MyHashTable<VertexIDType, bool> visited;  //hash table for if vertex has been visited 
    
        stk.push(v_src); //put first vertex on stack 
        while(!stk.empty()) 
        {   VertexIDType current=stk.top(); //save top of stack 
            stk.pop(); //remove it from stack 
            if(visited.contains(current)) continue; //if already in visited skip iteration 
            
            visited.insert(HashedObj<VertexIDType, bool>(current, true));  //if not visited then put it on path 
            path.push_back(current);
    
            size_t vpos=vertexID2SetPos(current);
            MyLinkedList<EdgeIDType>* adj = adj_list[vpos];  //LL for adjacent vertecies of current 
            
            for(auto itr=adj->begin(); itr!=adj->end(); ++itr) //go through hash table normally since its already sorted 
            {   Edge* edge=getEdge(*itr);
                if (!edge) continue; //if edge doesnt exist skip iteration 
                
                VertexIDType neighbor=(edge->src==current) ? edge->tgt : edge->src; //if current edges source is itself then neighbor is its target, otherwise neighbor is the source depending on which way were coming from 
                if(!visited.contains(neighbor)) 
                    stk.push(neighbor);
            }
        }
    }

    bool isConnected(const VertexIDType vid1, const VertexIDType vid2) // determines whether the two nodes vid1 and vid2 are connected, if yes return true otherwise false 
    {   if(vid1==vid2) return true; //already connected becuase same node 
        MyVector<VertexIDType> path; //path that search will save to 
        breadthFirstSearch(vid1, path); 
        for(size_t i=0; i<path.size(); ++i) //iterate through path of vid1... 
        {   if(path[i]==vid2) //if vid2 is found then theyre connected 
                return true; 
        }
        return false; 
    }

    size_t numVertices(void) const // returns the number of vertices in the graph
    {   return num_vertices;   }

    size_t numEdges(void) const // returns the number of edges in the graph
    {   return num_edges;   }

    bool empty(void) const // checks whether the graph is empty (no vertex)
    {   return (num_vertices == 0);   }

    void printInfoVertexSet(void) // prints the content in vertex_set
    {   for(size_t i = 0; i < num_vertices; ++ i)
        {   std::cout << "Array Position: " << i << "\t";
            std::cout << "Vertex ID: " << vertex_set[i]->id << "\t";
            std::cout << "Vertex data: " << vertex_set[i]->data << std::endl;
        }
        return;
    }

    void printInfoEdgeSet(void) // prints the content in edge_set
    {   for(size_t i = 0; i < num_edges; ++ i)
        {   std::cout << "Array Position: " << i << "\t";
            std::cout << "Edge ID: " << edge_set[i]->id << "\t";
            std::cout << "Edge ends: " << edge_set[i]->src << "\t" << edge_set[i]->tgt << "\t";
            std::cout << "Edge data: " << edge_set[i]->data << std::endl;
        }
        return;
    }

    void printInfoVertexMap(void) // prints the content in vertex_map
    {   MyVector<VertexIDType> v_keys;
        vertex_map.keys(v_keys);
        for(size_t i = 0; i < v_keys.size(); ++ i)
        {   std::cout << "VertexID-Position:\t" << v_keys[i] << "\t" << vertexID2SetPos(v_keys[i]) << std::endl;    }
        return;
    }

    void printInfoEdgeMap(void) // prints the content in edge_map
    {   MyVector<EdgeIDType> e_keys;
        edge_map.keys(e_keys);
        for(size_t i = 0; i < e_keys.size(); ++ i)
        {   std::cout << "EdgeID-Position:\t" << e_keys[i] << "\t" << edgeID2SetPos(e_keys[i]) << std::endl;    }
        return;
    }

    void printAdjList(void) // prints the content in the adjacency list
    {   for(size_t i = 0; i < adj_list.size(); ++ i)
        {   std::cout << "Vertex: " << vertex_set[i]->id << " degree: " << degree(vertex_set[i]->id) << "\tIt is connected with: ";
            for(auto itr = adj_list[i]->begin(); itr != adj_list[i]->end(); ++ itr)
            {   if(vertex_set[i]->id == getEdge(*itr)->src)
                    std::cout << getEdge(*itr)->tgt << "\t";
                else if(vertex_set[i]->id == getEdge(*itr)->tgt)
                    std::cout << getEdge(*itr)->src << "\t";
                else
                    throw std::domain_error("MyGraph::printAdjList: edge information may have been corrupted.");
            }
            std::cout << std::endl;
        }
    }

  private:
    VertexIDType current_vertex_ID;                 // the ID to be assigned to the next vertex (ID are assigned based on the order of insertion)
    EdgeIDType current_edge_ID;                     // the ID to be assigned to the next edge (IDs are assigned based on the order of insertion)
    size_t num_vertices;                            // the number of vertices
    size_t num_edges;                               // the number of edges
    MyVector<Vertex*> vertex_set;                   // the set of vertices
    MyVector<Edge*> edge_set;                       // the set of edges
    MyHashTable<VertexIDType, size_t> vertex_map;   // the mapping between a vertex ID and its index in vertex_set
    MyHashTable<EdgeIDType, size_t> edge_map;       // the mapping between an edge ID and its index in vertex_set
    MyVector<MyLinkedList<EdgeIDType>* > adj_list;  // the adjacency list (stores the IDs for the corresponding edges)

    size_t vertexID2SetPos(const VertexIDType vid) // maps the vertex ID to its position in the vertex_set array
    {   HashedObj<VertexIDType, size_t> result; 
        if (!vertex_map.retrieve(vid, result)) //tries to find the key to the given ID and if found copies it to result 
            throw std::runtime_error("vertex ID not found in vertex_map"); //if not found give error 
        return result.value; //index in vertex_set array 
    }
    //changed the accepted param from vertexIDtype to edgeIDtype 
    size_t edgeID2SetPos(const EdgeIDType eid) // maps the edge ID to its position in the edge_set array
    {   HashedObj<EdgeIDType, size_t> result;
        if (!edge_map.retrieve(eid, result)) {
            throw std::runtime_error("edge ID not found in edge_map");
        }
        return result.value; 
    }

    VertexIDType vertexSetPos2ID(const size_t vpos) // gets the vertex ID for one in a given position in the vertex_set array
    {   return vertex_set[vpos]->id;    } //returns ID of the vertex at the given position 

    EdgeIDType edgeSetPos2ID(const size_t epos) // gets the edge ID for one in a given position in the edge_set array
    {   return edge_set[epos]->id;  } //returns ID of the edge at the given position 
};

#endif  // _MYGRAPH_H_