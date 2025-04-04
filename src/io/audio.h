#pragma once

/**
 * @file audio.h
 * @brief Audio playback functions for sound effects and music.
 *
 * This header file contains declarations for functions that handle
 * the playback of sound effects and music in the application.
 */

 /**
 * @brief Plays a sound effect.
 *
 * This function loads and plays a sound effect from the specified path.
 *
 * @param path The file path to the sound effect file. It should be a null-terminated string.
 * @param volume The volume level at which to play the sound effect. The value should be within the acceptable range for the audio system.
 * @param loops The number of times the sound effect should loop. If loops is -1, the sound effect will loop indefinitely.
 * 
 * @return A pointer to the Mix_Chunk structure representing the sound effect.
 * @note The function will automatically cache the sound effect for future use.
 */
Mix_Chunk * play_sfx(const char * const path, const int volume, const int loops);

/**
 * @brief Plays a music file.
 *
 * This function loads and plays a music file from the specified path.
 *
 * @param path The file path to the music file. It should be a null-terminated string.
 * @param volume The volume level at which to play the music. The value should be within the acceptable range for the audio system.
 * @param loops The number of times the music should loop. If loops is -1, the music will loop indefinitely.
 */
void play_music(const char * const path, const int volume, const int loops);

/**
 * @brief Fades out the currently playing music.
 *
 * This function gradually reduces the volume of the currently playing music
 * over the specified duration until it is completely silent.
 *
 * @param ms The duration of the fade-out effect, in milliseconds.
 */
void fade_out_music(const int ms);

/**
 * @brief Refreshes the music volume.
 *
 * This function updates the volume of the currently playing music to match
 * the current volume settings of the audio system.
 */
void refresh_music_volume();

/**
 * @brief Sets the volume of a sound effect.
 *
 * This function sets the volume of a specific sound effect channel.
 *
 * @param chunk The sound effect channel to set the volume for. This should be a valid Mix_Chunk pointer.
 * @param volume The volume level to set for the sound effect. The value should be within the acceptable range for the audio system.
 */
void set_sfx_volume(Mix_Chunk * const chunk, const int volume);