#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <array>
#include <numeric>
#include <map>
#include <unordered_map>


constexpr int MAX_BUTTONS = 10;


std::pair<int, int> get_target(const std::string& s){
    int target = 0;
    int i = 1;

    for(; s[i] != ']'; ++i)
        target |= (s[i] == '#') << (i - 1);

    return {target, i - 1};
}


std::vector<std::vector<int>> get_buttons(const std::string& s){
    std::vector<std::vector<int>> buttons;
    buttons.reserve(MAX_BUTTONS);

    for(int i = s.find('('); s[i] != '{'; ++i){
        if(s[i] == '(')
            buttons.push_back({});
        
        else if(s[i] >= '0' && s[i] <= '9')
            buttons.back().push_back(s[i] - '0');
    }

    return buttons;
}


std::vector<int> get_joltages(const std::string& s, const int num_lights){
    std::vector<int> joltages(num_lights, 0);

    for(int i = s.find('{') + 1, light = 0; s[i] != '}'; ++i){
        if(s[i] == ','){
            ++light;
            continue;
        }
        
        joltages[light] *= 10;
        joltages[light] += s[i] - '0';
    }

    return joltages;
}


std::unordered_map<int, std::vector<int>> find_combinations(const std::vector<std::vector<int>>& buttons){
    int num_buttons = buttons.size();
    int num_combinations = (1 << num_buttons);

    std::unordered_map<int, std::vector<int>> combination_map;

    for(int combination = 0; combination < num_combinations; ++combination){
        int result = 0;

        for(int button = 0; button < num_buttons; ++button){
            if(!(combination & (1 << button)))
                continue;

            for(const int light : buttons[button])
                result ^= 1 << light;
        }

        combination_map[result].push_back(combination);
    }

    return combination_map;
}



int find_target(const std::unordered_map<int, std::vector<int>>& combination_map, const int target){
    int result = INT_MAX;

    for(const int combination : combination_map.at(target))
        result = std::min(result, __builtin_popcount(combination));

    return result;
}


long long bifurcate(    const   std::vector<std::vector<int>>& buttons,
                        const   std::vector<int>& joltages,
                                std::unordered_map<int, std::vector<int>>& combination_map,
                                std::map<std::vector<int>, int>& explored                      )
{
    if(explored.contains(joltages))
        return explored[joltages];

    int num_lights = joltages.size();
    int num_buttons = buttons.size();

    int target = 0;
    long long result = INT_MAX;

    for(int light = 0; light < num_lights; ++light)
        target |= (joltages[light] & 1) << light;

    for(const int combination : combination_map[target]){
        int presses = __builtin_popcount(combination);
        std::vector<int> new_joltages = joltages;

        for(int button = 0; button < num_buttons; ++button){
            if(!(combination & (1 << button)))
                continue;

            for(const int light : buttons[button])
                if(--new_joltages[light] < 0)
                    goto next_combination;
        }

        for(int& joltage : new_joltages)
            joltage /= 2;

        result = std::min(result, presses + 2LL * bifurcate(buttons, new_joltages, combination_map, explored));

        next_combination:
    }

    return explored[joltages] = result;
}


int main(){
    std::ifstream file("manuals.txt");

    if(!file) throw std::runtime_error("Couldn't open file.");

    std::string line;
    int result1 = 0;
    long long result2 = 0LL;

    while(std::getline(file, line)){
        const auto [target, num_lights] = get_target(line);
        std::vector<std::vector<int>> buttons = get_buttons(line);
        std::vector<int> joltages = get_joltages(line, num_lights);

        std::unordered_map<int, std::vector<int>> combinations = find_combinations(buttons);
        std::map<std::vector<int>, int> explored = { {std::vector<int>(num_lights, 0), 0} };

        result1 += find_target(combinations, target);
        result2 += bifurcate(buttons, joltages, combinations, explored);
    }

    std::cout << "Result for part 1: " << result1 << "\n";
    std::cout << "Result for part 2: " << result2;
}