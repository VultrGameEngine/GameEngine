#pragma once
#include "directory.h"
#include "file.h"
#include <iostream>
#include <fstream>

namespace Vultr
{
    struct FileOutputter
    {
        FileOutputter(File &p_file);
        ~FileOutputter();

        File &m_file;
        std::ofstream os;

        void operator<<(std::string data)
        {
            os << data.c_str();
        }
    };
} // namespace Vultr
