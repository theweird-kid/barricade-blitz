#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include "net_common.hpp"
#include "net_thread_safe_queue.hpp"
#include "net_message.hpp"
#include "net_connection.hpp"

#include <cstdint>
#include <exception>
#include <memory>
#include <system_error>

namespace wkd
{
    namespace net
    {
        template <typename T>
        class server_interface
        {
        public:

            server_interface(uint16_t port)
                : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
            {

            }

            virtual ~server_interface()
            {
                Stop();
            }

            bool Start()
            {
                try
                {
                    // some work for the asio context
                    WaitForClientConnection();

                    m_thrContext = std::thread([this]() { m_asioContext.run(); });
                }
                catch (std::exception& e)
                {
                    // Something prohibited the server from listening
                    std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
                    return false;
                }

                std::cout << "[SERVER] Started!\n";
                return true;
            }

            void Stop()
            {
                // Request the context to close
                m_asioContext.stop();

                // tidy up the context thread
                if(m_thrContext.joinable()) m_thrContext.join();

                std::cout << "[SERVER] Stopped!\n";
            }

            // ASYNC - Instruct asio to wait for connection
            void WaitForClientConnection()
            {
                m_asioAcceptor.async_accept(
                    [this](std::error_code ec, asio::ip::tcp::socket socket)
                    {
                        if (!ec)
                        {
                            std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << std::endl;

                            std::shared_ptr<connection<T>> newconn =
                                std::make_shared<connection<T>>(connection<T>::owner::server,
                                    m_asioContext, std::move(socket), m_qMessageIn);

                            // Give the user a chance to deny connection
                            if(OnClientConnect(newconn))
                            {
                                // Connection allowed, add to the contianer of new connection
                                m_deqConnections.push_back(std::move(newconn));
                                // Assign ID to connections
                                m_deqConnections.back()->ConnectToClient(this, nIDCounter++);

                                std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved\n";
                            }
                            else
                            {
                                std::cout << "[-----] Connection Denied\n";
                            }
                        }
                        else
                        {
                            std::cerr <<"[SERVER] New Connection Error: " << ec.message() << std::endl;
                        }

                        // Wait for another connection..
                        WaitForClientConnection();
                    });
            }

            // Send a message to a specific client
            void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
            {
                if(client && client->IsConnected())
                {
                    client->Send(msg);
                }
                else
                {
                    // Remove client
                    OnClientDisconnect(client);
                    client.reset();
                    m_deqConnections.erase(
                        std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
                }
            }

            // Send to all clients
            void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr)
            {
                bool bInvalidClientExists = false;

                for (auto& client: m_deqConnections)
                {
                    // Check client is connected...
                    if (client && client->IsConnected())
                    {
                        if(client != pIgnoreClient)
                            client->Send(msg);
                    }
                    else
                    {
                        // The client couldn't be contacted, so assume it is disconnected
                        OnClientDisconnect(client);
                        client.reset();
                        bInvalidClientExists = true;
                    }
                }

                if(bInvalidClientExists)
                {
                    // remove client from the deque
                    m_deqConnections.erase(
                        std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
                }
            }

            // Force server to respond to incoming messages
            void Update(size_t nMaxMessages = -1, bool bWait = false)
            {
                // Optimize CPU Utilization
                if(bWait) m_qMessageIn.wait();

                size_t nMessageCount = 0;
                while (nMessageCount < nMaxMessages && !m_qMessageIn.empty())
                {
                    // extract the owned message
                    auto msg = m_qMessageIn.pop_front();

                    // Pass it on to message handler
                    OnMessage(msg.remote, msg.msg);
                    nMessageCount++;
                }
            }

        protected:
            // Called when a client connects, you can veto the connection by returning false
            virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
            {
                return false;
            }

            // Called when a client appears to have disconnected
            virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
            {

            }

            // Called when a message arrives
            virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
            {

            }

        public:
            // Called when the client is validated
            virtual void OnClientValidated(std::shared_ptr<connection<T>> client)
            {

            }

        protected:
            // Thread safe queue for incoming message packets
            thread_safe_queue<owned_message<T>> m_qMessageIn;

            // Container of active validated connections
            std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

            // Order of declaration is important - it is also the order of initialization
            asio::io_context m_asioContext;
            std::thread m_thrContext;

            // Needs an asio context
            asio::ip::tcp::acceptor m_asioAcceptor;

            // Clients will be identified "system wide" via an ID
            uint32_t nIDCounter = 10000;
        };
    }
}

#endif // NET_SERVER_HPP
