#pragma once

#include <climits>

namespace constants {
    constexpr int   CHAR_SIZE   {1};
    constexpr int   INT_SIZE    {4};
    constexpr int   FLOAT_SIZE  {4};

    constexpr int   ACK_SIZE    {1};
    constexpr int   ID_SIZE     {4};
    constexpr int   HEADER_SIZE {4};

    constexpr int   NO_TIMEOUT  {INT_MAX};
    constexpr int   STD_TIMEOUT {3};

    constexpr char  ACK_SUCCESS {'1'};
    constexpr char  ACK_FAIL    {'0'};
}