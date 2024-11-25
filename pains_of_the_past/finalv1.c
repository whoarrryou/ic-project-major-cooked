#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <process.h>
#include <ctype.h>

#define MAX_RECIPES 100
#define MAX_NAME_LEN 50
#define MAX_INGREDIENTS_LEN 500
#define MAX_INSTRUCTIONS_LEN 1000

// Recipe structure
typedef struct{
    char name[MAX_NAME_LEN];
    char ingredients[MAX_INGREDIENTS_LEN];
    char instructions[MAX_INSTRUCTIONS_LEN];
    int prep_time;     // in minutes
    char category[20]; // Breakfast, Snacks, Lunch, or Dinner
    char youtube_link[100];
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
void speakText(char text[]);

// Global array to hold recipes and a counter
Recipe recipes[MAX_RECIPES];
int recipe_count = 0;
const char *filename = "recipes.dat";

// Main menu function
void menu(){
    int choice;

    while (1){
        clearScreen();
        displayWelcomeArt();
        printf("\t\t\t\t\t\t");
        drawSeparator('=', 39);
        setTextColor(13);
        typeText("\n\t\t\t\t\t\t1. Add a Recipe\n", 10);
        typeText("\t\t\t\t\t\t2. Display All Recipes\n", 10);
        typeText("\t\t\t\t\t\t3. Search Recipe by Name\n", 10);
        typeText("\t\t\t\t\t\t4. Exit\n", 10);
        printf("\t\t\t\t\t\t");
        drawSeparator('=', 39);
        typeText("Enter your choice: ", 10);
        char textMsg[] = "Enter your choice";
        speakText(textMsg);
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
            typeText("Invalid choice. Please try again.\n", 10);
            char invalidChoice[] = "Invalid choice. Please try again.";
            speakText(invalidChoice);
        }
        typeText("\nPress Enter to return to the main menu...", 10);
        char returnMenu[] = "Press Enter to return to the main menu";
        speakText(returnMenu);
        getchar();
    }
}

// Function to clear the screen (using cls for Windows)
void clearScreen(){
    system("cls");
}

// Function to set text color (Windows-specific)
void setTextColor(int color){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Function to create a typing effect
void typeText(const char *text, int delay_ms){
    while (*text){
        putchar(*text++);
        fflush(stdout);  // Ensure character is printed immediately
        Sleep(delay_ms); // Delay in milliseconds
    }
}

// ASCII art for welcome
void displayWelcomeArt()
{
    static int temp = 1;
    if (temp == 1)
    {
        char welcomeMessage[] = "\t\t\t\t\t\tWelcome to the recipe app!";
        speakText(welcomeMessage);
    }
    setTextColor(14); // Yellow color
    typeText("\t\t\t\t\t\t#######################################\n", 10);
    typeText("\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t#     WELCOME TO THE RECIPE APP!      #\n", 10);
    typeText("\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t#######################################\n\n", 10);
    setTextColor(7); // Reset to default color
    temp = 0;
}

// ASCII art for exit
void displayExitArt()
{
    setTextColor(10); // Green color
    char seeYa[] = "\t\t\t\t\t\tThank you for using the recipe storing app! See you next time";
    typeText("\n\t\t\t\t\t\tThank you for using the Recipe Storing App! \n", 10);
    speakText(seeYa);
    typeText("\t\t\t\t\t\t#######################################\n", 10);
    typeText("\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t#         SEE YOU NEXT TIME!          #\n", 10);
    typeText("\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t#######################################\n", 10);
    setTextColor(7); // Reset to default color
}

// Function to draw a separator
void drawSeparator(char symbol, int length){
    for (int i = 0; i < length; i++){
        putchar(symbol);
    }
    putchar('\n');
}

// Function to add a new recipe
void addRecipe(){
    if (recipe_count >= MAX_RECIPES){
        setTextColor(12); // Red color for error message
        char full[] = "\t\t\t\t\t\tRecipe storage is full!";
        typeText("Recipe storage is full!\n", 20);
        speakText(full);
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
    char categoryMsg[] = "\t\t\t\t\t\tChoose the category from below options!";
    speakText(categoryMsg);
    typeText("\nChoose the category:\n", 20);
    typeText("1. Breakfast\n", 20);
    typeText("2. Snacks\n", 20);
    typeText("3. Lunch\n", 20);
    typeText("4. Dinner\n", 20);
    typeText("Enter your choice: ", 20);

    int category_choice;
    scanf("%d", &category_choice);
    clearInputBuffer();

    switch (category_choice){
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
    char recipeAdded[] = "\t\t\t\t\t\tRecipe added successfully!";
    typeText("\nRecipe added successfully! \n", 20);
    speakText(recipeAdded);
    setTextColor(7);
}

// Function to display all recipes
void displayRecipes(){
    if (recipe_count == 0){
        setTextColor(12);
        typeText("No recipes found. \n", 20);
        setTextColor(7);
        return;
    }

    char display[] = "Here are all the recipes!";
    speakText(display);

    printf("\n--- Predefined Recipes ---\n");
    for (int i = 0; i < predefinedCount; i++){
        printf("\nRecipe %d:\n", i + 1);
        printf("Name: %s\n", predefinedRecipes[i].name);
        printf("Ingredients: %s\n", predefinedRecipes[i].ingredients);
        printf("Instructions: %s\n", predefinedRecipes[i].instructions);
        printf("Preparation Time: %d minutes\n", predefinedRecipes[i].prep_time);
        printf("Category: %s\n", predefinedRecipes[i].category);
        printf("YouTube Link: %s\n", predefinedRecipes[i].youtube_link);
        printf("----------------------------\n");
    }


    if(recipe_count > 0){
        printf("\n--- User-Defined Recipes ---\n");
        for (int i = 0; i < recipe_count; i++){
            printf("\nRecipe %d:\n", i + 1);
            drawSeparator('-', 30);
            printf("Name: %s\n", recipes[i].name);
            printf("Ingredients: %s\n", recipes[i].ingredients);
            printf("Instructions: %s\n", recipes[i].instructions);
            printf("Preparation time: %d minutes\n", recipes[i].prep_time);
            printf("Category: %s\n", recipes[i].category); // Display the category
            drawSeparator('-', 30);
        }
    }
    else{
        typeText("No user-defined recipes available.\n", 20);
    }
}

// Custom implementation of strcasestr for Windows
char *strcasestr(const char *haystack, const char *needle){
    if (!haystack || !needle)
        return NULL;

    size_t needle_len = strlen(needle);

    while (*haystack)
    {
        if (strncasecmp(haystack, needle, needle_len) == 0)
            return (char *)haystack;
        haystack++;
    }

    return NULL;
}

// Case-insensitive string comparison for the first n characters
int strncasecmp(const char *s1, const char *s2, size_t n){
    while (n-- && *s1 && *s2)
    {
        char c1 = tolower((unsigned char)*s1++);
        char c2 = tolower((unsigned char)*s2++);
        if (c1 != c2)
            return c1 - c2;
    }
    return 0;
}

// Function to search recipes by name or category
void searchRecipe(){
    int search_choice;
    typeText("Choose search option:\n", 20);
    typeText("1. Search by Name\n", 20);
    typeText("2. Search by Category\n", 20);
    typeText("Enter your choice: ", 20);
    scanf("%d", &search_choice);
    clearInputBuffer();

    if (search_choice == 1){ // Search by Name
        char searchName[MAX_NAME_LEN];
        typeText("Enter the name or part of the ntextame of the recipe to search: ", 20);
        fgets(searchName, MAX_NAME_LEN, stdin);
        searchName[strcspn(searchName, "\n")] = 0;

        int found = 0;
        for (int i = 0; i < recipe_count; i++){
            if (strcasestr(recipes[i].name, searchName) != NULL){ // Partial match
                printf("\nRecipe found:\n");
                drawSeparator('-', 30);
                printf("Name: %s\n", recipes[i].name);
                printf("Ingredients: %s\n", recipes[i].ingredients);
                printf("Instructions: %s\n", recipes[i].instructions);
                printf("Preparation time: %d minutes\n", recipes[i].prep_time);
                printf("Category: %s\n", recipes[i].category);
                drawSeparator('-', 30);
                found = 1;
            }
        }

        if (!found){
            setTextColor(12);
            printf("No recipes found matching the name.\n");
            setTextColor(7);
        }
    }
    else if (search_choice == 2){ // Search by Category
        int category_choice;
        typeText("\nChoose the category to search:\n", 20);
        typeText("1. Breakfast\n", 20);
        typeText("2. Snacks\n", 20);
        typeText("3. Lunch\n", 20);
        typeText("4. Dinner\n", 20);
        typeText("Enter your choice: ", 20);
        scanf("%d", &category_choice);
        clearInputBuffer();

        const char *category = NULL;
        switch (category_choice){
        case 1:
            category = "Breakfast";
            break;
        case 2:
            category = "Snacks";
            break;
        case 3:
            category = "Lunch";
            break;
        case 4:
            category = "Dinner";
            break;
        default:
            setTextColor(12);
            printf("Invalid category choice.\n");
            setTextColor(13);
            searchRecipe();
            return;
        }

        int found = 0;
        for (int i = 0; i < recipe_count; i++){
            if (strcasecmp(recipes[i].category, category) == 0)
            {
                printf("\nRecipe found:\n");
                drawSeparator('-', 30);
                printf("Name: %s\n", recipes[i].name);
                printf("Ingredients: %s\n", recipes[i].ingredients);
                printf("Instructions: %s\n", recipes[i].instructions);
                printf("Preparation time: %d minutes\n", recipes[i].prep_time);
                printf("Category: %s\n", recipes[i].category);
                drawSeparator('-', 30);
                found = 1;
            }
        }

        if (!found){
            setTextColor(12);
            printf("No recipes found in the '%s' category.\n", category);
            setTextColor(7);
        }
    }
    else{
        setTextColor(12);
        printf("Invalid choice, try again.\n");
        setTextColor(13);
        searchRecipe();
    }
}

// The rest of the functions remain unchanged.
// Function to save recipes to a file
void saveRecipes()
{
    FILE *file = fopen(filename, "wb");
    if (!file){
        errorHandling("Failed to open file for writing.");
        return;
    }

    fwrite(&recipe_count, sizeof(int), 1, file);         // Save the number of recipes
    fwrite(recipes, sizeof(Recipe), recipe_count, file); // Save all recipes

    fclose(file);
    printf("Recipes saved to file.\n");
}

// Function to load recipes from a file
void loadRecipes(){
    FILE *file = fopen(filename, "rb");
    if (!file){
        printf("No existing recipe file found. Starting fresh.\n");
        return;
    }

    fread(&recipe_count, sizeof(int), 1, file);         // Load the number of recipes
    fread(recipes, sizeof(Recipe), recipe_count, file); // Load all recipes

    fclose(file);
    printf("Recipes loaded from file.\n");
}

// Error handling function
void errorHandling(const char *message){
    setTextColor(12); // Red color for error message
    printf("Error: %s\n", message);
    setTextColor(7); // Reset to default
    exit(1);
}

// Utility function to clear input buffer
void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void speakText(char text[]){
    // char text[100];
    // printf("Enter the text you want to speak: ");
    // fgets(text, 100, stdin);
    // scanf("%s", text);
    // fgets(text, 99, stdin);
    text[strcspn(text, "\n")] = '\0';
    char command[512];

    // Format the PowerShell command
    snprintf(command, sizeof(command),
             "PowerShell -Command \"Add-Type -AssemblyName System.Speech; "
             "$synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
             "$synth.Speak('%s');\"",
             text);

    // Execute the command
    system(command);
}

// Main function
int main(){
    // AudioData audio_data = {argv[1], true, false}; // Looping audio

    // // Start audio in a separate thread
    // unsigned audio_thread_handle = _beginthreadex(NULL, 0, audio_thread, &audio_data, 0, NULL);
    // if (!audio_thread_handle)
    // {
    //     printf("Failed to create audio thread.\n");
    //     return 1;
    // }
    // WaitForSingleObject((HANDLE)audio_thread_handle, INFINITE);
    // CloseHandle((HANDLE)audio_thread_handle);
    loadRecipes();
    menu();
    return 0;
}