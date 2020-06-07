#include <SDL2/SDL_image.h>
#include <iostream>

#include "./AssetManager.h"

namespace bomberman
{
    void AssetManager::load(SDL_Renderer* renderer)
    {
        // load font
        loadFont();
        // load textures
        loadTexture(renderer, Texture::MenuBack, "app0:assets/menu_title.png");
        loadTexture(renderer, Texture::Stone, "app0:assets/stone.png");
        loadTexture(renderer, Texture::Grass, "app0:assets/grass.png");
        loadTexture(renderer, Texture::Brick, "app0:assets/brick.png");
        loadTexture(renderer, Texture::Player, "app0:assets/player.png");
        loadTexture(renderer, Texture::Enemy1, "app0:assets/enemy_1.png");
        loadTexture(renderer, Texture::Enemy2, "app0:assets/enemy_2.png");
        loadTexture(renderer, Texture::Enemy3, "app0:assets/enemy_3.png");
        loadTexture(renderer, Texture::Bomb, "app0:assets/bomb.png");
        loadTexture(renderer, Texture::Explosion, "app0:assets/explosion.png");
        loadTexture(renderer, Texture::Door, "app0:assets/door.png");
        // load music
        loadMusic(MusicEnum::MainMenu, "app0:assets/main_theme.ogg");
        loadMusic(MusicEnum::Level, "app0:assets/level.ogg");
        // load sounds
        loadSound(SoundEnum::Win, "app0:assets/win.wav");
        loadSound(SoundEnum::Lose, "app0:assets/lose.wav");
        loadSound(SoundEnum::Explosion, "app0:assets/explosion.wav");
        if (auto explosionSound = getSound(SoundEnum::Explosion))
        {
            Mix_VolumeChunk(explosionSound.get(), 35);
        }
    }

    std::shared_ptr<TTF_Font> AssetManager::getFont() const
    {
        return font;
    }

    std::shared_ptr<SDL_Texture> AssetManager::getTexture(Texture texture)
    {
        return textures[texture];
    }

    std::shared_ptr<Mix_Music> AssetManager::getMusic(MusicEnum music)
    {
        return musics[music];
    }

    std::shared_ptr<Mix_Chunk> AssetManager::getSound(SoundEnum sound)
    {
        return sounds[sound];
    }

    void AssetManager::loadFont()
    {
        // load font
        font = std::shared_ptr<TTF_Font>(TTF_OpenFont("app0:assets/font.ttf", 32), TTF_CloseFont);
        if(!font)
        {
            std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        }
    }

    void AssetManager::loadTexture(SDL_Renderer* renderer, Texture texture, const std::string& filePath)
    {
        textures[texture] =
            std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, filePath.c_str()), SDL_DestroyTexture);
        if(!textures[texture])
        {
            std::cout << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
        }
    }

    void AssetManager::loadMusic(MusicEnum music, const std::string& filePath)
    {
        musics[music] = std::shared_ptr<Mix_Music>(Mix_LoadMUS(filePath.c_str()), Mix_FreeMusic);
        if(!musics[music])
        {
            std::cout << "loadMusic Error: " << Mix_GetError() << std::endl;
        }
    }

    void AssetManager::loadSound(SoundEnum sound, const std::string& filePath)
    {
        sounds[sound] = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV(filePath.c_str()), Mix_FreeChunk);
        if(!sounds[sound])
        {
            std::cout << "loadSound Error: " << Mix_GetError() << std::endl;
        }
    }
} // namespace bomberman
