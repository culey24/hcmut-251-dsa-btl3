#include "KnowledgeGraph.h"
#include "main.h"
// =============================================================================
// Class Edge Implementation
// =============================================================================

template <class T>
Edge<T>::Edge(VertexNode<T>* from, VertexNode<T>* to, float weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
}

template <class T>
string Edge<T>::toString() {
    stringstream ss;
    ss << "(" << this->from->vertex2str(this->from->vertex)
    << ", " << this->to->vertex2str(this->to->vertex)
    << ", " << this->weight <<")";
    return ss.str();
}

// TODO: Implement other methods of Edge:
template <class T>
bool Edge<T>::equals(Edge<T>* edge) {
    return (this->from == edge->from && this->to == edge->to);
}

template <class T>
bool Edge<T>::edgeEQ(Edge<T> *&edge1, Edge<T> *&edge2) {
    return edge1->equals(edge2);       
}

// =============================================================================
// Class VertexNode Implementation
// =============================================================================

template <class T>
VertexNode<T>::VertexNode(T vertex, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&)) {
    this->vertex = vertex;
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
    this->inDegree_ = 0;
    this->outDegree_ = 0;
}

template <class T>
T& VertexNode<T>::getVertex() {
    return this->vertex;
}

template <class T>
void VertexNode<T>::connect(VertexNode<T>* to, float weight) {
    // TODO: Connect this vertex to the 'to' vertex
    Edge<T>* edging = new Edge<T>(this, to, weight);
    this->adList.push_back(edging);
    to->adList.push_back(edging);
    this->outDegree_++;
    to->inDegree_++;
}

template <class T>
Edge<T>* VertexNode<T>::getEdge(VertexNode<T>* to) {
    for (Edge<T>* edging : adList) {
        if (edging->to == to) return edging;
    }
    return nullptr;
}

template <class T>
bool VertexNode<T>::equals(VertexNode<T>* node) {
    return vertexEQ(this->vertex, node->vertex);
}

template <class T>
void VertexNode<T>::removeTo(VertexNode<T>* to) {
    for (int i = 0; i < adList.size(); i++) {
        Edge<T>* edging = adList[i];
        if (edging->to->equals(to)) {
            adList.erase(adList.begin() + i);
            
            for (int j = 0; j < to->adList.size(); j++) {
                if (to->adList[j] == edging) to->adList.erase(to->adList.begin() + j);
                break;
            }

            delete edging;
            this->outDegree_--;
            to->inDegree_--;
            break;
        }
    }
    return;
}

template <class T>
int VertexNode<T>::inDegree() {
    return this->inDegree_;
}

template <class T>
int VertexNode<T>::outDegree() {
    return this->outDegree_;
}

template <class T>
string VertexNode<T>::toString() {
    stringstream ss;
    ss << "(" << this->vertex2str(this->vertex)
    << ", " << this->inDegree_
    << ", " << this->outDegree_
    << ", " << "[";

    for (int i = 0; i < adList.size(); i++) {
        ss << adList[i]->toString();
        if (i != adList.size() - 1) ss << ", ";
    }

    ss << "]";
    return ss.str();
}

template <class T>
vector<T> VertexNode<T>::getAdjacentVertices() {
    vector<T> adjacentVertices;
    for (Edge<T>* edging : adList) {
        adjacentVertices.push_back(edging->to->vertex);
    }
    return adjacentVertices;
}

// =============================================================================
// Class DGraphModel Implementation
// =============================================================================

template <class T>
DGraphModel<T>::DGraphModel(bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&)) {
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
}

template <class T>
DGraphModel<T>::~DGraphModel() {
    // TODO: Clear all vertices and edges to avoid memory leaks
    this->clear();
}

template <class T>
VertexNode<T>* DGraphModel<T>::getVertexNode(T& vertex) {
    if (nodeList.empty()) return nullptr;
    for (VertexNode<T>* node : nodeList) {
        if (node->vertex == vertex) return node;
    }
    return nullptr;
}

template <class T>
string DGraphModel<T>::vertex2Str(VertexNode<T>& node) {
    return vertex2str(node.vertex);
}

template <class T>
string DGraphModel<T>::edge2Str(Edge<T>& edge) {
    return edge.toString();
}

template <class T>
void DGraphModel<T>::add(T vertex) {
    // TODO: Add a new vertex to the graph
    VertexNode<T>* newNode = new VertexNode<T>(vertex, this->vertexEQ, this->vertex2str);
    nodeList.push_back(newNode);
}

template <class T>
bool DGraphModel<T>::contains(T vertex) {
    return (getVertexNode(vertex) != nullptr);
}

template <class T>
float DGraphModel<T>::weight(T from, T to) {
    VertexNode<T>* fromNode = getVertexNode(from);
    VertexNode<T>* toNode = getVertexNode(to);
    if (fromNode == nullptr || toNode == nullptr) throw VertexNotFoundException();
    Edge<T>* edging = fromNode->getEdge(toNode);
    if (edging == nullptr) throw EdgeNotFoundException();
    return edging->weight;
}
template <class T>
vector<Edge<T>*> DGraphModel<T>::getOutwardEdges(T from) {
    VertexNode<T>* fromNode = getVertexNode(from);
    if (fromNode == nullptr) throw VertexNotFoundException();
    vector<Edge<T>*> outwardEdges;
    for (Edge<T>* edging : fromNode->adList) {
        if (edging->from == fromNode) outwardEdges.push_back(edging);
    }     
    return outwardEdges;
}

template <class T>
void DGraphModel<T>::connect(T from, T to, float weight) {
    // TODO: Connect two vertices 'from' and 'to'
    VertexNode<T>* fromNode = getVertexNode(from);
    if (fromNode == nullptr) throw VertexNotFoundException();
    VertexNode<T>* toNode = getVertexNode(to);
    if (toNode == nullptr) throw VertexNotFoundException();

    fromNode->connect(toNode, weight);
}

template <class T>
void DGraphModel<T>::disconnect(T from, T to) {
    VertexNode<T>* fromNode = getVertexNode(from);
    if (fromNode == nullptr) throw VertexNotFoundException();
    VertexNode<T>* toNode = getVertexNode(to);
    if (toNode == nullptr) throw VertexNotFoundException();

    fromNode->removeTo(toNode);
}

template <class T>
bool DGraphModel<T>::connected(T from, T to) {
    VertexNode<T>* fromNode = getVertexNode(from);
    if (fromNode == nullptr) throw VertexNotFoundException();
    VertexNode<T>* toNode = getVertexNode(to);
    if (toNode == nullptr) throw VertexNotFoundException();

    Edge<T>* edging = fromNode->getEdge(toNode);
    return (edging != nullptr);
}

template <class T>
int DGraphModel<T>::size() {
    return this->nodeList.size();
}

template <class T>
bool DGraphModel<T>::empty() {
    return (this->nodeList.size() == 0);
}

template <class T>
void DGraphModel<T>::clear() {
    for (VertexNode<T>* node : nodeList) {
        for (int i = 0; i < node->adList.size(); i++) {
            Edge<T>* edging = node->adList[i];
            if (edging == nullptr) continue;

            if (edging->from == node) {
                // NOTE: IF THERE EXISTS A LOOP ADD LOGIC HERE FIXME
                delete edging;
                node->adList[i] = nullptr;
            }
        }
    }

    for (VertexNode<T>* node : nodeList) delete node;

    nodeList.clear();
}

template <class T>
int DGraphModel<T>::inDegree(T vertex) {
    VertexNode<T>* node = getVertexNode(vertex);
    if (node == nullptr) throw VertexNotFoundException();
    return node->inDegree_;
}

template <class T>
int DGraphModel<T>::outDegree(T vertex) {
    VertexNode<T>* node = getVertexNode(vertex);
    if (node == nullptr) throw VertexNotFoundException();
    return node->outDegree_;
}

template <class T>
vector<T> DGraphModel<T>::vertices() {
    vector<T> vertexList;
    for (VertexNode<T>* node : nodeList) {
        vertexList.push_back(node->vertex);
    }
    return vertexList;
}

template <class T>
string DGraphModel<T>::toString() {
    stringstream ss;
    ss << "[";
    
    for (int i = 0; i < nodeList.size(); i++) {
        ss << nodeList[i]->toString();
        if (i != nodeList.size() - 1) ss << ", ";
    }

    ss << "]";
    return ss.str();
}

template <class T>
string DGraphModel<T>::BFS(T start) {
    // FIXME
    vector<VertexNode<T>*> visited;
    

    
    
    return "";
}

template <class T>
string DGraphModel<T>::DFS(T start) {
    return "";
}

// TODO: Implement other methods of DGraphModel:

// =============================================================================
// Class KnowledgeGraph Implementation
// =============================================================================

int KnowledgeGraph::getEntityIndex(string entity) {
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i] == entity) return i;
    }
    return -1;
}

KnowledgeGraph::KnowledgeGraph() {
    // TODO: Initialize the KnowledgeGraph
    // aura farming
}

void KnowledgeGraph::addEntity(string entity) {
    // TODO: Add a new entity to the Knowledge Graph
    for (string entitit : entities) if (entitit == entity) throw EntityExistsException();
    
    graph.add(entity);

    entities.push_back(entity);
}

void KnowledgeGraph::addRelation(string from, string to, float weight) {
    // TODO: Add a directed relation
    VertexNode<string>* fromNode = graph.getVertexNode(from);
    VertexNode<string>* toNode = graph.getVertexNode(to);    
    if (fromNode == nullptr || toNode == nullptr) throw EntityNotFoundException();

    fromNode->connect(toNode, weight);
}

vector<string> KnowledgeGraph::getAllEntities() {
    return entities;
}

vector<string> KnowledgeGraph::getNeighbors(string entity) {
    VertexNode<string>* node = graph.getVertexNode(entity);
    if (node == nullptr) throw EntityNotFoundException();

    return node->getAdjacentVertices();
}

string KnowledgeGraph::bfs(string start) {
    // FIXME
    return "";
}

string KnowledgeGraph::dfs(string start) {
    //FIXME
    return "";
}

bool KnowledgeGraph::isReachable(string from, string to) {
    // implemented using bfs
    vector<bool> visited(entities.size(), false);
    vector<string> result;
    Queue<string> queue;

    queue.push(from);
    visited[this->getEntityIndex(from)] = true;    
    
    while (!queue.empty()) {

    }
}

string KnowledgeGraph::toString() {
    // FIXME
    return "";
}
// TODO: Implement other methods of KnowledgeGraph:

// =============================================================================
// QUEUE // MY IMPLEMENTATION
// =============================================================================

template <class T>
Queue<T>::Queue() {
    // aura farming
}

template <class T>
Queue<T>::~Queue() {
    // also aura farming
}

template <class T>
void Queue<T>::pop() {
    if (!data.empty()) data.erase(data.begin());
}

template <class T>
void Queue<T>::push(T element) {
    data.push_back(element);
}

template <class T>
T Queue<T>::front() {
    return data.front();
}

template <class T>
T Queue<T>::back() {
    return data.back();
}

template <class T>
bool Queue<T>::empty() {
    return (data.size() == 0);
}

template <class T>
int Queue<T>::size() {
    return data.size();
}

// =============================================================================
// STACK // MY IMPLEMENTATION
// =============================================================================

template <class T>
Stack<T>::Stack() {
    // aura farming
}

template <class T>
Stack<T>::~Stack() {
    // aura farming
}

template <class T>
void Stack<T>::pop() {
    data.pop_back();
}

template <class T>
T Stack<T>::top() {
    return data.back();
}

template <class T> 
bool Stack<T>::empty() {
    return (data.size() == 0);
}

template <class T>
int Stack<T>::size() {
    return data.size();
}
// =============================================================================
// Explicit Template Instantiation
// =============================================================================

template class Edge<string>;
template class Edge<int>;
template class Edge<float>;
template class Edge<char>;

template class VertexNode<string>;
template class VertexNode<int>;
template class VertexNode<float>;
template class VertexNode<char>;

template class DGraphModel<string>;
template class DGraphModel<int>;
template class DGraphModel<float>;
template class DGraphModel<char>;