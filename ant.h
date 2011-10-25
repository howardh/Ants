#ifndef ANT_H_
#define ANT_H_

#include "State.h"
#include <list>

struct Ant
{
    Ant() : location(0,0) {}
    Ant(Location l) : location(l), dir(rand()%TDIRECTIONS) {}
    Ant(int r, int c) : location(r,c), dir(rand()%TDIRECTIONS) {}

    Location location;  //Location of the ant
    int dir;  //Stores the prefered direction (between 0 and 3)

    int status; //Records whether this ant is after food, an ant hill, etc..
    enum
    {
        NONE,
        FOOD_FOUND,
        ANT_HILL_FOUND,
        ENEMY_FOUND
    };
    std::list<Location> path;   //Stores the path to the target (e.g. if status == FOOD_FOUND, this would store the path from the ant to the food)
                                //front of "path" is adjacent to the ant, back of "path" is the target
};

#endif
