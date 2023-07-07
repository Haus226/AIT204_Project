#pragma once
#include <bits/stdc++.h>
#include "func.hpp"
using namespace std;

class Ant {
    private:
        size_t CITY_NUM = 0;
        int MOVE_CNT = 0;
        int CURRENT_CITY;
        vector<bool> VISITED;
        int startCITY;

        void clean();

        void next(const double &ALPHA, const double &BETA,
                const vector<vector<double>> &DIST,
                const vector<vector<double>> &PHER);
    public:
        double TOTAL_DISTANCE = 0;
        vector<int> PATH;
        explicit Ant(const size_t &NUM, int startIndex);
        void search(const double &ALPHA, const double &BETA,
                    const vector<vector<double>> &DIST,
                    const vector<vector<double>> &PHER);
};

void updatePheromone(vector<vector<double>> &pher,
                     const vector<Ant> &ants, const size_t &num,
                     const double &rho, const double &q);


TSPResult antColonyOptimization(const pair<string, vector<pair<int, int>>> &coords, const int &ant_num,
                                                       const double &alpha, const double &beta,
                                                       const double &tau, const double &rho,
                                                       const double &q, const int &iter,
                                                       const bool &verbose, int numThreads = 0);

void antColonyOptimizationThread(const vector<pair<int, int>>& coords, const int startAntIndex, const int endAntIndex, const double alpha, const double beta, vector<vector<double>>& pher, vector<vector<double>>& dist, vector<Ant>& ants);
 




Ant::Ant(const size_t &NUM, int startIndex) : CITY_NUM(NUM), PATH(NUM), VISITED(NUM), startCITY(startIndex) {
}

void Ant::clean() {
    TOTAL_DISTANCE = 0;
    MOVE_CNT = 0;
    fill(VISITED.begin(), VISITED.end(), false);
    CURRENT_CITY = startCITY;
    PATH[MOVE_CNT++] = CURRENT_CITY;
    VISITED[CURRENT_CITY] = true;
}

void Ant::next(const double &ALPHA, const double &BETA,
            const vector<vector<double>> &DIST,
            const vector<vector<double>> &PHER) {
    vector<double> probabilities(CITY_NUM, 0.0);
    while (MOVE_CNT < CITY_NUM){
        for (int idx = 0; idx < CITY_NUM; idx++) {
            if (!VISITED[idx]) probabilities[idx] = pow(PHER[CURRENT_CITY][idx], ALPHA) / pow(DIST[CURRENT_CITY][idx], BETA);
            else probabilities[idx] = 0;
        }
        // Just choose the largest element as the next city.
        int idx = max_element(probabilities.begin(), probabilities.end()) - probabilities.begin();
        PATH[MOVE_CNT++] = idx;
        VISITED[idx] = true;
        TOTAL_DISTANCE += DIST[CURRENT_CITY][idx];
        CURRENT_CITY = idx;
    }
}

void Ant::search(const double &ALPHA, const double &BETA,
            const vector<vector<double>> &DIST,
            const vector<vector<double>> &PHER){
    clean();
    next(ALPHA, BETA, DIST, PHER);
    TOTAL_DISTANCE += DIST[PATH[CITY_NUM - 1]][PATH[0]];
}


void updatePheromone(vector<vector<double>> &pher,
                     const vector<Ant> &ants, const size_t &num,
                     const double &rho, const double &q){
    vector<vector<double>> temp(num, vector<double>(num, 0));
    for (const Ant &x : ants){
        for (int idx = 1; idx < num; ++idx){
            int start = x.PATH[idx - 1], end = x.PATH[idx];
            temp[start][end] += q / x.TOTAL_DISTANCE;
            temp[end][start] = temp[start][end];
        }
    }
    for (int idx = 0; idx < num; ++idx){
        for (int jdx = 0; jdx < num; ++jdx){
            pher[idx][jdx] = pher[idx][jdx] * rho + temp[idx][jdx];
        }
    }
}


void antColonyOptimizationThread(const vector<pair<int, int>>& coords, const int startAntIndex, const int endAntIndex, const double alpha, const double beta, vector<vector<double>>& pher, vector<vector<double>>& dist, vector<Ant>& ants) {
    for (int idx = startAntIndex; idx < endAntIndex; ++idx) {
        Ant& ant = ants[idx];
        ant.search(alpha, beta, dist, pher);
    }
}

TSPResult
antColonyOptimization(const vector<pair<int, int>> &coords, const int &ant_num,
                        const double &alpha, const double &beta,
                        const double &tau, const double &rho,
                        const double &q, const int &iter,
                        string h = "euc", int numThreads = 0,
                        const bool &verbose = false) {

    vector<double> distHistory(iter, numeric_limits<double>::infinity());
    vector<vector<int>> pathHistory(iter);
    size_t city_num = coords.size();
    vector<vector<double>> pher(city_num, vector<double>(city_num, tau));
    
    vector<vector<double>> dist = initDist(coords, h);
    vector<Ant> ants (ant_num, Ant(city_num, coords.size() - 1));
    double bestDist = numeric_limits<double>::infinity();
    vector<int> bestPath;

    if (!numThreads) numThreads = thread::hardware_concurrency();
    vector<thread> threads(numThreads);
    const int antsPerThread = (ant_num + numThreads - 1) / numThreads;

    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int idx = 0; idx < iter; ++idx){
        if (verbose && (idx % 100 == 0))
            cout << "Current Iteration : " << idx << endl;
        double curDist = numeric_limits<double>::infinity();

        for (int t = 0; t < numThreads; ++t) {
            const int startAntIndex = t * antsPerThread;
            const int endAntIndex = min(startAntIndex + antsPerThread, ant_num);
            threads[t] = thread(antColonyOptimizationThread, ref(coords), startAntIndex, endAntIndex, alpha, beta, ref(pher), ref(dist), ref(ants));
        }

        // Join threads
        for (int t = 0; t < numThreads; ++t) {
            threads[t].join();
        }
//        Without threading
//        for (Ant &ant : ants){
//            ant.search(alpha, beta, dist, pher);
//            if (ant.TOTAL_DISTANCE < bestDist){
//                bestDist = ant.TOTAL_DISTANCE; bestPath = ant.PATH;
////                if (verbose) cout << "Best Distance : " << bestDist << endl;
//            }
//            if (ant.TOTAL_DISTANCE < curDist) curDist = ant.TOTAL_DISTANCE;
//        }

        for (const Ant& ant : ants) {
            if (ant.TOTAL_DISTANCE < distHistory[idx]) {
                distHistory[idx] = ant.TOTAL_DISTANCE;
                pathHistory[idx] = ant.PATH;
                // for (size_t jdx = 0; jdx < pathHistory[idx].size(); jdx++) {
                //     cout << idx << " : ";
                //     for (int e : pathHistory[idx])
                //         cout << e << " ";
                //     cout << endl;
                // }

            }
            if (ant.TOTAL_DISTANCE < bestDist) {
                bestDist = ant.TOTAL_DISTANCE;
                bestPath = ant.PATH;

            }
            // if (ant.TOTAL_DISTANCE < curDist) {
            //     curDist = ant.TOTAL_DISTANCE;
            // }

        }
        updatePheromone(pher, ants, city_num, rho, q);
    }
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    if (verbose) {
        cout << "Time used : " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" <<endl;
        cout << "Best Distance : " << bestDist << endl;
    }
    return TSPResult(pathHistory, bestPath, distHistory, bestDist, chrono::duration_cast<chrono::microseconds>(end - begin).count());
}