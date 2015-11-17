#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <queue>
#include <vector>

#include "util.hpp"
#include "terrain.hpp"
#include "pathfinder.hpp"
#include "model.hpp"

// NOTE: everything in this game runs on a tempo system. every entity has a
// counter for a certain action that ticks down every beat. actions are executed
// when counter hits 0. counter is then updated to a pace that matches the
// entity. note that this is not the pace_level. pace = 2^pace_level
// pace is only allowed to be in powers of 2 to give a synced movement pattern
static const int max_pace_level = 31;    // <- i guess? 32 bit assumed
inline int pace(int pace_level) {
    assert(pace_level > 0);
    return 1 << pace_level - 1;
}

static const float alpha_completion_threashold = 0.999;

// states that entities can be in
enum EntityState {IDLE, MOVE};// , ATTACK_MOVE, PATROL, HOLD};

// default values for Entity member variables
static const Direction default_dir = NORTH;
static const Coordinate init_coord(10.0, 0.0);
static const float init_alpha = 1.0;
static const int default_width = 1;
static const int default_length = 1;
static const float default_health = 10.0;

class Entity
{
private:
    ModelID id;

    // direction that the entity is facing
    Direction dir;
    // prev position
    Coordinate prev;
    Coordinate curr;
    float alpha;    // true position is always prev * alpha + curr * (1 - alpha)
                    // where alpha is between 0 and 1 inclusive
    // dimensions
    int width;
    int length;
    // game mechanics specific
    float health;

    // disable default constructor
    // no entity if no model
    explicit Entity();

public:
    explicit Entity(ModelID id);
    virtual ~Entity();

    const Coordinate& getPrevCoord();
    const Coordinate& getCurrCoord();

    void setNewCoord(const Coordinate& coord);

    ModelID getModelID();

    // tick down counters and act accordingly
    virtual void act() = 0;
};

// Core: tier 1 unit USER CONTROLLED

// default values for Core member variables
static int default_pace_level_move = 4;

class Core : public Entity {
private:
    int pace_level_move;
    bool dynamic;
    int counter_move;

public:
    explicit Core();
    ~Core();

    void act();
};

class Group {
private:
    Pathfinder pathfinder;
    vector<Entity*> members;

public:
    explicit Group();
    ~Group();
    
    void addEntity(Entity* entity);
    void actGroup();
};

#endif