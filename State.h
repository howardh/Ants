#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>

#include "consts.h"
#include "Timer.h"
#include "Bug.h"
#include "Square.h"
#include "Location.h"
#include "ant.h"

/*
    struct to store current state information
*/
struct State
{
    /*
        Variables
    */
    int rows, cols,     //Size of the grid?
        turn, turns,
        noPlayers;
    double attackradius, spawnradius, viewradius;
    double loadtime, turntime;
    std::vector<double> scores;
    bool gameover;

    std::vector<std::vector<Square> > grid;     //grid[row][col]
    std::vector<Location> enemyAnts, myHills, enemyHills, food;
    std::vector<Ant> myAnts;

    Timer timer;
    Bug bug;

    /*
        Functions
    */
    State();
    ~State();

    void setup();
    void reset();

    void makeMove(const Location &loc, int direction);

    double distance(const Location &loc1, const Location &loc2);
    Location getLocation(const Location &startLoc, int direction);

    void updateVisionInformation();

    /*
        My functions
    */
    int getClosest(Location &from, std::vector<Location> &to);      //Finds a point in the vector "to" which can be reached from the point "from" with the least number of moves
    Square getSquare(Location loc);    //Returns the square at "loc"
};

std::ostream& operator<<(std::ostream &os, const State &state);
std::istream& operator>>(std::istream &is, State &state);

#endif //STATE_H_
