#include "KnowledgeGraph.h"
#include "main.h"
#include <vector>
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
    // TODO: Return the string representation of the edge
    // return
    // FIXME
    return "";
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
            delete edging;
            this->outDegree_--;
            to->inDegree_--;
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
    // FIXME
    return "";
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
vector<T> DGraphModel<T>::getOutwardEdges(T from) {
    // FIXME
    VertexNode<T>* fromNode = getVertexNode(from);
    if (fromNode == nullptr) throw VertexNotFoundException();
    vector<T> vertexList;
    for (Edge<T>* edging : fromNode->adList) {
        vertexList.push_back(edging->to->vertex);
    }     
    return vertexList;
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
    // FIXME
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
    // FIXME
    return "";
}

template <class T>
string DGraphModel<T>::BFS(T start) {
    // FIXME
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

KnowledgeGraph::KnowledgeGraph() {
    // TODO: Initialize the KnowledgeGraph
}

void KnowledgeGraph::addEntity(string entity) {
    // TODO: Add a new entity to the Knowledge Graph
}

void KnowledgeGraph::addRelation(string from, string to, float weight) {
    // TODO: Add a directed relation
}

// TODO: Implement other methods of KnowledgeGraph:



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