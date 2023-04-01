#include <iostream>
#include "maze.h"

int main(){
    int rowNum,colNum,X_entry,Y_entry,X_dest,Y_dest,k_value, mazeID;
    
    cout << "Enter the number of mazes: ";
    cin >> k_value;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> rowNum >> colNum;
    
    vector<maze*> mazeVec;
    for (int i=0; i < k_value ; i++){
        maze* M = new maze(rowNum, colNum, i+1);
        M->MazeGenerator(rowNum, colNum, i+1);
        mazeVec.push_back(M);
    }

    cout << "All mazes are generated.\n" << endl;
    
    cout << "Enter a maze ID between 1 to " << k_value << " inclusive to find a path: ";
    cin >> mazeID;
    
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> X_entry >> Y_entry;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> X_dest >> Y_dest;

    mazeVec[mazeID-1]->PathFinder(X_entry, Y_entry, X_dest, Y_dest, rowNum, colNum);
    
    return 0;
}
