#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include "./net_common.hpp"
#include <enet/enet.h>
#include <vector>

namespace net
{
    class Server
    {
        static const int MAX_CLIENTS = 2;
        static const int MAX_CHANNELS = 2;
        static const int MAX_CLIENTS_PER_CHANNEL = 1;

    public:
        Server()
        {
            if(enet_initialize() != 0)
            {
                std::cerr << "Failed to initialize ENET\n";
            }
            atexit(enet_deinitialize);

        }

        ~Server()
        {
            enet_host_destroy(server);
        }

        void run()
        {
            server = enet_host_create(&address, MAX_CLIENTS, MAX_CHANNELS, 0, 0);
            if(server == nullptr)
            {
                std::cerr << "An error occured while creating server\n";
            }

            while(enet_host_service(server, &event, 1000) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                        clientConnected(event.peer);
                        break;
                    case ENET_EVENT_TYPE_RECEIVE:
                        onMessageReceive(event.peer, event.packet);
                        break;
                    case ENET_EVENT_TYPE_DISCONNECT:
                        clientDisconnected(event.peer);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        void onMessageReceive(ENetPeer* peer, ENetPacket* packet)
        {
            MessageClient message;
            message.Deserialize(packet);

            // Print the message
            std::cout << "A packet of length " << event.packet->dataLength
            << " containing " << event.packet->data
            << " was recieved from " << event.peer->data
            << " on channel " << event.channelID << std::endl;

            std::cout << "Message from " << peer->data << std::endl;

            // If any Players score reaches 20 then send a message to other clients and close the connection

            // Fill the message buffer
            MessageServer msg;
            // Remember to invert player and enemy positions,velocities and scores
            msg.x_enemy_position = message.x_player_position;
            msg.y_enemy_position = message.y_player_position;
            msg.x_enemy_velocity = message.x_player_velocity;
            msg.x_enemy_velocity = message.y_player_velocity;
            msg.PlayerScore = message.EnemyScore;
            msg.EnemyScore = message.PlayerScore;

            // Send the message to other clients
            sendMessageToClients(msg, message.ID);

            // Check if any player has reached 20, if yes then close the connection
            if(message.PlayerScore == 20 || message.EnemyScore == 20)
            {
                // Close the connection
                enet_peer_disconnect(peer, 0);
                clientDisconnected(peer);
            }
        }

        void sendMessageToClients(MessageServer& msg, NetID notSend = -1)
        {
            ENetPacket* packet = enet_packet_create(&msg, sizeof(MessageServer), ENET_PACKET_FLAG_RELIABLE);
            for(auto& client : clients)
            {
                if(client.ID != notSend)
                {
                    enet_peer_send(client.peer, 0, packet);
                }
            }
        }

        void clientConnected(ENetPeer* peer)
        {
            std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
            ClientState client_state;
            client_state.ID = ++COUNTER;
            client_state.peer = peer;
            client_state.lastMessage = MessageClient();
            client_state.x_position = 0; client_state.y_position = 0;
            client_state.x_velocity = 0; client_state.y_velocity = 0;

            clients.push_back(client_state);
        }

        void clientDisconnected(ENetPeer* peer)
        {
            std::cout << event.peer->data << " disconnected\n";
            event.peer->data = nullptr;
            clients.erase(std::__remove_if(clients.begin(), clients.end(),
                [peer](const ClientState& c) { return c.peer == peer; }),
                clients.end());
        }


    private:
        ENetHost* server;
        ENetAddress address;
        ENetEvent event;

        // client counter
        int COUNTER = 0;

        // Maintain a list of connected clients and state
        std::vector<ClientState> clients;
    };
}

# endif // NET_SERVER_HPP
