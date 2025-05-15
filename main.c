#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>

#define SDL_NO_OLDNAMES


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

//rysowanie przycisków na oknie wejsciowym

const SDL_FRect buttonRect = {150.0f, 120.0f, 100.0f, 50.0f};

void DrawButton(SDL_Renderer *renderer, SDL_FRect *rect, bool hovered, bool pressed) {
    if (pressed || hovered) {
        SDL_SetRenderDrawColor(renderer, 30, 19, 237, 200);
    } else {
        SDL_SetRenderDrawColor(renderer, 30, 19, 237, 200);
    }

    SDL_RenderFillRect(renderer, rect); // te dwie chat trzy razu chyba zmineniał ale nadal nie jestem pewien czy są dobrze bo warningi cały czas sie wyswietlają
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, rect); //
}


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
    
    bool gameRunning=true;
    while(gameRunning){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    gameRunning=false;
                    break;
            }
        }
        SDL_RenderClear(renderer);
        SDL_Log("Cleared");
        DrawButton(renderer, &buttonRect, false, false);
        SDL_RenderPresent(renderer);
    }
    SDL_Log("SDL version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);//logów nie widać w terminalu
    SDL_Delay(2000);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
