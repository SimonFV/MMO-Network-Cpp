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
        };
    } // namespace net
} // namespace sfv