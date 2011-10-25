#ifndef BOT_H_
#define BOT_H_

#include "State.h"
#include <list>

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;

    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves

private:
    //Misc methods
    int getDirection(Location &loc);    //Returns a direction (0 to 3) corresponding to "loc"

    //Breadth-first search
    const static int MEM_SIZE = 17;
    const static int MEM_MID = MEM_SIZE/2;
    Location mem[MEM_SIZE][MEM_SIZE];   //Array the size of the ant's vision
                                        //   Contains the square that reaches that point the fastest
                                        //   -1 = Not yet explored
                                        //   -2 = Unattainable
                                        //   >0 = Number of steps required to go from there to a goal
    std::list<Location> bfsQueue;   //Locations to be tested in the next round of BFS
                                    //FIFO (push_back and pop_front)
    Location firstFood;          //Contains location of the first food found
    Location firstNonVisible;    //Contains location of the first non-visible squares found
    int bfs(Location &from, int prefDir = 0);
    int bfsBacktrack(Location &goal);   //Backtrack to the centre from goal
};

#endif //BOT_H_
