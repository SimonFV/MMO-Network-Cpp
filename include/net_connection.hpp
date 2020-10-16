#pragma once

#include "net_common.hpp"
#include "net_message.hpp"
#include "net_thr_safe_queue.hpp"

namespace sfv
{
    namespace net
    {
        template <typename T>
        class Connection : public std::enable_shared_from_this<Connection<T>>
        {
        public:
            Connection() {}
            virtual ~Connection() {}

            bool connectToServer();
            bool disconnect();
            bool isConnected() const;
            bool send(const message<T> &msg);

        protected:
            asio::ip::tcp::socket m_socket;
            asio::io_context &m_context;
            Queue<message<T>> m_qMsgOut;       //messages to be sent
            Queue<owned_message<T>> &m_qMsgIn; //received messages: it is a reference because will be provided by the owner (client or server)
        };
    } // namespace net
} // namespace sfv