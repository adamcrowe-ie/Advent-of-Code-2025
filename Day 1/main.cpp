#include <vector>
#include <fstream>
#include <string>
#include <iostream>

std::vector<int> get_turns(){
    std::ifstream file("document.txt");

    if(!file.is_open())
        throw std::runtime_error("Couldn't open file.");

    std::string line;
    std::vector<int> turns;

    while(std::getline(file, line)){;
        int sign = (line[0] == 'R' ? 1 : -1);
        turns.push_back(sign * std::stoi(line.substr(1)));
    }
    
    return turns;
}


int main(){
    int dial = 50, lands_on_zero = 0, crosses_zero = 0;
    std::vector<int> turns = get_turns();

    for(const int turn : turns){
        dial += turn; 

        if(dial >= 100){
            crosses_zero += (dial - 1) / 100;
            dial %= 100;
        }

        if(dial < 0){
            crosses_zero += (dial - turn > 0) - (dial + 1) / 100;
            dial %= 100;
            if(dial < 0) dial += 100;
        }

        if(dial == 0) ++lands_on_zero;
    }

    std::cout << "Lands on zero " << lands_on_zero << " times.\n";
    std::cout << "Crosses zero " << crosses_zero << " times.\n";
    std::cout << "In total " << lands_on_zero + crosses_zero << " time.";
}