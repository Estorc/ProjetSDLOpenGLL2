#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <stdio.h>
#include <stdlib.h>
#include <io/video.h>
#include "test.h"


// Define window dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define AUDIO_BUFFER_SIZE 4096

SDL_Window* test_window;
SDL_GLContext context;

// OpenGL texture for rendering
GLuint textureID;

int test() {
    return 0;
}


// Function to initialize SDL and OpenGL
int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }


    if (init_video() == 0) {
        fprintf(stderr, "Failed to open audio device: %s\n", SDL_GetError());
        exit(1);
    }
    

    test_window = SDL_CreateWindow("MP4 Player with FFmpeg, SDL2 and OpenGL",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!test_window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    context = SDL_GL_CreateContext(test_window);
    if (!context) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    if (glewInit() != GLEW_OK) {
        printf("Error initializing GLEW!\n");
        return -1;
    }

    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return 0;
}

// Clean up resources
void cleanup() {
    free_video();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(test_window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please provide a video file!\n");
        return -1;
    }

    if (initSDL() < 0) {
        return -1;
    }

    if (loadVideo(argv[1]) < 0) {
        return -1;
    }

    SDL_Event event;
    int quit = 0;
    int64_t start_time = SDL_GetTicks(); // Get the start time
    while (quit != -1) {
        SDL_PollEvent(&event);
        quit = play_video(); // Call play_video to decode and render frames
        if (get_video_image(start_time, textureID)) {
            glClear(GL_COLOR_BUFFER_BIT);

            // Set up vertices for a fullscreen quad
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
            glEnd();
            SDL_GL_SwapWindow(test_window);
        }
        get_video_audio(start_time, textureID); // Pass the start time to playVideo
        flush_video();
        if (event.type == SDL_QUIT)
            quit = -1;
    }
    cleanup();

    return 0;
}
