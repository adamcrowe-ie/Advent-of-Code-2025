#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <functional>
#include <queue>


using ll = long long;
using Comp = std::greater<std::pair<ll, int>>;
using PQ = std::priority_queue<std::pair<ll, int>, std::vector<std::pair<ll, int>>, Comp>;


void fill_pq(PQ& pq){
    std::fstream file("IDs.txt");

    if(!file.is_open())
        throw std::runtime_error("Couldn't open file.");

    std::string line;

    // get ranges
    while(std::getline(file, line)){
        if(line.empty())
            break;

        int i = line.find('-');

        ll start = std::stoll(line.substr(0, i));
        ll end = std::stoll(line.substr(i + 1));

        pq.emplace(start, 1);
        pq.emplace(end + 1, -1);
    }

    // get individual IDs
    while(std::getline(file, line))
        pq.emplace(std::stoll(line), 0);
}


int main(){ 
    PQ pq;
    fill_pq(pq);

    int result1 = 0;
    ll result2 = 0LL;

    int open = 0;
    ll start;

    while(!pq.empty()){
        const auto [i, aux] = pq.top();
        pq.pop();

        switch(aux){
            case 0: {
                if(open)
                    ++result1;

                break;
            }

            case 1: {
                if(!open)
                    start = i;

                break;
            }

            case -1: {
                if(open == 1)
                    result2 += i - start;

                break;
            }
        }

        open += aux;
    }

    std::cout << "Result for part 1: " << result1 << "\n";
    std::cout << "Result for part 2: " << result2;
}