#ifndef NET_CLIENT_HPP
#define NET_CLIENT_HPP

#include "./net_common.hpp"
#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace net
{
    class Client
    {
        static const int MAX_CHANNELS = 2;
    public:
        Client()
        {
            if(enet_initialize() != 0)
            {
                std::cerr << "Failed to initialize ENET\n";
                exit(EXIT_FAILURE);
            }
            atexit(enet_deinitialize);
        }

        ~Client()
        {
            if (client != nullptr)
            {
                enet_host_destroy(client);
            }
        }

        bool connect(const char* ip, int port)
        {
            enet_address_set_host(&address, ip);
            address.port = port;

            client = enet_host_create(nullptr, 1, MAX_CHANNELS, 0, 0);
            if(client == nullptr)
            {
                std::cerr << "An error occurred while creating client\n";
                return false;
            }

            peer = enet_host_connect(client, &address, MAX_CHANNELS, 0);
            if(peer == nullptr)
            {
                std::cerr << "No available peers for initiating an ENet connection\n";
                return false;
            }

            if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
            {
                std::cout << "Connection to " << ip << ":" << port << " succeeded.\n";
                return true;
            }
            else
            {
                std::cerr << "Connection to " << ip << ":" << port << " failed.\n";
                enet_peer_reset(peer);
                return false;
            }
        }

        void run()
        {
            while (true)
            {
                while(enet_host_service(client, &event, 10000) > 0)
                {
                    switch(event.type)
                    {
                        case ENET_EVENT_TYPE_CONNECT:
                            std::cout << "Connected to server\n";
                            break;
                        case ENET_EVENT_TYPE_RECEIVE:
                            onMessageReceive(event.packet);
                            enet_packet_destroy(event.packet);
                            break;
                        case ENET_EVENT_TYPE_DISCONNECT:
                            std::cout << "Disconnected from server\n";
                            return;
                        default:
                            break;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        void sendMessage(const MessageClient& msg)
        {
            ENetPacket* packet = enet_packet_create(&msg, sizeof(MessageClient), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, packet);
            enet_host_flush(client);
        }

    private:
        void onMessageReceive(ENetPacket* packet)
        {
            MessageServer message;
            message.Deserialize(packet);

            // Update the local game state
            updateLocalGameState(message);

            // Print the game state
            std::cout << "Game State Updated: " << std::endl;
            std::cout << "Enemy Position: (" << message.x_enemy_position << ", " << message.y_enemy_position << ")" << std::endl;
            std::cout << "Enemy Velocity: (" << message.x_enemy_velocity << ", " << message.y_enemy_velocity << ")" << std::endl;
            std::cout << "Player Score: " << message.PlayerScore << std::endl;
            std::cout << "Enemy Score: " << message.EnemyScore << std::endl;
        }

        void updateLocalGameState(const MessageServer& message)
        {

        }

        ENetAddress address;
        ENetHost* client = nullptr;
        ENetEvent event;
        ENetPeer* peer = nullptr;

        NetID ID;
    };
}

#endif // NET_CLIENT_HPP
