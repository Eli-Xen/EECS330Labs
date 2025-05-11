#ifndef _MYMAZE_H_
#define _MYMAZE_H_

#include <iostream>
#include <cstdlib> 
#include <algorithm> 
#include <ctime>
#include <utility> 

#include "MyDisjointSets.h"

class MyMaze 
{ public:
	explicit MyMaze(const size_t r, const size_t c) : numRows(r), numCols(c), dSets(r * c)  // constructor
	{   leftRightWalls=new bool*[numRows]; // like: grid = [_, _, _]; allocate array of row pointers; bool* is a pointer to a single row or bools; bool** is an array or bool row pointers 
        for (size_t i=0; i<numRows; ++i) //go through each row and makes numCols-1 bools and fills it with true 
        {   leftRightWalls[i]=new bool[numCols-1]; //allocate array of column bools for that row, cols-1 becase for 4 columns we need 3 walls 
            std::fill(leftRightWalls[i], leftRightWalls[i]+numCols-1, true); //fills the row with true 
        } //fill takes start to end and what to fill with, leftRightWalls[i] is the start of the row and the leftRightWalls[i]+numCols-1 is the pointer just after the last element 

        topDownWalls=new bool*[numRows-1]; //allocate array of row pointers 
        for (size_t i=0; i<numRows-1; ++i) 
        {   topDownWalls[i]=new bool[numCols]; //allocate array of column bools for that row 
            std::fill(topDownWalls[i], topDownWalls[i]+numCols, true); //fill w true to indicate there are walls 
        }
	}

    ~MyMaze(void) // destructor deletes the 2d arrays 
    {   for (size_t i=0; i<numRows; ++i) //delete each array in each row 
            delete[] leftRightWalls[i]; 
        delete[] leftRightWalls; //delete outside array 

        for (size_t i=0; i<numRows-1; ++i) //delete each array in each column 
            delete[] topDownWalls[i]; 
        delete[] topDownWalls; //delete outside array 
    }

    // the top wall of the first cell (the top-left corner) should be open as the entrance
    // the bottom wall of the last cell (the bottom-right corner) should be open as the exit
	void buildMaze(void) // builds a random maze; randomly keep breaking walls until the entrance and exit are connected (in the same set); Kruskal's algorithm 
	{   topDownWalls[0][0]=false; //opens the top wall of the first cell 
        topDownWalls[numRows-2][numCols-1]=false; //opens bottom wall of last cell; theres only numRows-1 topDown walls so to break the bottom one we do numRows-2 
        std::vector<std::pair<int,int>> walls; //vector that stores all possible walls; easier to manage than two 2d arrays 

        for (size_t row=0; row<numRows; ++row) //iterate through the walls vector and initilize all walls; starts from top right corner and makes row of vertical walls | then makes the bottom walls for them 
        {   for (size_t col=0; col<numCols; ++col) //iterate through columns of walls 
            {   int current=row*numCols+col; //current place were at is, flattening 2D cells (row,col) to 1D index 
                if (col+1<numCols) //if right neighbor exists and within bounds...
                {   int right=row*numCols+(col+1); //get index of right wall 
                    walls.push_back({current,right}); //pushes pair of indecies that is the wall betweent the current and right neighbors 
                }
                if (row+1<numRows) //if neighbor below exists and within bounds...
                {   int down=(row+1)*numCols+col; //get index of neighbor below 
                    walls.push_back({current,down}); //pushes pair of cell indecies that is the wall between the current and cell bellow 
                }
            }
        }

        std::srand(unsigned(std::time(nullptr))); //uses current time as a seed to shuffle the walls 
        std::random_shuffle(walls.begin(), walls.end()); //shuffle all the walls randomly so we get random maze 

        for (const auto & wall:walls) //for wall in walls...
        {   int cellA=wall.first; //gets pair of indecies for first cell 
            int cellB=wall.second; //gets pair of indecies for second cell 
    
            if (dSets.find(cellA)!=dSets.find(cellB)) //if cells arent in the same set merge/union the sets and remove the wall between the two cells 
            {   dSets.unionSets(cellA, cellB); //merge the sets that contain those cells 
    
                // Convert cellA and cellB back to (row, col)
                int rowA=cellA/numCols; //gets which row first cell is in, rounds down/floors 
                int colA=cellA%numCols; //gets column of first cell, round up/ceiling 
                int rowB=cellB/numCols; //these calulcations come from current=row*numCols+col that we did earlier so index 6=1*4+2 
                int colB=cellB%numCols; 
    
                //remove wall between cellA and cellB
                if (rowA==rowB) //if in the same row then remove vertical wall | between then 
                    leftRightWalls[rowA][std::min(colA, colB)] = false; //since rowA and rowB are in the same set update left right walls to false to show we dont have a wall 
                else //if not in the same row then in the same column, remove horizatnal wall --
                    topDownWalls[std::min(rowA, rowB)][colA] = false; //update to show we dont have wall where we picked 
            }
        }
	}

    void printMaze(std::ostream& out = std::cout) // prints the maze using the two 2D arrays horizantal walls (topDown) use -- and + for corner, vertical walls (leftRight) use | 
    {   out << "+"; //print the top border starts with corner even if there isnt wall, helps count 
        for (size_t col=0; col<numCols; ++col) //start with top of maze and iterate through columns and ...
        {   if (topDownWalls[0][col])//...if first row and column is wall (true)... 
                out << "---+"; //...then draw start to end of wall plus corner 
            else //if no wall (false)...
                out << "   +"; // then draw no wall but still indicate corner; Entrance
        }
        out << "\n"; 

        for (size_t row=0; row<numRows; ++row) //for each row and each column print the row's left/right walls |
        {   for (size_t col=0; col<numCols; ++col) 
            {   if (col==0) out << "|"; //leftmost wall
                    out << "   "; //space for the walkpath/cell 

                if (col<numCols-1) //if its a column inside the maze and not on the edges 
                    out << (leftRightWalls[row][col] ? "|" : " ");
                else 
                    out << "|"; //right wall of last column 
            }
            out << "\n"; //print new line, each set of walls is on a new line reguardless if theyre vertical or hoizantal walls becuase its impossible to make characters actually --+|, only make it look like that 
            
            if (row<numRows-1) //print the row's bottom walls
            {   out << "+";
                for (size_t col=0; col<numCols; ++col) 
                    out << (topDownWalls[row][col] ? "---+" : "   +");
                out << "\n";
            }
        }

        out << "+"; //print the final bottom border
        for (size_t col=0; col<numCols; ++col) 
            out << (topDownWalls[numRows-2][col] ? "---+" : "   +");
        out << "\n";
    }

  private:
	size_t numRows = 0;         // the number of rows of the maze
	size_t numCols = 0;         // the number of columns of the maze
    MyDisjointSets dSets;       // the disjoint set class that keeps track of maze cell connection
    bool** leftRightWalls;      // horizantal walls; the 2D array that indicates whether walls between left-right neighbors present
    bool** topDownWalls;        // vertical walls; the 2D array that indicates whether walls between the top-down neighbors preset

};


#endif // __MYMAZE_H__