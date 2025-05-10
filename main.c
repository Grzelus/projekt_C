#include<SDL3/SDL.h>

struct Puzzle
{
    bool whichBoard;
    int actualPosition;
    int startPosition;
    int destination;
    //image ;  do wyszukania
};

struct Board
{
    int widthOfBoard;
    int heightOfBoard;
    //int TableOfPositions[heightOfBoard][widthOfBoard][2];
    //image
};

/*struct resetButton
{
    
};*/




int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Puzzle", 800, 600, 0);
    if(!win){
        SDL_Log("Failed to make a window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);
    if(!renderer){
        SDL_Log("Failed to make renderer %s", SDL_GetError());
        SDL_Quit();
        return -2;
    }

    SDL_Delay(2000);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
