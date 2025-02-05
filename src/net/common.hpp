#include "./asio-framework/net.hpp"
#include <array>
#include <cstdint>
#include <vector>

enum class GameMsg : uint32_t
{
    Server_Accept,
    Server_Deny,
    Server_Ping,
    Message_All,
    Server_Message,

    Player_Update,
    Enemy_Update,
};

/*

    Message Body For Player_Update
    Player_X, Player_Y, Player_Score, Enemy_Score
    Player_Velocity_X, Player_Velocity_Y
*/

enum class MessageType
{
    Text,
    Binary,
    Command,
};

struct sMessageType
{
    uint32_t nID;
    uint32_t nSize;
    uint32_t nType;
    std::array<uint8_t, 1024> data;

    void copyToServerBuffer(wkd::net::message<GameMsg>& msg, const std::string& message) {
        this->nSize = message.size();
        this->nType = 1;
        std::memcpy(data.data(), message.data(), message.size());
        msg << *this;
    }
};
