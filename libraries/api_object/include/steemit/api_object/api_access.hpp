#pragma once

#include <fc/reflect/reflect.hpp>

#include <map>
#include <string>
#include <vector>

namespace steemit {
    namespace application {

        struct api_access_info {
            std::string username;
            std::string password_hash_b64;
            std::string password_salt_b64;
            std::vector<std::string> allowed_apis;
        };

        struct api_access {
            std::map<std::string, api_access_info> permission_map;
        };

    }
} // steemit::application

FC_REFLECT(steemit::application::api_access_info,
        (username)
                (password_hash_b64)
                (password_salt_b64)
                (allowed_apis)
)

FC_REFLECT(steemit::application::api_access,
        (permission_map)
)