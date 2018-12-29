#pragma once

#include <fc/optional.hpp>
#include <golos/protocol/exceptions.hpp>

namespace golos { namespace plugins { namespace worker_api {

    struct worker_proposal_query {
        void validate() const;

        uint32_t                          limit = 20;
        fc::optional<std::string>         start_author;
        fc::optional<std::string>         start_permlink;
    };

} } } // golos::plugins::worker_api

FC_REFLECT((golos::plugins::worker_api::worker_proposal_query),
        (limit)(start_author)(start_permlink)
);
