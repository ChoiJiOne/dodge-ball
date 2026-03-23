#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <msgpack.hpp>

struct EnemyDataPack
{
    int32_t Num;
    int32_t MoveSpeed;
    int32_t RotationSpeed;
    std::vector<float> Color;

    MSGPACK_DEFINE(Num, MoveSpeed, RotationSpeed, Color);
};
