#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>


using ll = long long;
using UMap = std::unordered_map<int, ll>;
using UMapPtr = std::unique_ptr<UMap>;


int main(){
    std::ifstream file("tachyon.txt");

    if(!file)
        throw std::runtime_error("Couldn't open file.");

    int splits = 0;

    std::string line;
    UMapPtr beams_ptr(new UMap);    // we need a new map after each iteration, so pointer avoids copy constructions

    while(std::getline(file, line)){
        if(beams_ptr -> empty()){
            (*beams_ptr)[line.find('S')] = 1LL;
            continue;
        }

        UMapPtr new_beams_ptr(new UMap);
        new_beams_ptr -> reserve(std::min(2 * beams_ptr -> size(), line.size()));

        for(const auto [pos, count] : *beams_ptr){
            if(line[pos] == '.'){
                (*new_beams_ptr)[pos] += count;
                continue;
            }

            (*new_beams_ptr)[pos - 1] += count;
            (*new_beams_ptr)[pos + 1] += count;
            ++splits;
        }
    
        beams_ptr = std::move(new_beams_ptr);
    }

    ll timelines = 0LL;

    for(const auto [_, count] : *beams_ptr)
        timelines += count;

    std::cout << "Result for part 1: " << splits << "\n";
    std::cout << "Result for part 2: " << timelines;
}