#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 50

typedef struct {
    SDL_Rect rect;
    const char *text;
} Button;

const char *font_path = "arial.ttf";  // Path to a TTF font file
const int font_size = 24;

// Function prototypes
void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color);
void drawButton(SDL_Renderer *renderer, TTF_Font *font, Button *button, SDL_Color textColor, SDL_Color buttonColor);
bool isMouseOverButton(Button *button, int mouseX, int mouseY);

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Recipe Storage App",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont(font_path, font_size);
    if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Button definitions
    Button buttons[] = {
        {{250, 150, BUTTON_WIDTH, BUTTON_HEIGHT}, "Add Recipe"},
        {{250, 220, BUTTON_WIDTH, BUTTON_HEIGHT}, "Display Recipes"},
        {{250, 290, BUTTON_WIDTH, BUTTON_HEIGHT}, "Search Recipe"},
        {{250, 360, BUTTON_WIDTH, BUTTON_HEIGHT}, "Exit"}
    };

    const int numButtons = sizeof(buttons) / sizeof(buttons[0]);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color buttonColor = {50, 50, 200, 255}; // Blue button

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                for (int i = 0; i < numButtons; i++) {
                    if (isMouseOverButton(&buttons[i], mouseX, mouseY)) {
                        switch (i) {
                            case 0: printf("Add Recipe clicked!\n"); break;
                            case 1: printf("Display Recipes clicked!\n"); break;
                            case 2: printf("Search Recipe clicked!\n"); break;
                            case 3: running = false; break; // Exit button
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render buttons
        for (int i = 0; i < numButtons; i++) {
            drawButton(renderer, font, &buttons[i], white, buttonColor);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawButton(SDL_Renderer *renderer, TTF_Font *font, Button *button, SDL_Color textColor, SDL_Color buttonColor) {
    // Draw button background
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &button->rect);

    // Draw button border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &button->rect);

    // Draw button text
    int textX = button->rect.x + (button->rect.w / 2) - (strlen(button->text) * font_size / 4);  // Center text
    int textY = button->rect.y + (button->rect.h / 4);
    drawText(renderer, font, button->text, textX, textY, textColor);
}

bool isMouseOverButton(Button *button, int mouseX, int mouseY) {
    return (mouseX > button->rect.x &&
            mouseX < (button->rect.x + button->rect.w) &&
            mouseY > button->rect.y &&
            mouseY < (button->rect.y + button->rect.h));
}
