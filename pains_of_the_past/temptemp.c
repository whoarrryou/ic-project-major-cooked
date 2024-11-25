#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <process.h>

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
void speakText(char text[]);

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
            typeText("Invalid choice. Please try again.\n", 20);
            char invalidChoice[] = "Invalid choice. Please try again.";
            speakText(invalidChoice);
        }
        typeText("\nPress Enter to return to the main menu...", 20);
        char returnMenu[] = "Press Enter to return to the main menu";
        speakText(returnMenu);
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
    static int temp = 1;
    if(temp == 1){
    PlaySound("Welcome.wav", NULL, SND_FILENAME | SND_ASYNC);
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
    typeText("\nRecipe added successfully! \n", 20);
    setTextColor(7);
}

// Function to display all recipes
void displayRecipes()
{
    if (recipe_count == 0)
    {
        setTextColor(12);
        typeText("No recipes found. \n", 20);
        setTextColor(7);
        return;
    }

    char display[] = "Here are all the recipes!";
    speakText(display);

    for (int i = 0; i < recipe_count; i++)
    {
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

// Function to search recipes by name
void searchRecipe()
{
    char searchName[MAX_NAME_LEN];
    printf("Enter the name of the recipe to search: ");
    fgets(searchName, MAX_NAME_LEN, stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < recipe_count; i++)
    {
        if (strcasecmp(recipes[i].name, searchName) == 0)
        {
            printf("\nRecipe found:\n");
            drawSeparator('-', 30);
            printf("Name: %s\n", recipes[i].name);
            printf("Ingredients: %s\n", recipes[i].ingredients);
            printf("Instructions: %s\n", recipes[i].instructions);
            printf("Preparation time: %d minutes\n", recipes[i].prep_time);
            printf("Category: %s\n", recipes[i].category); // Display the category
            drawSeparator('-', 30);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        setTextColor(12);
        printf("Recipe not found. \n");
        setTextColor(7);
    }
}

// The rest of the functions remain unchanged.
// Function to save recipes to a file
void saveRecipes()
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        errorHandling("Failed to open file for writing.");
        return;
    }

    fwrite(&recipe_count, sizeof(int), 1, file);         // Save the number of recipes
    fwrite(recipes, sizeof(Recipe), recipe_count, file); // Save all recipes

    fclose(file);
    printf("Recipes saved to file.\n");
}

// Function to load recipes from a file
void loadRecipes()
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("No existing recipe file found. Starting fresh.\n");
        return;
    }

    fread(&recipe_count, sizeof(int), 1, file);         // Load the number of recipes
    fread(recipes, sizeof(Recipe), recipe_count, file); // Load all recipes

    fclose(file);
    printf("Recipes loaded from file.\n");
}

// Error handling function
void errorHandling(const char *message)
{
    setTextColor(12); // Red color for error message
    printf("Error: %s\n", message);
    setTextColor(7); // Reset to default
    exit(1);
}

// Utility function to clear input buffer
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void speakText(char text[])
{
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
int main()
{
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
