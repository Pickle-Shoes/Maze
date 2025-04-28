#include "maze.h"

Cell::Cell(int _x, int _y) 
{
    x = _x;
    y = _y;
    visited = false;
    walls[0] = walls[1] = walls[2] = walls[3] = true;
}

Maze::Maze(int r, int c)
{
    score = 0; //intialize the score to zero
    rows = r;
    cols = c;
    playerPosition = {0, 0};
    grid = new Cell**[rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new Cell*[cols];
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = new Cell(i, j);
        }
    }


    for(int i = 0; i < rows; i++){ //randomly sprinkle in rewards for the player to grab
        for(int j = 0; j < cols; j++){
            int prize = rand() % 10;
            rewardMap[grid[i][j]] = prize; //insert it into the hash map
        }
    }



}

Maze::~Maze() 
{ //delete the grid
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            delete grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
}

void Maze::displayMaze() 
{
    for (int i = 0; i < rows; ++i) {
        // Print top walls
        for (int j = 0; j < cols; ++j) {
            cout << (grid[i][j]->walls[0] ? "+---" : "+   ");
        }
        cout << "+" << endl;
        // Print left walls and cells
        for (int j = 0; j < cols; ++j) {
            if (playerPosition.first == i && playerPosition.second == j) {
                cout << (grid[i][j]->walls[3] ? "| P " : "  P ");
            } else {
                cout << (grid[i][j]->walls[3] ? "|   " : "    ");
            }

            if(rewardMap.at(grid[i][j]) == 4){ //if there is a reward there place one
                cout << "\b*";
            }
        }
        cout << "|" << endl;
    }
    // Print bottom line
    for (int j = 0; j < cols; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}

void Maze::generateMaze() 
{
    // Implementing randomized DFS for maze generation using stack
    stack<Cell*> cellStack;
    Cell* startCell = grid[0][0];
    startCell->visited = true;
    cellStack.push(startCell);
    while (!cellStack.empty()) {
        Cell* current = cellStack.top();
        Cell* neighbors[4];
        int neighborCount = getUnvisitedNeighbors(current, neighbors);
        if (neighborCount > 0) {
            Cell* next = neighbors[rand() % neighborCount];
            removeWalls(*current, *next);
            next->visited = true;
            cellStack.push(next);
        } else {
            cellStack.pop();
        }
    }

    setUnvisited();
}

void Maze::play() 
{
    char input;
    int h = 0; //to determine wether or not to print the hint
    minHeapThread();
    while (true) {
        system("cls"); // Clear screen for Windows
        displayMaze();
        
        if(h == 1){ //call after clear so it doesn't get erased
            Cell* start = grid[playerPosition.first][playerPosition.second];
            bfsHint(start); //pass the current player position
        }
        h = 0; //reset h so it doesn't give the hint every time


        cout << "Use WASD to move (W = up, A = left, S = down, D = right), Q to quit. Z to undo." << endl;
        input = _getch();
        if (input == 'q' || input == 'Q') {
            break;
        }
        movePlayer(input);

        if (input == 'f' || input == 'F'){ //hint function
            h = 1;
        }

        if (playerPosition == make_pair(rows - 1, cols - 1)) {
            system("cls");

            displayMaze();
            cout << "Congratulations! You've reached the end of the maze!" << endl;
            cout << "Your score was: " << score << endl;
            break;
        }
    }
    
}

int Maze::getUnvisitedNeighbors(Cell* cell, Cell* neighbors[]) 
{
    int x = cell->x;
    int y = cell->y;
    int count = 0;

    // Check all four neighbors
    //check if the cell is not on left edge and then see if the left cell is visited
    if (x > 0 && !grid[x - 1][y]->visited) neighbors[count++] = grid[x - 1][y];
    //check if the cell is not on top edge and then see if the top cell is visited
    if (y > 0 && !grid[x][y - 1]->visited) neighbors[count++] = grid[x][y - 1];
    //check if the cell is not on the right edge and then see if the right cell is visited
    if (x < rows - 1 && !grid[x + 1][y]->visited) neighbors[count++] = grid[x + 1][y];
    //check if the cell is not in the bottom edge and then see if the bottom cell is visited
    if (y < cols - 1 && !grid[x][y + 1]->visited) neighbors[count++] = grid[x][y + 1];

    return count;
}

void Maze::removeWalls(Cell& current, Cell& next) 
{
    int xDiff = current.x - next.x;
    int yDiff = current.y - next.y;
    if (xDiff == 1) {
        current.walls[0] = false;
        next.walls[2] = false;
    } else if (xDiff == -1) {
        current.walls[2] = false;
        next.walls[0] = false;
    } else if (yDiff == 1) {
        current.walls[3] = false;
        next.walls[1] = false;
    } else if (yDiff == -1) {
        current.walls[1] = false;
        next.walls[3] = false;
    }
}

void Maze::movePlayer(char direction) 
{

    int x = playerPosition.first;
    int y = playerPosition.second;

    if(rewardMap.at(grid[x][y]) == 4){ // if there is a reward at this cell update score and change the prize value
        score++;
        rewardMap.at(grid[x][y]) = 3;
    }
    

    //push to the stack every time the player moves to get their position then pop it twice for x and y when they undo
    if (direction == 'w' || direction == 'W') {
        if (x > 0 && !grid[x][y]->walls[0]) {
            playerPosition.first--;
            undoX.push(x);
            undoY.push(y);
        }
    } else if (direction == 'a' || direction == 'A') {
        if (y > 0 && !grid[x][y]->walls[3]) {
            playerPosition.second--;
            undoX.push(x);
            undoY.push(y);
        }
    } else if (direction == 's' || direction == 'S') {
        if (x < rows - 1 && !grid[x][y]->walls[2]) {
            playerPosition.first++;
            undoX.push(x);
            undoY.push(y);
        }
    } else if (direction == 'd' || direction == 'D') {
        if (y < cols - 1 && !grid[x][y]->walls[1]) {
            playerPosition.second++;
            undoX.push(x);
            undoY.push(y);
        }
    } else if (direction == 'z' || direction == 'Z'){ //undo that checks if the stacks are empty and then moves to the previous position
        if(!(undoX.empty() || undoY.empty())){
        
        playerPosition = pair(undoX.top(), undoY.top()); //set player position to previous position

        undoX.pop();
        undoY.pop();
        }
    }
    
}

// HEAP FUNCTIONS ------------------------------------------------------------------------------------------------

void Maze::minHeapThread(){
    thread heapThread(&Maze::heapTimer, this); //create a seperate thread to run at the same time
    heapThread.detach(); //runs on its own
}

void Maze::heapTimer(){
    HEAP heap(10); //intiliaze a heap
    int i = 0; //the timer value
    while(true){
        heap.insertH(i); //insert the time value into the heap
        int timer = heap.peek(); //set the timer value as the top of the min heap
        cout << "Timer: " << timer << endl;
        heap.deleteH(i);//delete the value in the heap that has the same value as i
        i++;//incriment the time
        if (playerPosition == make_pair(rows - 1, cols - 1)) { //keeps running until the player reaches the end
            cout << "Finished in : " << timer << "secs \n"; //show the time the player finished the game in
            break;
        }
        Sleep(1000); //wait one second
    }
}


//BFS traversal---------------------------------------------------------------------------------------------------

void Maze::bfsHint(Cell* start){ 
    queue<Cell*> cellQueue; //queue for bfs traversal
    cellQueue.push(start); //push the starting cell and set to visited, its parent to null
    start->visited = true; 
    start->parent = nullptr; 
    Cell* end = nullptr; //initialize the end cell pointer

    while(!cellQueue.empty()){
        Cell* current = cellQueue.front();
        cellQueue.pop();

        if(current == grid[rows - 1][cols -1]){//if the cell is at the end of the maze 
            end = current; //set the end pointer to the last cell or finish
            break;
        }

        neighbors(current, cellQueue); //enqueue the neighbors possible
    }

    if(end == nullptr){
        cout << "no path found" << endl;
        return;
    }

    while(end->parent != start){ //go backwards until your the cell right before the starting point
        end = end->parent;
    }

    if(end->y == start->y - 1){
        cout << "Hint: go left" << endl;
    }
    if(end->x == start->x + 1){
        cout << "Hint: go down" << endl;
    }
    if(end->y == start->y + 1){
        cout << "Hint: go right" << endl;
    }
    if(end->x == start->x - 1){
        cout << "Hint: go up" << endl;
    }

    setUnvisited(); //unvisit all the cells
}

void Maze::neighbors(Cell* current, queue<Cell*>& cellQueue){ //enqueue the unvisited neighbors
    int x = current->x;
    int y = current->y;

    if((x > 0) && !(grid[x -1][y]->visited) && !(current->walls[0])){ //check if you can enqueue the top... no walls, not visited
        cellQueue.push(grid[x - 1][y]); //enqueue the top
        grid[x - 1][y]->parent = current; //set its parent to the previous cell
        grid[x - 1][y]->visited = true; //make the cell visited
    } 
    if ((y < cols - 1) && !(grid[x][y + 1]->visited) && !(current->walls[1])){//check if you can enqueue the right... no walls, not visited
        cellQueue.push(grid[x][y + 1]);//enqueue the right
        grid[x][y + 1]->parent = current;
        grid[x][y + 1]->visited = true;
    } 
    if ((x < rows - 1) && !(grid[x + 1][y]->visited) && !(current->walls[2])){//check if you can enqueue the bottom... no walls, not visited
        cellQueue.push(grid[x + 1][y]);//enqueue the bottom
        grid[x + 1][y]->parent = current; 
        grid[x + 1][y]->visited = true; 
    } 
    if ((y > 0) && !(grid[x][y - 1]->visited) && !(current->walls[3])){
        cellQueue.push(grid[x][y - 1]); //enqueue the left
        grid[x][y - 1]->parent = current;
        grid[x][y - 1]->visited = true;

    }

}

void Maze::setUnvisited(){ //set every cell's visited bool to false and its parent to null ptr;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            grid[i][j]->visited = false;
            grid[i][j]->parent = nullptr;
        }
    }
}
