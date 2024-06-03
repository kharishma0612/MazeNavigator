#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

#define ROW 20
#define COL 20
#define MOVE_LIMIT 10

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* wallTexture = NULL;
SDL_Texture* pathTexture = NULL;
SDL_Texture* playerTexture = NULL;
SDL_Texture* solutionTexture = NULL;
int maze[ROW][COL];
int playerRow = 0, playerCol = 0;
int movesLeft = MOVE_LIMIT;
bool gameRunning = true;

// Function to initialize SDL and create a window
void initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, COL * 30, ROW * 30, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

// Function to load textures
void loadTextures() {
    wallTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("wall.bmp"));
    pathTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("path.bmp"));
    playerTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("player.bmp"));
    solutionTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("solution.bmp"));
}
   int predefinedMaze[ROW][COL] = {
        {0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0},
    };

void generateMaze() {
    // Define the maze with a solution path
   int predefinedMaze[ROW][COL] = {
        {0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0},
    };

    // Copy the predefined maze to the maze array
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            maze[i][j] = predefinedMaze[i][j];
        }
    }
}

// Function to generate random maze
//void generateMaze() {
  //  srand(time(NULL));
    //for (int i = 0; i < ROW; ++i) {
      //  for (int j = 0; j < COL; ++j) {
        //    if (rand() % 3 == 0) {
          //      maze[i][j] = 1; // 1 represents wall
            //} else {
              //  maze[i][j] = 0; // 0 represents path
            //}
        //}
    //}
    //maze[0][0] = 0; // Starting point
    //maze[ROW - 1][COL - 1] = 0; // Destination
//}

// Function to draw the maze and player
void draw() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            SDL_Rect destRect = { j * 30, i * 30, 30, 30 };
            if (maze[i][j] == 1) {
                SDL_RenderCopy(renderer, wallTexture, NULL, &destRect);
            }
	else if (maze[i][j] == 2){
		SDL_RenderCopy(renderer,solutionTexture,NULL,&destRect);
	}
	 else {
                SDL_RenderCopy(renderer, pathTexture, NULL, &destRect);
            }
        }
    }
    
    SDL_Rect playerRect = { playerCol * 30, playerRow * 30, 30, 30 };
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
    SDL_RenderPresent(renderer);
}

// Function to handle user input
void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gameRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (playerRow > 0 && maze[playerRow - 1][playerCol] == 0) {
                            playerRow--;
                            movesLeft--;
                        }
                        break;
                    case SDLK_DOWN:
                        if (playerRow < ROW - 1 && maze[playerRow + 1][playerCol] == 0) {
                            playerRow++;
                            movesLeft--;
                        }
                        break;
                    case SDLK_LEFT:
                        if (playerCol > 0 && maze[playerRow][playerCol - 1] == 0) {
                            playerCol--;
                            movesLeft--;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (playerCol < COL - 1 && maze[playerRow][playerCol + 1] == 0) {
                            playerCol++;
                            movesLeft--;
                        }
                        break;
                }
                break;
        }
    }
}
bool dfs(int row, int col, bool visited[ROW][COL]) {
    if (row == ROW - 1 && col == COL - 1) {
        return true; // Reached destination
    }
    
    static const int dr[] = {1, -1, 0, 0};
    static const int dc[] = {0, 0, 1, -1};
    
    for (int i = 0; i < 4; ++i) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (newRow >= 0 && newRow < ROW && newCol >= 0 && newCol < COL && !visited[newRow][newCol] && maze[newRow][newCol] == 0) {
            visited[newRow][newCol] = true;
            if (dfs(newRow, newCol, visited)) {
                return true;
            }
        }
    }
    
    return false;
}


// Function to solve the maze
void solveMaze() {
    bool visited[ROW][COL];
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            visited[i][j] = false;
        }
    }
    visited[0][0] = true;
	for(int i = 0; i<ROW;++i){
		for(int j = 0; j<COL;++j){
			maze[i][j] = predefinedMaze[i][j];
		}
	}
    // Depth-first search to find solution
    bool found = dfs(0, 0, visited);
    if (found) {
        for (int i = 0; i < ROW; ++i) {
            for (int j = 0; j < COL; ++j) {
                if (visited[i][j]) {
                    maze[i][j] = 2; // Mark solution path in maze array
                }
            }
        }
	
    }
         

}

// Recursive function for depth-first search

// Main function
int main() {
    initSDL();
    loadTextures();
    generateMaze();
    
    while (gameRunning) {
        handleInput();
        draw();
        if (movesLeft == 0) {
            solveMaze();
        }
    }


    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(pathTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(solutionTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
