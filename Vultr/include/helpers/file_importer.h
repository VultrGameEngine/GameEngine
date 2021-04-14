#pragma once
#include "file.h"
#include "directory.h"

namespace Vultr
{
class FileImporter
{
  public:
    static void ImportFile(File &file);
};
} // namespace Vultr
