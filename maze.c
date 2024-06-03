#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#define ROW 20
#define COL 20
#define MOVE_LIMIT 25 

SDL_Window*window = NULL;
SDL_Renderer*renderer = NULL;
SDL_Texture*wallTexture = NULL;
SDL_Texture*pathTexture = NULL;
SDL_Texture*solutionTexture = NULL;
SDL_Texture*playerTexture = NULL;
int NO = 0;
int maze[ROW][COL];
int playerRow =0,playerCol = 0;
int movesLeft = MOVE_LIMIT;
bool gameRunning = true;
void createPopup(SDL_Window *parentWindow) {
    int w = 300;
    int h = 200;
    SDL_bool running = SDL_TRUE;
    SDL_Window *popup = SDL_CreateWindow("Moves up", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    SDL_Renderer *popuprenderer = SDL_CreateRenderer(popup, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("roboto.ttf", 36);
    if (!font) {
        printf("Error: Font loading failed - %s\n", TTF_GetError());
        return;
    }
    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Moves up", textColor);
    if (!textSurface) {
        printf("Error: Text surface creation failed\n");
        TTF_CloseFont(font);
        return;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(popuprenderer, textSurface);
    if (!tex) {
        printf("Error: Texture creation failed\n");
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }
    SDL_FreeSurface(textSurface);

    int tw, th;
    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);

    int lineY = h;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE))
                running = SDL_FALSE;
        }

        SDL_SetRenderDrawColor(popuprenderer, 255, 255, 255, 255);
        SDL_RenderClear(popuprenderer);

        SDL_Rect textRect = { 50, 80, tw, th };
        SDL_RenderCopy(popuprenderer, tex, NULL, &textRect);


        SDL_RenderPresent(popuprenderer);
        SDL_Delay(30);
    }

    SDL_DestroyTexture(tex);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(popuprenderer);
    SDL_DestroyWindow(popup);
}

void initSDL(){
	SDL_Init(SDL_INIT_VIDEO);
	if (TTF_Init() != 0) {
        printf("Error: SDL TTF initialization failed - %s\n", TTF_GetError());
        SDL_Quit();
        return;
    }
	window = SDL_CreateWindow("Maze Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,COL*30,ROW*30,SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
}
void loadTextures(){
	wallTexture = SDL_CreateTextureFromSurface(renderer,SDL_LoadBMP("wall.bmp"));
	pathTexture = SDL_CreateTextureFromSurface(renderer,SDL_LoadBMP("path.bmp"));
	playerTexture = SDL_CreateTextureFromSurface(renderer,SDL_LoadBMP("player.bmp"));
	solutionTexture = SDL_CreateTextureFromSurface(renderer,SDL_LoadBMP("solution.bmp"));
}
void generateMaze(){
	int predefinedMaze[3][ROW][COL] = {
	{ {0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
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
    },
      { {0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0},
	},
      { {0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
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

	}
	};
	for(int i = 0; i<ROW;++i){
		for(int j = 0; j<COL;++j){
			maze[i][j] = predefinedMaze[NO%3][i][j];
		}
	}
	NO += 1;
}
void draw(){
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
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

void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gameRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_KP_8:
                        if (playerRow > 0 && maze[playerRow - 1][playerCol] == 0) {
                            playerRow--;
                            movesLeft--;
                        }
                        break;
                    case SDLK_KP_2:
                        if (playerRow < ROW - 1 && maze[playerRow + 1][playerCol] == 0) {
                            playerRow++;
                            movesLeft--;
                        }
                        break;
                    case SDLK_KP_4:
                        if (playerCol > 0 && maze[playerRow][playerCol - 1] == 0) {
                            playerCol--;
                            movesLeft--;
                        }
                        break;
                    case SDLK_KP_6:
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
bool isSafe(int row,int col){
	return (row>=0 &&row<ROW&&col>=0&&col<COL&&maze[row][col] == 0);
}
bool solveMazeUtil(int row, int col){
	if(row == ROW-1&& col == COL-1){
		maze[row][col] = 2;
		return true;
	}
	if(isSafe(row,col)){
		maze[row][col] = 2;
		if(solveMazeUtil(row+1,col))
			return true;
		if(solveMazeUtil(row,col+1))
			return true;
		if(solveMazeUtil(row-1,col))
			return true;
		if(solveMazeUtil(row,col-1))
			return true;
		maze[row][col] = 0;
		return false;
	}
	return false;
}
void solveMaze(){
	solveMazeUtil(0,0);
}
int main() {
    initSDL();
    loadTextures();
    while (gameRunning) {
	generateMaze();
	playerRow = 0;
	playerCol = 0;
	movesLeft = MOVE_LIMIT;
	bool solved = false;
	while(gameRunning &&!solved ){
        	handleInput();
        	draw();
        	if (movesLeft == 0) {
            		solveMaze();
	    		draw();
//			initSDL();
			createPopup(window);
			SDL_Delay(5000);
			solved = true;
		}
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

