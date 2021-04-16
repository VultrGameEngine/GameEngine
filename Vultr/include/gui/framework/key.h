#pragma once
#include <string>

namespace Vultr::GUI
{
class Key
{
  public:
    Key()
    {
        key = "";
    }
    Key(const std::string &p_key) : key(p_key)
    {
    }

    bool operator==(const Key &other) const
    {
        return other.key == this->key;
    }

  private:
    std::string key;
};
} // namespace Vultr::GUI
