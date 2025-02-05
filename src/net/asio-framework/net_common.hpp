#ifndef NET_COMMON_HPP
#define NET_COMMON_HPP

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>


#define ASIO_STANDALONE             // Use only ASIO (No Boost)
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <asio/io_context.hpp>

#endif // NET_COMMON_HPP
