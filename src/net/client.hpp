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
    void SendScore(int playerScore, int enemyScore) {
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

    void SendBallData(float xPos, float yPox, float xVel, float yVel) {
        wkd::net::message<GameMsg> msg;
        msg.header.id = GameMsg::Ball_Update;
        sMessageType msgType;
        std::string message = "Xpos: " + std::to_string(xPos) + "\tYpos: " + std::to_string(yPox) + "\tXvel: " + std::to_string(xVel) + "\tYvel: " + std::to_string(yVel);
        msgType.copyToServerBuffer(msg, message);
        Send(msg);
    }

    void SendReset() {
        wkd::net::message<GameMsg> msg;
        msg.header.id = GameMsg::Reset_Game;
        Send(msg);
    }

    void Connect(const std::string& host, const uint16_t port) {
        std::cout << "Connecting to " << host << ":" << port << std::endl;
        wkd::net::client_interface<GameMsg>::Connect(host, port);

    }

    InternalMessageType OnMessage(wkd::net::message<GameMsg>& msg) {
        switch(msg.header.id) {
            case GameMsg::Score_Update: {
                // Extract the NetworkMessage from the received message
                sMessageType netMsg;
                msg >> netMsg;

                std::string clientMessage{netMsg.data.begin(), netMsg.data.begin() + netMsg.nSize};
                return {GameMsg::Score_Update, clientMessage};
            }
            break;
            case GameMsg::Enemy_Update: {
                // Extract the NetworkMessage from the received message
                sMessageType netMsg;
                msg >> netMsg;

                std::string clientMessage{netMsg.data.begin(), netMsg.data.begin() + netMsg.nSize};
                return {GameMsg::Enemy_Update, clientMessage};
            }
            break;
            case GameMsg::Ball_Update: {
                // Extract the NetworkMessage from the received message
                sMessageType netMsg;
                msg >> netMsg;

                std::string clientMessage{netMsg.data.begin(), netMsg.data.begin() + netMsg.nSize};
                return {GameMsg::Ball_Update, clientMessage};
            }
            break;
            case GameMsg::Reset_Game: {
                return {GameMsg::Reset_Game, "Reset Game"};
            }
            default:
                return {GameMsg::Server_Message, "Unknown message received from server."};
                break;
        }
    }

private:
};

#endif // CLIENT_HPP
