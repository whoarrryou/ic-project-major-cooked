#include <stdio.h>

// Recipe structure
typedef struct {
    char name[MAX_NAME_LEN];
    char ingredients[MAX_INGREDIENTS_LEN];
    char instructions[MAX_INSTRUCTIONS_LEN];
    int prep_time;  // in minutes
} Recipe;

const Recipe predefinedRecipes[] = {
    {"Pancakes",
     "Flour, Eggs, Milk, Baking Powder, Sugar",
     "1. Mix ingredients.\n2. Heat pan and cook batter until golden brown.",
     20,
     "Breakfast",
     "https://www.youtube.com/watch?v=pancakes",
     227},
    {"Grilled Cheese Sandwich",
     "Bread, Cheese, Butter",
     "1. Heat pan.\n2. Assemble sandwich.\n3. Grill until golden.",
     10,
     "Snacks",
     "https://www.youtube.com/watch?v=grilledcheese",
     378},
    {"Spaghetti Bolognese",
     "Spaghetti, Ground Beef, Tomato Sauce, Onion, Garlic",
     "1. Cook spaghetti.\n2. Prepare sauce.\n3. Mix and serve.",
     40,
     "Lunch",
     "https://www.youtube.com/watch?v=spaghettibolognese",
     382},
    {"Chicken Curry",
     "Chicken, Onion, Tomato, Spices, Cream",
     "1. Saute onions and spices.\n2. Add chicken and cook.\n3. Add cream and simmer.",
     45,
     "Dinner",
     "https://www.youtube.com/watch?v=chickencurry",
     550},
    {"Avocado Toast",
     "Bread, Avocado, Olive Oil, Lemon, Salt",
     "1. Toast bread.\n2. Mash avocado and spread on toast.\n3. Season and drizzle with olive oil.",
     5,
     "Breakfast",
     "https://www.youtube.com/watch?v=avocadotoast",
     200},
    {"French Toast",
     "Bread, Eggs, Milk, Sugar, Cinnamon",
     "1. Mix eggs, milk, sugar, and cinnamon.\n2. Dip bread and fry until golden.",
     15,
     "Breakfast",
     "https://www.youtube.com/watch?v=frenchtoast",
     250},
    {"Caesar Salad",
     "Lettuce, Croutons, Caesar Dressing, Parmesan, Chicken (optional)",
     "1. Toss ingredients together.\n2. Serve with dressing.",
     10,
     "Snacks",
     "https://www.youtube.com/watch?v=caesarsalad",
     350},
    {"Vegetable Stir Fry",
     "Bell Peppers, Broccoli, Carrot, Soy Sauce, Ginger, Garlic",
     "1. Heat oil.\n2. Stir fry vegetables with soy sauce and spices.",
     20,
     "Lunch",
     "https://www.youtube.com/watch?v=stirfry",
     300},
    {"Beef Steak",
     "Beef Steak, Salt, Pepper, Butter, Garlic, Herbs",
     "1. Season steak.\n2. Cook in skillet until desired doneness.\n3. Rest and serve.",
     25,
     "Dinner",
     "https://www.youtube.com/watch?v=beefsteak",
     650},
    {"Fruit Smoothie",
     "Banana, Berries, Yogurt, Honey, Milk",
     "1. Blend all ingredients until smooth.\n2. Serve chilled.",
     5,
     "Snacks",
     "https://www.youtube.com/watch?v=fruitsmoothie",
     180},
    {"Miso Soup",
     "Miso Paste, Tofu, Seaweed, Green Onions, Dashi Broth",
     "1. Prepare dashi broth.\n2. Add miso paste, tofu, and seaweed.\n3. Heat gently and serve.",
     15,
     "Dinner",
     "https://www.youtube.com/watch?v=misosoup",
     90},
    {"Egg Fried Rice",
     "Rice, Eggs, Vegetables, Soy Sauce, Oil",
     "1. Cook and cool rice.\n2. Stir fry with eggs and vegetables.",
     20,
     "Lunch",
     "https://www.youtube.com/watch?v=eggfriedrice",
     400},
    {"Tacos",
     "Tortillas, Ground Meat, Lettuce, Tomato, Cheese, Salsa",
     "1. Cook meat.\n2. Assemble tacos with toppings.\n3. Serve.",
     30,
     "Dinner",
     "https://www.youtube.com/watch?v=tacos",
     500},
    {"Vegetable Soup",
     "Carrots, Celery, Onion, Potatoes, Broth, Herbs",
     "1. Sauté vegetables.\n2. Add broth and simmer.\n3. Blend (optional) and serve.",
     40,
     "Lunch",
     "https://www.youtube.com/watch?v=vegetablesoup",
     150},
    {"Scrambled Eggs",
     "Eggs, Milk, Butter, Salt, Pepper",
     "1. Whisk eggs and milk.\n2. Cook in butter over low heat, stirring.",
     10,
     "Breakfast",
     "https://www.youtube.com/watch?v=scrambledeggs",
     200},
    {"Chicken Sandwich",
     "Chicken, Bread, Lettuce, Mayo, Tomato",
     "1. Grill chicken.\n2. Assemble sandwich with ingredients.",
     15,
     "Snacks",
     "https://www.youtube.com/watch?v=chickensandwich",
     320},
    {"Chocolate Brownie",
     "Butter, Sugar, Cocoa Powder, Flour, Eggs",
     "1. Mix ingredients.\n2. Bake at 350°F for 25 minutes.",
     40,
     "Snacks",
     "https://www.youtube.com/watch?v=chocolatebrownie",
     450},
    {"Pizza",
     "Pizza Dough, Tomato Sauce, Cheese, Toppings",
     "1. Spread sauce and toppings on dough.\n2. Bake at 400°F until crispy.",
     30,
     "Dinner",
     "https://www.youtube.com/watch?v=pizza",
     700},
    {"Pasta Primavera",
     "Pasta, Vegetables, Olive Oil, Parmesan, Garlic",
     "1. Cook pasta.\n2. Sauté vegetables and mix with pasta.",
     25,
     "Lunch",
     "https://www.youtube.com/watch?v=pastaprimavera",
     360},
    {"Cupcakes",
     "Flour, Sugar, Butter, Eggs, Milk, Frosting",
     "1. Prepare batter.\n2. Bake and decorate with frosting.",
     45,
     "Snacks",
     "https://www.youtube.com/watch?v=cupcakes",
     350},
    {"Chicken Alfredo",
     "Chicken, Pasta, Cream, Parmesan, Garlic",
     "1. Cook pasta.\n2. Prepare Alfredo sauce and mix with chicken.",
     35,
     "Dinner",
     "https://www.youtube.com/watch?v=chickenalfredo",
     650},
    {"Greek Salad",
     "Tomato, Cucumber, Onion, Feta, Olives, Dressing",
     "1. Chop vegetables.\n2. Toss with dressing and feta.",
     15,
     "Lunch",
     "https://www.youtube.com/watch?v=greeksalad",
     250},
    {"Omelette",
     "Eggs, Onion, Bell Peppers, Cheese, Salt",
     "1. Whisk eggs.\n2. Cook in skillet with fillings.",
     10,
     "Breakfast",
     "https://www.youtube.com/watch?v=omelette",
     210},
    {"Cheesecake",
     "Cream Cheese, Sugar, Eggs, Graham Cracker Crust",
     "1. Prepare crust.\n2. Mix and bake cheesecake filling.",
     60,
     "Snacks",
     "https://www.youtube.com/watch?v=cheesecake",
     420},
    {"Masala Dosa",
     "Rice, Urad Dal, Potato, Spices, Curry Leaves",
     "1. Prepare dosa batter.\n2. Cook dosa on a pan.\n3. Add spiced potato filling and serve.",
     45,
     "Breakfast",
     "https://www.youtube.com/watch?v=masaladosa",
     206},
    {"Palak Paneer",
     "Spinach, Paneer, Onion, Tomato, Spices, Cream",
     "1. Blanch spinach.\n2. Prepare masala base.\n3. Add paneer and cream.",
     40,
     "Lunch",
     "https://www.youtube.com/watch?v=palakpaneer",
     250},
    {"Aloo Paratha",
     "Wheat Flour, Potato, Spices, Ghee",
     "1. Knead dough.\n2. Prepare spiced potato filling.\n3. Roll and cook parathas.",
     30,
     "Breakfast",
     "https://www.youtube.com/watch?v=alooparatha",
     300},
    {"Chole Bhature",
     "Chickpeas, Flour, Yogurt, Spices",
     "1. Prepare spiced chole curry.\n2. Make and fry bhature.",
     50,
     "Lunch",
     "https://www.youtube.com/watch?v=cholebhature",
     450},
    {"Samosa",
     "Potato, Peas, Flour, Spices, Oil",
     "1. Prepare dough.\n2. Make spiced potato filling.\n3. Shape and fry samosas.",
     45,
     "Snacks",
     "https://www.youtube.com/watch?v=samosa",
     280},
    {"Paneer Butter Masala",
     "Paneer, Butter, Tomato, Cashew, Cream, Spices",
     "1. Prepare creamy tomato gravy.\n2. Add paneer and simmer.",
     35,
     "Dinner",
     "https://www.youtube.com/watch?v=paneerbuttermasala",
     320},
    {"Hyderabadi Biryani",
     "Rice, Chicken, Yogurt, Spices, Saffron",
     "1. Marinate chicken.\n2. Layer rice and chicken.\n3. Cook on low heat.",
     90,
     "Dinner",
     "https://www.youtube.com/watch?v=hyderabadibiryani",
     500},
    {"Idli with Sambhar",
     "Rice, Urad Dal, Vegetables, Spices",
     "1. Prepare idli batter and steam.\n2. Cook sambhar with vegetables and spices.",
     50,
     "Breakfast",
     "https://www.youtube.com/watch?v=idliandsambhar",
     190},
    {"Pav Bhaji",
     "Pav Bread, Potatoes, Tomatoes, Butter, Spices",
     "1. Prepare spicy vegetable mash.\n2. Toast pav with butter and serve.",
     40,
     "Snacks",
     "https://www.youtube.com/watch?v=pavbhaji",
     300},
    {"Rajma Chawal",
     "Kidney Beans, Rice, Tomato, Onion, Spices",
     "1. Cook rajma in spiced tomato gravy.\n2. Serve with steamed rice.",
     60,
     "Lunch",
     "https://www.youtube.com/watch?v=rajmachawal",
     350},
    {"Dhokla",
     "Gram Flour, Yogurt, Spices, Mustard Seeds",
     "1. Prepare batter.\n2. Steam until fluffy.\n3. Garnish with mustard seeds.",
     30,
     "Snacks",
     "https://www.youtube.com/watch?v=dhokla",
     150},
    {"Malai Kofta",
     "Paneer, Potato, Cream, Spices, Tomato",
     "1. Make kofta balls.\n2. Prepare creamy gravy.\n3. Simmer kofta in gravy.",
     50,
     "Dinner",
     "https://www.youtube.com/watch?v=malaikofta",
     400},
    {"Chicken Tikka",
     "Chicken, Yogurt, Spices, Lemon",
     "1. Marinate chicken.\n2. Grill until cooked.",
     30,
     "Snacks",
     "https://www.youtube.com/watch?v=chickentikka",
     320},
    {"Baingan Bharta",
     "Eggplant, Tomato, Onion, Spices",
     "1. Roast eggplant.\n2. Mash and cook with spices.",
     40,
     "Dinner",
     "https://www.youtube.com/watch?v=bainganbharta",
     200},
    {"Matar Paneer",
     "Paneer, Peas, Tomato, Onion, Spices",
     "1. Prepare spiced tomato gravy.\n2. Add peas and paneer.",
     30,
     "Lunch",
     "https://www.youtube.com/watch?v=matarpaneer",
     280},
    {"Dosa with Coconut Chutney",
     "Rice, Urad Dal, Coconut, Spices",
     "1. Prepare dosa batter and cook.\n2. Serve with fresh coconut chutney.",
     40,
     "Breakfast",
     "https://www.youtube.com/watch?v=dosawithchutney",
     230},
    {"Butter Chicken",
     "Chicken, Butter, Cream, Tomato, Spices",
     "1. Cook spiced chicken.\n2. Prepare creamy tomato gravy.\n3. Simmer together.",
     60,
     "Dinner",
     "https://www.youtube.com/watch?v=butterchicken",
     490},
    {"Rasgulla",
     "Milk, Lemon, Sugar",
     "1. Prepare chhena.\n2. Shape and cook in sugar syrup.",
     45,
     "Snacks",
     "https://www.youtube.com/watch?v=rasgulla",
     150},
    {"Kheer",
     "Rice, Milk, Sugar, Cardamom, Nuts",
     "1. Cook rice in milk.\n2. Add sugar and cardamom.\n3. Garnish with nuts.",
     40,
     "Snacks",
     "https://www.youtube.com/watch?v=kheer",
     210},
    {"Gulab Jamun",
     "Milk Powder, Flour, Sugar, Ghee",
     "1. Prepare dough.\n2. Fry and soak in sugar syrup.",
     50,
     "Snacks",
     "https://www.youtube.com/watch?v=gulabjamun",
     280},
    {"Tandoori Roti",
     "Wheat Flour, Yogurt, Spices",
     "1. Knead dough.\n2. Cook in tandoor or hot pan.",
     20,
     "Dinner",
     "https://www.youtube.com/watch?v=tandooriroti",
     150},
    {"Bhel Puri",
     "Puffed Rice, Vegetables, Tamarind Chutney, Spices",
     "1. Mix all ingredients.\n2. Serve immediately.",
     10,
     "Snacks",
     "https://www.youtube.com/watch?v=bhelpuri",
     120},
    {"Pongal",
     "Rice, Moong Dal, Ghee, Pepper, Cumin",
     "1. Cook rice and dal.\n2. Temper spices and mix.",
     30,
     "Breakfast",
     "https://www.youtube.com/watch?v=pongal",
     190}
    // Add additional recipes here if needed...
};
const int predefinedCount = sizeof(predefinedRecipes) / sizeof(predefinedRecipes[0]);
