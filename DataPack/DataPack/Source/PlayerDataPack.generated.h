#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <msgpack.hpp>

struct PlayerDataPack
{
    int32_t Level;
    int32_t Speed;
    int32_t LevelUpTime;

    MSGPACK_DEFINE(Level, Speed, LevelUpTime);
};
