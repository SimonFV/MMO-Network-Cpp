#pragma once

#include "net_connection.hpp"

namespace sfv
{
    namespace net
    {
        template <typename T>
        class ClientInterface
        {
        public:
            ClientInterface() : m_socket(m_context) {}
            ~ClientInterface() { disconnect(); }

            bool connect(const std::string &host, const uint16_t port)
            {
                try
                {
                    //Create connection
                    m_connection = std::make_unique<Connection<T>>();

                    asio::ip::tcp::resolver res(m_context);
                    m_endpoints = res.resolve(host, std::to_string(port));

                    m_connection->connectToServer(m_endpoints);

                    //Starts the context thread
                    thr_context = std::thread([this]() { m_context.run(); });
                }
                catch (std::exception &e)
                {
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Client exception: " << e.what() << '\n';
                    return false;
                }

                return true;
            }

            void disconnect()
            {
                if (isConnected())
                {
                    m_connection->Disconnect();
                }
                m_context.stop();
                if (thr_context.joinable())
                {
                    thr_context.join();
                }
                m_connection.release();
            }

            bool isConnected()
            {
                if (m_connection)
                {
                    return m_connection->isConnected();
                }
                else
                {
                    return false;
                }
            }

            Queue<owned_message<T>> &incoming()
            {
                return m_qMsgIn;
            }

        protected:
            asio::io_context m_context; //creates a platform speficic interface
            std::thread thr_context;    //thread for the asio context
            asio::ip::tcp::socket m_socket;
            std::unique_ptr<Connection<T>> m_connection;

        private:
            Queue<owned_message<T>> m_qMsgIn;
        };
    } // namespace net
} // namespace sfv