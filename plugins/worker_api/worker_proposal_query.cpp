#include <golos/plugins/worker_api/worker_proposal_query.hpp>

namespace golos { namespace plugins { namespace worker_api {

    void worker_proposal_query::validate() const {
        GOLOS_CHECK_LIMIT_PARAM(limit, 100);
    }

} } } // golos::plugins::worker_api

