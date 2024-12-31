#ifndef SOUND_HPP
#define SOUND_HPP

#include <memory>
#include <SDL2/SDL_mixer.h>

class GameSound {
public:
    GameSound()
    : m_Music(nullptr, Mix_FreeMusic),
      mBallPlayerCollision(nullptr, Mix_FreeChunk),
      mBallWallCollision(nullptr, Mix_FreeChunk)
    {
        m_Music = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>(Mix_LoadMUS("assets/sound/stranger-things.mp3"), Mix_FreeMusic);
        mBallPlayerCollision = std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>(Mix_LoadWAV("assets/sound/jump.wav"), Mix_FreeChunk);
        mBallWallCollision = std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>(Mix_LoadWAV("assets/sound/impact.wav"), Mix_FreeChunk);

        // Set initial volume (e.g., 50% volume)
        Mix_VolumeMusic(MIX_MAX_VOLUME / 10);
        Mix_VolumeChunk(mBallPlayerCollision.get(), MIX_MAX_VOLUME / 20);
        Mix_VolumeChunk(mBallWallCollision.get(), MIX_MAX_VOLUME / 20);
    }

    void playMusic() {
        Mix_PlayMusic(m_Music.get(), -1);
    }

    void pauseMusic()
    {
        Mix_PauseMusic();
    }

    void playBallPlayerCollision() {
        Mix_PlayChannel(-1, mBallPlayerCollision.get(), 0);
    }

    void playBallWallCollision() {
        Mix_PlayChannel(-1, mBallWallCollision.get(), 0);
    }

    bool getMusicStatus() { return m_MusicStatus; }

private:
    bool m_MusicStatus = false;
    std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> m_Music;
    std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> mBallPlayerCollision;
    std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> mBallWallCollision;
};

#endif // SOUND_HPP
