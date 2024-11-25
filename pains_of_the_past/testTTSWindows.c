#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // const char *text = "Hello, welcome to text to speech in C!";
    char text[100];
    printf("Enter the text you want to speak: ");
    fgets(text, 100, stdin);
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

    return 0;
}
