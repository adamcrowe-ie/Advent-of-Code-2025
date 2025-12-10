#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <numeric>
#include <functional>


using ll = long long;

void get_lines(std::vector<std::string>& lines, std::string& operations){
    std::ifstream file("homework.txt");

    if(!file)
        throw std::runtime_error("Couldn't open file.");

    std::string line;
    
    while(std::getline(file, line))
        lines.push_back(line);

    operations = lines.back();
    lines.pop_back();
}


ll fold_nums(std::vector<int>& nums, char operation){
    if(operation == '+')
        return std::accumulate(nums.begin(), nums.end(), 0LL);

    return std::accumulate(nums.begin(), nums.end(), 1LL, std::multiplies<ll>());
}


ll process_horizontal(std::vector<std::string>& lines, std::string& operations){
    int n = lines[0].size(), num_lines = lines.size();
    ll result = 0LL;

    char operation;
    std::vector<int> nums(num_lines, 0);

    for(int i = 0; i < n; ++i){
        bool new_sum = true;

        for(int line = 0; line < num_lines; ++line){
            if(lines[line][i] == ' ') continue;

            nums[line] *= 10;
            nums[line] += lines[line][i] - '0';
            new_sum = false;
        }

        if(operations[i] != ' ')
            operation = operations[i];

        if(new_sum){
            result += fold_nums(nums, operation);
            nums = std::vector(num_lines, 0);
        }
    }

    return result + fold_nums(nums, operation);
}


ll process_vertical(std::vector<std::string>& lines, std::string& operations){
    int n = lines[0].size();
    ll result = 0LL;

    char operation;
    std::vector<int> nums;

    for(int i = 0; i < n; ++i){
        int num = 0;

        for(const std::string& line : lines){
            if(line[i] == ' ') continue;

            num *= 10;
            num += line[i] - '0';
        }

        if(operations[i] != ' ')
            operation = operations[i];

        
        if(num == 0){
            result += fold_nums(nums, operation);
            nums.clear();
            continue;
        }

        nums.push_back(num);
    }

    return result + fold_nums(nums, operation);
}


int main(){
    std::vector<std::string> lines;
    std::string operations;

    get_lines(lines, operations);

    std::cout << "Result for part 1: " << process_horizontal(lines, operations) << "\n";
    std::cout << "Result for part 2: " << process_vertical(lines, operations);
}