#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdint>

// Windows only problem: "Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately."
// Each version of windows has a slightly different way of handling networking.
// This simply specifies which version to use.
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>