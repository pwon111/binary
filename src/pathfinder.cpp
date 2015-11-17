#include "pathfinder.hpp"

/****************************** Pathfinder Class ******************************/

Pathfinder::Pathfinder():
    dist_map(),
    prev_map(),
    visited_map(),
    search_frontier(),
    loaded_src(-1, -1)
{
    
}

Pathfinder::~Pathfinder() {
    
}

void Pathfinder::findPath(
    const Terrain& trn,
    const Coordinate& src,
    const Coordinate& dst)
{
    this->DJK_search(trn, src, dst);
    this->DJK_buildPath(dst);
}

vector<Coordinate>& Pathfinder::getFoundPath() {
    return found_path;
}

void Pathfinder::DJK_search(
    const Terrain& trn,
    const Coordinate& src,
    const Coordinate& dst)
{
    assert(trn.gridExists());
    assert(trn.validCoord(src));
    assert(trn.validCoord(dst));

    // if loaded_src != src, load new src
    if(this->loaded_src != src) {
        this->dist_map.clear();
        this->prev_map.clear();
        this->visited_map.clear();
        this->search_frontier.clear();

        this->loaded_src = src;
        this->search_frontier.push_back(src);
        this->dist_map[src] = 0.0;
        this->prev_map[src] = Coordinate(-1, -1);
    }

    // if dst has already been visited, DONE
    map<Coordinate, bool>::iterator src_check_it = this->visited_map.find(dst);
    if(src_check_it != this->visited_map.end() && src_check_it->second)
        return;

    while(true) {
        // we'll search from the closest frontier block
        Coordinate search_coord = this->search_frontier.back();
        if(search_coord == dst)
            return;
        this->search_frontier.pop_back();
        // search in four directions
        this->DJK_SearchAdjCoord(trn, search_coord, NORTH);
        this->DJK_SearchAdjCoord(trn, search_coord, SOUTH);
        this->DJK_SearchAdjCoord(trn, search_coord, EAST);
        this->DJK_SearchAdjCoord(trn, search_coord, WEST);
        this->visited_map[search_coord] = true;
    }
}

/*************************** Pathfinder Core Logic ****************************/

void Pathfinder::DJK_SearchAdjCoord(
    const Terrain& trn,
    const Coordinate& src,
    const Direction dir)
{
    Coordinate src_adj = src + dir;
    if(!trn.validCoord(src_adj))
        return;
    map<Coordinate, float>::iterator dist_it;
    map<Coordinate, bool>::iterator vstd_it = this->visited_map.find(src_adj);
    float new_dist;
    if(vstd_it == this->visited_map.end() || vstd_it->second == false)
    {
        new_dist = trn.distance(src, src_adj);
        if(new_dist == FLT_MAX)
            return;
        dist_it = this->dist_map.find(src_adj);
        if(dist_it != this->dist_map.end()) {
            assert(this->prev_map.find(src_adj) != this->prev_map.end());
            if(dist_it->second > new_dist) {
                dist_it->second = new_dist;
                this->prev_map[src_adj] = src;
                DJK_sortedFrontierInsert(src_adj);
            }
        } else {
            assert(this->prev_map.find(src_adj) == this->prev_map.end());
            this->dist_map[src_adj] = new_dist;
            this->prev_map[src_adj] = src;
            DJK_sortedFrontierInsert(src_adj);
        }
    }
}

// use UTIL SORTED INSERT!!!!
void Pathfinder::DJK_sortedFrontierInsert(const Coordinate& coord)
{
    assert(this->dist_map.find(coord) != this->dist_map.end());

    vector<Coordinate>::iterator it;
    float coord_dist = this->dist_map.at(coord);
    for(it = this->search_frontier.begin();
        it != this->search_frontier.end();
        it++)
    {
        assert(this->dist_map.find(*it) != this->dist_map.end());
        if(*it == coord)
            return;
        if(coord_dist >= this->dist_map.at(*it)) {
            this->search_frontier.insert(it, coord);
            return;
        }
    }
    this->search_frontier.push_back(coord);
}

void Pathfinder::DJK_buildPath(const Coordinate& dst)
{
    found_path.clear();
    found_path.push_back(dst);
    while(true) {
        assert(this->prev_map.find(this->found_path.back()) != this->prev_map.end());
        this->found_path.push_back(this->prev_map.at(this->found_path.back()));
        if(this->found_path.back() == this->loaded_src)
            return;
    }
}