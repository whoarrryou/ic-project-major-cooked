import struct

# Define the recipe structure based on the provided C code
# name: MAX_NAME_LEN (50 bytes)
# ingredients: MAX_INGREDIENTS_LEN (500 bytes)
# instructions: MAX_INSTRUCTIONS_LEN (1000 bytes)
# prep_time: int (4 bytes)

MAX_NAME_LEN = 50
MAX_INGREDIENTS_LEN = 500
MAX_INSTRUCTIONS_LEN = 1000
RECIPE_STRUCT_FORMAT = f"{MAX_NAME_LEN}s{MAX_INGREDIENTS_LEN}s{MAX_INSTRUCTIONS_LEN}sI"

# Sample recipes for Indian and continental dishes
recipes = [
    # Indian dishes
    ("Butter Chicken", "Chicken, Butter, Tomato, Cream, Spices", "Cook chicken in butter, add tomato puree and cream, and simmer.", 40),
    ("Paneer Tikka", "Paneer, Yogurt, Spices, Onion, Bell Peppers", "Marinate paneer and veggies, grill or bake.", 30),
    ("Biryani", "Rice, Chicken, Spices, Yogurt, Onions", "Layer cooked rice and chicken, simmer with spices.", 60),
    ("Samosa", "Potato, Flour, Spices, Peas", "Make dough, prepare filling, and deep fry.", 45),
    ("Dal Makhani", "Black Lentils, Butter, Cream, Tomato, Spices", "Cook lentils, add butter and cream, and simmer.", 50),
    # Continental dishes
    ("Spaghetti Carbonara", "Spaghetti, Eggs, Pancetta, Parmesan, Pepper", "Cook pasta, mix with pancetta, eggs, and cheese.", 20),
    ("Caesar Salad", "Romaine Lettuce, Croutons, Caesar Dressing", "Mix lettuce with dressing and croutons.", 15),
    ("Ratatouille", "Zucchini, Eggplant, Tomato, Bell Peppers", "Layer veggies, bake with tomato sauce.", 50),
    ("Beef Wellington", "Beef, Puff Pastry, Mushrooms, Prosciutto", "Wrap beef with mushroom mixture and pastry, bake.", 120),
    ("Quiche Lorraine", "Eggs, Cream, Bacon, Cheese, Pastry", "Mix eggs and cream, pour into pastry, and bake.", 60),
]

# Add additional recipes to make up to 80 total
# while len(recipes) < 80:
#     recipes.append(
#         (
#             f"Dish {len(recipes) + 1}",
#             f"Ingredient A, Ingredient B, Ingredient C (for Dish {len(recipes) + 1})",
#             f"Cook Ingredient A, mix with B and C for Dish {len(recipes) + 1}.",
#             30 + len(recipes) % 10,  # Vary prep times slightly
#         )
#     )

# Ensure all recipes meet length requirements by padding or truncating
def prepare_recipe(name, ingredients, instructions, prep_time):
    name = name.encode("utf-8")[:MAX_NAME_LEN].ljust(MAX_NAME_LEN, b"\0")
    ingredients = ingredients.encode("utf-8")[:MAX_INGREDIENTS_LEN].ljust(MAX_INGREDIENTS_LEN, b"\0")
    instructions = instructions.encode("utf-8")[:MAX_INSTRUCTIONS_LEN].ljust(MAX_INSTRUCTIONS_LEN, b"\0")
    return struct.pack(RECIPE_STRUCT_FORMAT, name, ingredients, instructions, prep_time)

# Create the binary data
binary_data = struct.pack("I", len(recipes))  # Number of recipes
binary_data += b"".join(prepare_recipe(*recipe) for recipe in recipes)

# Save to a .dat file
output_file_path = "recipes.dat"
with open(output_file_path, "wb") as file:
    file.write(binary_data)

output_file_path