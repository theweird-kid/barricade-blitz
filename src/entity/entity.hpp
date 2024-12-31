#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../texture.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
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

    Entity(const std::string& path, SDL_Renderer* renderer, int x, int y, int w, int h, std::pair<int,int> s, enum Type t)
    : m_Texture(nullptr),
      m_Xpos(x), m_Ypos(y),
      m_Xspeed(s.first), m_Yspeed(s.second),
      m_Alive(true),
      m_Type(t)
    {
        m_Texture = std::make_unique<Texture>(path, renderer, w, h);
    }

    ~Entity() {}

    void handleEvent(SDL_Event& event);
    void render(SDL_Renderer* renderer);

    // delta time
    float delataTime = 1.0f;

    // Setters
    void setX(int x) { m_Xpos = x; }
    void setY(int y) { m_Ypos = y; }
    void setWidth(int w) { m_Texture->setWidth(w); }
    void setHeight(int h) { m_Texture->setHeight(h); }
    void setSpeed(std::pair<int,int> s) { m_Xspeed = s.first; m_Yspeed = s.second; }
    void setAlive(bool a) { m_Alive = a; }

    // Getters
    enum Type getType() { return m_Type; }
    int getX() { return m_Xpos; }
    int getY() { return m_Ypos; }
    int getWidth() { return m_Texture->getWidth(); }
    int getHeight() { return m_Texture->getHeight(); }
    std::pair<int,int> getSpeed() { return {m_Xspeed, m_Yspeed}; }
    bool isAlive() { return m_Alive; }

private:

    std::unique_ptr<Texture> m_Texture;
    enum Type m_Type;
    float m_Xpos, m_Ypos;
    float m_Xspeed, m_Yspeed;
    bool m_Alive;
};

\

#endif // ENTITY_HPP
