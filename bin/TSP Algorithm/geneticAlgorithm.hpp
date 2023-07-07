#pragma once
#include <bits/stdc++.h>
#include "func.hpp"
using namespace std;

vector<vector<int>> initPopulation(long long int popSize, const vector<int> &order);

double calFitness(const vector<int> &order, const vector<vector<double>> &dist);

vector<pair<int, double>> ranking(const vector<vector<int>> &population, const vector<vector<double>> &dist);

vector<int> selection(const int &eliteSize, const vector<pair<int, double>> &ranking);

vector<vector<int>> mating(const vector<vector<int>> &population, const vector<int> &selection);

vector<vector<int>> breedPopulation(const int &eliteSize, vector<vector<int>> &matingPool);

void mutatePopulation(vector<vector<int>> &population, const double &mutateRate);

vector<vector<int>> nextGen(const vector<vector<int>> &curPop, 
            const vector<vector<double>> &initDistance, 
            const int& eliteSize, const double &mutateRate);

TSPResult geneticAlgorithm(const pair<string, vector<pair<int, int>>> &coords,
                      const int &iterations, const double &mutateRate, 
                      const int &popSize, const int &eliteSize, const bool &verbose);

vector<vector<int>> initPopulation(long long int popSize, const vector<int> &order){
    vector<vector<int>> population(popSize);
    for (int cnt = 0; cnt < popSize; cnt++){
        vector<int> newIndividual = order;
        shuffle(newIndividual.begin() + 1, newIndividual.end(), random_device());
        population[cnt] = newIndividual;
    }
    return population;
}

double calFitness(const vector<int> &order, const vector<vector<double>> &dist){
    size_t length = order.size();
    double distSum = 0;
    for (int idx = 0; idx < length - 1; idx++) distSum += dist[order[idx]][order[idx + 1]];
    distSum += dist[order[length - 1]][order[0]];
    return 1 / distSum;
}

vector<pair<int, double>> ranking(const vector<vector<int>> &population, const vector<vector<double>> &dist){
    size_t size = population.size();
    vector<pair<int, double>> fitness(size);
    for(int idx = 0; idx < size; idx++) {
        fitness[idx] = make_pair(idx, calFitness(population[idx], dist));
    }

    sort(fitness.begin(), fitness.end(),
            // Lambda function to sort in descending order based on second element
         [](const pair<int, double> &x, const pair<int, double> &y){
             return (x.second > y.second);
         });
    return fitness;
}

vector<int> selection(const int &eliteSize, const vector<pair<int, double>> &ranking){
    size_t length = ranking.size();
    vector<int> selection_;
    selection_.reserve(length);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0.0, 1.0);
    uniform_int_distribution<int> idxDistribution(0, length - 1);
    for(int cnt = 0; cnt < eliteSize; cnt++) selection_.push_back(ranking[cnt].first);
    while (selection_.size() < length){
        if (distribution(gen) > 0.5){
            selection_.push_back(ranking[idxDistribution(gen)].first);
        }
    }
    return selection_;
}

vector<vector<int>> mating(const vector<vector<int>> &population, const vector<int> &selection){
    size_t length = selection.size();
    vector<vector<int>> pool(length);
    for (int cnt = 0; cnt < length; cnt++) pool[cnt] = population[selection[cnt]];
    return pool;
}

vector<vector<int>> breedPopulation(const int &eliteSize, vector<vector<int>> &matingPool){
    vector<vector<int>> children;
    children.reserve(matingPool.size());
    size_t length = matingPool.size() - eliteSize;
    for (int idx = 0; idx < eliteSize; idx++) children.push_back(matingPool[idx]);
    shuffle(matingPool.begin(), matingPool.end(), random_device());
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(1, matingPool[0].size() - 1);

    for (int idx = 1; idx < length; idx++) {
        vector<int> &parent1 = matingPool[idx], &parent2 = matingPool[matingPool.size() - idx - 1];
        vector<int> childP1 = {parent1[0]}, childP2;
        int GeneA = distribution(gen);
        int GeneB = distribution(gen);
        int startGene = min(GeneA, GeneB);
        int endGene = max(GeneA, GeneB);
        for(int idx = startGene; idx < endGene; idx++) childP1.push_back(parent1[idx]);
        for(int &idx : parent2){
            if (find(childP1.begin(), childP1.end(), idx) == childP1.end()) childP2.push_back(idx);
        }
        childP1.insert(childP1.end(), childP2.begin(), childP2.end());
        children.push_back(childP1);
    }
    return children;
}

void mutatePopulation(vector<vector<int>> &population, const double &mutateRate){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0.0, 1.0);
    uniform_int_distribution<int> intDistribution(1, population[0].size() - 1);
    for (vector<int> &idx : population) {
        for (int cnt = 0; cnt < idx.size(); cnt++){
            if (distribution(gen) < mutateRate){
                int x = intDistribution(gen), y = intDistribution(gen);
                swap(idx[x], idx[y]);
            }
        }
    }

}

vector<vector<int>> nextGen(const vector<vector<int>> &curPop,
                            const vector<vector<double>> &initDistance,
                            const int& eliteSize, const double &mutateRate){
    vector<pair<int, double>> ranking_ = ranking(curPop, initDistance);
    vector<int> selection_ = selection(eliteSize, ranking_);
    vector<vector<int>> mating_ = mating(curPop, selection_);
    vector<vector<int>> children_ = breedPopulation(eliteSize, mating_);
    mutatePopulation(children_, mutateRate);
    return children_;
}

TSPResult
geneticAlgorithm(const vector<pair<int, int>> &coords,
                 const int &iterations, const double &mutateRate,
                 const int &popSize, const int &eliteSize, string h="euc", const bool &verbose = false) {
    vector<int> initOrder(coords.size());
    initOrder[0] = coords.size() - 1;
    for (int idx = 1; idx < coords.size() - 1; idx++) initOrder[idx] = idx;
    vector<vector<int>> pop = initPopulation(popSize, initOrder);
    vector<vector<double>> dist = initDist(coords, h);
    double curDist = 1 / (ranking(pop, dist)[0].second);
    double bestDist = curDist;
    vector<int> bestPath = initOrder;
    vector<double> distHistory(iterations);
    vector<vector<int>> pathHistory(iterations);
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int cnt = 0; cnt < iterations; cnt++){
        if (verbose && (cnt % 100 == 0)) cout << "Current Iteration : " << cnt << endl;
        pop = nextGen(pop, dist, eliteSize, mutateRate);
        pair<int, double> cur = ranking(pop, dist)[0];
        curDist = 1 / cur.second;
        if (curDist < bestDist) {
            bestDist = curDist;
            bestPath = pop[cur.first];
        }
        distHistory[cnt] = curDist;
        pathHistory[cnt] = pop[cur.first];
    }
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    if (verbose) {
        cout << "Time used : " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" <<endl;
        cout << "Best Distance : " << bestDist << endl;
    }
    return TSPResult(pathHistory, bestPath, distHistory, bestDist, chrono::duration_cast<chrono::microseconds>(end - begin).count());
}