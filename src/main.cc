#include "./game/game.hpp"

int main(int argc, char* argv[])
{
    Game game;
    game.addEntity("assets/sample.bmp", Entity::Type::PLAYER);
    game.addEntity("assets/sample.bmp", Entity::Type::ENEMY);
    game.addEntity("assets/ball.bmp", Entity::Type::BALL);
    game.run();
    return 0;
}
