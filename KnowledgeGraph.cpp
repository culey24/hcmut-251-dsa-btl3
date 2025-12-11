#include "KnowledgeGraph.h"

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

    ss << "])";
    return ss.str();
}

template <class T>
vector<T> VertexNode<T>::getOutVertices() {
    vector<T> outVertices;
    for (Edge<T>* edging : adList) {
        if (edging->from == this) outVertices.push_back(edging->to->vertex);
    }
    return outVertices;
}

template <class T>
vector<T> VertexNode<T>::getInVertices() {
    vector<T> inVertices;
    for (Edge<T>* edging : adList) {
        if (edging->to == this) inVertices.push_back(edging->from->vertex);
    }
    return inVertices;
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
        for (Edge<T>* edging : node->adList) {
            if (edging->from == node) delete edging;
        }
    }

    for (VertexNode<T>* node : nodeList) node->adList.clear();

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
    VertexNode<T>* startingNode = this->getVertexNode(start);
    if (startingNode == nullptr) throw VertexNotFoundException();
    Set<T> visited(1009, this->vertex2str, this->vertexEQ);
    Queue<VertexNode<T>*> queue;   
    stringstream ss;

    ss << "[";
    bool first = true;

    queue.push(startingNode);
    visited.insert(startingNode->vertex);

    while (!queue.empty()) {
        VertexNode<T>* node = queue.front();
        queue.pop();
        
        if (!first) ss << ", ";
        ss << node->toString();
        first = false;

        for (Edge<T>* edging : node->adList) {
            if (edging->from == node) {
                VertexNode<T>* toNode = edging->to;
                if (!visited.contains(toNode->vertex)) {
                    visited.insert(toNode->vertex);
                    queue.push(toNode);
                }
            }
        }
    }

    ss << "]";

    return ss.str();
}

template <class T>
string DGraphModel<T>::DFS(T start) {
    VertexNode<T>* startingNode = this->getVertexNode(start);
    if (startingNode == nullptr) throw VertexNotFoundException();

    Set<T> visited(1009, this->vertex2str, this->vertexEQ);
    Stack<VertexNode<T>*> stack;
    stringstream ss;
    bool first = true;

    ss << "[";

    stack.push(startingNode);

    while (!stack.empty()) {
        VertexNode<T>* node = stack.top();
        stack.pop();

        if (visited.contains(node->vertex)) continue;
        visited.insert(node->vertex);

        if (!first) ss << ", ";
        ss << node->toString();
        first = false;

        for (int i = node->adList.size() - 1; i >= 0; i--) {
            Edge<T>* edging = node->adList[i];
            if (edging->from == node) {
                VertexNode<T>* toNode = edging->to;
                if (!visited.contains(toNode->vertex)) stack.push(toNode);
            }
        }
    }

    ss << "]";

    return ss.str();
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
    graph = DGraphModel<string>([](string& a, string& b){ return a == b; }, 
                                [](string& s){ return s; });
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

    return node->getOutVertices();
}

string KnowledgeGraph::bfs(string start) {
    if (!graph.contains(start)) throw EntityNotFoundException();
    return graph.BFS(start);
}

string KnowledgeGraph::dfs(string start) {
    if (!graph.contains(start)) throw EntityNotFoundException();
    return graph.DFS(start);
}

bool KnowledgeGraph::isReachable(string from, string to) {
    // implemented using bfs
    if (!graph.contains(from) || !graph.contains(to)) throw EntityNotFoundException();

    if (from == to) return true;

    VertexNode<string>* startingNode = graph.getVertexNode(from);
    Set<string> visited(1009, 
                        [](string& s){ return s; }, 
                        [](string& a, string& b){ return a == b; });
    Queue<VertexNode<string>*> queue;

    queue.push(startingNode);
    visited.insert(startingNode->getVertex());

    while (!queue.empty()) {
        VertexNode<string>* node = queue.front();
        queue.pop();

        vector<string> outVertices = node->getOutVertices();
        for (string vertex : outVertices) {
            if (vertex == to) return true;
            if (!visited.contains(vertex)) {
                visited.insert(vertex);
                queue.push(graph.getVertexNode(vertex));
            }
        }
    }
    return false;
}

string KnowledgeGraph::toString() {
    return graph.toString();
}

vector<string> KnowledgeGraph::getRelatedEntities(string entity, int depth) {
    VertexNode<string>* startingNode = graph.getVertexNode(entity);
    if (startingNode == nullptr) throw EntityNotFoundException();
    vector<string> related;
    Set<string> visited(1009, 
                        [](string& s){ return s; },
                        [](string& a, string& b){ return a == b; });
    Queue<VertexNode<string>*> queueNode;
    Queue<int> queueDepth;

    queueNode.push(startingNode);
    queueDepth.push(0);
    visited.insert(entity);

    while (!queueNode.empty() && !queueDepth.empty()) {
        VertexNode<string>* node = queueNode.front();
        int nodeDepth = queueDepth.front();
        queueNode.pop();
        queueDepth.pop();

        if (nodeDepth > 0) related.push_back(node->getVertex());
        if (nodeDepth < depth) {
            vector<string> outVertices = node->getOutVertices();
            for (string& vertex : outVertices) {
                VertexNode<string>* outNode = graph.getVertexNode(vertex);
                if (!visited.contains(vertex)) {
                    visited.insert(vertex);
                    queueNode.push(outNode);
                    queueDepth.push(nodeDepth + 1);
                }
            }
        }
    }
    return related;
}

vector<string> KnowledgeGraph::getAncestors(string start) {
    VertexNode<string>* startingNode = graph.getVertexNode(start);
    if (startingNode == nullptr) return {};
    
    vector<string> ancestors;
    Set<string> visited(1009, 
                        [](string& s){ return s; },
                        [](string& a, string& b){ return a == b; });
    Stack<VertexNode<string>*> stack;
    
    stack.push(startingNode);
    visited.insert(start);

    while (!stack.empty()) {
        VertexNode<string>* node = stack.top();
        stack.pop();
        ancestors.push_back(node->getVertex());
        
        vector<string> parents = node->getInVertices();
        for (string parent : parents) {
            VertexNode<string>* parentNode = graph.getVertexNode(parent);
            if (!visited.contains(parent)) {
                visited.insert(parent);
                stack.push(parentNode);
            }
        }
    }
    return ancestors;
}

int KnowledgeGraph::bfsDistance(string start, string target) {
    if (start == target) return 0;
    
    Set<string> visited(1009, 
                        [](string& s){ return s; },
                        [](string& a, string& b){ return a == b; });
    Queue<VertexNode<string>*> queueNode;
    Queue<int> queueDistance;

    queueNode.push(graph.getVertexNode(start));
    queueDistance.push(0);
    visited.insert(start);
    while (!queueNode.empty() && !queueDistance.empty()) {
        VertexNode<string>* node = queueNode.front();
        int distance = queueDistance.front();
        queueNode.pop();
        queueDistance.pop();

        if (node->getVertex() == target) return distance;
        vector<string> outVertices = node->getOutVertices();
        for (string outVertex : outVertices) {
            if (!visited.contains(outVertex)) {
                visited.insert(outVertex);
                queueNode.push(graph.getVertexNode(outVertex));
                queueDistance.push(distance + 1);
            }
        }
    }
    return 999999;
}

string KnowledgeGraph::findCommonAncestors(string entity1, string entity2) {
    VertexNode<string>* entityOne = graph.getVertexNode(entity1);
    VertexNode<string>* entityTwo = graph.getVertexNode(entity2);

    if (entityOne == nullptr || entityTwo == nullptr) throw EntityNotFoundException();

    vector<string> ancestorsOfOne = this->getAncestors(entity1);
    vector<string> ancestorsOfTwo = this->getAncestors(entity2);
    
    vector<string> common;
    for (string ancestorOne : ancestorsOfOne) {
        for (string ancestorTwo : ancestorsOfTwo) {
            if (ancestorOne == ancestorTwo) {
                common.push_back(ancestorOne);
                break;
            }
        }
    }
    if (common.empty()) return "No common ancestor";
    string bestAncestor;
    int minTotalDistance = 999999;
    for (string ancestor : common) {
        int distanceOne = bfsDistance(ancestor, entity1);
        int distanceTwo = bfsDistance(ancestor, entity2);
        if (distanceOne + distanceTwo < minTotalDistance) {
            minTotalDistance = distanceOne + distanceTwo;
            bestAncestor = ancestor;
        }
    }
    return bestAncestor;
}

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
void Stack<T>::push(T element) {
    data.push_back(element);
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
// SET // MY IMPLEMENTATION
// =============================================================================

template <class T>
unsigned long Set<T>::hashFunction(string str) {
    unsigned long hash = 5381;
    for (char ch : str) {
        hash = ((hash << 5) + hash) + ch; // hash * 33 + c 
    }
    return hash % capacity;
}

template <class T>
Set<T>::Set(int capacity, string (*item2Str)(T&),bool (*itemEQ)(T&, T&)) :  capacity(capacity), 
                                                                            item2Str(item2Str), 
                                                                            itemEQ(itemEQ)
{
    this->buckets.resize(capacity);
}

template <class T>
void Set<T>::insert(T item) {
    if (this->contains(item)) return;
    string str = item2Str(item);
    int index = hashFunction(str);
    buckets[index].push_back(item);
}

template <class T>
bool Set<T>::contains(T item) {
    string str = item2Str(item);
    int index = hashFunction(str);
    for (T& exists : buckets[index]) {
        if (itemEQ(exists, item)) return true;
    }
    return false;
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

template class Stack<VertexNode<string>*>; 
template class Queue<VertexNode<string>*>; 
template class Set<string>;

template class Stack<VertexNode<int>*>;
template class Queue<VertexNode<int>*>;
template class Set<int>;

template class Stack<VertexNode<float>*>;
template class Queue<VertexNode<float>*>;
template class Set<float>;

template class Stack<VertexNode<char>*>;
template class Queue<VertexNode<char>*>;
template class Set<char>;

template class Queue<int>;