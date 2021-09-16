#pragma once
#include "resource_hash.h"
#include <hashtable.h>

namespace Vultr
{
    struct ResourceManager
    {
        vtl::HashTable<const char *, ResourceHash> resources;
    };

    ResourceHash get_resource(ResourceManager *manager, const char *res);
} // namespace Vultr
