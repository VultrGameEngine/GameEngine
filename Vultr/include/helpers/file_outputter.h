#pragma once
#include "directory.h"
#include "file.h"
#include <iostream>
#include <fstream>

namespace Vultr
{
class FileOutputter
{
  public:
    FileOutputter(File &p_file);
    ~FileOutputter();

    void operator<<(std::string data)
    {
        os << data.c_str();
    }

  private:
    File &m_file;
    std::ofstream os;
};
} // namespace Vultr
