#include "func.hpp"
#include <bits/stdc++.h>
#include "algorithm.hpp"
#include "TSP Algorithm/antColonyOptimization.hpp"
#include "TSP Algorithm/geneticAlgorithm.hpp"
#include "TSP Algorithm/simulatedAnnealing.hpp"



using namespace std;
using pii = pair<int, int>;

struct timeitResult {
    string fileName;
    long timeElapsed;
    double totalDistance;
    size_t calculated;
    timeitResult(string fileName, long timeElapsed, double totalDistance, size_t calculated) :
                 fileName(fileName), timeElapsed(timeElapsed),
                 totalDistance(totalDistance), calculated(calculated) {}
};

template<typename T>
timeitResult timeit(string filename, pair<int, int> &SOURCE, vector<pair<int, int>> &TARGETS, T f, vector<string> &_map, double &dist, vector<pair<int, int>> &bestPermutation, string h="euc", int dir = 8)
{
    pii t = SOURCE;
    long long total_time = 0;
    double total_distance = 0;
    vector<vector<bool>> prev_calculated(_map.size(), vector<bool>(_map[0].size(), false));
    vector<vector<bool>> calculated(_map.size(), vector<bool>(_map[0].size(), false));
    size_t calc = 0;
    tuple<vector<pii>, vector<vector<bool>>, size_t, double, long long> result;
    vector<pii> route_;

    for (pii TARGET : TARGETS)
    {
        result = f(_map, t, TARGET, h, dir);
        total_time += get<4>(result);
        total_distance += get<3>(result);
        calc += get<2>(result);
        prev_calculated = calculated;
        calculated = get<1>(result);

        for(size_t idx = 0; idx < calculated.size(); idx++) {
            for(size_t jdx = 0; jdx < calculated[0].size(); jdx++) {
                if (calculated[idx][jdx]) {
                    prev_calculated[idx][jdx] = true;
                }
            }
        }

        t = TARGET;
        route_.insert(route_.end(), get<0>(result).begin(), get<0>(result).end());
    }

    saveResult(filename + to_string(static_cast<int>(total_distance)) + ".txt", _map, route_, prev_calculated, TARGETS);
    if (total_distance < dist) {
        dist = total_distance;
        bestPermutation = TARGETS;
    }

    return timeitResult(filename, total_time, total_distance, calc);

}

int main(int argc, char **argv) {
    system("cls");
    if (argc < 2) {
        std::cerr << "No filename provided." << std::endl;
        return 1;
    }

    const char* filename;
    string heuristic = "mht";
    int direction = 8;
    const char* permute = "-n";
    const char* visualize = "-n";
    for (size_t idx = 1; idx < argc; idx++) {
        if (string(argv[idx]).find(".txt") != string::npos) filename = argv[idx];
        if (!strcmp(argv[idx], "euc")) heuristic = string(argv[idx]);
        if (!strcmp(argv[idx], "euc2")) heuristic = string(argv[idx]);
        if (!strcmp(argv[idx], "dia")) heuristic = string(argv[idx]);
        if (!strcmp(argv[idx], "che")) heuristic = string(argv[idx]);
        if (!strcmp(argv[idx], "dia")) heuristic = string(argv[idx]);
        if (!strcmp(argv[idx], "8")) direction = stoi(argv[idx]);
        if (!strcmp(argv[idx], "4")) direction = stoi(argv[idx]);
        if (!strcmp(argv[idx], "-v")) visualize = "-v";
        if (!strcmp(argv[idx], "-V")) visualize = "-V";
        if (!strcmp(argv[idx], "-p")) permute = "-p";
        if (!strcmp(argv[idx], "-P")) permute = "-P";
    }

    std::cout << "Filename : " << filename << std::endl;
    std::cout << "Heuristic : " << heuristic << std::endl;
    std::cout << "Direction : " << direction << std::endl;
    if (!strcmp(permute, "-p") || !strcmp(permute, "-P"))
        std::cout << "Permutation : " << "true" << std::endl;
    else
        std::cout << "Permutation : " << "false" << std::endl;
    if (!strcmp(visualize, "-v") || !strcmp(visualize, "-V")) {
        std::cout << "Visualization : " << "true" << std::endl;
    }
    else
        std::cout << "Visualization : " << "false" << std::endl;
    if (!strcmp(visualize, "-v") || !strcmp(visualize, "-V") || !strcmp(permute, "-p") || !strcmp(permute, "-P")) {
                system("python createDirectory.py");
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    pair<int, int> SOURCE;
    vector<pair<int, int>> TARGETS;

    vector<string> vec = readMap(filename);
    for (size_t idx = 0; idx < vec.size(); idx++) 
    {
        for (size_t jdx = 0; jdx < vec[idx].size(); jdx++)
        {
            if (vec[idx][jdx] == 'S') 
            {
                SOURCE.first = idx;
                SOURCE.second = jdx;
                cout << "Source found at : (" << idx << ", " << jdx << ")" << endl;
            }
            else if (vec[idx][jdx] == 'T') 
            {
                pair<int, int> TARGET = {idx, jdx};
                cout << "Target found at : (" << idx << ", " << jdx << ")" << endl;

                TARGETS.push_back(TARGET);
            }
        }
    }
    cout << "Number of Target : " << TARGETS.size() << endl;
    std::cout << std::endl;
    
    exploreResult dfs_ = dfs(vec, SOURCE, TARGETS.size(), direction);
    exploreResult bfs_ = bfs(vec, SOURCE, TARGETS.size(), direction);

    {
        vector<pair<int, int>> exploringOrder;
        cout << left;
        cout << setw(12) << "Name" << setw(15) << "Time (mirco)" << setw(15) << "Explored" << "Targets Seq" << endl;

        cout << setw(12) << "DFS" << setw(15) << dfs_.timeElapsed << setw(15) << dfs_.explored;
        exploringOrder = dfs_.targets;
        for (size_t cnt = 0; cnt < exploringOrder.size() - 1; cnt++) cout << "(" << exploringOrder[cnt].first << ", " << exploringOrder[cnt].second << ") "<< "-->";
        cout << "(" << exploringOrder[exploringOrder.size() - 1].first << ", " << exploringOrder[exploringOrder.size() - 1].second << ") "<< endl;

        cout << setw(12) << "BFS" << setw(15) << bfs_.timeElapsed << setw(15) << bfs_.explored;
        exploringOrder = bfs_.targets;
        for (size_t cnt = 0; cnt < exploringOrder.size() - 1; cnt++) cout << "(" << exploringOrder[cnt].first << ", " << exploringOrder[cnt].second << ") "<< "-->";
        cout << "(" << exploringOrder[exploringOrder.size() - 1].first << ", " << exploringOrder[exploringOrder.size() - 1].second << ") "<< endl;
    }
    cout << endl;

    vector<pair<int, int>>  COORDS = dfs_.targets;
    COORDS.push_back(SOURCE);
    auto aco_result_dfs = antColonyOptimization(COORDS, (int)(COORDS.size() * 2 / 3), 1, .5, 5, 0.9, 10, 100, heuristic, 1);
    auto ga_result_dfs = geneticAlgorithm(COORDS, 1000, 0.001, 100, 20, heuristic);
    auto sa_result_dfs = simulatedAnnealing(COORDS, 100000, 1e-5, 0.999, heuristic);
    {
        vector<int> visitingOrder;
        cout << left;
        cout << setw(12) << "Name" << setw(15) << "Time (mirco)" << "Sequence" << endl;

        cout << setw(12) << "ACO" << setw(15) << aco_result_dfs.timeElapsed;
        visitingOrder = aco_result_dfs.bestVisitingOrder;
        for (size_t cnt = 0; cnt < visitingOrder.size() - 1; cnt++) cout << "(" << COORDS[visitingOrder[cnt]].first << ", " << COORDS[visitingOrder[cnt]].second << ") "<< "-->";
        cout << "(" << COORDS[visitingOrder[visitingOrder.size() - 1]].first << ", " << COORDS[visitingOrder[visitingOrder.size() - 1]].second << ") "<< endl;

        cout << setw(12) << "GA" << setw(15) << ga_result_dfs.timeElapsed;
        visitingOrder = ga_result_dfs.bestVisitingOrder;
        for (size_t cnt = 0; cnt < visitingOrder.size() - 1; cnt++) cout << "(" << COORDS[visitingOrder[cnt]].first << ", " << COORDS[visitingOrder[cnt]].second << ") "<< "-->";
        cout << "(" << COORDS[visitingOrder[visitingOrder.size() - 1]].first << ", " << COORDS[visitingOrder[visitingOrder.size() - 1]].second << ") "<< endl;

        cout << setw(12) << "SA" << setw(15) << sa_result_dfs.timeElapsed;
        visitingOrder = sa_result_dfs.bestVisitingOrder;
        for (size_t cnt = 0; cnt < visitingOrder.size() - 1; cnt++) cout << "(" << COORDS[visitingOrder[cnt]].first << ", " << COORDS[visitingOrder[cnt]].second << ") "<< "-->";
        cout << "(" << COORDS[visitingOrder[visitingOrder.size() - 1]].first << ", " << COORDS[visitingOrder[visitingOrder.size() - 1]].second << ") "<< endl;

        cout << endl;
    }

    COORDS = bfs_.targets;
    COORDS.push_back(SOURCE);
    auto aco_result_bfs = antColonyOptimization(COORDS, (int)(COORDS.size() * 2 / 3), 1, .5, 5, 0.9, 10, 100, heuristic, 1);
    auto ga_result_bfs = geneticAlgorithm(COORDS, 1000, 0.001, 100, 20, heuristic);
    auto sa_result_bfs = simulatedAnnealing(COORDS, 100000, 1e-5, 0.999, heuristic);
    {
        vector<int> visitingOrder;
        cout << left;
        cout << setw(12) << "Name" << setw(15) << "Time (mirco)" << "Sequence" << endl;

        cout << setw(12) << "ACO" << setw(15) << aco_result_bfs.timeElapsed;
        visitingOrder = aco_result_bfs.bestVisitingOrder;
        for (size_t cnt = 0; cnt < visitingOrder.size() - 1; cnt++) cout << "(" << COORDS[visitingOrder[cnt]].first << ", " << COORDS[visitingOrder[cnt]].second << ") "<< "-->";
        cout << "(" << COORDS[visitingOrder[visitingOrder.size() - 1]].first << ", " << COORDS[visitingOrder[visitingOrder.size() - 1]].second << ") "<< endl;

        cout << setw(12) << "GA" << setw(15) << ga_result_bfs.timeElapsed;
        visitingOrder = ga_result_bfs.bestVisitingOrder;
        for (size_t cnt = 0; cnt < visitingOrder.size() - 1; cnt++) cout << "(" << COORDS[visitingOrder[cnt]].first << ", " << COORDS[visitingOrder[cnt]].second << ") "<< "-->";
        cout << "(" << COORDS[visitingOrder[visitingOrder.size() - 1]].first << ", " << COORDS[visitingOrder[visitingOrder.size() - 1]].second << ") "<< endl;

        cout << setw(12) << "SA" << setw(15) << sa_result_bfs.timeElapsed;
        visitingOrder = sa_result_bfs.bestVisitingOrder;
        for (size_t cnt = 0; cnt < visitingOrder.size() - 1; cnt++) cout << "(" << COORDS[visitingOrder[cnt]].first << ", " << COORDS[visitingOrder[cnt]].second << ") "<< "-->";
        cout << "(" << COORDS[visitingOrder[visitingOrder.size() - 1]].first << ", " << COORDS[visitingOrder[visitingOrder.size() - 1]].second << ") "<< endl;
        cout << endl;
    }



    vector<pair<int, int>> bfs_targets = bfs_.targets;
    vector<pair<int, int>> dfs_targets = dfs_.targets;
    vector<string> bfs_map = bfs_.map;
    vector<string> dfs_map = dfs_.map;
    saveMap("output/DFS_MAP.txt", dfs_map, SOURCE, TARGETS);
    saveMap("output/BFS_MAP.txt", bfs_map, SOURCE, TARGETS);



    double shortest = numeric_limits<double>::infinity();
    vector<pair<int, int>> SA_Order_DFS;
    for (size_t idx = 1; idx < sa_result_dfs.bestVisitingOrder.size(); idx++) SA_Order_DFS.push_back(dfs_targets[sa_result_dfs.bestVisitingOrder[idx]]);
    vector<pair<int, int>> ACO_Order_DFS;
    for (size_t idx = 1; idx < aco_result_dfs.bestVisitingOrder.size(); idx++) ACO_Order_DFS.push_back(dfs_targets[aco_result_dfs.bestVisitingOrder[idx]]);
    vector<pair<int, int>> GA_Order_DFS;
    for (size_t idx = 1; idx < ga_result_dfs.bestVisitingOrder.size(); idx++) GA_Order_DFS.push_back(dfs_targets[ga_result_dfs.bestVisitingOrder[idx]]);
    vector<pair<int, int>> SA_Order_BFS;
    for (size_t idx = 1; idx < sa_result_bfs.bestVisitingOrder.size(); idx++) SA_Order_BFS.push_back(bfs_targets[sa_result_bfs.bestVisitingOrder[idx]]);
    vector<pair<int, int>> ACO_Order_BFS;
    for (size_t idx = 1; idx < aco_result_bfs.bestVisitingOrder.size(); idx++) ACO_Order_BFS.push_back(bfs_targets[aco_result_bfs.bestVisitingOrder[idx]]);
    vector<pair<int, int>> GA_Order_BFS;
    for (size_t idx = 1; idx < ga_result_bfs.bestVisitingOrder.size(); idx++) GA_Order_BFS.push_back(bfs_targets[ga_result_bfs.bestVisitingOrder[idx]]);

    vector<pair<int, int>> bestPermutation;
    vector<timeitResult> results;
    
    {
        string binFileNameDfs = "output/Bin_DFS";
        string binFileNameBfs = "output/Bin_BFS";
        string binFileName = "output/Bin_ORI";

        cout << endl;
        results.push_back(timeit(binFileNameDfs + "_ACO", SOURCE, ACO_Order_DFS, AStarBin, dfs_map, shortest, bestPermutation, heuristic, direction));
        results.push_back(timeit(binFileNameDfs + "_GA", SOURCE, GA_Order_DFS, AStarBin, dfs_map, shortest, bestPermutation, heuristic, direction));
        results.push_back(timeit(binFileNameDfs + "_SA", SOURCE, SA_Order_DFS, AStarBin, dfs_map, shortest, bestPermutation, heuristic, direction));

        results.push_back(timeit(binFileNameBfs + "_ACO", SOURCE, ACO_Order_BFS, AStarBin, bfs_map, shortest, bestPermutation, heuristic, direction));
        results.push_back(timeit(binFileNameBfs + "_GA", SOURCE, GA_Order_BFS, AStarBin, bfs_map, shortest, bestPermutation, heuristic, direction));
        results.push_back(timeit(binFileNameBfs + "_SA", SOURCE, SA_Order_BFS, AStarBin, bfs_map, shortest, bestPermutation, heuristic, direction));

        results.push_back(timeit(binFileName + "_ACO_DFS", SOURCE, ACO_Order_DFS, AStarBin, vec, shortest, bestPermutation, heuristic, direction));
        results.push_back(timeit(binFileName + "_GA_DFS", SOURCE, GA_Order_DFS, AStarBin, vec, shortest, bestPermutation, heuristic, direction));
        results.push_back(timeit(binFileName + "_SA_DFS", SOURCE, SA_Order_DFS, AStarBin, vec, shortest, bestPermutation, heuristic, direction));

        results.push_back(timeit(binFileName + "_ACO_BFS", SOURCE, ACO_Order_BFS, AStarBin, vec, shortest, bestPermutation, heuristic, direction));
        results.push_back(timeit(binFileName + "_GA_BFS", SOURCE, GA_Order_BFS, AStarBin, vec, shortest, bestPermutation, heuristic, direction));
        results.push_back(timeit(binFileName + "_SA_BFS", SOURCE, SA_Order_BFS, AStarBin, vec, shortest, bestPermutation, heuristic, direction));
    }


    cout << left;
    cout << setw(20) << "Name" << setw(15) << "Time (micro)" << setw(15) << "Cost" << setw(10) << "Calculated" << endl; 
    for (timeitResult &r : results) {
        cout << setw(20) << r.fileName.substr(7) << setw(15) << r.timeElapsed << setw(15) << r.totalDistance << setw(10) << r.calculated << endl; 
    }
    cout << endl;

    cout << "Shortest : " << shortest << endl;
    cout << "Visiting Order : " << "(" << SOURCE.first << ", " << SOURCE.second << ")" << "-->";
    for (size_t cnt = 0; cnt < bestPermutation.size() - 1; cnt++) cout << "(" << bestPermutation[cnt].first << ", " << bestPermutation[cnt].second << ") "<< "-->";
    cout << "(" << bestPermutation[bestPermutation.size() - 1].first << ", " << bestPermutation[bestPermutation.size() - 1].second << ") "<< endl;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << "Total Time Used for Optimal Sequence: " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " microseconds" << std::endl;
    cout << endl;

    vector<timeitResult> PermuteResults;
    map<pair<int, int>, int> TARGET_IDX;
    for (size_t idx = 0; idx < TARGETS.size(); idx++) TARGET_IDX[TARGETS[idx]] = idx;
    
    if (!strcmp(permute, "-p") || !strcmp(permute, "-P")) {
        do 
        {
            string fileName = "output/Permutation";
            for (pair<int, int> &p : TARGETS) fileName += "_" + to_string(TARGET_IDX[p]);
            PermuteResults.push_back(timeit(fileName + "_", SOURCE, TARGETS, AStarBin, vec, shortest, bestPermutation, heuristic, direction));
        } while (std::next_permutation(TARGETS.begin(), TARGETS.end()));

        cout << left;
        cout << setw(30) << "Name" << setw(15) << "Time (micro)" << setw(15) << "Cost" << setw(10) << "Calculated" << endl; 
        for (timeitResult &r : PermuteResults) {
            cout << setw(30) << r.fileName.substr(7) << setw(15) << r.timeElapsed << setw(15) << r.totalDistance << setw(10) << r.calculated << endl; 
        }
        cout << endl;

        cout << "Shortest : " << shortest << endl;
        cout << "Visiting Order : " << "(" << SOURCE.first << ", " << SOURCE.second << ")" << "-->";
        for (size_t cnt = 0; cnt < bestPermutation.size() - 1; cnt++) cout << "(" << bestPermutation[cnt].first << ", " << bestPermutation[cnt].second << ") "<< "-->";
        cout << "(" << bestPermutation[bestPermutation.size() - 1].first << ", " << bestPermutation[bestPermutation.size() - 1].second << ") "<< endl;
        cout << endl;
        cout << endl;
    }


    end = std::chrono::steady_clock::now();


    cout << "Total Time Used : " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " microseconds" << std::endl;
    cout << "Exit..." << endl;
    cout << endl;


    if (!strcmp(visualize, "-v") || !strcmp(visualize, "-V")) system("python visualize.py");
    return 0;
}

