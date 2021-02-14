#pragma once
#include <string>

struct CubemapComponent
{
    std::string files[6];

    CubemapComponent(const std::string &directory, const std::string &pos_x, const std::string &neg_x, const std::string &pos_y, const std::string &neg_y, const std::string &pos_z, const std::string &neg_z)
    {
        files[0] = pos_x;
        files[1] = neg_x;
        files[2] = pos_y;
        files[3] = neg_y;
        files[4] = pos_z;
        files[5] = neg_z;
    }
};