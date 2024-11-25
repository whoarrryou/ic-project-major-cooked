#include <stdio.h>
#include <unistd.h> // For usleep

// Function to set text color using RGB
void setTextColor(int r, int g, int b) {
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

// Function to reset terminal attributes
void resetTextColor() {
    printf("\033[0m");
}

// Main animation function
void animateHello() {
    const char *text = "HELLO";
    int steps = 100; // Number of gradient steps
    int r1 = 255, g1 = 0, b1 = 0;   // Start color (Red)
    int r2 = 0, g2 = 0, b2 = 255;   // End color (Blue)

    for (int i = 0; i < steps; ++i) {
        // Calculate intermediate color for the gradient
        int r = r1 + (r2 - r1) * i / steps;
        int g = g1 + (g2 - g1) * i / steps;
        int b = b1 + (b2 - b1) * i / steps;

        // Clear the screen and move to the top-left
        printf("\033[2J\033[H");

        // Set text color and print
        setTextColor(r, g, b);
        printf("%s\n", text);

        // Reset color for terminal safety
        resetTextColor();

        // Delay for animation effect
        usleep(100000); // 100ms
    }
}

int main() {
    animateHello();
    return 0;
}
