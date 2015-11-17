#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <utility> // pair
#include <list>
#include <vector>
#include <deque>
#include <array>
#include <queue>
#include <math.h>
#include <float.h>
#include <limits>
#include <fstream>
#include <algorithm> //swap
#include <cassert>

#include "util.hpp"
#include "terrain.hpp"

// #include <Eigen/Eigen>

using namespace std;
// using namespace Eigen;

// uses DJK
class Pathfinder {
private:
    map<Coordinate, float> dist_map;      // distance from src
    map<Coordinate, Coordinate> prev_map;   // previous coords (dijkstra specific)
    map<Coordinate, bool> visited_map;       // visited coords
    vector<Coordinate> search_frontier;

    Coordinate loaded_src; // src being used to build paths
    vector<Coordinate> found_path;

public:
    explicit Pathfinder();
    ~Pathfinder();
    
    void findPath(
        const Terrain& trn,
        const Coordinate& src,
        const Coordinate& dst);
    vector<Coordinate>& getFoundPath();

    // builds DJK search till dst is found
    void DJK_search(
        const Terrain& trn,
        const Coordinate& src,
        const Coordinate& dst);
    void DJK_SearchAdjCoord(
        const Terrain& trn,
        const Coordinate& src,
        const Direction dir);
    void DJK_sortedFrontierInsert(const Coordinate& coord); // insert for vectors suck

    // build found_path
    void DJK_buildPath(const Coordinate& dst);
};

#endif