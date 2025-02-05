#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "asio-framework/net_message.hpp"
#include "common.hpp"
#include <string>

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;


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

    void SendPlayerData(int xPos, int yPos) {
        wkd::net::message<GameMsg> msg;
        msg.header.id = GameMsg::Player_Update;
        sMessageType msgType;
        std::string message = "Xpos: " + std::to_string(xPos) + "\tYpos: " + std::to_string(yPos);
        msgType.copyToServerBuffer(msg, message);
        Send(msg);
    }

    void Connect(const std::string& host, const uint16_t port) {
        std::cout << "Connecting to " << host << ":" << port << std::endl;
        wkd::net::client_interface<GameMsg>::Connect(host, port);

    }

    void OnMessage(wkd::net::message<GameMsg>& msg) {
        switch(msg.header.id) {
            case GameMsg::Enemy_Update: {
            // Extract the NetworkMessage from the received message
            sMessageType netMsg;
            msg >> netMsg;

            std::string clientMessage{netMsg.data.begin(), netMsg.data.begin() + netMsg.nSize};
            int tmp_x, tmp_y;
            ParseEnemyUpdateMessage(clientMessage, tmp_x, tmp_y);
            TransformCoordinates(tmp_x, tmp_y);
            std::cout << "Enemy at: Xpos: " << tmp_x << "\tYpos: " << tmp_y << std::endl;
            }
            break;
            default:
                break;
        }
    }

private:
void ParseEnemyUpdateMessage(const std::string& message, int& xPos, int& yPos) {
        // Assuming the message format is "Xpos: <xPos>\tYpos: <yPos>"
        size_t xPosStart = message.find("Xpos: ") + 6;
        size_t xPosEnd = message.find("\t", xPosStart);
        xPos = std::stoi(message.substr(xPosStart, xPosEnd - xPosStart));

        size_t yPosStart = message.find("Ypos: ") + 6;
        yPos = std::stoi(message.substr(yPosStart));
    }

    void TransformCoordinates(int& xPos, int& yPos) {
        // Example transformation: mirror the position horizontally and vertically
        xPos = SCREEN_WIDTH - xPos;
        yPos = SCREEN_HEIGHT - yPos;
    }
};

#endif // CLIENT_HPP
