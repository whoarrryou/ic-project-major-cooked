#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <process.h> // For _beginthreadex

typedef struct
{
    const char *audio_file;
    bool loop;
    bool stop;
} AudioData;

// Function to play audio in a separate thread
unsigned __stdcall audio_thread(void *arg)
{
    AudioData *data = (AudioData *)arg;

    do
    {
        PlaySound(data->audio_file, NULL, SND_FILENAME | SND_ASYNC);
        while (!data->stop)
        {
            Sleep(100); // Check periodically if the audio should stop
        }
    } while (data->loop && !data->stop);

    // Stop playback when done
    PlaySound(NULL, NULL, 0);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <audio_file.wav>\n", argv[0]);
        return 1;
    }

    // Audio setup
    AudioData audio_data = {argv[1], true, false}; // Looping audio

    // Start audio in a separate thread
    unsigned audio_thread_handle = _beginthreadex(NULL, 0, audio_thread, &audio_data, 0, NULL);
    if (!audio_thread_handle)
    {
        printf("Failed to create audio thread.\n");
        return 1;
    }

    // Main program logic (e.g., simple counter)
    printf("Background music is playing... Press 'q' to quit.\n");
    printf("Main program running: Counting to infinity!\n");

    int counter = 0;
    while (true)
    {
        printf("Counter: %d\n", counter++);
        Sleep(500); // Simulate work with a half-second delay

        // Check for user input to quit
        if (GetAsyncKeyState('Q') & 0x8000)
        {
            printf("Exiting...\n");
            audio_data.stop = true; // Signal the audio thread to stop
            break;
        }
    }

    // Wait for the audio thread to finish
    WaitForSingleObject((HANDLE)audio_thread_handle, INFINITE);
    CloseHandle((HANDLE)audio_thread_handle);

    return 0;
}
