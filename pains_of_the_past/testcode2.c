#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Function declarations
void addRecipe();
void displayRecipes();
void searchRecipe();
void saveRecipes();
void loadRecipes();
void errorHandling(const char *message);
void clearInputBuffer();
void clearScreen();

// Global array to hold recipes and a counter
Recipe recipes[MAX_RECIPES];
int recipe_count = 0;
const char *filename = "recipes.dat";

// Main menu function
void menu() {
    int choice;

    while (1) {
        clearScreen();  // Clear screen before showing menu

        printf("\n============================\n");
        printf("        Recipe Storing App        \n");
        printf("============================\n");
        printf("1. Add a Recipe\n");
        printf("2. Display All Recipes\n");
        printf("3. Search Recipe by Name\n");
        printf("4. Exit\n");
        printf("============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();  // Clear the buffer after taking user input

        switch (choice) {
            case 1:
                clearScreen();  // Clear screen before adding a recipe
                addRecipe();
                break;
            case 2:
                clearScreen();  // Clear screen before displaying recipes
                displayRecipes();
                break;
            case 3:
                clearScreen();  // Clear screen before searching for recipes
                searchRecipe();
                break;
            case 4:
                saveRecipes();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }

        printf("\nPress Enter to return to the main menu...");
        getchar();  // Pause before returning to the main menu
    }
}

// Function to clear the screen (using cls for Windows)
void clearScreen() {
    system("cls");
}

// Function to add a new recipe
void addRecipe() {
    if (recipe_count >= MAX_RECIPES) {
        printf("Recipe storage is full!\n");
        return;
    }

    Recipe newRecipe;

    printf("Enter recipe name: ");
    fgets(newRecipe.name, MAX_NAME_LEN, stdin);
    newRecipe.name[strcspn(newRecipe.name, "\n")] = 0;  // Remove newline

    printf("Enter ingredients: ");
    fgets(newRecipe.ingredients, MAX_INGREDIENTS_LEN, stdin);
    newRecipe.ingredients[strcspn(newRecipe.ingredients, "\n")] = 0;

    printf("Enter instructions: ");
    fgets(newRecipe.instructions, MAX_INSTRUCTIONS_LEN, stdin);
    newRecipe.instructions[strcspn(newRecipe.instructions, "\n")] = 0;

    printf("Enter preparation time (in minutes): ");
    scanf("%d", &newRecipe.prep_time);
    clearInputBuffer();

    recipes[recipe_count++] = newRecipe;

    printf("\nRecipe added successfully!\n");
}

// Function to display all recipes
void displayRecipes() {
    if (recipe_count == 0) {
        printf("No recipes found.\n");
        return;
    }

    for (int i = 0; i < recipe_count; i++) {
        printf("\nRecipe %d:\n", i + 1);
        printf("Name: %s\n", recipes[i].name);
        printf("Ingredients: %s\n", recipes[i].ingredients);
        printf("Instructions: %s\n", recipes[i].instructions);
        printf("Preparation time: %d minutes\n", recipes[i].prep_time);
    }
}

// Function to search recipes by name
void searchRecipe() {
    char searchName[MAX_NAME_LEN];
    printf("Enter the name of the recipe to search: ");
    fgets(searchName, MAX_NAME_LEN, stdin);
    searchName[strcspn(searchName, "\n")] = 0;  // Remove newline

    int found = 0;
    for (int i = 0; i < recipe_count; i++) {
        if (strcasecmp(recipes[i].name, searchName) == 0) {
            printf("\nRecipe found:\n");
            printf("Name: %s\n", recipes[i].name);
            printf("Ingredients: %s\n", recipes[i].ingredients);
            printf("Instructions: %s\n", recipes[i].instructions);
            printf("Preparation time: %d minutes\n", recipes[i].prep_time);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Recipe not found.\n");
    }
}

// Function to save recipes to a file
void saveRecipes() {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        errorHandling("Failed to open file for writing.");
        return;
    }

    fwrite(&recipe_count, sizeof(int), 1, file);  // Save the number of recipes
    fwrite(recipes, sizeof(Recipe), recipe_count, file);  // Save all recipes

    fclose(file);
    printf("Recipes saved to file.\n");
}

// Function to load recipes from a file
void loadRecipes() {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("No existing recipe file found. Starting fresh.\n");
        return;
    }

    fread(&recipe_count, sizeof(int), 1, file);  // Load the number of recipes
    fread(recipes, sizeof(Recipe), recipe_count, file);  // Load all recipes

    fclose(file);
    printf("Recipes loaded from file.\n");
}

// Error handling function
void errorHandling(const char *message) {
    printf("Error: %s\n", message);
    exit(1);
}

// Utility function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Main function
int main() {
    loadRecipes();
    menu();
    return 0;
}
