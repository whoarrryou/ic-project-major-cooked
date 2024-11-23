#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>  // For colored text and system commands on Windows

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
void setTextColor(int color);
void displayWelcomeArt();
void displayExitArt();
void drawSeparator(char symbol, int length);
void typeText(const char *text, int delay_ms);

// Global array to hold recipes and a counter
Recipe recipes[MAX_RECIPES];
int recipe_count = 0;
const char *filename = "recipes.dat";

// Main menu function
void menu() {
    int choice;

    while (1) {
        clearScreen();  // Clear screen before showing menu
        displayWelcomeArt();  // Display fancy title
        drawSeparator('=', 30);  // Decorative separator

        typeText("\n1. Add a Recipe\n", 20);
        typeText("2. Display All Recipes\n", 20);
        typeText("3. Search Recipe by Name\n", 20);
        typeText("4. Exit\n", 20);
        drawSeparator('=', 30);  // Decorative separator
        typeText("Enter your choice: ", 20);
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
                clearScreen();
                displayExitArt();
                exit(0);
            default:
                typeText("Invalid choice. Please try again.\n", 20);
        }

        typeText("\nPress Enter to return to the main menu...", 20);
        getchar();  // Pause before returning to the main menu
    }
}

// Function to clear the screen (using cls for Windows)
void clearScreen() {
    system("cls");
}

// Function to set text color (Windows-specific)
void setTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Function to create a typing effect
void typeText(const char *text, int delay_ms) {
    while (*text) {
        putchar(*text++);
        fflush(stdout);  // Ensure character is printed immediately
        Sleep(delay_ms); // Delay in milliseconds
    }
}

// ASCII art for welcome
void displayWelcomeArt() {
    PlaySound("Welcome.wav", NULL, SND_FILENAME | SND_ASYNC);
    setTextColor(14);  // Yellow color
    typeText("#######################################\n", 10);
    typeText("#                                     #\n", 10);
    typeText("#   WELCOME TO THE RECIPE APP! 🍽️   #\n", 10);
    typeText("#                                     #\n", 10);
    typeText("#######################################\n\n", 10);
    setTextColor(7);  // Reset to default color
}

// ASCII art for exit
void displayExitArt() {
    setTextColor(10);  // Green color
    typeText("\nThank you for using the Recipe Storing App! 👨‍🍳👩‍🍳\n", 10);
    typeText("#######################################\n", 10);
    typeText("#                                     #\n", 10);
    typeText("#   SEE YOU NEXT TIME! 🥘🍲🍜        #\n", 10);
    typeText("#                                     #\n", 10);
    typeText("#######################################\n", 10);
    setTextColor(7);  // Reset to default color
}

// Function to draw a separator
void drawSeparator(char symbol, int length) {
    for (int i = 0; i < length; i++) {
        putchar(symbol);
    }
    putchar('\n');
}

// Function to add a new recipe
void addRecipe() {
    if (recipe_count >= MAX_RECIPES) {
        setTextColor(12);  // Red color for error message
        typeText("Recipe storage is full!\n", 20);
        setTextColor(7);  // Reset to default
        return;
    }
    
    Recipe newRecipe;

    typeText("Enter recipe name: ", 20);
    fgets(newRecipe.name, MAX_NAME_LEN, stdin);
    newRecipe.name[strcspn(newRecipe.name, "\n")] = 0;  // Remove newline

    typeText("Enter ingredients: ", 20);
    fgets(newRecipe.ingredients, MAX_INGREDIENTS_LEN, stdin);
    newRecipe.ingredients[strcspn(newRecipe.ingredients, "\n")] = 0;

    typeText("Enter instructions: ", 20);
    fgets(newRecipe.instructions, MAX_INSTRUCTIONS_LEN, stdin);
    newRecipe.instructions[strcspn(newRecipe.instructions, "\n")] = 0;

    typeText("Enter preparation time (in minutes): ", 20);
    scanf("%d", &newRecipe.prep_time);
    clearInputBuffer();

    recipes[recipe_count++] = newRecipe;

    setTextColor(10);  // Green color for success message
    typeText("\nRecipe added successfully! ✅\n", 20);
    setTextColor(7);  // Reset to default
}

// Function to display all recipes
void displayRecipes() {
    if (recipe_count == 0) {
        setTextColor(12);  // Red color for error message
        typeText("No recipes found. 😕\n", 20);
        setTextColor(7);  // Reset to default
        return;
    }
    PlaySound("Display.wav", NULL, SND_FILENAME | SND_ASYNC);
    for (int i = 0; i < recipe_count; i++) {
        printf("\nRecipe %d:\n", i + 1);
        drawSeparator('-', 30);  // Decorative separator
        printf("🍽️ Name: %s\n", recipes[i].name);
        printf("🧂 Ingredients: %s\n", recipes[i].ingredients);
        printf("📝 Instructions: %s\n", recipes[i].instructions);
        printf("⏱️ Preparation time: %d minutes\n", recipes[i].prep_time);
        drawSeparator('-', 30);  // Decorative separator
    }
}

// The rest of the code (searchRecipe, saveRecipes, loadRecipes, errorHandling, etc.) remains unchanged.


// Function to search recipes by name
void searchRecipe() {
    PlaySound("Search.wav", NULL, SND_FILENAME | SND_ASYNC);
    char searchName[MAX_NAME_LEN];
    printf("Enter the name of the recipe to search: ");
    fgets(searchName, MAX_NAME_LEN, stdin);
    searchName[strcspn(searchName, "\n")] = 0;  // Remove newline

    int found = 0;
    for (int i = 0; i < recipe_count; i++) {
        if (strcasecmp(recipes[i].name, searchName) == 0) {
            printf("\nRecipe found:\n");
            drawSeparator('-', 30);  // Decorative separator
            printf("🍽️ Name: %s\n", recipes[i].name);
            printf("🧂 Ingredients: %s\n", recipes[i].ingredients);
            printf("📝 Instructions: %s\n", recipes[i].instructions);
            printf("⏱️ Preparation time: %d minutes\n", recipes[i].prep_time);
            drawSeparator('-', 30);  // Decorative separator
            found = 1;
            break;
        }
    }

    if (!found) {
        setTextColor(12);  // Red color for error message
        printf("Recipe not found. 😕\n");
        setTextColor(7);  // Reset to default
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
    setTextColor(12);  // Red color for error message
    printf("Error: %s\n", message);
    setTextColor(7);  // Reset to default
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
