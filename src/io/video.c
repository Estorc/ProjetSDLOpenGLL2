#include <raptiquax.h>
#include <window.h>
#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <stdio.h>
#include <stdlib.h>

#define AUDIO_BUFFER_SIZE 4096

// FFmpeg-related variables
static AVFormatContext* formatContext = NULL;
static AVCodecContext* codecContext = NULL;
static AVCodecContext* audioCodecContext = NULL;
static AVCodec* codec = NULL;
static AVCodec* audioCodec = NULL;
static AVFrame* frame = NULL;
static AVFrame* rgbFrame = NULL;
static uint8_t* buffer = NULL;
static struct SwsContext* swsContext = NULL;
static int videoStreamIndex = -1;
static int audioStreamIndex = -1;

static SDL_mutex* sdl_mutex;
static SDL_AudioDeviceID audioDevice;

static uint8_t *audioBuffer = NULL;
static int audioBufferSize = 0;
static int quit = 0;

AVPacket packet;
int packet_state = -1;


int init_video_player() {
    // Initialize SDL audio
    SDL_AudioSpec wantedSpec;
    SDL_zero(wantedSpec);
    wantedSpec.freq = 48000; // Sample rate
    wantedSpec.format = AUDIO_F32; // Use float format for audio
    wantedSpec.channels = 1;
    wantedSpec.samples = AUDIO_BUFFER_SIZE;
    wantedSpec.callback = NULL; // We will push audio manually

    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wantedSpec, NULL, 0);
    return audioDevice;
}


void free_video() {
    // Free resources
    av_freep(&buffer);
    av_frame_free(&frame);
    av_frame_free(&rgbFrame);
    sws_freeContext(swsContext);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}

void free_video_player() {
    SDL_CloseAudioDevice(audioDevice); // Close the audio device
}


// Function to load the MP4 file using FFmpeg
int load_video(const char* filename) {
    //av_register_all(); DEPRECATED
    avformat_network_init();

    formatContext = avformat_alloc_context();

    // Open video file
    if (avformat_open_input(&formatContext, filename, NULL, NULL) != 0) {
        PRINT_WARNING("Could not open video file: %s\n", filename);
        return -1;
    }

    // Find video stream
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        PRINT_WARNING("Could not find stream information\n");
        return -1;
    }

    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex == -1) {
            videoStreamIndex = i;
        }
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex == -1) {
            audioStreamIndex = i;
        }
    }

    if (videoStreamIndex == -1) {
        PRINT_WARNING("No video stream found!\n");
        return -1;
    }

    if (audioStreamIndex == -1) {
        PRINT_WARNING("No audio stream found!\n");
        return -1;
    }


    // Audio codec context
    audioCodecContext = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(audioCodecContext, formatContext->streams[audioStreamIndex]->codecpar);
    audioCodec = avcodec_find_decoder(audioCodecContext->codec_id);
    if (!audioCodec || avcodec_open2(audioCodecContext, audioCodec, NULL) < 0) {
        PRINT_ERROR("Failed to open audio codec\n");
        exit(1);
    }

    // Get codec
    codecContext = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar);
    codec = avcodec_find_decoder(codecContext->codec_id);
    if (!codec) {
        PRINT_WARNING("Codec not found!\n");
        return -1;
    }

    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        PRINT_WARNING("Could not open codec!\n");
        return -1;
    }

    // Allocate frames
    frame = av_frame_alloc();
    rgbFrame = av_frame_alloc();
    if (!frame || !rgbFrame) {
        PRINT_WARNING("Could not allocate frame memory!\n");
        return -1;
    }

    // Determine required buffer size and allocate buffer
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
    buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer, AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);

    // Initialize SwsContext for conversion
    swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
                                codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,
                                SWS_FAST_BILINEAR, NULL, NULL, NULL);
    if (!swsContext) {
        PRINT_ERROR("Could not initialize the conversion context\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}


// Function to render the decoded frame using OpenGL
void renderFrame(GLuint textureID) {
    if (quit == -1) return; // Check if quit flag is set
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, codecContext->width, codecContext->height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbFrame->data[0]);
}

int play_video() {
    if (packet_state < 0 && av_read_frame(formatContext, &packet) < 0)
        return -1;
    return 0;
}


void get_last_frame(GLuint textureID) {
    renderFrame(textureID);
}


int get_video_audio(int64_t start_time, GLuint textureID) {

    if (packet.stream_index == audioStreamIndex) {
        // Handle audio decoding here
        if (packet_state < 0)
            packet_state = avcodec_send_packet(audioCodecContext, &packet);
        if (packet_state < 0)
            return 0;

        while (packet_state >= 0) {
            packet_state = avcodec_receive_frame(audioCodecContext, frame);
            if (packet_state == AVERROR(EAGAIN) || packet_state == AVERROR_EOF) {
                packet_state = -1;
                break;
            }

            int dataSize = av_samples_get_buffer_size(NULL, 1, frame->nb_samples, audioCodecContext->sample_fmt, 1);
            if (dataSize > 0) {
                if (audioBufferSize < dataSize) {
                    audioBuffer = realloc(audioBuffer, dataSize);
                    audioBufferSize = dataSize;
                }

                memcpy(audioBuffer, frame->data[0], dataSize);

                // Play audio
                SDL_QueueAudio(audioDevice, audioBuffer, dataSize);
                SDL_PauseAudioDevice(audioDevice, 0); // Start playing audio
            }
        }
    }
    return 0;
}

// Function to decode and display frames
int get_video_image(int64_t start_time, GLuint textureID) {

    // Separate timing variables for audio/video synchronization
    int update_image = 0;

    if (packet.stream_index == videoStreamIndex) {
        if (packet_state < 0)
            packet_state = avcodec_send_packet(codecContext, &packet);
        if (packet_state < 0)
            return 0;

        if (packet_state >= 0) {
            packet_state = avcodec_receive_frame(codecContext, frame);
            if (packet_state == AVERROR(EAGAIN) || packet_state == AVERROR_EOF) {
                packet_state = -1;
                return 0;
            }

            // Convert to RGB
            sws_scale(swsContext, (const uint8_t* const*)frame->data, frame->linesize, 0,
                        codecContext->height, rgbFrame->data, rgbFrame->linesize);

            // 🕓 Wait until it's time to show this frame
            AVRational time_base = formatContext->streams[videoStreamIndex]->time_base;
            double timestamp_ms = av_q2d(time_base) * frame->pts * 1000.0;

            // Sync video display with its timestamp
            int64_t now = SDL_GetTicks() - start_time;

            if (timestamp_ms > now) {
                SDL_Delay(timestamp_ms - now);
            }

            // Render frame
            renderFrame(textureID);
            update_image = 1;

        }
    }
    return update_image;
}

int play_video_loop(void *args) {
    void **args_list = args;
    GLuint *textureID = args_list[0]; // Get the texture ID from args
    void (*callback)() = args_list[1]; // Get the callback function from args
    free(args); // Free the args list
    quit = 0;
    int64_t start_time = SDL_GetTicks(); // Get the start time
    while (quit != -1) {
        if (play_video() == -1) quit = -1; // Call play_video to decode and render frames
        SDL_LockMutex(sdl_mutex); // Lock the mutex before accessing shared resources
        get_video_image(start_time, *textureID);
        SDL_UnlockMutex(sdl_mutex); // Unlock the mutex after accessing shared resources
        get_video_audio(start_time, *textureID); // Pass the start time to playVideo
        flush_video();
    }
    free_video(); // Free video resources
    SDL_DestroyMutex(sdl_mutex); // Destroy the mutex
    if (callback) callback(); // Call the callback function after video playback
    return 0;
}

void stop_video() {
    quit = -1; // Set quit flag to stop the video thread
}

int play_video_async(GLuint *textureID, void (*callback)()) {
    // Initialize SDL mutex
    sdl_mutex = SDL_CreateMutex();
    if (!sdl_mutex) {
        PRINT_ERROR("Failed to create SDL mutex: %s\n", SDL_GetError());
        return -1;
    }
    // Create a thread to handle video playback
    void **args = malloc(2 * sizeof(void*));
    if (!args) {
        PRINT_ERROR("Failed to allocate memory for thread arguments\n");
        return -1;
    }
    args[0] = textureID; // Pass the texture ID to the thread
    args[1] = callback; // Pass the callback function to the thread
    SDL_Thread* videoThread = SDL_CreateThread(play_video_loop, "VideoThread", args);
    if (!videoThread) {
        PRINT_ERROR("Failed to create video thread: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

int flush_video() {
    if (packet_state < 0) {
        packet_state = -1;
        av_packet_unref(&packet);
    }
    return 0;
}