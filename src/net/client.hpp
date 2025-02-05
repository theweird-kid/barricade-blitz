#include "common.hpp"

class GameClient : public wkd::net::client_interface<GameMsg>
{
public:
    void SendStatus(int playerScore, int enemyScore) {
        wkd::net::message<GameMsg> msg;
        msg.header.id = GameMsg::Server_Message;
        sMessageType msgType;
        std::string message = "PLAYER: " + std::to_string(playerScore) + "\tENEMY: " + std::to_string(enemyScore);
        msgType.copyToServerBuffer(msg, message);
        Send(msg);
    }

    void Connect(const std::string& host, const uint16_t port) {
        std::cout << "Connecting to " << host << ":" << port << std::endl;
        wkd::net::client_interface<GameMsg>::Connect(host, port);
    }
};
