#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 500
#define HEIGHT 500
#define col 50
#define row 50

const float colW = ((float)WIDTH)/col;
const float rowW = ((float)HEIGHT)/row;


SDL_Window *window;
SDL_Renderer *renderer;


typedef struct {
    int i;
    int j;
}Vector;

Vector vel = {.i=0, .j=1};

SDL_FRect rect = {.w = colW, .h = rowW};

Vector snake[col*row];
int length = 0;

Vector food;

int over = 0;

void generateFood() {
    food.i = rand() % row;
    food.j = rand() % col; 
}

void display() {
    rect.x = food.j * colW;
    rect.y = food.i * rowW;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRectF(renderer, &rect);

    for (int i = 0; i < length; ++i) {
        rect.x = snake[i].j * colW;
        rect.y = snake[i].i * rowW;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRectF(renderer, &rect);
    }
}

void checkOver() {
    if (snake[0].i < 0 || snake[0].i >= row || snake[0].j < 0|| snake[0].j >= col) {
        over = 1;
        return;
    }
    for (int i = 1; i < length; ++i) {
        if (snake[i].i == snake[0].i && snake[i].j == snake[0].j) {
            over = 1;
            return;
        }
    }
}


void update() {

    checkOver();

    if (snake[0].i == food.i && snake[0].j == food.j) {
        generateFood();
        snake[length].i = 2*snake[length-1].i - snake[length-2].i;
        snake[length].j = 2*snake[length-1].j - snake[length-2].j;
        ++length;
    }

    int pi = snake[0].i;
    int pj = snake[0].j;
    snake[0].i += vel.i;
    snake[0].j += vel.j;
    for (int i = 1; i < length; ++i) {
        int mi = snake[i].i;
        int mj = snake[i].j;
        snake[i].i = pi;
        snake[i].j = pj;
        pi = mi;
        pj = mj;
    }
}



int main() {

    srand(time(0));

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
        "SNAKE GAME",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(window, -1, 0);


    // the snake
    snake[0].i = row/2;
    snake[0].j = col/2;
    length ++;

    generateFood();

    while (!over) {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT: 
                over = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        if (vel.j!=1) {vel.i=0;vel.j=-1;}
                        break;
                    case SDLK_RIGHT:
                        if (vel.j!=-1) {vel.i=0;vel.j=1;}
                        break;
                    case SDLK_UP:
                        if (vel.i!=1) {vel.i=-1;vel.j=0;}
                        break;
                    case SDLK_DOWN:
                        if (vel.i!=-1) {vel.i=1;vel.j=0;}
                        break;
                }
        }
        if (over) break;
        SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
        SDL_RenderClear(renderer);
        
        display();
        update();

        SDL_RenderPresent(renderer);
        SDL_Delay(60);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

