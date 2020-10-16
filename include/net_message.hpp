#ifndef NETMSG_H
#define NETMSG_H

namespace sfv
{
    namespace net
    {
        //HEADER: Using templates to check for errors at compile time with enum class
        template <typename T>
        struct message_header
        {
            T id{};
            uint32_t size = 0;
        };

        //COMPLETE MESSAGE
        template <typename T>
        struct message
        {
            message_header<T> header{};
            std::vector<uint8_t> body;

            //Return the size of the entire message in bytes
            size_t size()
            {
                return sizeof(message_header<T>) + body.size();
            }

            //Override for std::cout <<
            friend std::ostream &operator<<(std::ostream &os, const message<T> &msg)
            {
                os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
                return os;
            }

            //Copies data into the message body
            template <typename DataT>
            friend message<T> &operator<<(message<T> &msg, const DataT &data)
            {
                static_assert(std::is_standard_layout<DataT>::value, "Data is too complex."); //Checks data can be copied
                size_t i = msg.body.size();                                                   //Current size of vector
                msg.body.resize(msg.body.size() + sizeof(DataT));                             //Resize the vector
                std::memcpy(msg.body.data() + i, &data, sizeof(DataT));                       //Copies the data into the new allocated space
                msg.header.size = msg.size();                                                 //Recalculates the size
                return msg;                                                                   //Return the msg so it can be chained
            }

            //Retreives the data from the message body
            template <typename DataT>
            friend message<T> &operator>>(message<T> &msg, DataT &data)
            {
                static_assert(std::is_standard_layout<DataT>::value, "Data is too complex."); //Checks data can be copied
                size_t i = msg.body.size() - sizeof(DataT);                                   //Location where the pulled data starts
                std::memcpy(&data, msg.body.data() + i, sizeof(DataT));                       //Copies the data from the vector
                msg.body.resize(i);                                                           //Shrinks the vector to remove read bytes
                msg.header.size = msg.size();                                                 //Recalculates the size
                return msg;                                                                   //Return the msg so it can be chained
            }
        };
    } // namespace net
} // namespace sfv

#endif