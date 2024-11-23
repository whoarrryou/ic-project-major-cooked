#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *text = "Hello, welcome to text to speech in C!";
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
