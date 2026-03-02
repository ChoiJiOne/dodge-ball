#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <msgpack.hpp>

struct TestDataPack
{
    int32_t A;
    float B;
    std::string C;
    bool D;
    std::vector<int32_t> E;

    MSGPACK_DEFINE(A, B, C, D, E);
};
