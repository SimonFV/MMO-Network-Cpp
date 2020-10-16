#ifndef NETCOM_H
#define NETCOM_H

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdint>

#ifndef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>



#endif