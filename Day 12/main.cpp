#include <fstream>
#include <string>
#include <array>
#include <numeric>
#include <iostream>

constexpr int NUM_PRESENTS = 6;

std::array<int, NUM_PRESENTS> get_present_densities(std::ifstream& file){
    std::array<int, NUM_PRESENTS> densities = {};
    std::string line;

    for(int present = 0; present < NUM_PRESENTS; ++present){
        std::getline(file, line);       // header line

        for(int r = 0; r < 3; ++r){
            std::getline(file, line);

            for(const char ch : line)
                if(ch == '#')
                    ++densities[present];
        }
        
        std::getline(file, line);       // blank line
    }

    return densities;
}


const std::array<int, NUM_PRESENTS> get_presents(const std::string& line){
    int present = 0;
    std::array<int, NUM_PRESENTS> presents = {};

    for(const char ch : line){
        if(ch == ' '){
            ++present;
            continue;
        }

        presents[present] *= 10;
        presents[present] += ch - '0';
    }

    return presents;
}


const std::tuple<int, int, std::array<int, NUM_PRESENTS>> parse_region(const std::string& line){
    const int n = line.size();

    const int x = line.find('x');
    const int colon = line.find(':');

    const int w = std::stoi(line.substr(0, x));
    const int h = std::stoi(line.substr(x + 1, colon - x));
    
    const std::array<int, NUM_PRESENTS> presents = get_presents(line.substr(colon + 2));

    return {w, h, presents};
}


int main(){
    std::ifstream file("presents.txt");

    if(!file) throw std::runtime_error("Couldn't open file.");

    int result = 0;
    std::string line;
    std::array<int, NUM_PRESENTS> densities = get_present_densities(file);

    while(std::getline(file, line)){
        const auto [w, h, presents] = parse_region(line);

        const int sum_presents = std::accumulate(presents.begin(), presents.end(), 0);

        if((w / 3) * (h / 3) >= sum_presents){
            ++result;
            continue;
        }

        int area = 0;

        for(int present = 0; present < NUM_PRESENTS; ++present)
            area += densities[present] * presents[present];

        if(area <= w * h)
            throw std::runtime_error("Present packing may/may not be possible, but cannot be determined by this algorithm");
    }

    std::cout << "Result for part 1: " << result;
}