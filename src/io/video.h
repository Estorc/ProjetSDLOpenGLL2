#pragma once

int init_video_player();
int load_video(const char* filename);
int play_video();
int get_video_audio(int64_t start_time, GLuint textureID);
int get_video_image(int64_t start_time, GLuint textureID);
int flush_video();
void free_video();
void free_video_player();
void stop_video();
int async_video(GLuint *textureID, void (*callback)());
void get_last_frame(GLuint textureID);