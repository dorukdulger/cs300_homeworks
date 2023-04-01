#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include "maze.h"
#include "LinkedListStack.h"
#include <algorithm>

using namespace std;

int maze::Random(unsigned long n){
    // Create a random number generator engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, n-1);
    int rndm = dis(gen);
    return rndm;
}

maze::maze(int RowNum, int ColNum, int mazeid)
{
    row = RowNum;
    col = ColNum;
    Cell temp(0, 0);
    this->mazeMap = vector<vector<Cell>>(col, vector<Cell>(row, temp));
    
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            this->mazeMap[j][i] = Cell(j, i);
        }
    }
    mazeID = mazeid;
}


vector<vector<Cell>> maze::MazeGenerator(int M, int N, int mazeID){
    LinkedListStack<Cell> cellStack;
    unsigned long randIndex;
    // Push the starting cell to the stack
    cellStack.push(mazeMap[0][0]);
    
    // While there are unvisited cells
    while (!cellStack.isEmpty()) {
        // Pop the top cell from the stack
        Cell currentCell = cellStack.topAndPop();
        
        // Mark the current cell as visited
        mazeMap[currentCell.cols][currentCell.rows].visited = true;
        currentCell.visited = true;
        
        // Create a vector of unvisited neighbors
        vector<Cell> neighbors;
        
        if (currentCell.rows < M - 1 && !mazeMap[currentCell.cols ][currentCell.rows + 1].visited) {
            neighbors.push_back(mazeMap[currentCell.cols][currentCell.rows + 1]); // Top neighbor
        }
        if (currentCell.rows > 0 && !mazeMap[currentCell.cols][currentCell.rows - 1].visited) {
            neighbors.push_back(mazeMap[currentCell.cols ][currentCell.rows - 1]); // Bottom neighbor
        }
        if (currentCell.cols > 0 && !mazeMap[currentCell.cols - 1][currentCell.rows ].visited) {
            neighbors.push_back(mazeMap[currentCell.cols - 1][currentCell.rows ]); // Left neighbor
        }
        if (currentCell.cols < N - 1 && !mazeMap[currentCell.cols + 1][currentCell.rows ].visited) {
            neighbors.push_back(mazeMap[currentCell.cols + 1][currentCell.rows]); // Right neighbor
        }
        
        // If there are unvisited neighbors
        if (!neighbors.empty()) {
            // Choose a random unvisited neighbor
            randIndex = Random(neighbors.size());
            Cell neighborCell = neighbors[randIndex];
            
            // Remove the wall between the current cell and the chosen neighbor
            if (neighborCell.rows == currentCell.rows + 1) { //Top wall/neighbor
                mazeMap[currentCell.cols][currentCell.rows].up = false;
                mazeMap[neighborCell.cols][neighborCell.rows].down = false;
            }
            else if (neighborCell.rows == currentCell.rows - 1) { //Bottom wall/neighbor
                mazeMap[currentCell.cols][currentCell.rows].down = false;
                mazeMap[neighborCell.cols][neighborCell.rows].up = false;
            }
            else if (neighborCell.cols == currentCell.cols - 1) { //Left wall/neighbor
                mazeMap[currentCell.cols][currentCell.rows].left = false;
                mazeMap[neighborCell.cols][neighborCell.rows].right = false;
            }
            else if (neighborCell.cols == currentCell.cols + 1) { //Right wall/neighbor
                mazeMap[currentCell.cols][currentCell.rows].right = false;
                mazeMap[neighborCell.cols][neighborCell.rows].left = false;
            }
            
            // Push the current cell and the chosen neighbor to the stack
            cellStack.push(currentCell);
            cellStack.push(neighborCell);
        }
    }
    
    //Print maze
    ofstream print("maze_" + to_string(mazeID) + ".txt");
    string final = to_string(row) + " " + to_string(col) + "\n" ;
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            Cell curr = mazeMap[j][i];
            final += "x="+ to_string(j) + " y=" + to_string(i) + " l=" + to_string(curr.left) + " r=" + to_string(curr.right) + " u=" + to_string(curr.up) + " d=" + to_string(curr.down) + "\n";
        }
    }
    
    print << final;
    print.close();
    
    return mazeMap;
}

void maze::PathFinder(int X_entry, int Y_entry, int X_dest, int Y_dest, int M, int N){
    LinkedListStack<Cell> cellStack;
    unsigned long randIndex;
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            mazeMap[j][i].visited = false;
        }
    }
    
    // Push the starting cell to the stack
    cellStack.push(mazeMap[X_entry][Y_entry]);
    Cell exit = mazeMap[X_dest][Y_dest];
    Cell entry = mazeMap[X_entry][Y_entry];
    
    while(!cellStack.isEmpty()){
        Cell currentCell = cellStack.top();
        
        // Mark the current cell as visited
        mazeMap[currentCell.cols][currentCell.rows].visited = true;
        currentCell.visited = true;
        
        if (mazeMap[exit.cols][exit.rows].visited) {
            // The exit cell is reached, backtrack from the exit cell to the entry cell using the stack
            vector<Cell> printList;
            while (!(cellStack.top().rows == entry.rows && cellStack.top().cols == entry.cols)) {
                Cell currentCell = cellStack.topAndPop();
                cellStack.pop();
                printList.push_back(currentCell);
            }
            
            //Reverse the stack and print it
            reverse(printList.begin(), printList.end());
            printList.pop_back();
            ofstream printPath("maze_" + to_string(mazeID) + "_" + to_string(X_entry) + "_" + to_string(Y_entry) + "_" + to_string(X_dest)  + "_" + to_string(Y_dest) + ".txt");
            string finalize = to_string(entry.cols) + " " + to_string(entry.rows) + "\n" ;
            
            for(auto cells : printList){
                finalize += to_string(cells.cols) + " " + to_string(cells.rows) + "\n";
            }
            finalize += to_string(exit.cols) + " " + to_string(exit.rows);
            printPath << finalize;
            printPath.close();
            
            break;
        }
        
        // Create a vector of unvisited neighbors
        vector<Cell> neighbors;
        
        if (currentCell.rows < M - 1 && !mazeMap[currentCell.cols][currentCell.rows + 1].down && !mazeMap[currentCell.cols ][currentCell.rows + 1].visited) {
            neighbors.push_back(mazeMap[currentCell.cols][currentCell.rows + 1]); // Top neighbor
        }
        if (currentCell.rows > 0 && !mazeMap[currentCell.cols][currentCell.rows - 1].up && !mazeMap[currentCell.cols][currentCell.rows - 1].visited) {
            neighbors.push_back(mazeMap[currentCell.cols ][currentCell.rows - 1]); // Bottom neighbor
        }
        if (currentCell.cols > 0 && !mazeMap[currentCell.cols - 1][currentCell.rows].right && !mazeMap[currentCell.cols - 1][currentCell.rows ].visited) {
            neighbors.push_back(mazeMap[currentCell.cols - 1][currentCell.rows ]); // Left neighbor
        }
        if (currentCell.cols < N - 1 && !mazeMap[currentCell.cols + 1][currentCell.rows].left && !mazeMap[currentCell.cols + 1][currentCell.rows ].visited) {
            neighbors.push_back(mazeMap[currentCell.cols + 1][currentCell.rows]); // Right neighbor
        }
        
        if (!neighbors.empty()) {
            // Choose a random unvisited neighbor
            randIndex = Random(neighbors.size());
            Cell neighborCell = neighbors[randIndex];
            
            // Push the current cell and the chosen neighbor to the stack
            cellStack.push(currentCell);
            cellStack.push(neighborCell);
        }else{
            cellStack.pop();
        }
    }
}
