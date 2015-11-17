#ifndef WORLD_HPP
#define WORLD_HPP

#include <queue>

#include "terrain.hpp"
#include "entity.hpp"

static const int max_beat_counter = 1000000;

class World {
private:
    // restrict most functionalities only to Game
    friend class Game;

    static World* world;

    Terrain* terrain;
    vector<Entity*> entities;
    vector<Group*> groups;

    int beat_counter;

    explicit World();
    explicit World(Terrain* terrain);
    ~World();

    void setTerrain(Terrain* terrain);
    void addEntity(Entity* entity);
    void addGroup(Group* group);

    void update(int msecs);

public:
    // allow access to information
    const Terrain* getTerrain();
    const vector<Entity*>& getEntities();
    const vector<Group*>& getGroups();
};

#endif