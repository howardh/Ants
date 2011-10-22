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
    int moveTo(Location &from, Location &to);   //Returns a direction in which to move to go from point "from" to "to"

    //Breadth-first search
    const static int MEM_SIZE = 15;
    const static int MEM_MID = MEM_SIZE/2;
    Location mem[MEM_SIZE][MEM_SIZE];   //Array the size of the ant's vision
                                        //   Contains the square that reaches that point the fastest
                                        //   -1 = Not yet explored
                                        //   -2 = Unattainable
                                        //   >0 = Number of steps required to go from there to a goal
    std::list<Location> bfsQueue;   //Locations to be tested in the next round of BFS
                                    //FIFO (push_back and pop_front)
    int bfs(Location &from);
    int bfsBacktrack(Location &goal);   //Backtrack to the centre from goal
};

#endif //BOT_H_
