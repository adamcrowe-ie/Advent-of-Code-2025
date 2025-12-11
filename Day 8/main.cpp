#include <array>
#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>


using ll = long long;
using Coord = std::array<int, 3>;
using DistPair = std::tuple<ll, int, int>;


void get_coords(std::vector<Coord>& coords){
    std::ifstream file("junction_boxes.txt");

    if(!file)
        throw std::runtime_error("Couldn't open file.");

    std::string line;

    while(std::getline(file, line)){
        Coord coord = {};
        int i = 0, n = line.size();

        for(int j = 0; j < n; ++j){
            if(line[j] == ','){
                ++i;
                continue;
            }

            coord[i] *= 10;
            coord[i] += line[j] - '0';
        }

        coords.push_back(coord);
    }
}


inline ll euclid_dist(const Coord& l, const Coord& r){
    ll x = l[0] - r[0];
    ll y = l[1] - r[1];
    ll z = l[2] - r[2];

    return x*x + y*y +z*z;
}


void find_pairs(const std::vector<Coord>& coords, std::vector<DistPair>& pairs){
    int n = coords.size();

    for(int i = 0; i < n; ++i)
    for(int j = i + 1; j < n; ++j)
        pairs.emplace_back(euclid_dist(coords[i], coords[j]), i, j);

    std::sort(pairs.begin(), pairs.end());
}


class DisjointSet{
    std::vector<int> parent;

    public:
        int partitions;

        explicit DisjointSet(int n) : partitions{n} {
            parent.resize(n);

            for(int i = 0; i < n; ++i)
                parent[i] = i;
        }

        int find(int node){
            if(parent[node] == node)
                return node;

            return parent[node] = find(parent[node]);
        }

        void unite(int a, int b){
            int parent_a = find(a);
            int parent_b = find(b);

            if(parent_a != parent_b){
                parent[parent_a] = parent_b;
                --partitions;
            }
        }
};


int find_circuit_sizes(DisjointSet& ds, int n){
    std::vector<int> circuit_size(n, 0);

    for(int i = 0; i < n; ++i)
        ++circuit_size[ds.find(i)];

    std::sort(circuit_size.begin(), circuit_size.end(), std::greater<int>());

    return circuit_size[0] * circuit_size[1] * circuit_size[2];
}


ll complete_circuit(const std::vector<Coord>& coords, std::vector<DistPair>& pairs, 
                    DisjointSet& ds, int start, int end){
    for(int i = start; i < end; ++i){
        int c1 = std::get<1>(pairs[i]);
        int c2 = std::get<2>(pairs[i]);

        ds.unite(c1, c2);

        if(ds.partitions == 1)
            return coords[c1][0] * coords[c2][0];
    }

    return 0LL;
}


int main(){
    std::vector<Coord> coords;
    coords.reserve(1000);
    get_coords(coords);
    
    std::vector<DistPair> pairs;
    find_pairs(coords, pairs);

    int n = coords.size(), num_pairs = pairs.size();

    DisjointSet ds(n);

    complete_circuit(coords, pairs, ds, 0, n);

    std::cout << "Result for part 1: " << find_circuit_sizes(ds, n) << "\n";
    std::cout << "Result for part 1: " << complete_circuit(coords, pairs, ds, n, num_pairs);
}