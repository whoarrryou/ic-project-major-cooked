#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gui/gui.h" // Include your GUI header
#ifdef _WIN32
#include <windows.h>
#else
#include <ctype.h>
#endif

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
void displayWelcomeArt();
void displayExitArt();
void drawSeparator();
void setColor(int color);
void clearScreen();  // Function to clear the screen

// Global array to hold recipes and a counter
Recipe recipes[MAX_RECIPES];
int recipe_count = 0;
const char *filename = "recipes.dat";

// Main menu function
void menu() {
    int choice;

    while (1) {
        clearScreen();  // Clear screen before showing the menu
        setColor(7);  // Reset to default color
        printf("\n");
        drawSeparator();
        printf("          Recipe Storing App          \n");
        drawSeparator();
        printf("1. Add a Recipe\n");
        printf("2. Display All Recipes\n");
        printf("3. Search Recipe by Name\n");
        printf("4. Exit\n");
        drawSeparator();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();  // Clear the buffer after taking user input

        switch (choice) {
            case 1:
                addRecipe();
                break;
            case 2:
                displayRecipes();
                break;
            case 3:
                searchRecipe();
                break;
            case 4:
                saveRecipes();
                displayExitArt();
                exit(0);
            default:
                setColor(12);  // Red for errors
                printf("Invalid choice. Please try again.\n");
                setColor(7);  // Reset to default color
        }
        printf("Press Enter to continue...");
        clearInputBuffer();
    }
}

// Function to add a new recipe
void addRecipe() {
    clearScreen();  // Clear screen before adding a new recipe
    if (recipe_count >= MAX_RECIPES) {
        setColor(12);  // Red for errors
        printf("Recipe storage is full!\n");
        setColor(7);  // Reset to default color
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

    setColor(10);  // Green for success
    printf("Recipe added successfully!\n");
    setColor(7);  // Reset to default color
}

// Function to display all recipes
void displayRecipes() {
    clearScreen();  // Clear screen before displaying recipes
    if (recipe_count == 0) {
        setColor(12);  // Red for errors
        printf("No recipes found.\n");
        setColor(7);  // Reset to default color
        return;
    }

    for (int i = 0; i < recipe_count; i++) {
        printf("\n");
        drawSeparator();
        printf("Recipe %d:\n", i + 1);
        printf("Name: %s\n", recipes[i].name);
        printf("Ingredients: %s\n", recipes[i].ingredients);
        printf("Instructions: %s\n", recipes[i].instructions);
        printf("Preparation time: %d minutes\n", recipes[i].prep_time);
        drawSeparator();
    }
}

// Function to search recipes by name
void searchRecipe() {
    clearScreen();  // Clear screen before searching for a recipe
    char searchName[MAX_NAME_LEN];
    printf("Enter the name of the recipe to search: ");
    fgets(searchName, MAX_NAME_LEN, stdin);
    searchName[strcspn(searchName, "\n")] = 0;  // Remove newline

    int found = 0;
    for (int i = 0; i < recipe_count; i++) {
#ifdef _WIN32
        if (_stricmp(recipes[i].name, searchName) == 0)
#else
        if (strcasecmp(recipes[i].name, searchName) == 0)
#endif
        {
            printf("\n");
            drawSeparator();
            printf("Recipe found:\n");
            printf("Name: %s\n", recipes[i].name);
            printf("Ingredients: %s\n", recipes[i].ingredients);
            printf("Instructions: %s\n", recipes[i].instructions);
            printf("Preparation time: %d minutes\n", recipes[i].prep_time);
            drawSeparator();
            found = 1;
            break;
        }
    }

    if (!found) {
        setColor(12);  // Red for errors
        printf("Recipe not found.\n");
        setColor(7);  // Reset to default color
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
    setColor(10);  // Green for success
    printf("Recipes saved to file.\n");
    setColor(7);  // Reset to default color
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
    setColor(10);  // Green for success
    printf("Recipes loaded from file.\n");
    setColor(7);  // Reset to default color
}

// Error handling function
void errorHandling(const char *message) {
    setColor(12);  // Red for errors
    printf("Error: %s\n", message);
    setColor(7);  // Reset to default color
    exit(1);
}

// Utility function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to display welcome ASCII art
void displayWelcomeArt() {
    clearScreen();  // Clear screen for welcome message
    printf("\n");
    drawSeparator();
    printf(" WELCOME TO THE RECIPE STORING APP \n");
    drawSeparator();
}

// Function to display exit ASCII art
void displayExitArt() {
    printf("\n");
    drawSeparator();
    printf(" THANK YOU FOR USING THE APP! \n");
    printf(" SEE YOU NEXT TIME! \n");
    drawSeparator();
}

// Function to draw a separator line
void drawSeparator() {
    printf("########################################\n");
}

// Function to set text color
void setColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#else
    if (color == 12) printf("\033[1;31m");  // Red for errors
    else if (color == 10) printf("\033[1;32m");  // Green for success
    else printf("\033[0m");  // Reset to default
#endif
}

// Function to clear the screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Main function
int main() {
    displayWelcomeArt();   // Display the welcome message
    loadRecipes();         // Load existing recipes from file
    // init_gui();           // Initialize and launch the GUI
    menu();                // Enter the text-based menu
    return 0;
}
