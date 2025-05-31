#define SDL_ENABLE_OLD_NAMES
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

//wymiary obrazka
#define PUZZLE_COLS 3
#define PUZZLE_ROWS 2
#define PUZZLE_IMG_WIDTH 600
#define PUZZLE_IMG_HEIGHT 400

//koordynaty obrazka
#define PUZZLE_START_X 100
#define PUZZLE_START_Y 100

//wymiary puzzla
#define TILE_COUNT (PUZZLE_ROWS * PUZZLE_COLS)
#define TILE_WIDTH (PUZZLE_IMG_WIDTH / PUZZLE_COLS)
#define TILE_HEIGHT (PUZZLE_IMG_HEIGHT / PUZZLE_ROWS)

//wymiary planszy
#define BORDER WIDTH 5

typedef struct
{
    SDL_FRect srcRect;
    SDL_FRect dstRect;
    bool dragging;
    float offsetX, offsetY;
    float winX,winY;
} PuzzlePiece;

PuzzlePiece puzzlePieces[TILE_COUNT];

// zmienne

int selectedPiece = -1;
bool inMainMenu = true;
bool inGame = false;

//kwadrat plansza
const SDL_FRect BoardDestiny={PUZZLE_START_X,PUZZLE_START_Y,PUZZLE_IMG_WIDTH,PUZZLE_IMG_HEIGHT}; 

// wymiary i pozycje przycisków
const SDL_FRect buttonNewGame = {300.0f, 200.0f, 200.0f, 60.0f};
const SDL_FRect buttonContinue = {300.0f, 290.0f, 200.0f, 60.0f};
const SDL_FRect buttonExit = {700.0f, 20.0f, 80.0f, 60.0f};

// funkcje


//losowanie liczby
float random_float_in_range(float min, float max) {
    if (min > max) {
        float temp = min;
        min = max;
        max = temp;
    }
    float scale = rand() / (float) RAND_MAX; 
    return min + scale * (max - min);
}
//losowanie koordynatów
void random_coords(float* outX, float* outY) {
    // Randomly pick range for x
    *outX = random_float_in_range(100.0f, 600.0f);
    // Randomly pick range for y
    *outY = random_float_in_range(100.0f, 400.0f);

}
//początkowe położenie puzzli 
void initPuzzle()
{
    int index = 0;
    for (int row = 0; row < PUZZLE_ROWS; row++)
    {
        for (int col = 0; col < PUZZLE_COLS; col++)
        {
            //co powinno być
            
            //textura
            puzzlePieces[index].srcRect.x = col * TILE_WIDTH;
            puzzlePieces[index].srcRect.y = row * TILE_HEIGHT;
            puzzlePieces[index].srcRect.w = TILE_WIDTH;
            puzzlePieces[index].srcRect.h = TILE_HEIGHT;

            //winning destination
            puzzlePieces[index].winX = PUZZLE_START_X + col * (TILE_WIDTH );
            puzzlePieces[index].winY = PUZZLE_START_Y + row * (TILE_HEIGHT );
            //start destination
           random_coords(&puzzlePieces[index].dstRect.x, &puzzlePieces[index].dstRect.y);
            puzzlePieces[index].dstRect.w = TILE_WIDTH;
            puzzlePieces[index].dstRect.h = TILE_HEIGHT;

            //przenoszenie puzzla
            puzzlePieces[index].dragging = false;
            //kolejny puzzle
            index++;
        }
    }
}

//rysuj tablice
void drawBoard(SDL_Renderer *renderer, const SDL_FRect *rect)
{
    //wypełnienie
    SDL_SetRenderDrawColor(renderer,4,184,76,200);
    SDL_RenderFillRectF(renderer, rect);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    // Rysowanie pionowych linii siatki co wysokość kafelka
    for (int x = PUZZLE_START_X; x <= PUZZLE_START_X+PUZZLE_IMG_WIDTH; x += TILE_WIDTH) {
       SDL_RenderDrawLine(renderer, x, PUZZLE_START_Y, x, PUZZLE_IMG_HEIGHT+PUZZLE_START_Y);
    }
    // Rysowanie poziomych linii siatki co długość kafelka
    for (int y = PUZZLE_START_Y; y <= PUZZLE_IMG_HEIGHT+PUZZLE_START_Y; y += TILE_HEIGHT) {
        SDL_RenderDrawLine(renderer, PUZZLE_START_X, y, PUZZLE_START_X+PUZZLE_IMG_WIDTH, y);
    }

}

//rysuj puzzle
void drawPuzzle(SDL_Renderer *renderer, SDL_Texture *image)
{
    for (int i = 0; i < TILE_COUNT; i++)
    {
        SDL_RenderCopyF(renderer, image, &puzzlePieces[i].srcRect, &puzzlePieces[i].dstRect);
    }
}

bool isPointInRect(float x, float y, const SDL_FRect *rect)
{
    return (x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h);
}

//narysuj przycisk
void DrawButton(SDL_Renderer *renderer, TTF_Font *font, const SDL_FRect *rect, const char *label, bool hovered, bool pressed)
{
    if (pressed || hovered)
    {
        SDL_SetRenderDrawColor(renderer, 30, 19, 237, 200);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 19, 237, 200);
    }

    SDL_RenderFillRectF(renderer, rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRectF(renderer, rect);

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, label, strlen(label), color);
    if (!surface)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect dst;
    dst.w = (float)surface->w;
    dst.h = (float)surface->h;
    dst.x = rect->x + (rect->w - dst.w) / 2.0f;
    dst.y = rect->y + (rect->h - dst.h) / 2.0f;

    SDL_RenderCopyF(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

// void DrawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, float x, float y)
// {
//     SDL_Color color = {255, 255, 255, 255};
//     SDL_Surface *surface = TTF_RenderText_Solid(font, text, strlen(text), color);
//     if (!surface)
//     {
//         SDL_Log("RenderText Error: %s", SDL_GetError());
//         return;
//     }

//     SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
//     if (!texture)
//     {
//         SDL_DestroySurface(surface);
//         return;
//     }

//     SDL_FRect dst = {x, y, (float)surface->w, (float)surface->h};
//     SDL_RenderCopyF(renderer, texture, NULL, &dst);

//     SDL_DestroyTexture(texture);
//     SDL_DestroySurface(surface);
// }

//oddalenie od celu
bool IfCanSnap(float WinX, float WinY, float ActualX,float ActualY){
 //   SDL_Log("%f",sqrt(pow(WinX-ActualX,2)+pow(WinY-ActualY,2)));
    return sqrt(pow(WinX-ActualX,2)+pow(WinY-ActualY,2))<20;
}


//główna funkcja
int main(void)
{

    initPuzzle();
    // błędy

    if (TTF_Init() < 0)
    {
        SDL_Log("TTF_Init error: %s", SDL_GetError());
        return 1;
    }
    TTF_Font *font = TTF_OpenFont("build/Roboto-Regular.ttf", 24);
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

    SDL_Texture *image = IMG_LoadTexture(renderer, "build/puzzle.png");
    if (!image)
    {
        SDL_Log("Nie udało się załadować obrazka: %d", SDL_GetError());
        return 1;
    }
    // gra
    bool gameRunning = true;
    while (gameRunning)
    {
        float mouseX = 0, mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);

        bool hoverNewGame = isPointInRect(mouseX, mouseY, &buttonNewGame);
        bool hoverContinue = isPointInRect(mouseX, mouseY, &buttonContinue);
        bool hoverExit = isPointInRect(mouseX, mouseY, &buttonExit);
        // obsługa event'ów
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameRunning = false;
                break;
            }

            if (inGame)
            {
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    float mx = event.button.x;
                    float my = event.button.y;

                    for (int i = TILE_COUNT - 1; i >= 0; i--)
                    {
                        SDL_FRect *rect = &puzzlePieces[i].dstRect;
                        if (isPointInRect(mx, my, rect))
                        {
                            puzzlePieces[i].dragging = true;
                            puzzlePieces[i].offsetX = mx - rect->x;
                            puzzlePieces[i].offsetY = my - rect->y;
                            selectedPiece = i;
                            break;
                        }
                    }
                }

                if (event.type == SDL_EVENT_MOUSE_MOTION && selectedPiece != -1)
                {
                    if (puzzlePieces[selectedPiece].dragging)
                    {
                        float mx = event.motion.x;
                        float my = event.motion.y;

                        puzzlePieces[selectedPiece].dstRect.x = mx - puzzlePieces[selectedPiece].offsetX;
                        puzzlePieces[selectedPiece].dstRect.y = my - puzzlePieces[selectedPiece].offsetY;
                    }
                }

                if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && selectedPiece != -1)
                {
                    puzzlePieces[selectedPiece].dragging = false;

                    //snap na właściwe miejsce
                    if(IfCanSnap(puzzlePieces[selectedPiece].winX,puzzlePieces[selectedPiece].winY,puzzlePieces[selectedPiece].dstRect.x,puzzlePieces[selectedPiece].dstRect.y)){
                        puzzlePieces[selectedPiece].dstRect.x = puzzlePieces[selectedPiece].winX;
                        puzzlePieces[selectedPiece].dstRect.y = puzzlePieces[selectedPiece].winY;
                    }

                    selectedPiece = -1;

                }
            }

            // menu
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && inMainMenu)
            {
                float mouseX = event.button.x;
                float mouseY = event.button.y;

                if (isPointInRect(mouseX, mouseY, &buttonNewGame))
                {
                    inMainMenu = false;
                    inGame = true;
                    selectedPiece = -1;
                    initPuzzle();
                }
                if (isPointInRect(mouseX, mouseY, &buttonContinue))
                {
                    inMainMenu = false;
                    inGame = true;
                }
            }

        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // czarne tło
        SDL_RenderClear(renderer);

        // wybór widoku
        if (inMainMenu)
        {
            DrawButton(renderer, font, &buttonNewGame, "Nowa gra", hoverNewGame, false);
            DrawButton(renderer, font, &buttonContinue, "Kontynuuj", hoverContinue, false);
        }
        else if (inGame)
        {
            DrawButton(renderer, font, &buttonExit, "X", hoverExit, false);
            drawBoard(renderer, &BoardDestiny);
            drawPuzzle(renderer, image);
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
