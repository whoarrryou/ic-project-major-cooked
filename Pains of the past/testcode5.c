#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Window constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Button constants
#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 50

// Recipe constants
#define MAX_RECIPES 100
#define MAX_NAME_LEN 50
#define MAX_INGREDIENTS_LEN 500
#define MAX_INSTRUCTIONS_LEN 1000

// Recipe structure
typedef struct {
    char name[MAX_NAME_LEN];
    char ingredients[MAX_INGREDIENTS_LEN];
    char instructions[MAX_INSTRUCTIONS_LEN];
    int prep_time;  // in minutes
} Recipe;

// UI components
typedef struct {
    SDL_Rect rect;
    const char *text;
} Button;

// Globals
Recipe recipes[MAX_RECIPES];
int recipe_count = 0;
const char *font_path = "arial.ttf";  // Path to TTF font
const int font_size = 24;
TTF_Font *font = NULL;

// File path for saving/loading recipes
const char *save_file = "recipes.txt";

// Function prototypes
void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color);
void drawButton(SDL_Renderer *renderer, TTF_Font *font, Button *button, SDL_Color textColor, SDL_Color buttonColor);
bool isMouseOverButton(Button *button, int mouseX, int mouseY);
void clearRenderer(SDL_Renderer *renderer, SDL_Color color);
void addRecipe(SDL_Renderer *renderer);
void displayRecipes(SDL_Renderer *renderer);
void searchRecipe(SDL_Renderer *renderer);
void viewRecipe(SDL_Renderer *renderer, int index);
void saveRecipes();
void loadRecipes();
void promptUserInput(SDL_Renderer *renderer, const char *prompt, char *output, size_t outputSize);

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        fprintf(stderr, "Failed to initialize SDL or TTF: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Recipe Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!window || !renderer) {
        fprintf(stderr, "Failed to create window/renderer: %s\n", SDL_GetError());
        return 1;
    }

    font = TTF_OpenFont(font_path, font_size);
    if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    loadRecipes();

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color buttonColor = {50, 50, 200, 255};

    Button buttons[] = {
        {{250, 150, BUTTON_WIDTH, BUTTON_HEIGHT}, "Add Recipe"},
        {{250, 220, BUTTON_WIDTH, BUTTON_HEIGHT}, "Display Recipes"},
        {{250, 290, BUTTON_WIDTH, BUTTON_HEIGHT}, "Search Recipe"},
        {{250, 360, BUTTON_WIDTH, BUTTON_HEIGHT}, "Exit"}
    };
    const int numButtons = sizeof(buttons) / sizeof(buttons[0]);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x, mouseY = event.button.y;
                for (int i = 0; i < numButtons; i++) {
                    if (isMouseOverButton(&buttons[i], mouseX, mouseY)) {
                        switch (i) {
                            case 0: addRecipe(renderer); break;
                            case 1: displayRecipes(renderer); break;
                            case 2: searchRecipe(renderer); break;
                            case 3: running = false; break;
                        }
                    }
                }
            }
        }

        clearRenderer(renderer, black);
        for (int i = 0; i < numButtons; i++) {
            drawButton(renderer, font, &buttons[i], white, buttonColor);
        }
        SDL_RenderPresent(renderer);
    }

    saveRecipes();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void clearRenderer(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawButton(SDL_Renderer *renderer, TTF_Font *font, Button *button, SDL_Color textColor, SDL_Color buttonColor) {
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &button->rect);

    int textWidth = strlen(button->text) * (font_size / 2);
    drawText(renderer, font, button->text, button->rect.x + (button->rect.w - textWidth) / 2, button->rect.y + button->rect.h / 4, textColor);
}

bool isMouseOverButton(Button *button, int mouseX, int mouseY) {
    return mouseX > button->rect.x && mouseX < button->rect.x + button->rect.w && mouseY > button->rect.y && mouseY < button->rect.y + button->rect.h;
}

void promptUserInput(SDL_Renderer *renderer, const char *prompt, char *output, size_t outputSize) {
    printf("%s: ", prompt);
    fgets(output, outputSize, stdin);
    output[strcspn(output, "\n")] = '\0';  // Remove newline character
}

void addRecipe(SDL_Renderer *renderer) {
    Recipe newRecipe;

    printf("Adding a new recipe...\n");
    promptUserInput(renderer, "Enter recipe name", newRecipe.name, MAX_NAME_LEN);
    promptUserInput(renderer, "Enter ingredients", newRecipe.ingredients, MAX_INGREDIENTS_LEN);
    promptUserInput(renderer, "Enter instructions", newRecipe.instructions, MAX_INSTRUCTIONS_LEN);
    printf("Enter preparation time (minutes): ");
    scanf("%d", &newRecipe.prep_time);
    getchar();  // Clear newline from buffer

    if (recipe_count < MAX_RECIPES) {
        recipes[recipe_count++] = newRecipe;
        printf("Recipe added successfully!\n");
    } else {
        printf("Recipe list is full!\n");
    }
}

void displayRecipes(SDL_Renderer *renderer) {
    printf("\nAll Recipes:\n");
    for (int i = 0; i < recipe_count; i++) {
        printf("%d. %s\n", i + 1, recipes[i].name);
    }
}

void searchRecipe(SDL_Renderer *renderer) {
    char query[MAX_NAME_LEN];
    printf("Enter recipe name to search: ");
    fgets(query, MAX_NAME_LEN, stdin);
    query[strcspn(query, "\n")] = '\0';

    for (int i = 0; i < recipe_count; i++) {
        if (strstr(recipes[i].name, query)) {
            printf("Recipe found: %s\n", recipes[i].name);
            viewRecipe(renderer, i);
            return;
        }
    }
    printf("Recipe not found.\n");
}

void viewRecipe(SDL_Renderer *renderer, int index) {
    if (index < 0 || index >= recipe_count) return;

    printf("\nRecipe Details:\n");
    printf("Name: %s\n", recipes[index].name);
    printf("Ingredients: %s\n", recipes[index].ingredients);
    printf("Instructions: %s\n", recipes[index].instructions);
    printf("Preparation Time: %d minutes\n", recipes[index].prep_time);
}

void saveRecipes() {
    FILE *file = fopen(save_file, "w");
    if (!file) {
        fprintf(stderr, "Error saving recipes: %s\n", strerror(errno));
        return;
    }

    // Save the number of recipes
    fwrite(&recipe_count, sizeof(recipe_count), 1, file);

    // Save each recipe
    for (int i = 0; i < recipe_count; i++) {
        fwrite(&recipes[i], sizeof(Recipe), 1, file);
    }

    fclose(file);
    printf("Recipes saved successfully!\n");
}

void loadRecipes() {
    FILE *file = fopen(save_file, "r");
    if (!file) {
        printf("No saved recipes found.\n");
        return;
    }

    // Load the number of recipes
    fread(&recipe_count, sizeof(recipe_count), 1, file);

    // Load each recipe
    for (int i = 0; i < recipe_count; i++) {
        fread(&recipes[i], sizeof(Recipe), 1, file);
    }

    fclose(file);
    printf("Recipes loaded successfully!\n");
}