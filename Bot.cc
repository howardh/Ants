#include "Bot.h"

#include <string.h>

using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
//    state.bug << "turn " << state.turn << ":" << endl;
//    state.bug << state << endl;

    //picks out moves for each ant
    for(int ant=0; ant<(int)state.myAnts.size(); ant++)
    {
        int dir = bfs(state.myAnts[ant].location, state.myAnts[ant].dir);

        if (dir != -1)
        {
            Location loc = state.getLocation(state.myAnts[ant].location, dir);
            if(!state.grid[loc.row][loc.col].isWater)
                state.makeMove(state.myAnts[ant].location, dir);
            continue;
        }
//        dir = rand()%TDIRECTIONS;
        dir = state.myAnts[ant].dir;

        for(int i = 0; i<TDIRECTIONS; i++, dir = (dir+1)%TDIRECTIONS)   //Loop through all possible directions
        {
            Location loc = state.getLocation(state.myAnts[ant].location, dir);

            if(!state.grid[loc.row][loc.col].isWater &&             //If the picked direction is not water
               !(state.grid[loc.row][loc.col].hillPlayer == 0) &&   // or my own anthill
               state.grid[loc.row][loc.col].ant == -1)              //  and doesn't contain an ant
            {
                state.makeMove(state.myAnts[ant].location, dir);
                break;
            }
        }
    }

//    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};

/**
@param loc
    A unit direction
    can have the values (-1,0), (1,0), (0,-1), or (0,1)
@return
    A direction value between 0 and 3 (0=N, 1=E, 2=S, 3=W) corresponding to "loc"
*/
int Bot::getDirection(Location &loc)
{
    /*
        N = (-1,0) = (-2+0)+2 = 0 -> val[0] = 0
        E = (0,1)  = (0+1)+2  = 3 -> val[3] = 1
        S = (1,0)  = (2+0)+2  = 4 -> val[4] = 2
        W = (0,-1) = (0-1)+2  = 1 -> val[1] = 3
    */
    const static int val[] = {0,3,-1,1,2};
    return val[loc.row*2+loc.col+2];
}

/**
@param from
    ?????
@return
    Direction in which to move to reach the closest food item.
    If no nearby food is found, returns -1
*/
int Bot::bfs(Location &from, int prefDir)
{
    //Reset everything to -1 (marks as unexplored)
    memset(mem,-1,sizeof(mem[0][0])*MEM_SIZE*MEM_SIZE);

    //Current position is marked as explored
    mem[MEM_MID][MEM_MID].row = 0;
    mem[MEM_MID][MEM_MID].col = 0;

    Location offset = from;
    offset.row -= MEM_MID;
    offset.col -= MEM_MID;

    bfsQueue.clear();
    firstNonVisible.set(-1,-1);

    bfsQueue.push_back(Location(MEM_MID,MEM_MID));    //Add starting location to queue
    Location current;   //Location in mem (between 0 and MEM_SIZE)
    Location tempLoc;   //Location in mem (between 0 and MEM_SIZE)
    Location realCurrent;   //Location on grid corresponding to "current"
    Location realTempLoc;   //Location on grid corresponding to "tempLoc"
    int dir = prefDir;
    while(bfsQueue.size() > 0)
    {
        current = bfsQueue.front();
        realCurrent = current+offset;
        bfsQueue.pop_front();

        //Check if a goal has been reached
        if (state.getSquare(realCurrent).isHill && state.getSquare(realCurrent).hillPlayer != 0)    //If "current" contains an enemy ant hill
        {
            return bfsBacktrack(current);
        }
        if (state.getSquare(realCurrent).isFood)    //if "current" is a square containing food)
        {
            return bfsBacktrack(current);
        }
        if (!state.getSquare(realCurrent).isVisible && firstNonVisible.col == -1)   //If we reached our first non-visible square, store this result somewhere
        {
            firstNonVisible = current;
        }

        //Check adjacent squares and add them to the queue
        for (int i = 0; i < TDIRECTIONS; i++, dir = (++dir)%TDIRECTIONS)
        {
//            tempLoc = state.getLocation(current+offset, dir)-offset; //Get the location one square in the direction "dir"
//            tempLoc.row %= state.rows;
//            tempLoc.col %= state.cols;
            tempLoc.row = current.row + DIRECTIONS[dir][0]; //Get the location one square in the direction "dir"
            tempLoc.col = current.col + DIRECTIONS[dir][1];

            if (tempLoc.row < 0 || tempLoc.row >= MEM_SIZE ||   //Check boundaries
                tempLoc.col < 0 || tempLoc.col >= MEM_SIZE )
                continue;
            if (mem[tempLoc.row][tempLoc.col].row != -1) //This square has already been explored
                continue;
            if (state.getSquare(tempLoc+offset).isWater ||       //This square contains water. Can't walk there.
                state.getSquare(tempLoc+offset).ant != -1 ||     //This square contains an ant
                state.getSquare(tempLoc+offset).hillPlayer == 0) //This square contains my anthill
                continue;

            mem[tempLoc.row][tempLoc.col] = current;    //point "current" leads to "tempLoc" the fastest
            bfsQueue.push_back(tempLoc);
        }
    }

    //if (firstNonVisible.row != -1) return rand()%5 ? -1 : bfsBacktrack(firstNonVisible);
    if (firstNonVisible.row != -1) return bfsBacktrack(firstNonVisible);

    //If nothing is found
    return -1;
}

int Bot::bfsBacktrack(Location &goal)
{
    Location current = goal;
    while (mem[current.row][current.col].row != MEM_MID || mem[current.row][current.col].col != MEM_MID)    //While the previous square is not the starting point
    {
        current = mem[current.row][current.col];
    }
    Location direction = current - mem[current.row][current.col];   //Get the difference from the current and next position

    //Get the direction
    return getDirection(direction);

    state.bug<<"ERROR: Bot::bfsBacktrack(), no direction found"<<std::endl;
    return -1;
}
