#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <utility> // pair
#include <list>
#include <vector>
#include <array>
#include <math.h>
#include <float.h>
#include <limits>
#include <fstream>
#include <algorithm> //swap
#include <cassert>

#include "util.hpp"

using namespace std;

// enum TerrainType {} <-- future feature

/********************************** COORDINATE ********************************/

enum Orientation {NONE, HORZ, VERT};
// ^valid enum value if between 0 and 2 inclusive
enum Direction {NORTH, SOUTH, EAST, WEST};
enum Corner {NW, NE, SW, SE};

// BOTTOM LEFT CORNER = 0, 0
// UP = NORTH
// RIGHT = EAST
struct Coordinate {
    int x;
    int y;

    explicit Coordinate();
    explicit Coordinate(int x, int y);
    // const Coordinate& operator=(const Coordinate& rhs);
    // ^ use compiler provided implicit copy assignment constructor
    ~Coordinate();

    bool operator==(const Coordinate& rhs) const;
    bool operator!=(const Coordinate& rhs) const;
    bool operator<(const Coordinate& rhs) const;
    bool operator>(const Coordinate& rhs) const;
    bool operator<=(const Coordinate& rhs) const;
    bool operator>=(const Coordinate& rhs) const;

    const Coordinate& operator+=(const Coordinate& rhs);
    const Coordinate& operator+=(const Direction rhs);

    friend const Coordinate operator+(
        const Coordinate& lhs,
        const Coordinate& rhs);
    friend const Coordinate operator-(
        const Coordinate& lhs,
        const Coordinate& rhs);
    friend const Coordinate operator+(
        const Coordinate& lhs,
        const Direction rhs);
};

/************************************ BLOCK ***********************************/

struct TerrainBlock {
    int alt;
    Orientation elevation_orient;
    int del_alt;
};


/*********************************** TERRAIN **********************************/

class Terrain {
private:
    int x_dim;
    int y_dim;

    int max_alt;
    int min_alt;

    TerrainBlock* block_grid;

    void constructBlockGrid();
    void destroyBlockGrid();
    void reset();

public:
    explicit Terrain();
    ~Terrain();

    const bool gridExists() const;

    // dimension
    void setDim(const int x_dim, const int y_dim);
    // void updateDimension(int x_dim, int y_dim) <-- future feature
    const int getXDim() const;
    const int getYDim() const;
    const bool validCoord(const Coordinate& coord) const;

    // max/min altitude
    const int getMaxAlt() const;
    const int getMinAlt() const;
    void setMaxAlt(const int new_max_alt);
    void setMinAlt(const int new_min_alt);

    // altitude
    const int getAlt(const Coordinate& coord) const;
    const int getEndAlt(const Coordinate& coord) const;
    void setAlt(const Coordinate& coord, const int alt);
    void incAlt(const Coordinate& coord);
    void decAlt(const Coordinate& coord);
    const int getCornerAlt(const Coordinate& coord, const Corner corner) const;

    // elevation direction
    const Orientation getElevationOrient(const Coordinate& coord) const;
    void setElevationOrient(const Coordinate& coord, const Orientation orient);

    // del_elevation
    const int getDelAlt(const Coordinate& coord) const;
    void setDelAlt(const Coordinate& coord, const int del_alt);
    void incDelAlt(const Coordinate& coord);
    void decDelAlt(const Coordinate& coord);

    // pathfinder
    const float distance(const Coordinate& src, const Coordinate& dst) const;

    // conversion to world space coordinates
    const Vector3f getWorldCoord(const Coordinate& coord) const;
    const Vector3f getCornerWorldCoord(
        const Coordinate& coord,
        const Corner corner) const;
};


#endif