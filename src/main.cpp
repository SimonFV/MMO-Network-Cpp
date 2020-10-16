/*
#include <iostream>
#include <thread>
#include <chrono>

#ifndef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(1 * 1024); //Buffer for incomming data

//read incomming data
void GrabSomeData(asio::ip::tcp::socket &asio_socket)
{
    asio_socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
                                [&](std::error_code er, std::size_t length) {
                                    if (!er)
                                    {
                                        std::cout << "\n\nRead " << length << "bytes\n\n";
                                        for (int i = 0; i < length; i++)
                                        {
                                            std::cout << vBuffer[i];
                                        }
                                        GrabSomeData(asio_socket);
                                    }
                                });
}

int main()
{
    asio::error_code er;      //error handler
    asio::io_context context; //create platform speficic interface
    asio::io_context::work idleWork(context);
    std::thread thrContext = std::thread([&]() { context.run(); });

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", er), 80); //get address to connect
    asio::ip::tcp::socket asio_socket(context);                                      //create a socket with the specific context

    asio_socket.connect(endpoint, er); //tries to connect the socket to the endpoint
    if (!er)
    {
        std::cout << "connected...\n";
    }
    else
    {
        std::cout << er.message() << std::endl;
    }

    //send and receive data
    if (asio_socket.is_open())
    {
        GrabSomeData(asio_socket);

        std::string sRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Conecction: close\r\n\r\n";

        asio_socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), er); //sends data

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20000ms);
        context.stop();
        if (thrContext.joinable())
        {
            thrContext.join();
        }
    }

    system("pause");
    return 0;
}

*/