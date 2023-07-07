#include "Heap/binaryHeap/binHeap.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <limits>
#include <set>
#include <tuple>
#include <chrono>
#include <queue>
#include <random>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <array>




using pii = std::pair<int, int>;
using vdd = std::vector<std::vector<double>>;
double sqrt2 = sqrt(2);

struct exploreResult {
    std::vector<std::string> map;
    std::vector<std::pair<int, int>> targets;
    long timeElapsed;
    size_t explored;
    exploreResult(std::vector<std::string> map, std::vector<std::pair<int, int>> targets, long timeElapsed, size_t explored) :
                  map(map), targets(targets), timeElapsed(timeElapsed), explored(explored) {}
};

double calHeu(pii &_node, pii &_target, const std::string &h="euc")
{
    if (!h.compare("euc"))
        return sqrt((_node.first - _target.first) * (_node.first - _target.first) + (_node.second - _target.second) * (_node.second - _target.second));
    else if(!h.compare("euc2"))
        // Instead of using sqrt, we can just compute the square to optimize the performance
        return (_node.first - _target.first) * (_node.first - _target.first) + (_node.second - _target.second) * (_node.second - _target.second);
    else if (!h.compare("mht"))
        return abs(_node.first - _target.first) + abs(_node.second - _target.second);
    else if (!h.compare("che"))
        return std::max(abs(_node.first - _target.first), abs(_node.second - _target.second));
    else if(!h.compare("dia"))
    {
        double dx = abs(_node.first - _target.first);
        double dy = abs(_node.second - _target.second);
        return (dx + dy) + ((sqrt2 - 2) * std::min(dx, dy));
    }
    else if (!h.compare("0"))
        return 0;
    return 0;
}

std::tuple<std::vector<pii>, std::vector<std::vector<bool>>, size_t, double, long long> 
AStarBin(const std::vector<std::string> &_map, pii &_start, pii &_target, const std::string &h="euc", const int &dir=8)
{
    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    size_t row = _map.size(), column = _map[0].size();

    binHeap<std::pair<double, pii>> prQ(std::make_pair(0.0, _start));
    pii null_ = {-1, -1}, node, child;
    vdd f_cost(row, std::vector<double>(column, std::numeric_limits<double>::infinity()));

    // std::unordered_map<pii, pii, piiHash> parent = {{_start, null_}};
    std::vector<std::vector<pii>> parent(row, std::vector<pii>(column, null_));
    
    // std::unordered_set<pii, piiHash, piiEqual> calculated = {_start};
    std::vector<std::vector<bool>> calculated(row, std::vector<bool>(column, false));
    size_t step = 0;

    // std::unordered_map<pii, double, piiHash> prev_cost = {{_start, 0.0}};
    std::vector<std::vector<double>> g_cost(row, std::vector<double>(column, 0.0));
    
    std::vector<pii> direction = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    double heu, prev, cost = 1.0;
    bool penalty = false;
    // std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    if (!h.compare("euc") || !h.compare("euc2"))
        penalty = true;

    while (!prQ.empty())
    {   
        node = prQ.pop().second;
        // std::cout << node.first << "    " << node.second << std::endl;
        if (node == _target) break;

        // calculated.insert(node);
        calculated[node.first][node.second] = true;
        step++;
        // prev = prev_cost[node];
        prev = g_cost[node.first][node.second];
                
        for (size_t idx = 0; idx < dir; idx++) {
            child = {direction[idx].first + node.first, direction[idx].second + node.second};
            if ((0 <= child.first && child.first < row && 0 <= child.second && child.second < column) && !(_map[child.first][child.second] == 'W') && !(_map[child.first][child.second] == 'N')) {            
                // if (!calculated[child.first][child.second])
                // if (!calculated.count(child))
                {
                    cost = ((idx > 3) && penalty) ? 1.41 : 1.0;
                    // cost = child.cost Modify here if the cost is not a constant
                    if (!calculated[child.first][child.second] || prev + cost < g_cost[child.first][child.second]) {
                        heu = calHeu(child, _target, h);
                        prQ.push(std::make_pair(heu + prev + cost, child));
                        f_cost[child.first][child.second] = heu + prev;
                        // parent[child] = node;
                        parent[child.first][child.second] = node;
                        // prev_cost[child] = prev + cost;
                        g_cost[child.first][child.second] = prev + cost;
                        // calculated.insert(child);
                        // if (!calculated[child.first][child.second])
                        // if (!calculated[child.first][child.second]) step++;
                        calculated[child.first][child.second] = true;
                    }
                }
            }
        }
    }
    // std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::vector<pii> route;
    double total_cost = g_cost[_target.first][_target.second];
    // double total_cost = prev_cost[_target];
    pii temp = _target;
    while (temp != null_)
    {
        route.insert(route.begin(), temp);
        // temp = parent[temp];
        temp = parent[temp.first][temp.second];
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    return std::make_tuple(route, calculated, step, total_cost, std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count());

}


exploreResult dfs(std::vector<std::string> &_map, pii _start, int targetCNT, int dir=8)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::stack<pii> stack;
    stack.emplace(_start);
    size_t row = _map.size(), column = _map[0].size();
    // std::unordered_set<pii, piiHash, piiEqual> explored = {_start};
    std::vector<std::vector<bool>> explored(row, std::vector<bool>(column, false));
    std::vector<std::string> map_(row, std::string(column, 'W'));


    pii node;
    size_t x, y, cnt = 0;
    std::vector<pii> direction = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1},
    };
    
    std::shuffle(direction.begin(), direction.begin() + dir, std::random_device());

    std::vector<std::pair<int, int>> targets;
    targets.reserve(targetCNT);

    while (!stack.empty())
    {
        node = stack.top();
        stack.pop();
        if (_map[node.first][node.second] == 'T') {
            targets.push_back({node.first, node.second});
            if (targets.size() == targetCNT)
                break;
        }

        explored[node.first][node.second] = true;
        for (int idx = 0; idx < dir; idx++) {
            // child = {direction[idx].first + node.first, direction[idx].second + node.second};
            x = direction[idx].first + node.first, y = direction[idx].second + node.second;
            {
            // if (((0 <= child.first && child.first < _map.size() && 0 <= child.second && child.second < _map[0].size())) && (!(_map[child.first][child.second] == 'W')))
            if (((0 <= x && x < row && 0 <= y && y < column)) && (!(_map[x][y] == 'W'))) {   
                // if (!explored[child.first][child.second])
                if (!explored[x][y]) {
                    stack.push({x, y});
                    explored[x][y] = true;
                    map_[x][y] = '.';
                    cnt++;
                    // explored[child.first][child.second] = true;
                    } 
                }
            }
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // std::cout << "DFS Time : " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " microseconds" << std::endl;

    // std::cout << "Explored : " << cnt << std::endl;
    
    // for (const pii &exp : explored)
    // {
    //     map_[exp.first][exp.second] = '.';
    // }
    for (size_t idx = 0; idx < explored.size(); idx++) {
        for (size_t jdx = 0; jdx < explored[0].size(); jdx++) {
            if ((!explored[idx][jdx]) && (!(_map[idx][jdx] == 'W'))) {
                map_[idx][jdx] = 'N';
            }
        }
    }
    for (const std::pair<int, int> &t : targets)
    {
        map_[t.first][t.second] = 'T';
        // std::cout << "TARGET FOUND AT : " << t.first << " " << t.second << std::endl;
    }
    
    return exploreResult(map_, targets, std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count(), cnt);
}

exploreResult bfs(std::vector<std::string> &_map, pii _start, int targetCNT, int dir=8)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::queue<pii> queue;
    queue.push(_start);
    size_t row = _map.size(), column = _map[0].size();
    // std::unordered_set<pii, piiHash, piiEqual> explored = {_start};
    std::vector<std::vector<bool>> explored(row, std::vector<bool>(column, false));
    std::vector<std::string> map_(row, std::string(column, 'W'));


    pii node;
    size_t x, y, cnt = 0;
    std::vector<pii> direction = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1},
    };
    std::shuffle(direction.begin(), direction.begin() + dir, std::random_device());

    std::vector<std::pair<int, int>> targets;

    while (!queue.empty())
    {
        node = queue.front();
        queue.pop();
        if (_map[node.first][node.second] == 'T') {
            targets.push_back({node.first, node.second});
            if (targets.size() == targetCNT)
                break;
        }

        explored[node.first][node.second] = true;
        for (size_t idx = 0; idx < dir; idx++) {
            // child = {direction[idx].first + node.first, direction[idx].second + node.second};
            x = direction[idx].first + node.first, y = direction[idx].second + node.second;
            {
            // if (((0 <= child.first && child.first < _map.size() && 0 <= child.second && child.second < _map[0].size())) && (!(_map[child.first][child.second] == 'W')))
            if (((0 <= x && x < row && 0 <= y && y < column)) && (!(_map[x][y] == 'W')))

                {   
                // if (!explored[child.first][child.second])
                if (!explored[x][y])
                    {
                        queue.emplace(std::make_pair(x, y));
                        explored[x][y] = true;
                        map_[x][y] = '.';
                        cnt++;
                        // explored[child.first][child.second] = true;
                    }
                }
            }
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // std::cout << "BFS Time : " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " microseconds" << std::endl;


    // std::cout << "Explored : " << cnt << std::endl;
    // for (const pii &exp : explored)
    // {
    //     map_[exp.first][exp.second] = '.';
    // }
    for (size_t idx = 0; idx < explored.size(); idx++) {
        for (size_t jdx = 0; jdx < explored[0].size(); jdx++) {
            if ((!explored[idx][jdx]) && (!(_map[idx][jdx] == 'W'))) {
                map_[idx][jdx] = 'N';
            }
        }
    }
    for (const std::pair<int, int> &t : targets)
    {
        map_[t.first][t.second] = 'T';
        // std::cout << "TARGET FOUND AT : " << t.first << " " << t.second << std::endl;
    }
    return exploreResult(map_, targets, std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count(), cnt);
}



// Optimization : 
//      Simple, fast and good hash function if you are using unordered_set since memory usage in concerned.
//      Use more advanced heap data structure such as Fibonacci Heap.