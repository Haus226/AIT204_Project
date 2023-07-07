#pragma once
#include <bits/stdc++.h>
using namespace std;

vector<vector<double>> initDist(const vector<pair<int, int>> &coords, string &h){
    size_t size = coords.size();
    vector<vector<double>> dist(size);
    for (int i = 0; i < size; i++){
        dist[i] = vector<double> (size);
        double x1 = coords[i].first, y1 = coords[i].second;
        for (int j = 0; j < size; j++){
            double x2 = coords[j].first, y2 = coords[j].second;

            if (!h.compare("euc"))
                dist[i][j] = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
            else if(!h.compare("euc2"))
                // Instead of using sqrt, we can just compute the square to optimize the performance
                dist[i][j] =  (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
            else if (!h.compare("mht"))
                dist[i][j] =  abs(x1 - x2) + abs(y1 - y2);
            else if (!h.compare("che"))
                dist[i][j] = std::max(abs(x1 - x2), abs(y1 - y2));
            else if(!h.compare("dia"))
            {
                double dx = abs(x1 - x2);
                double dy = abs(y1 - y2);
                dist[i][j] =  (dx + dy) + ((sqrt2 - 2) * std::min(dx, dy));
            }
        }
    }
    return dist;
}

struct TSPResult {
    vector<vector<int>> visitingHistory;
    vector<int> bestVisitingOrder;
    vector<double> distanceHistory;
    double bestDistance;
    long long timeElapsed;
    TSPResult(vector<vector<int>> visitingHistory, vector<int> bestVisitingOrder, 
              vector<double> distanceHistory, double bestDistance, long long timeElapsed) :
              visitingHistory(visitingHistory), bestVisitingOrder(bestVisitingOrder), 
              distanceHistory(distanceHistory), bestDistance(bestDistance), 
              timeElapsed(timeElapsed) {}

};




