#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <unordered_set>

using pii = std::pair<int, int>;

// For hashing so that we can use unordered_map and unordered_set
class piiHash {
    public:
        std::size_t operator()(const pii &p) const {
            auto h1 = std::hash<int>{}(p.first);
            auto h2 = std::hash<int>{}(p.second);
            return h1 ^ h2;
            // return p.first * 31 + p.second;
        }
};

class piiEqual {
    public:
        bool operator()(const pii &p1, const pii &p2) const {
            return p1.first == p2.first && p1.second == p2.second;
        }
};





std::vector<std::string> readMap(std::string filename)
{
    std::vector<std::string> vec;
    std::fstream file(filename);
    std::string line = "";
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << " returning a null vector..." << std::endl;
        return vec;
    }
    int cnt = 0;
    while (getline(file, line)) 
    {
        // std::cout << line << std::endl;
        if (!line.empty())
        {
            vec.push_back(line);
        }
    }
    return vec;
}

void saveResult(std::string filename, std::vector<std::string> _map, std::vector<pii> route, std::vector<std::vector<bool>>calculated, std::vector<std::pair<int, int>> targets)
{
    std::fstream file(filename, std::ios::out);
    for (size_t idx = 0; idx < calculated.size(); idx++) {
        for (size_t jdx = 0; jdx < calculated[0].size(); jdx++)
        {
            if (calculated[idx][jdx])
                _map[idx][jdx] = 'C';
        }
    }
    for (const pii &p : route)
    {
        _map[p.first][p.second] = 'R';
    }
    for (const pii &p : targets)
    {
        _map[p.first][p.second] = 'T';
    }
    _map[route[0].first][route[0].second] = 'S';
    for (const std::string &s : _map)
    {
        file << s << "\n";
    }
    // std::cout << filename << " saved successfully" << std::endl;
}

void saveMap(std::string filename, std::vector<std::string> _map,std::pair<int, int> source,  std::vector<std::pair<int, int>> targets)
{
    std::fstream file(filename, std::ios::out);

    for (const pii &p : targets)
    {
        _map[p.first][p.second] = 'T';
    }
    _map[source.first][source.second] = 'S';
    for (const std::string &s : _map)
    {
        file << s << "\n";
    }
    // std::cout << filename << " saved successfully" << std::endl;
}
