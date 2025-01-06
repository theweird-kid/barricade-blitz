#ifndef NET_COMMON_HPP
#define NET_COMMON_HPP

#include <enet/enet.h>
#include <cstring>
#include <iostream>
#include <cstdint>

namespace net {

using NetID = int32_t;

// Message sent by client
struct MessageClient {
  NetID ID; // Unique ID of the sender

  int32_t x_player_position; // x coordinate of sender
  int32_t y_player_position; // y coordinate of sender

  int32_t x_player_velocity; // velocity in x dir
  int32_t y_player_velocity; // velocity in y dir

  int32_t PlayerScore; // score of the sender
  int32_t EnemyScore;  // score of the enemy

  // Parameterized Constructor
  MessageClient(NetID id, int32_t x_pos, int32_t y_pos, int32_t x_vel, int32_t y_vel, int32_t p_score, int32_t e_score)
      : ID(id), x_player_position(x_pos), y_player_position(y_pos),
        x_player_velocity(x_vel), y_player_velocity(y_vel),
        PlayerScore(p_score), EnemyScore(e_score)
    {}

  // Default Constructor
  MessageClient()
      : ID(0), x_player_position(0), y_player_position(0), x_player_velocity(0),
        y_player_velocity(0), PlayerScore(0), EnemyScore(0) {}

  void Serialize(ENetPacket *packet) const {
    // Serialize the data
    std::memcpy(packet->data, &ID, sizeof(NetID)); // Copy ID
    std::memcpy(packet->data + sizeof(NetID), &x_player_position,
                sizeof(int32_t)); // Copy x position
    std::memcpy(packet->data + sizeof(NetID) + sizeof(int32_t),
                &y_player_position, sizeof(int32_t)); // Copy y position
    std::memcpy(packet->data + sizeof(NetID) + 2 * sizeof(int32_t),
                &x_player_velocity, sizeof(int32_t)); // Copy x velocity
    std::memcpy(packet->data + sizeof(NetID) + 3 * sizeof(int32_t),
                &y_player_velocity, sizeof(int32_t)); // Copy y velocity
    std::memcpy(packet->data + sizeof(NetID) + 4 * sizeof(int32_t),
                &PlayerScore, sizeof(int32_t)); // Copy Player Score
    std::memcpy(packet->data + sizeof(NetID) + 5 * sizeof(int32_t), &EnemyScore,
                sizeof(int32_t)); // Copy Enemy Score
  }

  void Deserialize(ENetPacket *packet) {
    // Deserialize the data
    memcpy(&ID, packet->data, sizeof(NetID));
    memcpy(&x_player_position, packet->data + sizeof(NetID), sizeof(int32_t));
    memcpy(&y_player_position, packet->data + sizeof(NetID) + sizeof(int32_t),
           sizeof(int32_t));
    memcpy(&x_player_velocity,
           packet->data + sizeof(NetID) + 2 * sizeof(int32_t), sizeof(int32_t));
    memcpy(&y_player_velocity,
           packet->data + sizeof(NetID) + 3 * sizeof(int32_t), sizeof(int32_t));
    memcpy(&PlayerScore, packet->data + sizeof(NetID) + 4 * sizeof(int32_t),
           sizeof(int32_t));
    memcpy(&EnemyScore, packet->data + sizeof(NetID) + 5 * sizeof(int32_t),
           sizeof(int32_t));
  }
};

// Message sent by server
struct MessageServer {
  int32_t x_enemy_position; // Enemy Position
  int32_t y_enemy_position;

  int32_t x_enemy_velocity; // Enemy Velocity
  int32_t y_enemy_velocity;

  int32_t PlayerScore; // Scores
  int32_t EnemyScore;

  void Serialize(ENetPacket *packet) const {
    memcpy(packet->data, &x_enemy_position, sizeof(int32_t));
    memcpy(packet->data + sizeof(int32_t), &y_enemy_position, sizeof(int32_t));
    memcpy(packet->data + 2 * sizeof(int32_t), &x_enemy_velocity, sizeof(int32_t));
    memcpy(packet->data + 3 * sizeof(int32_t), &y_enemy_velocity, sizeof(int32_t));
    memcpy(packet->data + 4 * sizeof(int32_t), &PlayerScore, sizeof(int32_t));
    memcpy(packet->data + 5 * sizeof(int32_t), &EnemyScore, sizeof(int32_t));
  }

  void Deserialize(const ENetPacket *packet) {
    memcpy(&x_enemy_position, packet->data, sizeof(int32_t));
    memcpy(&y_enemy_position, packet->data + sizeof(int32_t), sizeof(int32_t));
    memcpy(&x_enemy_velocity, packet->data + 2 * sizeof(int32_t), sizeof(int32_t));
    memcpy(&y_enemy_velocity, packet->data + 3 * sizeof(int32_t), sizeof(int32_t));
    memcpy(&PlayerScore, packet->data + 4 * sizeof(int32_t), sizeof(int32_t));
    memcpy(&EnemyScore, packet->data + 5 * sizeof(int32_t), sizeof(int32_t));
  }
};

// Client State on the server
struct ClientState {
  NetID ID; // Unique ID of the client
  ENetPeer *peer; // Peer of the client
  MessageClient lastMessage; // Last message sent by the client

  int x_position; // x coordinate of sender
  int y_position; // y coordinate of sender

  int x_velocity; // velocity in x dir
  int y_velocity; // velocity in y dir

  int ClientScore; // score of the sender

  // Parameterized Constructor
  ClientState(NetID id, ENetPeer *p, const MessageClient &msg)
      : ID(id), peer(p), lastMessage(msg) {}

  // Default Constructor
  ClientState() : ID(0), peer(nullptr), lastMessage() {}
};

} // namespace net

#endif // NET_COMMON_HPP
