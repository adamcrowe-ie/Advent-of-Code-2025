#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>


using ll = long long;


ll get_joltage(const std::string& battery, int len){
    const int n = battery.size();
    ll result = 0LL;

    std::vector<int> digit;
    digit.reserve(len);

    std::string::const_iterator it = battery.begin() - 1;

    for(int i = 0; i < len; ++i){
        it = std::max_element(it + 1, battery.begin() + n - len + i + 1);
        digit.push_back(*it - '0');
    }

    for(int i = 0; i < digit.size(); ++i){
        result *= 10LL;
        result += digit[i];
    }

    return result;
}


int main(){
    std::fstream file("batteries.txt");

    if(!file.is_open())
        throw std::runtime_error("Couldn't open file.");

    int result1 = 0;
    ll result2 = 0LL;
    std::string line;

    while(std::getline(file, line)){
        result1 += get_joltage(line, 2);
        result2 += get_joltage(line, 12);
    }

    std::cout << "Result for part 1: " << result1 << "\n";
    std::cout << "Result for part 2: " << result2;
}