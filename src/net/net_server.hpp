#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include "./net_common.hpp"
#include <enet/enet.h>
#include <algorithm>
#include <thread>
#include <vector>
#include <unordered_map>

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

            // Ip and port
            enet_address_set_host(&address, "127.0.0.1");
            address.port = 7777;
            COUNTER = 0;
            ROOM_COUNTER = 0;
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

            // Run the server
            std::cout << "Server started\n";
            while(true)
            {
                while(enet_host_service(server, &event, 50000) > 0)
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
        }

    private:
        void onMessageReceive(ENetPeer* peer, ENetPacket* packet)
        {
            MessageClient message;
            message.Deserialize(packet);

            // Print the message
            std::cout << "A packet of length " << event.packet->dataLength
            << " containing " << event.packet->data
            << " was received from " << event.peer->data
            << " on channel " << event.channelID << std::endl;

            std::cout << "Message from " << peer->data << std::endl;

            // If any Players score reaches 20 then send a message to other clients and close the connection

            // Fill the message buffer
            MessageServer msg;
            // Remember to invert player and enemy positions, velocities and scores
            msg.x_enemy_position = message.x_player_position;
            msg.y_enemy_position = message.y_player_position;
            msg.x_enemy_velocity = message.x_player_velocity;
            msg.y_enemy_velocity = message.y_player_velocity;
            msg.PlayerScore = message.EnemyScore;
            msg.EnemyScore = message.PlayerScore;

            // Send the message to other clients in the same room
            sendMessageToClients(msg, message.ID, peer);

            // Check if any player has reached 20, if yes then close the connection
            if(message.PlayerScore == 20 || message.EnemyScore == 20)
            {
                // Close the connection
                enet_peer_disconnect(peer, 0);
                clientDisconnected(peer);
            }
        }

        void sendMessageToClients(MessageServer& msg, NetID notSend, ENetPeer* peer)
        {
            int roomID = peerToRoom[peer];
            for(auto& client : rooms[roomID].clients)
            {
                if(client.ID != notSend)
                {
                    msg.ID = client.ID;
                    ENetPacket* packet = enet_packet_create(&msg, sizeof(MessageServer), ENET_PACKET_FLAG_RELIABLE);
                    enet_peer_send(client.peer, 0, packet);
                }
            }
        }

        void clientConnected(ENetPeer* peer)
        {
            std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
            ClientState client_state;
            COUNTER += 1;
            client_state.ID = COUNTER;
            client_state.peer = peer;
            client_state.lastMessage = MessageClient();
            client_state.x_position = 0; client_state.y_position = 0;
            client_state.x_velocity = 0; client_state.y_velocity = 0;

            // Assign client to a room
            int roomID = assignClientToRoom(client_state);
            peerToRoom[peer] = roomID;

            // Send msg to the client
            MessageServer msg;
            // Fill the message buffer
            msg.ID = client_state.ID;

            // wait 1 second
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Sending message to client ID: " << msg.ID << std::endl;
            sendMessageToClient(msg, peer);
        }

        int assignClientToRoom(ClientState& client)
        {
            for(auto& room : rooms)
            {
                if(room.second.clients.size() < MAX_CLIENTS_PER_CHANNEL)
                {
                    room.second.clients.push_back(client);
                    return room.first;
                }
            }

            // Create a new room if no available room is found
            ROOM_COUNTER += 1;
            Room newRoom;
            newRoom.roomID = ROOM_COUNTER;
            newRoom.clients.push_back(client);
            rooms[ROOM_COUNTER] = newRoom;
            return ROOM_COUNTER;
        }

        void sendMessageToClient(MessageServer& msg, ENetPeer* peer)
        {
            ENetPacket* packet = enet_packet_create(&msg, sizeof(MessageServer), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, packet);
        }

        void clientDisconnected(ENetPeer* peer)
        {
            std::cout << event.peer->data << " disconnected\n";
            event.peer->data = nullptr;

            int roomID = peerToRoom[peer];
            auto& clients = rooms[roomID].clients;
            clients.erase(std::remove_if(clients.begin(), clients.end(),
                [peer](const ClientState& c) { return c.peer == peer; }),
                clients.end());

            if(clients.empty())
            {
                rooms.erase(roomID);
            }

            peerToRoom.erase(peer);
        }

    private:
        ENetHost* server;
        ENetAddress address;
        ENetEvent event;

        // client counter
        int COUNTER;
        int ROOM_COUNTER;

        // Maintain a list of rooms and their clients
        std::unordered_map<int, Room> rooms;
        std::unordered_map<ENetPeer*, int> peerToRoom;
    };
}

#endif // NET_SERVER_HPP
