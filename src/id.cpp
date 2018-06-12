//
// Copyright (C) 2018 Codership Oy <info@codership.com>
//

#include "wsrep/id.hpp"
#include <wsrep_api.h>

#include <cctype>
#include <sstream>
#include <algorithm>

wsrep::id::id(const std::string& str)
    :  data_()
{
    wsrep_uuid_t wsrep_uuid;
    if (wsrep_uuid_scan(str.c_str(), str.size(), &wsrep_uuid) ==
        WSREP_UUID_STR_LEN)
    {
        std::memcpy(data_, wsrep_uuid.data, sizeof(data_));
    }
    else if (str.size() <= 16)
    {
        std::memcpy(data_, str.c_str(), str.size());
    }
    else
    {
        std::ostringstream os;
        os << "String '" << str
           << "' does not contain UUID or is longer thatn 16 bytes";
        throw wsrep::runtime_error(os.str());
    }
}

std::ostream& wsrep::operator<<(std::ostream& os, const wsrep::id& id)
{
    const char* ptr(static_cast<const char*>(id.data()));
    ssize_t size(id.size());
    if (std::count_if(ptr, ptr + size, ::isalnum) == size)
    {
        return (os << std::string(ptr, size));
    }
    else
    {
        char uuid_str[WSREP_UUID_STR_LEN + 1];
        wsrep_uuid_t uuid;
        std::memcpy(uuid.data, ptr, sizeof(uuid.data));
        if (wsrep_uuid_print(&uuid, uuid_str, sizeof(uuid_str)) < 0)
        {
            throw wsrep::runtime_error("Could not print uuid");
        }
        uuid_str[WSREP_UUID_STR_LEN] = '\0';
        return (os << uuid_str);
    }
}