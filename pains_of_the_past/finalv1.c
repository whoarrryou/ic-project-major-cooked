#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <process.h>
#include <ctype.h>
#include <time.h>

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
    int prep_time;     // in minutes
    char category[20]; // Breakfast, Snacks, Lunch, or Dinner
    char youtube_link[100];
    int calories;
} Recipe;

const Recipe predefinedRecipes[] = {
    {"Pancakes",
     "Flour, Eggs, Milk, Baking Powder, Sugar",
     "1. Mix ingredients.\n2. Heat pan and cook batter until golden brown.",
     20,
     "Breakfast",
     "https://www.youtube.com/results?search_query=pancakes",
     227},
    {"Grilled Cheese Sandwich",
     "Bread, Cheese, Butter",
     "1. Heat pan.\n2. Assemble sandwich.\n3. Grill until golden.",
     10,
     "Snacks",
     "https://www.youtube.com/results?search_query=grilledcheese",
     378},
    {"Spaghetti Bolognese",
     "Spaghetti, Ground Beef, Tomato Sauce, Onion, Garlic",
     "1. Cook spaghetti.\n2. Prepare sauce.\n3. Mix and serve.",
     40,
     "Lunch",
     "https://www.youtube.com/results?search_query=spaghettibolognese",
     382},
    {"Chicken Curry",
     "Chicken, Onion, Tomato, Spices, Cream",
     "1. Saute onions and spices.\n2. Add chicken and cook.\n3. Add cream and simmer.",
     45,
     "Dinner",
     "https://www.youtube.com/results?search_query=chickencurry",
     550},
    {"Avocado Toast",
     "Bread, Avocado, Olive Oil, Lemon, Salt",
     "1. Toast bread.\n2. Mash avocado and spread on toast.\n3. Season and drizzle with olive oil.",
     5,
     "Breakfast",
     "https://www.youtube.com/results?search_query=avocadotoast",
     200},
    {"French Toast",
     "Bread, Eggs, Milk, Sugar, Cinnamon",
     "1. Mix eggs, milk, sugar, and cinnamon.\n2. Dip bread and fry until golden.",
     15,
     "Breakfast",
     "https://www.youtube.com/results?search_query=frenchtoast",
     250},
    {"Caesar Salad",
     "Lettuce, Croutons, Caesar Dressing, Parmesan, Chicken (optional)",
     "1. Toss ingredients together.\n2. Serve with dressing.",
     10,
     "Snacks",
     "https://www.youtube.com/results?search_query=caesarsalad",
     350},
    {"Vegetable Stir Fry",
     "Bell Peppers, Broccoli, Carrot, Soy Sauce, Ginger, Garlic",
     "1. Heat oil.\n2. Stir fry vegetables with soy sauce and spices.",
     20,
     "Lunch",
     "https://www.youtube.com/results?search_query=stirfry",
     300},
    {"Beef Steak",
     "Beef Steak, Salt, Pepper, Butter, Garlic, Herbs",
     "1. Season steak.\n2. Cook in skillet until desired doneness.\n3. Rest and serve.",
     25,
     "Dinner",
     "https://www.youtube.com/results?search_query=beefsteak",
     650},
    {"Fruit Smoothie",
     "Banana, Berries, Yogurt, Honey, Milk",
     "1. Blend all ingredients until smooth.\n2. Serve chilled.",
     5,
     "Snacks",
     "https://www.youtube.com/results?search_query=fruitsmoothie",
     180},
    {"Miso Soup",
     "Miso Paste, Tofu, Seaweed, Green Onions, Dashi Broth",
     "1. Prepare dashi broth.\n2. Add miso paste, tofu, and seaweed.\n3. Heat gently and serve.",
     15,
     "Dinner",
     "https://www.youtube.com/results?search_query=misosoup",
     90},
    {"Egg Fried Rice",
     "Rice, Eggs, Vegetables, Soy Sauce, Oil",
     "1. Cook and cool rice.\n2. Stir fry with eggs and vegetables.",
     20,
     "Lunch",
     "https://www.youtube.com/results?search_query=eggfriedrice",
     400},
    {"Tacos",
     "Tortillas, Ground Meat, Lettuce, Tomato, Cheese, Salsa",
     "1. Cook meat.\n2. Assemble tacos with toppings.\n3. Serve.",
     30,
     "Dinner",
     "https://www.youtube.com/results?search_query=tacos",
     500},
    {"Vegetable Soup",
     "Carrots, Celery, Onion, Potatoes, Broth, Herbs",
     "1. Sauté vegetables.\n2. Add broth and simmer.\n3. Blend (optional) and serve.",
     40,
     "Lunch",
     "https://www.youtube.com/results?search_query=vegetablesoup",
     150},
    {"Scrambled Eggs",
     "Eggs, Milk, Butter, Salt, Pepper",
     "1. Whisk eggs and milk.\n2. Cook in butter over low heat, stirring.",
     10,
     "Breakfast",
     "https://www.youtube.com/results?search_query=scrambledeggs",
     200},
    {"Chicken Sandwich",
     "Chicken, Bread, Lettuce, Mayo, Tomato",
     "1. Grill chicken.\n2. Assemble sandwich with ingredients.",
     15,
     "Snacks",
     "https://www.youtube.com/results?search_query=chickensandwich",
     320},
    {"Chocolate Brownie",
     "Butter, Sugar, Cocoa Powder, Flour, Eggs",
     "1. Mix ingredients.\n2. Bake at 350°F for 25 minutes.",
     40,
     "Snacks",
     "https://www.youtube.com/results?search_query=chocolatebrownie",
     450},
    {"Pizza",
     "Pizza Dough, Tomato Sauce, Cheese, Toppings",
     "1. Spread sauce and toppings on dough.\n2. Bake at 400°F until crispy.",
     30,
     "Dinner",
     "https://www.youtube.com/results?search_query=pizza",
     700},
    {"Pasta Primavera",
     "Pasta, Vegetables, Olive Oil, Parmesan, Garlic",
     "1. Cook pasta.\n2. Sauté vegetables and mix with pasta.",
     25,
     "Lunch",
     "https://www.youtube.com/results?search_query=pastaprimavera",
     360},
    {"Cupcakes",
     "Flour, Sugar, Butter, Eggs, Milk, Frosting",
     "1. Prepare batter.\n2. Bake and decorate with frosting.",
     45,
     "Snacks",
     "https://www.youtube.com/results?search_query=cupcakes",
     350},
    {"Chicken Alfredo",
     "Chicken, Pasta, Cream, Parmesan, Garlic",
     "1. Cook pasta.\n2. Prepare Alfredo sauce and mix with chicken.",
     35,
     "Dinner",
     "https://www.youtube.com/results?search_query=chickenalfredo",
     650},
    {"Greek Salad",
     "Tomato, Cucumber, Onion, Feta, Olives, Dressing",
     "1. Chop vegetables.\n2. Toss with dressing and feta.",
     15,
     "Lunch",
     "https://www.youtube.com/results?search_query=greeksalad",
     250},
    {"Omelette",
     "Eggs, Onion, Bell Peppers, Cheese, Salt",
     "1. Whisk eggs.\n2. Cook in skillet with fillings.",
     10,
     "Breakfast",
     "https://www.youtube.com/results?search_query=omelette",
     210},
    {"Cheesecake",
     "Cream Cheese, Sugar, Eggs, Graham Cracker Crust",
     "1. Prepare crust.\n2. Mix and bake cheesecake filling.",
     60,
     "Snacks",
     "https://www.youtube.com/results?search_query=cheesecake",
     420},
    {"Masala Dosa",
     "Rice, Urad Dal, Potato, Spices, Curry Leaves",
     "1. Prepare dosa batter.\n2. Cook dosa on a pan.\n3. Add spiced potato filling and serve.",
     45,
     "Breakfast",
     "https://www.youtube.com/results?search_query=masaladosa",
     206},
    {"Palak Paneer",
     "Spinach, Paneer, Onion, Tomato, Spices, Cream",
     "1. Blanch spinach.\n2. Prepare masala base.\n3. Add paneer and cream.",
     40,
     "Lunch",
     "https://www.youtube.com/results?search_query=palakpaneer",
     250},
    {"Aloo Paratha",
     "Wheat Flour, Potato, Spices, Ghee",
     "1. Knead dough.\n2. Prepare spiced potato filling.\n3. Roll and cook parathas.",
     30,
     "Breakfast",
     "https://www.youtube.com/results?search_query=alooparatha",
     300},
    {"Chole Bhature",
     "Chickpeas, Flour, Yogurt, Spices",
     "1. Prepare spiced chole curry.\n2. Make and fry bhature.",
     50,
     "Lunch",
     "https://www.youtube.com/results?search_query=cholebhature",
     450},
    {"Samosa",
     "Potato, Peas, Flour, Spices, Oil",
     "1. Prepare dough.\n2. Make spiced potato filling.\n3. Shape and fry samosas.",
     45,
     "Snacks",
     "https://www.youtube.com/results?search_query=samosa",
     280},
    {"Paneer Butter Masala",
     "Paneer, Butter, Tomato, Cashew, Cream, Spices",
     "1. Prepare creamy tomato gravy.\n2. Add paneer and simmer.",
     35,
     "Dinner",
     "https://www.youtube.com/results?search_query=paneerbuttermasala",
     320},
    {"Hyderabadi Biryani",
     "Rice, Chicken, Yogurt, Spices, Saffron",
     "1. Marinate chicken.\n2. Layer rice and chicken.\n3. Cook on low heat.",
     90,
     "Dinner",
     "https://www.youtube.com/results?search_query=hyderabadibiryani",
     500},
    {"Idli with Sambhar",
     "Rice, Urad Dal, Vegetables, Spices",
     "1. Prepare idli batter and steam.\n2. Cook sambhar with vegetables and spices.",
     50,
     "Breakfast",
     "https://www.youtube.com/results?search_query=idliandsambhar",
     190},
    {"Pav Bhaji",
     "Pav Bread, Potatoes, Tomatoes, Butter, Spices",
     "1. Prepare spicy vegetable mash.\n2. Toast pav with butter and serve.",
     40,
     "Snacks",
     "https://www.youtube.com/results?search_query=pavbhaji",
     300},
    {"Rajma Chawal",
     "Kidney Beans, Rice, Tomato, Onion, Spices",
     "1. Cook rajma in spiced tomato gravy.\n2. Serve with steamed rice.",
     60,
     "Lunch",
     "https://www.youtube.com/results?search_query=rajmachawal",
     350},
    {"Dhokla",
     "Gram Flour, Yogurt, Spices, Mustard Seeds",
     "1. Prepare batter.\n2. Steam until fluffy.\n3. Garnish with mustard seeds.",
     30,
     "Snacks",
     "https://www.youtube.com/results?search_query=dhokla",
     150},
    {"Malai Kofta",
     "Paneer, Potato, Cream, Spices, Tomato",
     "1. Make kofta balls.\n2. Prepare creamy gravy.\n3. Simmer kofta in gravy.",
     50,
     "Dinner",
     "https://www.youtube.com/results?search_query=malaikofta",
     400},
    {"Chicken Tikka",
     "Chicken, Yogurt, Spices, Lemon",
     "1. Marinate chicken.\n2. Grill until cooked.",
     30,
     "Snacks",
     "https://www.youtube.com/results?search_query=chickentikka",
     320},
    {"Baingan Bharta",
     "Eggplant, Tomato, Onion, Spices",
     "1. Roast eggplant.\n2. Mash and cook with spices.",
     40,
     "Dinner",
     "https://www.youtube.com/results?search_query=bainganbharta",
     200},
    {"Matar Paneer",
     "Paneer, Peas, Tomato, Onion, Spices",
     "1. Prepare spiced tomato gravy.\n2. Add peas and paneer.",
     30,
     "Lunch",
     "https://www.youtube.com/results?search_query=matarpaneer",
     280},
    {"Dosa with Coconut Chutney",
     "Rice, Urad Dal, Coconut, Spices",
     "1. Prepare dosa batter and cook.\n2. Serve with fresh coconut chutney.",
     40,
     "Breakfast",
     "https://www.youtube.com/results?search_query=dosawithchutney",
     230},
    {"Butter Chicken",
     "Chicken, Butter, Cream, Tomato, Spices",
     "1. Cook spiced chicken.\n2. Prepare creamy tomato gravy.\n3. Simmer together.",
     60,
     "Dinner",
     "https://www.youtube.com/results?search_query=butterchicken",
     490},
    {"Rasgulla",
     "Milk, Lemon, Sugar",
     "1. Prepare chhena.\n2. Shape and cook in sugar syrup.",
     45,
     "Snacks",
     "https://www.youtube.com/results?search_query=rasgulla",
     150},
    {"Kheer",
     "Rice, Milk, Sugar, Cardamom, Nuts",
     "1. Cook rice in milk.\n2. Add sugar and cardamom.\n3. Garnish with nuts.",
     40,
     "Snacks",
     "https://www.youtube.com/results?search_query=kheer",
     210},
    {"Gulab Jamun",
     "Milk Powder, Flour, Sugar, Ghee",
     "1. Prepare dough.\n2. Fry and soak in sugar syrup.",
     50,
     "Snacks",
     "https://www.youtube.com/results?search_query=gulabjamun",
     280},
    {"Tandoori Roti",
     "Wheat Flour, Yogurt, Spices",
     "1. Knead dough.\n2. Cook in tandoor or hot pan.",
     20,
     "Dinner",
     "https://www.youtube.com/results?search_query=tandooriroti",
     150},
    {"Bhel Puri",
     "Puffed Rice, Vegetables, Tamarind Chutney, Spices",
     "1. Mix all ingredients.\n2. Serve immediately.",
     10,
     "Snacks",
     "https://www.youtube.com/results?search_query=bhelpuri",
     120},
    {"Pongal",
     "Rice, Moong Dal, Ghee, Pepper, Cumin",
     "1. Cook rice and dal.\n2. Temper spices and mix.",
     30,
     "Breakfast",
     "https://www.youtube.com/results?search_query=pongal",
     190}
    // Add additional recipes here if needed...
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
void displayRandomRecipe();

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
        printf("\t\t\t\t\t\t\t\t");
        drawSeparator('=', 39);
        setTextColor(13);
        typeText("\n\t\t\t\t\t\t\t\t1. Add a Recipe\n", 10);
        typeText("\t\t\t\t\t\t\t\t2. Display All Recipes\n", 10);
        typeText("\t\t\t\t\t\t\t\t3. Search Recipe by Name\n", 10);
        typeText("\t\t\t\t\t\t\t\t4. Display a Random Recipe\n", 10);
        typeText("\t\t\t\t\t\t\t\t5. Exit\n", 10);
        printf("\t\t\t\t\t\t\t\t");
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
            clearScreen();
            displayRandomRecipe();
            break;
        case 5:
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
    time_t now;
    struct tm *localTime;
    time(&now);                  // Get current time
    localTime = localtime(&now); // Convert to local time

    // Get the current hour (0 - 23)
    int hour = localTime->tm_hour;

    if (temp == 1)
    {

        // Print the appropriate greeting based on the hour
        if (hour >= 5 && hour < 12)
        {
            char greetingMessage[] = "\t\t\t\t\t\t\t\tGood morning!";
            speakText(greetingMessage);
        }
        else if (hour >= 12 && hour < 17)
        {
            char greetingMessage[] = "\t\t\t\t\t\t\t\tGood afternoon!";
            speakText(greetingMessage);
        }
        else
        {
            char greetingMessage[] = "\t\t\t\t\t\t\t\tGood evening!";
            speakText(greetingMessage);
        }

        char welcomeMessage[] = "\t\t\t\t\t\t\t\tWelcome to the recipe app!";
        speakText(welcomeMessage);
    }
    setTextColor(14); // Yellow color
    typeText("\t\t\t\t\t\t\t\t#######################################\n", 10);
    // typeText("\t\t\t\t\t#                                     #\n", 10);
    if (hour >= 5 && hour < 12)
    {
        typeText("\t\t\t\t\t\t\t\t#            Good Morning             #\n", 10);
    }
    else if (hour >= 12 && hour < 17)
    {
        typeText("\t\t\t\t\t\t\t\t#            Good Afternoon           #\n", 10);
    }
    else
    {
        typeText("\t\t\t\t\t\t\t\t#            Good Evening             #\n", 10);
    }

    typeText("\t\t\t\t\t\t\t\t#     WELCOME TO THE RECIPE APP!      #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#######################################\n\n", 10);
    setTextColor(7); // Reset to default color
    temp = 0;
}

// ASCII art for exit
void displayExitArt()
{
    setTextColor(10); // Green color
    char seeYa[] = "\t\t\t\t\t\t\t\tThank you for using the recipe storing app! See you next time";
    typeText("\n\t\t\t\t\t\t\t\tThank you for using the Recipe Storing App! \n", 10);
    speakText(seeYa);
    typeText("\t\t\t\t\t\t\t\t#######################################\n", 10);
    typeText("\t\t\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#         SEE YOU NEXT TIME!          #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#                                     #\n", 10);
    typeText("\t\t\t\t\t\t\t\t#######################################\n", 10);
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
        char full[] = "\t\t\t\t\t\t\t\tRecipe storage is full!";
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

    typeText("Enter number of calories: ", 20);
    scanf("%d", &newRecipe.calories);
    clearInputBuffer();

    // Category input
    char categoryMsg[] = "\t\t\t\t\t\t\t\tChoose the category from below options!";
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
    char recipeAdded[] = "\t\t\t\t\t\t\t\tRecipe added successfully!";
    typeText("\nRecipe added successfully! \n", 20);
    speakText(recipeAdded);
    setTextColor(7);
}

// Function to display all recipes
void displayRecipes()
{

    char display[] = "Here are all the recipes!";
    speakText(display);

    printf("\n--- Predefined Recipes ---\n");
    for (int i = 0; i < predefinedCount; i++)
    {
        printf("\nRecipe %d:\n", i + 1);
        printf("Name: %s\n", predefinedRecipes[i].name);
        printf("Ingredients: %s\n", predefinedRecipes[i].ingredients);
        printf("Instructions: %s\n", predefinedRecipes[i].instructions);
        printf("Preparation Time: %d minutes\n", predefinedRecipes[i].prep_time);
        printf("Category: %s\n", predefinedRecipes[i].category);
        printf("Calories: %d\n", predefinedRecipes[i].calories);
        printf("YouTube Link: %s\n", predefinedRecipes[i].youtube_link);
        printf("----------------------------\n");
    }

    // if (recipe_count == 0)
    // {
    //     setTextColor(12);
    //     typeText("No user-defined recipes found. \n", 20);
    //     setTextColor(7);
    //     return;
    // }
    if (recipe_count > 0)
    {
        printf("\n--- User-Defined Recipes ---\n");
        for (int i = 0; i < recipe_count; i++)
        {
            printf("\nRecipe %d:\n", i + 1);
            drawSeparator('-', 30);
            printf("Name: %s\n", recipes[i].name);
            printf("Ingredients: %s\n", recipes[i].ingredients);
            printf("Instructions: %s\n", recipes[i].instructions);
            printf("Preparation time: %d minutes\n", recipes[i].prep_time);
            printf("Category: %s\n", recipes[i].category); // Display the category
            printf("Calories: %d\n", recipes[i].calories);
            drawSeparator('-', 30);
        }
    }
    else
    {
        typeText("No user-defined recipes available.\n", 20);
    }
}

// Custom implementation of strcasestr for Windows
char *strcasestr(const char *haystack, const char *needle)
{
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
int strncasecmp(const char *s1, const char *s2, size_t n)
{
    while (n-- && *s1 && *s2)
    {
        char c1 = tolower((unsigned char)*s1++);
        char c2 = tolower((unsigned char)*s2++);
        if (c1 != c2)
            return c1 - c2;
    }
    return 0;
}

// Helper function to check if a word is found as a whole in a string
bool isWholeWordMatch(const char *haystack, const char *needle)
{
    const char *pos = strcasestr(haystack, needle);
    while (pos != NULL)
    {
        // Check if the character before and after the match are word boundaries
        bool before_is_boundary = (pos == haystack) || isspace(*(pos - 1)) || ispunct(*(pos - 1));
        bool after_is_boundary = (*(pos + strlen(needle)) == '\0') || isspace(*(pos + strlen(needle))) || ispunct(*(pos + strlen(needle)));

        if (before_is_boundary && after_is_boundary)
        {
            return true;
        }
        pos = strcasestr(pos + 1, needle);
    }
    return false;
}

// Function to search recipes by name or category
void searchRecipe()
{
    int search_choice;
    typeText("Choose search option:\n", 20);
    typeText("1. Search by Name\n", 20);
    typeText("2. Search by Category\n", 20);
    typeText("Enter your choice: ", 20);
    scanf("%d", &search_choice);
    clearInputBuffer();

    if (search_choice == 1)
    { // Search by Name
        char searchName[MAX_NAME_LEN];
        typeText("Enter the name or part of the name of the recipe to search: ", 20);
        fgets(searchName, MAX_NAME_LEN, stdin);
        searchName[strcspn(searchName, "\n")] = 0;

        int found = 0;
        for (int i = 0; i < MAX_RECIPES; i++)
        {
            if (isWholeWordMatch(recipes[i].name, searchName))
            { // Partial match
                printf("\nRecipe found:\n");
                drawSeparator('-', 30);
                printf("Name: %s\n", recipes[i].name);
                printf("Ingredients: %s\n", recipes[i].ingredients);
                printf("Instructions: %s\n", recipes[i].instructions);
                printf("Preparation time: %d minutes\n", recipes[i].prep_time);
                printf("Category: %s\n", recipes[i].category);
                printf("Calories: %d\n", recipes[i].calories);
                drawSeparator('-', 30);
                found = 1;
            }
            if (isWholeWordMatch(predefinedRecipes[i].name, searchName))
            { // Partial match
                printf("\nRecipe found:\n");
                drawSeparator('-', 30);
                printf("Name: %s\n", predefinedRecipes[i].name);
                printf("Ingredients: %s\n", predefinedRecipes[i].ingredients);
                printf("Instructions: %s\n", predefinedRecipes[i].instructions);
                printf("Preparation time: %d minutes\n", predefinedRecipes[i].prep_time);
                printf("Category: %s\n", predefinedRecipes[i].category);
                printf("Calories: %d\n", predefinedRecipes[i].calories);
                printf("YouTube Link: %s\n", predefinedRecipes[i].youtube_link);
                drawSeparator('-', 30);
                found = 1;
            }
        }

        if (!found)
        {
            setTextColor(12);
            printf("No recipes found matching the name.\n");
            setTextColor(7);
        }
    }
    else if (search_choice == 2)
    { // Search by Category
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
        switch (category_choice)
        {
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
            clearScreen();
            printf("Invalid category choice.\n");
            setTextColor(13);
            searchRecipe();
            return;
        }

        int found = 0;
        for (int i = 0; i < MAX_RECIPES; i++)
        {
            if (strcasecmp(recipes[i].category, category) == 0)
            {
                printf("\nRecipe found:\n");
                drawSeparator('-', 30);
                printf("Name: %s\n", recipes[i].name);
                printf("Ingredients: %s\n", recipes[i].ingredients);
                printf("Instructions: %s\n", recipes[i].instructions);
                printf("Preparation time: %d minutes\n", recipes[i].prep_time);
                printf("Category: %s\n", recipes[i].category);
                printf("Calories: %d\n", recipes[i].calories);
                drawSeparator('-', 30);
                found = 1;
            }
            if (strcasecmp(predefinedRecipes[i].category, category) == 0)
            {
                printf("\nRecipe found:\n");
                drawSeparator('-', 30);
                printf("Name: %s\n", predefinedRecipes[i].name);
                printf("Ingredients: %s\n", predefinedRecipes[i].ingredients);
                printf("Instructions: %s\n", predefinedRecipes[i].instructions);
                printf("Preparation time: %d minutes\n", predefinedRecipes[i].prep_time);
                printf("Category: %s\n", predefinedRecipes[i].category);
                printf("Calories: %d\n", predefinedRecipes[i].calories);
                printf("YouTube Link: %s\n", predefinedRecipes[i].youtube_link);
                drawSeparator('-', 30);
                found = 1;
            }
        }

        if (!found)
        {
            setTextColor(12);
            printf("No recipes found in the '%s' category.\n", category);
            setTextColor(7);
        }
    }
    else
    {
        setTextColor(12);
        clearScreen();
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

void displayRandomRecipe()
{
    if (recipe_count == 0 && predefinedCount == 0)
    {
        setTextColor(12);
        typeText("No recipes available.\n", 20);
        setTextColor(7);
        return;
    }

    // Seed the random number generator
    srand(time(NULL));

    // Randomly choose from the predefined recipes or the user-defined recipes
    int totalRecipes = predefinedCount + recipe_count;
    int randomIndex = rand() % totalRecipes;

    Recipe selectedRecipe;
    if (randomIndex < predefinedCount)
    {
        // Select from predefined recipes
        selectedRecipe = predefinedRecipes[randomIndex];
    }
    else
    {
        // Select from user-defined recipes
        selectedRecipe = recipes[randomIndex - predefinedCount];
    }

    // Display the selected recipe in the same format as in displayRecipes
    printf("\n--- Random Recipe ---\n");
    drawSeparator('-', 30);
    printf("Name: %s\n", selectedRecipe.name);
    printf("Ingredients: %s\n", selectedRecipe.ingredients);
    printf("Instructions: %s\n", selectedRecipe.instructions);
    printf("Preparation time: %d minutes\n", selectedRecipe.prep_time);
    printf("Category: %s\n", selectedRecipe.category);
    printf("Calories: %d\n", selectedRecipe.calories);

    if (randomIndex < predefinedCount)
    {
        printf("YouTube Link: %s\n", selectedRecipe.youtube_link);
    }

    drawSeparator('-', 30);
}

// Main function
int main()
{
    srand(time(NULL));
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