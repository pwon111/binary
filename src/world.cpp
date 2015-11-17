#include "world.hpp"

World* World::world = NULL;

World::World() :
    terrain(NULL),
    entities(),
    groups(),
    beat_counter(max_beat_counter)
{

}
World::World(Terrain* terrain) :
    terrain(terrain),
    entities(),
    groups(),
    beat_counter(max_beat_counter)
{

}
World::~World() {
    if (terrain) {
        delete terrain;
    }
    for (vector<Group*>::iterator it = groups.begin();
        it != groups.end();
        it++)
    {
        delete *it;
    }
    for (vector<Entity*>::iterator it = entities.begin();
        it != entities.end();
        it++)
    {
        delete *it;
    }
}

void World::setTerrain(Terrain* terrain) {
    if (this->terrain) {
        delete this->terrain;
    }
    this->terrain = terrain;
}
void World::addEntity(Entity* entity) {
    this->entities.push_back(entity);
}
void World::addGroup(Group* group) {
    this->groups.push_back(group);
}

void World::update(int msecs) {
    this->beat_counter -= msecs;
    while (this->beat_counter < 0) {
        for (vector<Entity*>::iterator it = this->entities.begin();
            it != this->entities.end();
            it++)
        {
            (*it)->act();
        }
        // why not += ?
        this->beat_counter = max_beat_counter;
    }
}


const Terrain* World::getTerrain() {
    return this->terrain;
}
const vector<Entity*>& World::getEntities() {
    return this->entities;
}
const vector<Group*>& World::getGroups() {
    return this->groups;
}