#include "maze.h"

int main() {
    int rows = 10;
    int cols = 10;

    
    Maze maze(rows, cols);
    maze.generateMaze();
    maze.play();

    return 0;
}
