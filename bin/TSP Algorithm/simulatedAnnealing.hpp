#pragma once
#include <bits/stdc++.h>
#include "func.hpp"
using namespace std;


double calDist(const vector<int> &order, const vector<vector<double>> &dist);

vector<int> nextOrder(const vector<int>preOrder, mt19937 gen);

TSPResult simulatedAnnealing(const vector<pair<int, int>> &coords,
                               double startTemp = 10000,
                               double endTemp = 0.0001,
                               double coolingRate = 0.999,
                               const bool &verbose=false);


double calDist(const vector<int> &order, const vector<vector<double>> &dist){
    double distSum = 0;
    for (int idx = 0; idx < order.size() - 1; idx++) distSum += dist[order[idx]][order[idx + 1]];
    distSum += dist[order[order.size() - 1]][order[0]];
    return distSum;
}

vector<int> nextOrder(const vector<int>preOrder, mt19937 gen){
    uniform_int_distribution<> intDistribution_1(1, preOrder.size() - 1);
    vector<int> nextOrder = preOrder;
    // Generate random integer 0 or 1
    int num = rand() % 2;
    if (num){
        int idx = intDistribution_1(gen);
        uniform_int_distribution<> intDistribution_2(idx, preOrder.size() - 1); // define the range
        int jdx = intDistribution_2(gen);
        while (jdx > idx){
            swap(nextOrder[idx], nextOrder[jdx]);
            idx++; jdx--;
        }
    }
    else {
        int idx = intDistribution_1(gen);
        int jdx = intDistribution_1(gen);
        swap(nextOrder[idx], nextOrder[jdx]);
    }
    return nextOrder;
}


TSPResult
simulatedAnnealing(const vector<pair<int, int>> &coords, double startTemp,
                   double endTemp, double coolingRate, string h = "euc", const bool &verbose = false) {
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_real_distribution<double> distribution(0.0, 1.0);
    size_t size = coords.size();
    vector<vector<double>> dist = initDist(coords, h);
    
    vector<int> curOrder(size), bestPath;
    curOrder[0] = size - 1;
    for (int idx = 1; idx < size - 1; idx++) curOrder[idx] = idx;
    bestPath = curOrder;
    double curDist = calDist(curOrder, dist);
    double bestDist = curDist;
    double curTemp = startTemp;
    vector<double> distHistory;
    vector<vector<int>> pathHistory;
    vector<int> __nextOrder;
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (curTemp > endTemp){
         __nextOrder = nextOrder(curOrder, gen);
        double nextDist = calDist(__nextOrder, dist);
        double delta = nextDist - curDist;
        if (delta < 0){
            curOrder = __nextOrder;
            curDist = nextDist;
        }
        else{
            double tol = distribution(gen);
            if (exp(-delta / curTemp) > tol) {
                curOrder = __nextOrder;
                curDist = nextDist;
            }
        }
        if (curDist < bestDist) {
            bestDist = curDist;
            bestPath = curOrder;
            }
        curTemp *= coolingRate;
        distHistory.emplace_back(curDist);
        pathHistory.emplace_back(curOrder);
    }
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    if (verbose) {
        cout << "Time used : " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" <<endl;
        cout << "Best Distance : " << bestDist << endl;
    }
    return TSPResult(pathHistory, bestPath, distHistory, bestDist, chrono::duration_cast<chrono::microseconds>(end - begin).count());

}

