#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define MAX_RECIPES 100
#define MAX_NAME_LEN 50
#define MAX_INGREDIENTS_LEN 500
#define MAX_INSTRUCTIONS_LEN 1000

// Recipe structure
typedef struct
{
    char name[MAX_NAME_LEN];
    char ingredients[MAX_INGREDIENTS_LEN];
    char instructions[MAX_INSTRUCTIONS_LEN];
    int prep_time; // in minutes
    char category[20]; // Breakfast, Snacks, Lunch, or Dinner
    char youtube_link[100]; // YouTube link for the recipe
} Recipe;

const Recipe predefinedRecipes[] = {
    {
        "Pancakes",
        "Flour, Eggs, Milk, Baking Powder, Sugar",
        "1. Mix ingredients.\n2. Heat pan and cook batter until golden brown.",
        20,
        "Breakfast",
        "https://www.youtube.com/watch?v=pancakes"
    },
    {
        "Grilled Cheese Sandwich",
        "Bread, Cheese, Butter",
        "1. Heat pan.\n2. Assemble sandwich.\n3. Grill until golden.",
        10,
        "Snacks",
        "https://www.youtube.com/watch?v=grilledcheese"
    },
    {
        "Spaghetti Bolognese",
        "Spaghetti, Ground Beef, Tomato Sauce, Onion, Garlic",
        "1. Cook spaghetti.\n2. Prepare sauce.\n3. Mix and serve.",
        40,
        "Lunch",
        "https://www.youtube.com/watch?v=spaghettibolognese"
    },
    {
        "Chicken Curry",
        "Chicken, Onion, Tomato, Spices, Cream",
        "1. Saute onions and spices.\n2. Add chicken and cook.\n3. Add cream and simmer.",
        45,
        "Dinner",
        "https://www.youtube.com/watch?v=chickencurry"
    }
};
const int predefinedCount = sizeof(predefinedRecipes) / sizeof(predefinedRecipes[0]);

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
void menu()
{
    int choice;

    while (1)
    {
        clearScreen();
        displayWelcomeArt();
        printf("\t\t\t\t\t\t");
        drawSeparator('=', 30);
        setTextColor(13);
        typeText("\n\t\t\t\t\t\t1. Add a Recipe\n", 20);
        typeText("\t\t\t\t\t\t2. Display All Recipes\n", 20);
        typeText("\t\t\t\t\t\t3. Search Recipe by Name\n", 20);
        typeText("\t\t\t\t\t\t4. Exit\n", 20);
        printf("\t\t\t\t\t\t");
        drawSeparator('=', 30);
        typeText("Enter your choice: ", 20);
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            clearScreen();
            addRecipe();
            break;
        case 2:
            clearScreen();
            displayRecipes();
            break;
        case 3:
            clearScreen();
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
        getchar();
    }
}

// Function to clear the screen (using cls for Windows)
void clearScreen()
{
    system("cls");
}

// Function to set text color (Windows-specific)
void setTextColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Function to create a typing effect
void typeText(const char *text, int delay_ms)
{
    while (*text)
    {
        putchar(*text++);
        fflush(stdout);  // Ensure character is printed immediately
        Sleep(delay_ms); // Delay in milliseconds
    }
}

// ASCII art for welcome
void displayWelcomeArt()
{
    setTextColor(14); // Yellow color
    typeText("\t\t\t\t\t\t#######################################\n", 10);
    typeText("\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t#     WELCOME TO THE RECIPE APP!      #\n", 10);
    typeText("\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t#######################################\n\n", 10);
    setTextColor(7); // Reset to default color
}

// ASCII art for exit
void displayExitArt()
{
    setTextColor(10); // Green color
    typeText("\n\t\t\t\t\t\tThank you for using the Recipe Storing App! \n", 10);
    typeText("\t\t\t\t\t\t#######################################\n", 10);
    typeText("\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t#         SEE YOU NEXT TIME!          #\n", 10);
    typeText("\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t#######################################\n", 10);
    setTextColor(7); // Reset to default color
}

// Function to draw a separator
void drawSeparator(char symbol, int length)
{
    for (int i = 0; i < length; i++)
    {
        putchar(symbol);
    }
    putchar('\n');
}

// Function to add a new recipe
void addRecipe()
{
    if (recipe_count >= MAX_RECIPES)
    {
        setTextColor(12); // Red color for error message
        typeText("Recipe storage is full!\n", 20);
        setTextColor(7);
        return;
    }

    Recipe newRecipe;

    typeText("Enter recipe name: ", 20);
    fgets(newRecipe.name, MAX_NAME_LEN, stdin);
    newRecipe.name[strcspn(newRecipe.name, "\n")] = 0;

    typeText("Enter ingredients: ", 20);
    fgets(newRecipe.ingredients, MAX_INGREDIENTS_LEN, stdin);
    newRecipe.ingredients[strcspn(newRecipe.ingredients, "\n")] = 0;

    typeText("Enter instructions: ", 20);
    fgets(newRecipe.instructions, MAX_INSTRUCTIONS_LEN, stdin);
    newRecipe.instructions[strcspn(newRecipe.instructions, "\n")] = 0;

    typeText("Enter preparation time (in minutes): ", 20);
    scanf("%d", &newRecipe.prep_time);
    clearInputBuffer();

    // Category input
    typeText("\nChoose the category:\n", 20);
    typeText("1. Breakfast\n", 20);
    typeText("2. Snacks\n", 20);
    typeText("3. Lunch\n", 20);
    typeText("4. Dinner\n", 20);
    typeText("Enter your choice: ", 20);

    int category_choice;
    scanf("%d", &category_choice);
    clearInputBuffer();

    switch (category_choice)
    {
    case 1:
        strcpy(newRecipe.category, "Breakfast");
        break;
    case 2:
        strcpy(newRecipe.category, "Snacks");
        break;
    case 3:
        strcpy(newRecipe.category, "Lunch");
        break;
    case 4:
        strcpy(newRecipe.category, "Dinner");
        break;
    default:
        strcpy(newRecipe.category, "Uncategorized");
        typeText("Invalid category. Recipe added as 'Uncategorized'.\n", 20);
    }

    recipes[recipe_count++] = newRecipe;

    setTextColor(10);
    typeText("\nRecipe added successfully! ✅\n", 20);
    setTextColor(7);
}

// Function to display all recipes
void displayRecipes()
{
    printf("\n--- Predefined Recipes ---\n");
    for (int i = 0; i < predefinedCount; i++)
    {
        printf("\nRecipe %d:\n", i + 1);
        printf("Name: %s\n", predefinedRecipes[i].name);
        printf("Ingredients: %s\n", predefinedRecipes[i].ingredients);
        printf("Instructions: %s\n", predefinedRecipes[i].instructions);
        printf("Preparation Time: %d minutes\n", predefinedRecipes[i].prep_time);
        printf("Category: %s\n", predefinedRecipes[i].category);
        printf("YouTube Link: %s\n", predefinedRecipes[i].youtube_link);
        printf("----------------------------\n");
    }

    if (recipe_count > 0)
    {
        printf("\n--- User-Defined Recipes ---\n");
        for (int i = 0; i < recipe_count; i++)
        {
            printf("\nRecipe %d:\n", i + 1);
            printf("Name: %s\n", recipes[i].name);
            printf("Ingredients: %s\n", recipes[i].ingredients);
            printf("Instructions: %s\n", recipes[i].instructions);
            printf("Preparation Time: %d minutes\n", recipes[i].prep_time);
            printf("Category: %s\n", recipes[i].category);
            printf("----------------------------\n");
        }
    }
    else
    {
        typeText("No user-defined recipes available.\n", 20);
    }
}

// Function to search for a recipe by name
void searchRecipe()
{
    char searchName[MAX_NAME_LEN];
    typeText("Enter the name of the recipe to search: ", 20);
    fgets(searchName, MAX_NAME_LEN, stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    bool found = false;

    printf("\n--- Searching in Predefined Recipes ---\n");
    for (int i = 0; i < predefinedCount; i++)
    {
        if (strcasecmp(predefinedRecipes[i].name, searchName) == 0)
        {
            printf("\nRecipe found in predefined recipes:\n");
            printf("Name: %s\n", predefinedRecipes[i].name);
            printf("Ingredients: %s\n", predefinedRecipes[i].ingredients);
            printf("Instructions: %s\n", predefinedRecipes[i].instructions);
            printf("Preparation Time: %d minutes\n", predefinedRecipes[i].prep_time);
            printf("Category: %s\n", predefinedRecipes[i].category);
            printf("YouTube Link: %s\n", predefinedRecipes[i].youtube_link);
            found = true;
            break;
        }
    }

    printf("\n--- Searching in User-Defined Recipes ---\n");
    for (int i = 0; i < recipe_count; i++)
    {
        if (strcasecmp(recipes[i].name, searchName) == 0)
        {
            printf("\nRecipe found in user-defined recipes:\n");
            printf("Name: %s\n", recipes[i].name);
            printf("Ingredients: %s\n", recipes[i].ingredients);
            printf("Instructions: %s\n", recipes[i].instructions);
            printf("Preparation Time: %d minutes\n", recipes[i].prep_time);
            printf("Category: %s\n", recipes[i].category);
            found = true;
            break;
        }
    }

    if (!found)
    {
        typeText("Recipe not found.\n", 20);
    }
}

// Function to save recipes to a file
void saveRecipes()
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        errorHandling("Error saving recipes to file.");
        return;
    }

    fwrite(&recipe_count, sizeof(recipe_count), 1, file);
    fwrite(recipes, sizeof(Recipe), recipe_count, file);

    fclose(file);
}

// Function to load recipes from a file
void loadRecipes()
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        return; // File doesn't exist yet
    }

    fread(&recipe_count, sizeof(recipe_count), 1, file);
    fread(recipes, sizeof(Recipe), recipe_count, file);

    fclose(file);
}

// Function to handle errors
void errorHandling(const char *message)
{
    setTextColor(12); // Red color for errors
    printf("\nError: %s\n", message);
    setTextColor(7); // Reset to default color
}

// Function to clear input buffer
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main()
{
    loadRecipes();
    menu();
    return 0;
}
