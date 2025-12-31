#include <fstream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <set>
#include <iterator>
#include <stack>


using Coord = std::pair<int, int>;

constexpr int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};


std::vector<Coord> get_tiles(){
    std::ifstream file("tiles.txt");

    if(!file) throw std::runtime_error("Couldn't open file.");

    std::string line;
    std::vector<Coord> tiles;
    tiles.reserve(500);

    while(std::getline(file, line)){
        int i = line.find(',');

        int x = std::stoi(line.substr(0, i));
        int y = std::stoi(line.substr(i + 1));

        tiles.emplace_back(x, y);
    }

    // add first tile to end to create loop
    tiles.emplace_back(tiles[0]);

    return tiles;
}


std::vector<Coord> compress_coords(const std::vector<Coord>& tiles){
    std::set<int> rows;
    std::set<int> cols;

    for(const auto [r, c] : tiles){
        rows.insert(r);
        cols.insert(c);
    }

    int n = tiles.size();
    std::vector<Coord> tiles_compressed;
    tiles_compressed.reserve(n);

    for(int i = 0; i < n; ++i){
        int r = std::distance(rows.begin(), rows.find(tiles[i].first));
        int c = std::distance(cols.begin(), cols.find(tiles[i].second));

        tiles_compressed.emplace_back(r, c);
    }

    return tiles_compressed;
}


std::vector<std::vector<char>> make_grid(const std::vector<Coord>& tiles_compressed){
    int rows = 0;
    int cols = 0;

    for(const auto [r, c] : tiles_compressed){
        rows = std::max(rows, r + 1);
        cols = std::max(cols, c + 1);
    }

    int n = tiles_compressed.size();
    std::vector<std::vector<char>> grid(rows, std::vector<char>(cols, 0));

    for(int i = 1; i < n; ++i){
        Coord st = tiles_compressed[i - 1];
        Coord end = tiles_compressed[i];

        // moving vertical
        if(st.second == end.second){
            int c = st.second;
            int mn = std::min(st.first, end.first);
            int mx = std::max(st.first, end.first);

            for(int r = mn; r <= mx; ++r)
                grid[r][c] = 1;
        }

        // moving horizontal
        else{
            int r = st.first;
            int mn = std::min(st.second, end.second);
            int mx = std::max(st.second, end.second);

            for(int c = mn; c <= mx; ++c)
                grid[r][c] = 1;
        }
    }

    return grid;
}


// could be optimized

void fill_exterior(const std::vector<Coord>& tiles_compressed, std::vector<std::vector<char>>& grid){
    int rows = grid.size();
    int cols = grid[0].size();

    std::stack<Coord> st;

    // add exterior cells to stack
    for(int r = 0; r < rows; ++r){
        if(!grid[r][0]) st.emplace(r, 0);
        if(!grid[r][cols - 1]) st.emplace(r, cols - 1);
    }

    for(int c = 0; c < cols; ++c){
        if(!grid[0][c]) st.emplace(0, c);
        if(!grid[rows - 1][c]) st.emplace(rows - 1, c);
    }

    while(!st.empty()){
        const auto [r, c] = st.top();
        st.pop();

        if(grid[r][c])
            continue;

        grid[r][c] = 2;

        for(const auto [dr, dc] : dirs){
            if(r + dr < 0 || r + dr >= rows || c + dc < 0 || c + dc >= cols || grid[r + dr][c + dc])
                continue;

            st.emplace(r + dr, c + dc);
        }
    }
}


inline long long calc_area(const Coord a, const Coord b){
    long long x_dist = std::abs(a.first - b.first) + 1;
    long long y_dist = std::abs(a.second - b.second) + 1;

    return x_dist * y_dist;
}


inline bool in_bounds(const Coord tile1, const Coord tile2, const std::vector<std::vector<int>>& sum_table){
    int r_min = std::min(tile1.first, tile2.first);
    int r_max = std::max(tile1.first, tile2.first);

    int c_min = std::min(tile1.second, tile2.second);
    int c_max = std::max(tile1.second, tile2.second);

    int exterior_points = sum_table[r_max][c_max];

    if(c_min) exterior_points -= sum_table[r_max][c_min - 1];
    if(r_min) exterior_points -= sum_table[r_min - 1][c_max];
    if(c_min && r_min) exterior_points += sum_table[r_min - 1][c_min - 1];

    return exterior_points == 0;
}


std::vector<std::vector<int>> make_sum_table(const std::vector<std::vector<char>>& grid){
    int rows = grid.size();
    int cols = grid[0].size();

    std::vector<std::vector<int>> sum_table(rows, std::vector<int>(cols, 0));

    sum_table[0][0] = (grid[0][0] == 2);

    for(int r = 1; r < rows; ++r)
        sum_table[r][0] = sum_table[r - 1][0] + (grid[r][0] == 2);

    for(int c = 1; c < cols; ++c)
        sum_table[0][c] = sum_table[0][c - 1] + (grid[0][c] == 2);

    for(int r = 1; r < rows; ++r)
    for(int c = 1; c < cols; ++c)
        sum_table[r][c] = sum_table[r - 1][c] + sum_table[r][c - 1] - sum_table[r - 1][c - 1] + (grid[r][c] == 2);

    return sum_table;
}


int main(){
    std::vector<Coord> tiles = get_tiles();
    std::vector<Coord> tiles_compressed = compress_coords(tiles);
    std::vector<std::vector<char>> grid = make_grid(tiles_compressed);      // use char for space
    fill_exterior(tiles_compressed, grid);
    std::vector<std::vector<int>> sum_table = make_sum_table(grid);         // sum table allows us to compute if rect in bounds in const time

    // no longer need looping, so we can pop back
    tiles.pop_back();
    tiles_compressed.pop_back();

    long long result1 = 0LL, result2 = 0LL;
    int n = tiles.size();

    for(int i = 0; i < n - 1; ++i)
    for(int j = i + 1; j < n; ++j){
        long long area = calc_area(tiles[i], tiles[j]);

        result1 = std::max(result1, area);

        if(area > result2 && in_bounds(tiles_compressed[i], tiles_compressed[j], sum_table))
            result2 = area;
    }

    std::cout << "Result for part 1: " << result1 << "\n";
    std::cout << "Result for part 2: " << result2;
}