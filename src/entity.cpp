#include "entity.hpp"

/********************************** Entity Class ******************************/

Entity::Entity(ModelID id) :
    id(id),
    dir(default_dir),
    prev(init_coord),
    curr(init_coord),
    alpha(init_alpha),
    width(default_width),
    length(default_length),
    health(default_health)
{
    //
}
Entity::~Entity() {

}

const Coordinate& Entity::getPrevCoord() {
    return this->prev;
}
const Coordinate& Entity::getCurrCoord() {
    return this->curr;
}

void Entity::setNewCoord(const Coordinate& coord) {
    assert(this->alpha > alpha_completion_threashold);

    this->prev = this->curr;
    this->curr = coord;
    this->alpha = 1.0;
}

ModelID Entity::getModelID() {
    return this->id;
}

/*********************************** Core Class *******************************/

Core::Core() :
    Entity(core_model_id),
    pace_level_move(default_pace_level_move),
    dynamic(true),
    counter_move(pace(default_pace_level_move)) // maybe start at 0?
{

}
Core::~Core() {

}

void Core::act() {
    assert(this->counter_move >= 0);
    if (this->counter_move == 0) {
        this->setNewCoord(this->getCurrCoord() + NORTH);
        this->counter_move = pace(pace_level_move);
    } else {
        this->counter_move--;
    }
    // printf("%d\n", this->counter_move);
}

/********************************** Group Class *******************************/

Group::Group() : pathfinder(), members() {

}
Group::~Group() {

}

void Group::addEntity(Entity* entity) {
    
}

void Group::actGroup() {
    
}
