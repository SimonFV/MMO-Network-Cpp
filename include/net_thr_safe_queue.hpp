#pragma once

#include "net_common.hpp"

namespace sfv
{
    namespace net
    {
        //Thread safe queue class for storing the received messages
        template <typename T>
        class Queue
        {
        public:
            Queue() = default;
            Queue(const Queue<T> &) = delete;
            virtual ~Queue() { clear(); }

            //Get the front element of the queue
            const T &getFront()
            {
                std::scoped_lock lock(queueMutex);
                return queueDeque.front();
            }

            //Get the back element of the queue
            const T &getBack()
            {
                std::scoped_lock lock(queueMutex);
                return queueDeque.back();
            }

            //Insert element in the front of the queue
            void pushFront(const T &element)
            {
                std::scoped_lock lock(queueMutex);
                queueDeque.push_front();
            }

            //Insert element in the front of the queue
            void pushBack(const T &element)
            {
                std::scoped_lock lock(queueMutex);
                queueDeque.push_back();
            }

            //Checks if the queue is empty
            bool empty()
            {
                std::scoped_lock lock(queueMutex);
                return queueDeque.empty();
            }

            //Returns the number of elements of the queue
            size_t count()
            {
                std::scoped_lock lock(queueMutex);
                return queueDeque.size();
            }

            //Removes all the elements from the queue
            void clear()
            {
                std::scoped_lock lock(queueMutex);
                queueDeque.clear();
            }

            //Removes and returns the front element of the queue
            T popFront()
            {
                std::scoped_lock lock(queueMutex);
                auto t = std::move(queueDeque.front());
                queueDeque.pop_front();
                return t;
            }

            //Removes and returns the back element of the queue
            T popBack()
            {
                std::scoped_lock lock(queueMutex);
                auto t = std::move(queueDeque.back());
                queueDeque.pop_back();
                return t;
            }

        protected:
            std::mutex queueMutex;
            std::deque<T> queueDeque;
        };
    } // namespace net
} // namespace sfv