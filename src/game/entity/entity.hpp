#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../texture.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <memory>
#include <iostream>

class Entity {
public:

    enum class Type {
        PLAYER,
        ENEMY,
        BALL
    };

    Entity(const std::string& path, SDL_Renderer* renderer, int x, int y, int w, int h, int s, enum Type t)
    : m_Texture(nullptr),
      m_Xpos(x), m_Ypos(y),
      m_Speed(s), m_Alive(true),
      m_Type(t)
    {
        m_Texture = std::make_unique<Texture>(path, renderer, w, h);
    }

    ~Entity() {}

    void update();
    void render(SDL_Renderer* renderer);

    // Setters
    void setX(int x) { m_Xpos = x; }
    void setY(int y) { m_Ypos = y; }
    void setWidth(int w) { m_Texture->setWidth(w); }
    void setHeight(int h) { m_Texture->setHeight(h); }
    void setSpeed(int s) { m_Speed = s; }
    void setAlive(bool a) { m_Alive = a; }

    // Getters
    int getX() { return m_Xpos; }
    int getY() { return m_Ypos; }
    int getWidth() { return m_Texture->getWidth(); }
    int getHeight() { return m_Texture->getHeight(); }
    int getSpeed() { return m_Speed; }
    bool isAlive() { return m_Alive; }

private:

    std::unique_ptr<Texture> m_Texture;
    enum Type m_Type;
    int m_Xpos, m_Ypos;
    int m_Speed;
    bool m_Alive;
};

\

#endif // ENTITY_HPP
