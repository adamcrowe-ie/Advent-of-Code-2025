#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <stack>


const int DIRS[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

// utilise '@' in binary is 64 and '.' is < 64, therefore 7th bit is a bitmask 
const int PRINTER = '@';


void fill_grid(std::vector<std::string>& grid){
    std::fstream file("grid.txt");

    if(!file.is_open())
        throw std::runtime_error("Couldn't open file.");

    std::string line;
    
    while(std::getline(file, line)){
        for(char& ch : line)
            ch &= PRINTER;

        grid.push_back(line);
    }

    int rows = grid.size();
    int cols = grid[0].size();

    for(int i = 0; i < rows; ++i)
    for(int j = 0; j < cols; ++j){
        if(!(grid[i][j] & PRINTER)) continue;

        for(const auto [di, dj] : DIRS){
            if(i + di < 0 || i + di >= rows) continue;
            if(j + dj < 0 || j + dj >= cols) continue;

            ++grid[i + di][j + dj];
        }
    }
}


int main(){
    std::vector<std::string> grid;
    fill_grid(grid);

    int rows = grid.size();
    int cols = grid[0].size();

    std::stack<std::pair<int, int>> st;

    for(int i = 0; i < rows; ++i)
    for(int j = 0; j < cols; ++j)
        if((grid[i][j] & PRINTER) && (grid[i][j] ^ PRINTER) <= 3)
            st.emplace(i, j);

    int result1 = st.size();
    int result2 = 0;

    while(!st.empty()){
        const auto [i, j] = st.top();
        st.pop();

        if(!(grid[i][j] & PRINTER)) continue;


        grid[i][j] &= ~PRINTER;
        ++result2;

        for(const auto [di, dj] : DIRS){
            if(i + di < 0 || i + di >= rows) continue;
            if(j + dj < 0 || j + dj >= cols) continue;

            char& sq = grid[i + di][j + dj];

            if((sq & PRINTER) && (sq ^ PRINTER) <= 4)
                st.emplace(i + di, j + dj);

            else --sq;
        }
    }

    std::cout << "Result for part 1: " << result1 << "\n";
    std::cout << "Result for part 2: " << result2;
}