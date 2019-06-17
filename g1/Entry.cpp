#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <list>
#include <cassert>
#include <limits>


namespace {
template<bool LOG_DEBUG=false>
class GraphFile{
private:
    int minEdge_ = std::numeric_limits<int>::max();
    int maxEdge_ = std::numeric_limits<int>::min();
    int minNode_ = std::numeric_limits<int>::max();
    int maxNode_ = std::numeric_limits<int>::min();
private:
    std::vector<std::vector<int>> data_;
    size_t row_;
    size_t col_;

public:
    size_t rows() const { return row_; }
    size_t cols() const { return col_; }

public:
    GraphFile(const std::string& filename){
        std::ifstream ifs(filename);
        std::string line;

        auto currentLine = 0;
        for (;std::getline(ifs, line);) {
            currentLine ++;
            std::vector<int> row;
            std::string tmp;
            for (size_t i = 0; i <= line.size(); ++i){
                char c = line[i];
                if (c == '\t'|| i == line.size()){
                    if(LOG_DEBUG){
                        printf("tmp=%s\n", tmp.c_str());
                    }
                    auto theValue = std::stoi(tmp);
                    row.push_back(theValue);
                    tmp = "";
                }
                else{
                    tmp.push_back(c);
                }
            }

            assert(row.size()==2);
            if (!row.empty()) {
                data_.push_back(row);
            }

            if (row[1] < minEdge_){
                minEdge_ =row[1];
            }
            if (row[1] > maxEdge_){
                maxEdge_ = row[1];
            }

            if (row[0] < minNode_){
                minNode_ = row[0];
            }
            if (row[0] > maxNode_){
                maxNode_ = row[0];
            }

            if (LOG_DEBUG){
                printf("\n");
            }
        }

        row_ = data_.size();
        col_ = data_.front().size();


        printf("minEdge=%d maxEdge=%d\n",  minEdge_, maxEdge_);
        printf("minNode=%d maxNode=%d\n",  minNode_, maxNode_);
    }

    int get(size_t row, size_t col) const{ return data_[row][col]; }
    void set(size_t row, size_t col, int v){ data_[row][col] = v; }
    auto maxEdge(){return maxEdge_;}
    auto minEdge(){return minEdge_;}
    auto maxNode(){return maxNode_;}
    auto minNode(){return minNode_;}
};
}



class Graph {
    size_t nbNodes_;
    std::vector<std::list<size_t>> adjacency_;

    void getAllPathRecursively(const size_t current, const size_t destination, std::vector<bool> &visited, std::vector<size_t> &path, size_t &path_index) {
        visited[current] = true;
        path[path_index] = current;
        path_index++;

        if (current == destination) {
            for (int i = 0; i<path_index; i++){
                std::cout << path[i] << " ";
            }
            std::cout << std::endl;
        } else {
            for (auto i = adjacency_[current].begin(); i != adjacency_[current].end(); ++i){
                if (!visited[*i]){
                    getAllPathRecursively(*i, destination, visited, path, path_index);
                }
            }
        }

        path_index--;
        visited[current] = false;
    }

public:
    Graph(const size_t nbNodes): nbNodes_(nbNodes){
        adjacency_.resize(nbNodes);
    }

    void addEdge(const size_t u, const size_t v) {
        adjacency_[u].push_back(v);
    }

    void printAllPath(const size_t s, const size_t d) {
        std::vector<bool> visited(nbNodes_);

        std::vector<size_t> path(nbNodes_);
        size_t path_index = 0;

        for (int i = 0; i < nbNodes_; i++){
            visited[i] = false;
        }

        getAllPathRecursively(s, d, visited, path, path_index);
    }
};




class Edge{
public:
    void addNode(const size_t v){
        data_.push_back(v);
    }
    auto edge(){return edge_;}
    void setEdge(const size_t edge){edge_=edge;}
    auto valid(){return data_.size()==2;}
    auto nbNode(){return data_.size();}
    auto data(){return data_;}
private:
    bool valid_ = false;
    size_t edge_;
    std::vector<size_t> data_;
};



int t0(void){
    GraphFile<false> f("input.txt");

    std::vector<Edge> edges;
    edges.resize(f.maxEdge());
    for (int i = 0; i < f.rows(); i++){
        auto theEdge = f.get(i,1) -1;
        edges[theEdge].setEdge(theEdge);
        edges[theEdge].addNode(f.get(i,0)-1);
    }


    for (auto &v : edges){
        if (!v.valid()){
            std::cout << v.edge() << " " << v.nbNode() << std::endl;
        }
    }


    Graph g(f.maxNode());
    for (auto &edge: edges){
        if (!edge.valid()) {
            continue;
        }
        auto theData = edge.data();
        g.addEdge(theData[0], theData[1]);
        g.addEdge(theData[1], theData[0]);
    }

    for(int i = 0; i < 5; i++){
        for (int j = i+1; j<5; j++){
            std::cout << "Paths from: " << i
                      << " to " << j << std::endl;
            g.printAllPath(i, j);
        }
    }
    return 0;
}



int main() {
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 3);
    g.addEdge(2, 0);
    g.addEdge(1, 3);

    auto _01 = 2;
    auto _02 = 3;
    std::cout << "Paths from: " << _01 << " to " << _02 << std::endl;
    g.printAllPath(_01, _02);

    return 0;
}

