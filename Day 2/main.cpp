#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_set>


const std::string input = "132454-182049,42382932-42449104,685933-804865,5330496-5488118,21-41,289741-376488,220191-245907,49-70,6438484-6636872,2-20,6666660113-6666682086,173-267,59559721-59667224,307-390,2672163-2807721,658272-674230,485679-647207,429-552,72678302-72815786,881990-991937,73-111,416063-479542,596-934,32825-52204,97951700-98000873,18335-27985,70203-100692,8470-11844,3687495840-3687599608,4861-8174,67476003-67593626,2492-4717,1442-2129,102962-121710,628612213-628649371,1064602-1138912";

using ll = long long;

static std::vector<ll> pow10 = {1};
static std::vector<std::vector<ll>> coef_mtx = {{}};
static std::unordered_set<ll> seen1, seen2;


template<typename T>
requires std::is_arithmetic_v<T>
inline T ceil_div(T p, T q){
    return (p + q - 1) / q;
}


std::pair<ll, int> extract(int& i, char end){
    int n = input.size(), start = i;
    ll number = 0LL;

    for(; i < n && input[i] != end; ++i)
        number = (number * 10) + (input[i] - '0');

    return {number, i - start};
}


void expand_coef_mtx(int new_len){
    // expand pow10 as necessary
    while(pow10.size() * 2 <= new_len)
        pow10.push_back(pow10.back() * 10);


    // expand coef matrix vertically
    while(coef_mtx.size() * 2 <= new_len)
        coef_mtx.push_back({1, 1});

    
    int n = coef_mtx.size();

    // expand coef matrix horizontally
    for(int len = 1; len < n; ++len){
        int reps = coef_mtx[len].size();

        while(reps * len <= new_len){
            ll new_coef = (coef_mtx[len][reps - 1] * pow10[len]) + 1;

            coef_mtx[len].push_back(new_coef);
            ++reps;
        }
    }
}

ll find_invalid1(ll start, int start_len, ll end, int end_len){
    ll result = 0LL;

    for(int len = ceil_div(start_len, 2); 2 * len <= end_len; ++len){
        const ll coef = coef_mtx[len][2];
        const int pref = std::max(ceil_div(start, coef), pow10[len - 1]);
        const ll lim = std::min(end + 1, coef * pow10[len]);

        for(ll prod = coef * pref; prod < lim; prod += coef){
            if(seen1.contains(prod)) continue;

            result += prod;
            seen1.insert(prod);
        }
    }

    return result;
}


ll find_invalid2(ll start, int start_len, ll end, int end_len){
    ll result = 0LL;

    // iterate through all prefix lengths
    for(int len = 1; 2 * len <= end_len; ++len){
        int reps = std::max(ceil_div(start_len, len), 2);

        for(; reps * len <= end_len; ++reps){
            const ll coef = coef_mtx[len][reps];
            const int pref = std::max(ceil_div(start, coef), pow10[len - 1]);
            const ll lim = std::min(end + 1, coef * pow10[len]);

            for(ll prod = coef * pref; prod < lim; prod += coef){
                if(seen2.contains(prod)) continue;

                result += prod;
                seen2.insert(prod);
            }
        }
    }

    return result;
}


int main(){
    int n = input.size(), coef_mtx_capacity = 0;
    ll result1 = 0LL, result2 = 0LL;

    pow10.reserve(10);
    coef_mtx.reserve(10);

    for(int i = 0; i < n; ++i){
        auto [start, start_len] = extract(i, '-');
        auto [end, end_len] = extract(++i, ',');

        if(end_len > coef_mtx_capacity){
            expand_coef_mtx(end_len);
            coef_mtx_capacity = end_len;
        }
        
        result1 += find_invalid1(start, start_len, end, end_len);
        result2 += find_invalid2(start, start_len, end, end_len);
    }

    std::cout << "Result for part 1: " << result1 << "\n";
    std::cout << "Result for part 2: " << result2;
}