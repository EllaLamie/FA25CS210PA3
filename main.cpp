//
// Created by Manju Muralidharan on 11/22/25.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    // Random generator using a seed
    srand(time(0));

    // Randomly assigns each maze cell
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            // 70% of the time will be 0, the other 30% will be 1
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    // Will run until a cell is found
    while (true) {

        // Choose a randomly selected side of the maze
        int side = rand() % 4;
        int r, c;

        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        // Check if the cell is open
        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            // Print S for the start of the maze if ent_r and ent_c are true
            if (r == ent_r && c == ent_c) {
                cout << "S ";

            // If exit_r and exit_c are true then print E for end of maze
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";

            // If neither of those, print 0 or 1 based on the cell number
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    // Find the exit
    int r = exitcell.first;
    int c = exitcell.second;

    // Make a new vector with the path list
    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    // Append ent_r and ent_c into the vector
    path.push_back({ent_r, ent_c});

    // Print path by iteration
    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------
 bool dfs(int r, int c,
          const vector<vector<int>>& maze,
          vector<vector<bool>>& visited,
          vector<vector<int>>& parent_r,
          vector<vector<int>>& parent_c,
          int exit_r, int exit_c) {

     int N = maze.size();
     int M = maze[0].size();

     // Checks for out of bounds cells
     if (r < 0 || r >= N) {
         return false;
     }
     if (c < 0 || c >= M) {
         return false;
     }

     // Checks for walls
     if (maze[r][c] == 1) {
         return false;
     }

     // Checks if cell has already been visited
     if (visited[r][c]) {
         return false;
     }

     // Immediately sets visited to true
     visited[r][c] = true;

     // Base case where we have found a way to get out of the maze
     if (r == exit_r && c == exit_c) {
         return true;
     }

     // Explore neighbors with up/right/down/left
     for (int d = 0; d < 4; d++) {
         int nr = r + dr[d];
         int nc = c + dc[d];

         // Check if the neighbor cell is out of bounds
         if (nr < 0 || nr >= N || nc < 0 || nc >= M) {
           continue;
         }

         //Check if neighbor cell is a wall
         if (maze[nr][nc] == 1) {
             continue;
         }

         // Check if we have already visited neighbor cell
         if (visited[nr][nc]) {
             continue;
         }

         // Assign parent to the original cell
         parent_r[nr][nc] = r;
         parent_c[nr][nc] = c;

         // Recursive call moving to the new column and new row cell
         if (dfs(nr, nc, maze, visited, parent_r, parent_c, exit_r, exit_c)) {

             // Need an if statement to return true because it will then return true in all children
             return true;
         }
     }
     return false;
 };


// ----------------------------------------------------------
// MAIN PROGRAM (students add DFS calls and logic)
// ----------------------------------------------------------
int main() {
    int N, M;

    // Getting the size of the maze
    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;

    // Creating the maze using generateMaze()
    vector<vector<int>> maze(N, vector<int>(M));
    generateMaze(maze, N, M);

    // Pick entrance and exit
    pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);

    while (exitcell == entrance) {
        exitcell = chooseBoundaryCell(maze);
    }

    // Assign entrance and exit (first and second) to row and column entrance and exit
    int ent_r = entrance.first;
    int ent_c = entrance.second;
    int exit_r = exitcell.first;
    int exit_c = exitcell.second;

    // Display the maze
    printMaze(maze, ent_r, ent_c, exit_r, exit_c);

    // Students must use these
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // ------------------------------------------------------
    // STUDENT WORK:
    // Call your DFS, track visited, and fill parent_r and parent_c
    // ------------------------------------------------------
     bool found = dfs(ent_r, ent_c, maze, visited, parent_r, parent_c, exit_r, exit_c);

    // ------------------------------------------------------
    // STUDENT WORK:
    // If found, print the path
    // ------------------------------------------------------
     if (found) {
         printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
     } else {
         cout << "\nNo path exists.\n";
     }

    return 0;
}