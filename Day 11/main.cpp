#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <iostream>


using Graph = std::unordered_map<std::string, std::vector<std::string>>;


void explore(Graph& graph, std::unordered_set<std::string>& nodes, const std::string& start){
    std::stack<std::string> st;
    st.push(start);
    
    while(!st.empty()){
        std::string node = st.top();
        st.pop();

        if(node != start && nodes.contains(node))
            continue;
        
        nodes.insert(node);

        for(const std::string child : graph[node])
            st.push(child);
    }
}


std::unordered_set<std::string> find_allowed(Graph& graph, Graph& reverse_graph){
    std::unordered_set<std::string> dac_nodes;
    std::unordered_set<std::string> fft_nodes;

    explore(graph, dac_nodes, "dac");
    explore(reverse_graph, dac_nodes, "dac");

    explore(graph, fft_nodes, "fft");
    explore(reverse_graph, fft_nodes, "fft");

    std::unordered_set<std::string> intersection;

    for(const std::string s : dac_nodes)
        if(fft_nodes.contains(s)) intersection.insert(s);

    return intersection;
}



int dfs1(Graph& graph, const std::string node){
    if(node == "out")
        return 1;

    int result = 0;

    for(const std::string next : graph[node])
        result += dfs1(graph, next);
    
    return result;
}


long long dfs2( Graph& graph,
                std::unordered_map<std::string, long long>& explored,
                const std::unordered_set<std::string>& allowed,
                const std::string node                              )
{
    if(explored.contains(node))
        return explored[node];

    long long result = 0LL;

    for(const std::string next : graph[node])
        if(allowed.contains(next))
            result += dfs2(graph, explored, allowed, next);
    
    return explored[node] = result;
}


int main(){
    std::ifstream file("devices.txt");

    if(!file) throw std::runtime_error("Couldn't open file.");

    std::string line;
    Graph graph;
    Graph reverse_graph;

    while(std::getline(file, line)){
        int n = line.size();
        int colon = line.find(':');
        
        std::string input = line.substr(0, colon);
        std::vector<std::string> outputs = {};

        for(int i = colon + 1; i < n; ++i){
            if(line[i] == ' ')
                outputs.push_back("");

            else outputs.back() += line[i];
        }

        for(const std::string child : outputs)
            reverse_graph[child].push_back(input);

        graph[input] = std::move(outputs);
    }

    std::unordered_set<std::string> allowed = find_allowed(graph, reverse_graph);
    
    std::unordered_map<std::string, long long> explored = { {"out", 1} };

    std::cout << "Result for part 1: " << dfs1(graph, "you") << "\n";
    std::cout << "Result for part 2: " << dfs2(graph, explored, allowed, "svr");
}