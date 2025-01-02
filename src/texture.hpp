#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <memory>

class Texture {
public:
    // Constructor for images
    Texture(const std::string& path, SDL_Renderer* renderer, int width, int height)
    : m_Texture(nullptr, SDL_DestroyTexture),
      m_Renderer(renderer),
      m_Width(width), m_Height(height)
    {
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());
        if(surface == nullptr) {
            std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Set the color key for the image
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

        // Create a texture from the surface
        m_Texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);

        // Free the surface
        SDL_FreeSurface(surface);
    }

    // Constructor Overload for text
    Texture(const std::string& textString, SDL_Renderer* renderer)
    : m_Texture(nullptr, SDL_DestroyTexture),
      m_Renderer(renderer)
    {
        // Load Fonts
        TTF_Font* m_Font = TTF_OpenFont("assets/fonts/JetBrainsMono-Bold.ttf", 24);
        if(m_Font == nullptr)
        {
            std::cerr << "[ERROR]: TTF_OpenFont() Failed! SDL_ttf Error: " << TTF_GetError() << std::endl;
        }

        SDL_Color textColor = {255, 255, 255};   // White Text Color

        SDL_Surface* textSurface = TTF_RenderText_Solid(m_Font, textString.c_str(), textColor);
        if(textSurface == nullptr)
        {
            std::cerr << "Unable to load textSurface! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Create texture
        m_Texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>
            (SDL_CreateTextureFromSurface(m_Renderer, textSurface), SDL_DestroyTexture);

        // width and height
        SDL_QueryTexture(m_Texture.get(), nullptr, nullptr, &m_Width, &m_Height);

        // Free the surface
        SDL_FreeSurface(textSurface);
    }

    // Update text [ONLY for Text Surfaces]
    void updateText(const std::string& newTextString)
    {
        // Load Fonts
        TTF_Font* m_Font = TTF_OpenFont("assets/fonts/JetBrainsMono-Bold.ttf", 24);
        if(m_Font == nullptr)
        {
            std::cerr << "[ERROR]: TTF_OpenFont() Failed! SDL_ttf Error: " << TTF_GetError() << std::endl;
        }

        // 1. Create new surface
        SDL_Color textColor = {255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(m_Font, newTextString.c_str(), textColor);
        if (textSurface == nullptr)
        {
            if(m_Font == nullptr)std::cout << "Meow\n";
            std::cerr << "Unable to load textSurface! SDL_Error: " << TTF_GetError() << std::endl;
            return;
        }

        // 2. Update existing texture
        if (SDL_UpdateTexture(m_Texture.get(), NULL, textSurface->pixels, textSurface->pitch) < 0)
        {
           std::cerr << "Unable to update texture! SDL_Error: " << SDL_GetError() << std::endl;
        }

        // width and height
        SDL_QueryTexture(m_Texture.get(), nullptr, nullptr, &m_Width, &m_Height);

        // 3. Free the surface
        SDL_FreeSurface(textSurface);
    }

    ~Texture()
    {
        // Free Font
    }

    // Draw to Screen
    void render(SDL_Renderer* renderer, int x, int y) {
        SDL_Rect dest = {x, y, m_Width, m_Height};
        SDL_RenderCopy(renderer, m_Texture.get(), nullptr, &dest);
    }

    // Getters
    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }
    // Setters
    void setWidth(int w) { m_Width = w; }
    void setHeight(int h) { m_Height = h; }

private:
    // Texture
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_Texture;

    // Refrence to Renderer
    SDL_Renderer* m_Renderer = nullptr;

    // Texture Width and Height
    int m_Width, m_Height;
    // Text font (Only Applicaple for text)
    TTF_Font* m_Font = nullptr;
};

#endif // TEXTURE_HPP
