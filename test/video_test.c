#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

// Define window dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Window* test_window;
SDL_GLContext context;

// OpenGL texture for rendering
GLuint textureID;

// FFmpeg-related variables
AVFormatContext* formatContext = NULL;
AVCodecContext* codecContext = NULL;
AVCodec* codec = NULL;
AVFrame* frame = NULL;
AVFrame* rgbFrame = NULL;
uint8_t* buffer = NULL;
struct SwsContext* swsContext = NULL;
int videoStreamIndex = -1;

int test() {
    return 0;
}

// Function to initialize SDL and OpenGL
int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
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
    return 0;
}

// Function to load the MP4 file using FFmpeg
int loadVideo(const char* filename) {
    //av_register_all(); DEPRECATED
    avformat_network_init();

    // Open video file
    if (avformat_open_input(&formatContext, filename, NULL, NULL) != 0) {
        printf("Could not open video file: %s\n", filename);
        return -1;
    }

    // Find video stream
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        printf("Could not find stream information\n");
        return -1;
    }

    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        printf("No video stream found!\n");
        return -1;
    }

    // Get codec
    codecContext = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar);
    codec = avcodec_find_decoder(codecContext->codec_id);
    if (!codec) {
        printf("Codec not found!\n");
        return -1;
    }

    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        printf("Could not open codec!\n");
        return -1;
    }

    // Allocate frames
    frame = av_frame_alloc();
    rgbFrame = av_frame_alloc();
    if (!frame || !rgbFrame) {
        printf("Could not allocate frame memory!\n");
        return -1;
    }

    // Determine required buffer size and allocate buffer
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
    buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer, AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);

    // Initialize SwsContext for conversion
    swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
                                codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,
                                SWS_BILINEAR, NULL, NULL, NULL);
    return 0;
}

// Function to render the decoded frame using OpenGL
void renderFrame() {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, codecContext->width, codecContext->height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbFrame->data[0]);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set up vertices for a fullscreen quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    SDL_GL_SwapWindow(test_window);
}

// Function to decode and display frames
void playVideo() {
    SDL_Event event;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Read a frame
        AVPacket packet;
        if (av_read_frame(formatContext, &packet) < 0) {
            break;
        }

        if (packet.stream_index == videoStreamIndex) {
            int ret = avcodec_send_packet(codecContext, &packet);
            if (ret < 0) {
                printf("Error sending packet for decoding\n");
                continue;
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(codecContext, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    printf("Error during decoding\n");
                    break;
                }

                // Convert to RGB
                sws_scale(swsContext, (const uint8_t* const*)frame->data, frame->linesize, 0, codecContext->height,
                          rgbFrame->data, rgbFrame->linesize);

                // Render the frame using OpenGL
                renderFrame();
            }
        }

        av_packet_unref(&packet);
        SDL_Delay(16); // ~60 FPS
    }
}

// Clean up resources
void cleanup() {
    av_freep(&buffer);
    av_frame_free(&frame);
    av_frame_free(&rgbFrame);
    sws_freeContext(swsContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
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

    playVideo();
    cleanup();

    return 0;
}
