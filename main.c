#define SDL_ENABLE_OLD_NAMES
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL3/SDL_ttf.h>

struct Puzzle
{
    bool whichBoard;
    int actualPosition;
    int startPosition;
    int destination;
    // image ;  do wyszukania
};

struct Board
{
    int widthOfBoard;
    int heightOfBoard;
    // int TableOfPositions[heightOfBoard][widthOfBoard][2];
    // image
};

/*struct resetButton
{

};*/

// rysowanie przycisków na oknie wejsciowym

bool inMainMenu = true;
bool inGame = false;

const SDL_FRect buttonNewGame = {300.0f, 200.0f, 200.0f, 60.0f};
const SDL_FRect buttonContinue = {300.0f, 290.0f, 200.0f, 60.0f};
const SDL_FRect buttonExit = {700.0f, 20.0f, 80.0f, 60.0f};

bool isPointInRect(float x, float y, const SDL_FRect *rect)
{
    return (x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h);
}

void DrawButton(SDL_Renderer *renderer, const SDL_FRect *rect, bool hovered, bool pressed)
{
    if (pressed || hovered)
    {
        SDL_SetRenderDrawColor(renderer, 30, 19, 237, 200);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 30, 19, 237, 200);
    }

    SDL_RenderFillRectF(renderer, rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRectF(renderer, rect);
}

void DrawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, float x, float y){
    SDL_Color color = {255,255,255,255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color)
}

int main(void)
{
    if (TTF_Init() < 0)
    {
        SDL_Log("TTF_Init error: %s", SDL_GetError());
        return 1;
    }
    TTF_Font *font = TTF_OpenFont("Roboto-Regular.ttf", 24);
    if (!font)
    {
        SDL_Log("Font loading error: %s", SDL_GetError());
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Puzzle", 800, 600, 0);
    if (!win)
    {
        SDL_Log("Failed to make a window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);
    if (!renderer)
    {
        SDL_Log("Failed to make renderer %s", SDL_GetError());
        SDL_Quit();
        return -2;
    }

    bool gameRunning = true;
    while (gameRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                float mouseX = event.button.x;
                float mouseY = event.button.y;

                if (isPointInRect(mouseX, mouseY, &buttonNewGame))
                {
                    inMainMenu = false;
                    inGame = true;
                }
                if (isPointInRect(mouseX, mouseY, &buttonContinue))
                {
                    SDL_Log("Kliknieto kontynuuj");
                }
                if (isPointInRect(mouseX, mouseY, &buttonExit))
                {
                    inMainMenu = true;
                    inGame = false;
                }
            }

            if (event.type == SDL_EVENT_QUIT)
            {
                gameRunning = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // czarne tło
        SDL_RenderClear(renderer);

        // wybór widoku
        if (inMainMenu)
        {
            DrawButton(renderer, &buttonNewGame, false, false);
            DrawButton(renderer, &buttonContinue, false, false);
        }
        else if (inGame)
        {
            DrawButton(renderer, &buttonExit, false, false);
            SDL_SetRenderDrawColor(renderer, 100, 150, 200, 255);
            SDL_FRect board = {100.0f, 100.0f, 600.0f, 400.0f};
            SDL_RenderFillRectF(renderer, &board);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_Log("SDL version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
    SDL_Delay(2000);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
