#include "terrain.hpp"

/********************************** COORDINATE ********************************/

Coordinate::Coordinate() {
    this->x = -1;
    this->y = -1;
}

Coordinate::Coordinate(int x, int y) {
    this->x = x;
    this->y = y;
}
// const Coordinate& Coordinate::operator=(const Coordinate& rhs) {
//     swap(this->x, rhs.x);
//     this->x = rhs.x;
//     this->y = rhs.y;
// }
Coordinate::~Coordinate() {

}

bool Coordinate::operator==(const Coordinate& rhs) const {
    return (this->x == rhs.x) & (this->y == rhs.y);
}
bool Coordinate::operator!=(const Coordinate& rhs) const {
    return (this->x != rhs.x) | (this->y != rhs.y);
}
bool Coordinate::operator<(const Coordinate& rhs) const {
    return this->x < rhs.x | (this->x == rhs.x & this->y < rhs.y);
}
bool Coordinate::operator>(const Coordinate& rhs) const {
    return this->x > rhs.x | (this->x == rhs.x & this->y > rhs.y);
}
bool Coordinate::operator<=(const Coordinate& rhs) const {
    return !(this->x > rhs.x | (this->x == rhs.x & this->y > rhs.y));
}
bool Coordinate::operator>=(const Coordinate& rhs) const {
    return !(this->x < rhs.x | (this->x == rhs.x & this->y < rhs.y));
}

const Coordinate& Coordinate::operator+=(const Coordinate& rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}
const Coordinate& Coordinate::operator+=(const Direction rhs) {
    switch(rhs) {
        case NORTH:
            this->y += 1;
            break;
        case SOUTH:
            this->y -= 1;
            break;
        case EAST:
            this->x += 1;
            break;
        case WEST:
            this->x -= 1;
            break;
        default:
            fprintf(stderr, "Error: invalid Direction for Coordinate + op %d\n",
                rhs);
            exit(1);
    }
    return *this;
}

const Coordinate operator+(const Coordinate& lhs, const Coordinate& rhs) {
    return Coordinate(lhs.x + rhs.x, lhs.y + lhs.y);
}
const Coordinate operator-(const Coordinate& lhs, const Coordinate& rhs) {
    return Coordinate(lhs.x - rhs.x, lhs.y - lhs.y);
}

const Coordinate operator+(const Coordinate& lhs, const Direction rhs) {
    switch(rhs) {
        case NORTH:
            return Coordinate(lhs.x, lhs.y + 1);
        case SOUTH:
            return Coordinate(lhs.x, lhs.y - 1);
        case EAST:
            return Coordinate(lhs.x + 1, lhs.y);
        case WEST:
            return Coordinate(lhs.x - 1, lhs.y);
        default:
            fprintf(stderr, "Error: invalid Direction for Coordinate + op %d\n",
                rhs);
            exit(1);
    }
}

/************************** TERRAIN INSTANCE METHODS **************************/

// PRIVATE

Terrain::Terrain() :
    x_dim(0),
    y_dim(0),
    min_alt(0),
    max_alt(5),
    block_grid(NULL)
{

}

Terrain::~Terrain() {
    this->destroyBlockGrid();
}

void Terrain::constructBlockGrid() {
    assert(this->x_dim > 0);
    assert(this->y_dim > 0);

    this->block_grid = new TerrainBlock[this->x_dim * this->y_dim];
}

void Terrain::destroyBlockGrid() {
    delete[] this->block_grid;
    this->block_grid = NULL;
}

void Terrain::reset() {
    this->destroyBlockGrid();
    this->x_dim = 0;
    this->y_dim = 0;
    this->min_alt = 0;
    this->max_alt = 5;
}


// PUBLIC
const bool Terrain::gridExists() const {
    return this->block_grid != NULL;
}

// dimension
void Terrain::setDim(const int x_dim, const int y_dim) {    // probably change?
    assert(this->x_dim == 0);
    assert(this->y_dim == 0);
    assert(x_dim > 0);
    assert(y_dim > 0);

    this->x_dim = x_dim;
    this->y_dim = y_dim;

    this->constructBlockGrid();
}

const int Terrain::getXDim() const {
    return this->x_dim;
}

const int Terrain::getYDim() const {
    return this->y_dim;
}

const bool Terrain::validCoord(const Coordinate& coord) const {
    return coord.x >= 0 && coord.y >=0 &&
        coord.x < this->x_dim && coord.y < this->y_dim;
}

// max/min altitude
const int Terrain::getMaxAlt() const {
    return this->max_alt;
}

const int Terrain::getMinAlt() const {
    return this->min_alt;
}

void Terrain::setMaxAlt(const int new_max_alt) {
    assert(new_max_alt >= min_alt);
    max_alt = new_max_alt;
}

void Terrain::setMinAlt(const int new_min_alt) {
    assert(max_alt >= new_min_alt);
    min_alt = new_min_alt;
}

// altitude
const int Terrain::getAlt(const Coordinate& coord) const {
    assert(this->gridExists());
    return this->block_grid[coord.y + this->y_dim * coord.x].alt;
}

const int Terrain::getEndAlt(const Coordinate& coord) const {
    assert(this->gridExists());
    return this->getAlt(coord) + this->getDelAlt(coord);
}

void Terrain::setAlt(const Coordinate& coord, const int alt) {
    assert(this->gridExists());
    if(alt >= this->min_alt && alt <= this->max_alt) {
        this->block_grid[coord.y + this->y_dim * coord.x].alt = alt;
    } else {
        printf("WARNING: alt not in range. Terrain::setAlt. action ignored\n");
    }
}

void Terrain::incAlt(const Coordinate& coord) {
    assert(this->gridExists());
    if(this->getAlt(coord) < this->max_alt) {
        this->block_grid[coord.y + this->y_dim * coord.x].alt++;
    } else {
        printf("WARNING: max alt reached. Terrain::incAlt. action ignored\n");
    }
}

void Terrain::decAlt(const Coordinate& coord) {
    assert(this->gridExists());
    if(this->getAlt(coord) < this->min_alt) {
        this->block_grid[coord.y + this->y_dim * coord.x].alt--;
    } else {
        printf("WARNING: min alt reached. Terrain::decAlt. action ignored\n");
    }
}

const int Terrain::getCornerAlt(
    const Coordinate& coord,
    const Corner corner) const
{
    switch (this->getElevationOrient(coord))
    {
        case NONE:
            return this->getAlt(coord);
        case HORZ:
            if (corner == NE || corner == SE) {
                return this->getEndAlt(coord);
            } else if (corner == NW || corner == SW) {
                return this->getAlt(coord);
            } else {
                fprintf(stderr, "Terrain::getCornerAlt ERROR invalid corner %d\n",
                    corner);
                exit(1);
            }
        case VERT:
            if (corner == NE || corner == NW) {
                return this->getEndAlt(coord);
            } else if (corner == SE || corner == SW) {
                return this->getAlt(coord);
            } else {
                fprintf(stderr, "Terrain::getCornerAlt ERROR invalid corner %d\n",
                    corner);
                exit(1);
            }
        default:
            fprintf(stderr, "Terrain::getCornerAlt ERROR invalid orientation %d\n",
                this->getElevationOrient(coord));
            exit(1);
    }
    // what to return on error?
}

// elevation direction
const Orientation Terrain::getElevationOrient(const Coordinate& coord) const {
    assert(this->gridExists());

    Orientation orient =
        this->block_grid[coord.y + this->y_dim * coord.x].elevation_orient;

    if(orient == NONE)
        assert(this->block_grid[coord.y + this->y_dim * coord.x].del_alt == 0);

    return orient;
}

void Terrain::setElevationOrient(
    const Coordinate& coord,
    const Orientation orient)
{
    assert(this->gridExists());
    this->block_grid[coord.y + this->y_dim * coord.x].elevation_orient = orient;
}

// del_elevation
const int Terrain::getDelAlt(const Coordinate& coord) const {
    assert(this->gridExists());
    return this->block_grid[coord.y + this->y_dim * coord.x].del_alt;
}

void Terrain::setDelAlt(const Coordinate& coord, const int del_alt) {
    assert(this->gridExists());
    int new_end_alt = this->getAlt(coord) + del_alt;
    if(new_end_alt + del_alt >= this->min_alt
        && new_end_alt + del_alt <= this->max_alt)
    {
        this->block_grid[coord.y + this->y_dim * coord.x].del_alt = del_alt;
    } else {
        printf("WARNING: del_alt not in range. Terrain::setDelAlt. action ignored\n");
    }
}

void Terrain::incDelAlt(const Coordinate& coord) {
    assert(this->gridExists());
    if(this->getEndAlt(coord) < this->max_alt) {
        this->block_grid[coord.y + this->y_dim * coord.x].del_alt++;
    } else {
        printf("WARNING: max del_alt reached. Terrain::incDelAlt. action ignored\n");
    }
}

void Terrain::decDelAlt(const Coordinate& coord) {
    assert(this->gridExists());
    if(this->getEndAlt(coord) > this->min_alt) {
        this->block_grid[coord.y + this->y_dim * coord.x].del_alt--;
    } else {
        printf("WARNING: min del_alt reached. Terrain::decDelAlt. action ignored\n");
    }
}

// pathfinder
const float Terrain::distance(
    const Coordinate& src,
    const Coordinate& dst) const
{
    assert(this->validCoord(src));
    assert(this->validCoord(dst));

    int del_x = dst.x - src.x;
    int del_y = dst.y - src.y;

    assert((abs(del_x) == 1) != (abs(del_y) == 1)); // != used as xor

    Orientation orient;
    if(abs(del_x) == 1)
        orient = HORZ;
    else if(abs(del_y) == 1)
        orient = VERT;
    else {
        orient = NONE;
        fprintf(stderr, "Error: invalid coordinates for distance function\n");
        exit(1);
    }

    // check if directions are compatible for a passage
    Orientation src_orient = this->getElevationOrient(src);
    if(src_orient != NONE && src_orient != orient)
        return FLT_MAX;
    Orientation dst_orient = this->getElevationOrient(dst);
    if(dst_orient != NONE && dst_orient != orient)
        return FLT_MAX;

    // check if altitudes match for a passage
    int src_alt = this->getAlt(src);
    int src_del_alt = this->getDelAlt(src);
    int src_end_alt = this->getEndAlt(src);
    int dst_alt = this->getAlt(dst);
    int dst_del_alt = this->getDelAlt(dst);
    int dst_end_alt = this->getEndAlt(dst);
    // calculate distance if passable
    float dist = sqrt(pow(src_del_alt, 2) + 1) / 2
            + sqrt(pow(dst_del_alt, 2) + 1) / 2;
    if(del_x + del_y == 1) {
        // return dist if passable
        if(src_end_alt == dst_alt) {
            return dist;
        }
    } else if(del_x + del_y == -1) {
        // return dist if passable
        if(dst_end_alt == src_alt) {
            return dist;
        }
    } else {
        // ERROR
        fprintf(stderr, "Error: invalid del_x + del_y (must be 1 or -1)\n");
        exit(1);
    }

    return FLT_MAX;
}