#include "Bot.h"

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
        state.bug<<"Starting position: "<<state.myAnts[ant].row<<","<<state.myAnts[ant].col<<std::endl;
        int dir = bfs(state.myAnts[ant]);
        state.bug<<CDIRECTIONS[dir]<<std::endl;

        if (dir != -1)
        {
            Location loc = state.getLocation(state.myAnts[ant], dir);
            if(!state.grid[loc.row][loc.col].isWater)
                state.makeMove(state.myAnts[ant], dir);
            continue;
        }
        dir = rand()%TDIRECTIONS;

        //If there's food nearby, then approach the food. 90% chance of approaching the food.
//        if (state.food.size() != 0 && rand()%10 != 0)
//        {
//            int closestFoodIndex = state.getClosest(state.myAnts[ant], state.food);
//            dir = moveTo(state.myAnts[ant], state.food[closestFoodIndex]);
//        }
//        else
//        {
//            dir = rand()%TDIRECTIONS;   //Pick a random direction
//        }

        for(int i = 0; i<TDIRECTIONS; i++, dir = (dir+1)%TDIRECTIONS)   //Loop through all possible directions
        {
            Location loc = state.getLocation(state.myAnts[ant], dir);

            if(!state.grid[loc.row][loc.col].isWater &&             //If the picked direction is not water
               !(state.grid[loc.row][loc.col].hillPlayer == 0) &&   // or my own anthill
               state.grid[loc.row][loc.col].ant == -1)              //  and doesn't contain an ant
            {
                state.makeMove(state.myAnts[ant], dir);
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

//int Bot::moveTo(Location &from, Location &to)
//{
//    Location direction(from.row - state.food[0].row,
//                       from.col - state.food[0].col);
//    if (direction.row < 0 && direction.col < 0)
//    {
//        return WEST;
//    }
//    if (direction.row > 0 && direction.col < 0)
//    {
//        return NORTH;
//    }
//    if (direction.row > 0 && direction.col > 0)
//    {
//        return EAST;
//    }
//    if (direction.row < 0 && direction.col > 0)
//    {
//        return SOUTH;
//    }
//    return 0;
//}

/**
@param from
    ?????
@return
    Direction in which to move to reach the closest food item.
    If no nearby food is found, returns -1
*/
int Bot::bfs(Location &from)
{
    //Reset everything to -1 (marks as unexplored)
    for (int i = 0; i < MEM_SIZE; i++)
    {
        for (int j = 0; j < MEM_SIZE; j++)
        {
            mem[i][j].col = -1;
            mem[i][j].row = -1;
        }
    }
    //Current position is marked as explored
    mem[MEM_MID][MEM_MID].row = 0;
    mem[MEM_MID][MEM_MID].col = 0;

    Location offset = from;
    offset.row -= MEM_MID;
    offset.col -= MEM_MID;

    bfsQueue.clear();
    bfsQueue.push_back(Location(MEM_MID,MEM_MID));    //Add starting location to queue
    Location current;
    Location tempLoc;
    while(bfsQueue.size() > 0)
    {
        current = bfsQueue.front();
        bfsQueue.pop_front();

        //If a goal has been reached (i.e. if "current" is a square containing food)
        if (state.grid[current.row+offset.row][current.col+offset.col].isFood)
        {
            return bfsBacktrack(current);
        }

        //Check adjacent squares and add them to the queue
        for (int dir = 0; dir < TDIRECTIONS; dir++)
        {
            tempLoc = state.getLocation(current+offset, dir)-offset; //Get the location one square in the direction "dir"

            if (tempLoc.row < 0 || tempLoc.row >= MEM_SIZE ||   //Check boundaries
                tempLoc.col < 0 || tempLoc.col >= MEM_SIZE )
                continue;
            if (mem[tempLoc.row][tempLoc.col].row != -1) //This square has already been explored
                continue;
            if (state.grid[tempLoc.row+offset.row][tempLoc.col+offset.col].isWater ||       //This square contains water. Can't walk there.
                state.grid[tempLoc.row+offset.row][tempLoc.col+offset.col].ant != -1 ||     //This square contains an ant
                state.grid[tempLoc.row+offset.row][tempLoc.col+offset.col].hillPlayer == 0) //This square contains my anthill
                continue;

            mem[tempLoc.row][tempLoc.col] = current;    //point "current" leads to "tempLoc" the fastest
            bfsQueue.push_back(tempLoc);
        }
    }

    //If nothing is found
    return -1;
}

int Bot::bfsBacktrack(Location &goal)
{
    Location current = goal;
    state.bug<<"Backtracking: \n";
    while (mem[current.row][current.col].row != MEM_MID || mem[current.row][current.col].col != MEM_MID)    //While the previous square is not the starting point
    {
        state.bug<<current.row<<","<<current.col<<" \t ";
        state.bug<<mem[current.row][current.col].row<<","<<mem[current.row][current.col].col<<std::endl;
        current = mem[current.row][current.col];
    }
    state.bug<<"\n";
    Location direction = current - mem[current.row][current.col];   //Get the difference from the current and next position

    //Get the direction
    for (int i = 0; i < TDIRECTIONS; i++)
    {
        if (DIRECTIONS[i][0] == direction.row && DIRECTIONS[i][1] == direction.col)
        {
            return i;
        }
    }
    state.bug<<"ERROR: Bot::bfsBacktrack(), no direction found"<<std::endl;
    return -1;
}
