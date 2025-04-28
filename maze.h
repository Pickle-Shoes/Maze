#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <stack>
#include <queue>
#include <cstdlib>  
#include <conio.h> 
#include <windows.h> //for sleep
#include <thread> //for timer
#include <unordered_map>

#include "heap.h"

using namespace std;

// Maze structure 
struct Cell {
    int x, y;
    bool visited;
    bool walls[4]; // Top, Right, Bottom, Left

    Cell(int _x, int _y);

    Cell* parent;
    
};

class Maze {
    private:
        int rows, cols;
        Cell*** grid;
        pair<int, int> playerPosition;
        
        int getUnvisitedNeighbors(Cell* cell, Cell* neighbors[]);
        void removeWalls(Cell& current, Cell& next);
        void movePlayer(char direction);

        stack<int> undoX; //stack for x
        stack<int> undoY; //stack for y

        unordered_map<Cell*, int> rewardMap; //for rewards on the maze, hashes a Cell* and stores a int value too
        int score;
    public:
        Maze(int r, int c);
        ~Maze();

        void displayMaze();
        void generateMaze();
        void play();

        void minHeapThread();
        void heapThread();
        void heapTimer();

        void bfsHint(Cell* start);
        void neighbors(Cell* current, queue<Cell*>& cellQueue);
        void setUnvisited();
};

#endif
 