#include <raptiquax.h>
#include <memory.h>
#include <settings.h>
#include <io/audio.h>

static Mix_Music * current_music = NULL;
static int current_music_volume = 0;

Mix_Chunk * play_sfx(const char * const path, const int volume, const int loops) {
    Mix_Chunk *chunk = get_mix_chunk_from_cache(path);
    if (!chunk) {
        chunk = Mix_LoadWAV(path);
        add_mix_chunk_to_cache(path, chunk);
    }
    if (!chunk) {
        PRINT_ERROR("Failed to load audio file %s\n", path);
        return NULL;
    }
    Mix_VolumeChunk(chunk, volume * Game.settings->master_volume * Game.settings->sfx_volume);
    Mix_PlayChannel(-1, chunk, loops);
    return chunk;
}

void play_music(const char * const path, const int volume, const int loops) {
    Mix_Music * music = get_mix_music_from_cache(path);
    if (!music) {
        music = Mix_LoadMUS(path);
        add_mix_music_to_cache(path, music);
    }
    if (!music) {
        PRINT_ERROR("Failed to load audio file %s\n", path);
        return;
    }

    current_music_volume = volume;
    refresh_music_volume();
    if (current_music == music) return;
    current_music = music;

    Mix_PlayMusic(current_music, loops);
}

void set_sfx_volume(Mix_Chunk * const channel, const int volume) {
    Mix_VolumeChunk(channel, volume * Game.settings->master_volume * Game.settings->sfx_volume);
}

void refresh_music_volume() {
    Mix_VolumeMusic(current_music_volume * Game.settings->master_volume * Game.settings->music_volume);
}

void fade_out_music(const int ms) {
    Mix_FadeOutMusic(ms);
    current_music = NULL;
}