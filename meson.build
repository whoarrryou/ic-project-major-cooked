# Set the project name and language
project('Project_Cooked', 'c')

# Add GTK as a dependency
gtk_dep = dependency('gtk4')

# Define the executable, including all source files and dependencies
executable('Project_cooked', ['src/main.c', 'gui/main_gui.c'], dependencies : gtk_dep)
