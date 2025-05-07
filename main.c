#include<SDL3/SDL.h>

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("SDL3", 800, 600, 0);
    SDL_Delay(2000);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
