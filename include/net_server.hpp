#pragma once

#include "net_connection.hpp"

namespace sfv
{
    namespace net
    {
        template <typename T>
        class ServerInterface
        {
            ServerInterface(uint16_t port)
                : m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
            {
            }

            ~ServerInterface()
            {
                stop();
            }

            bool start()
            {
                try
                {
                    waitForClientConnection();
                    thr_context = std::thread([this]() { m_context.run(); })
                }
                catch (std::exception &e)
                {
                    std::cerr << "[SERVER]: Error: " << e.what() << "\n";
                    return false;
                }
                std::cout << "[SERVER]: Started!\n";
                return true;
            }

            void stop()
            {
                m_context.stop();
                if (thr_context.joinable())
                {
                    thr_context.join();
                }
                std::cout << "[SERVER]: Stopped!\n";
            }

            //Async- for ASIO
            void waitForClientConnection()
            {
                m_acceptor.async_accept(
                    [this](std::error_code er, asio::ip::tcp::socket socket) {
                        if (!er)
                        {
                            std::cout << "[SERVER]: New connection: " << socket.remote_endpoint() << "\n";
                            std::shared_ptr<Connection<T>> newConnection =
                                std::make_shared<Connection<T>>(Connection<T>::owner::server,
                                                                m_context, std::move(socket), m_qMsgIn);

                            if (onClientConnect(newConnection))
                            {
                                m_deqConnections.push_back(std::move(newConnection));
                                m_deqConnections.back()->ConnectToClient(id_counter);
                                std::cout << "[" << m_deqConnections.back()->GetID() << "]: connection accepted\n";
                            }
                            else
                            {
                                std::cout << "Connection denied\n";
                            }
                        }
                        else
                        {
                            std::cerr << "[SERVER]: Connection error: " << er.message() << "\n";
                        }
                        waitForClientConnection();
                    })
            }

            void msgClient(std::shared_ptr<Connection<T>> client, const message<T> &msg)
            {
                if (client && client->isConnected())
                {
                    client->send(msg);
                }
                else
                {
                    onClientDisconnect(client);
                    client.reset();
                    m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), client),
                                           m_deqConnections.end());
                }
            }

            void msgAllClients(const message<T> &msg, std::shared_ptr<Connection<T>> ignoreClient = nullptr)
            {
                bool areInvalidClients = false;
                for (auto &client : m_deqConnections)
                {
                    if (client && client->isConnected())
                    {
                        if (client != ignoreClient)
                        {
                            client->send(msg);
                        }
                    }
                    else
                    {
                        onClientDisconnect(client);
                        client.reset();
                        areInvalidClients = true;
                    }
                    if (areInvalidClients)
                    {
                        client.reset();
                        m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr),
                                               m_deqConnections.end());
                    }
                }
            }

            void update(size_t maxMsgs = -1)
            {
                size_t msgCount = 0;
                while (msgCount < maxMsgs && !m_qMsgIn.empty())
                {
                    auto msg = m_qMsgIn.popFront();
                    onMsg(msg.remote, msg.msg);
                    msgCount++;
                }
            }

        protected:
            //Called when a client connects, can reject connections by returning false
            virtual bool onClientConnect(std::shared_ptr<Connection<T>> client)
            {
                return false;
            }

            //Called when a client disconnects
            virtual void onClientDisconnect(std::shared_ptr<Connection<T>> client)
            {
            }

            //Called when a message arrives
            virtual void onMsg(std::shared_ptr<Connection<T>> client, message<T> &msg)
            {
            }

            Queue<owned_message<T>> m_qMsgIn;

            std::deque<std::shared_ptr<Connection<T>>> m_deqConnections; //Container of active connecitions

            asio::io_context m_context;
            std::thread thr_context;
            asio::ip::tcp::acceptor m_acceptor;

            uint32_t id_counter = 10000;
        };
    } // namespace net
} // namespace sfv