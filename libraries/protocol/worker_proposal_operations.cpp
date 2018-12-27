#include <golos/protocol/worker_proposal_operations.hpp>
#include <golos/protocol/exceptions.hpp>
#include <golos/protocol/validate_helper.hpp>

namespace golos { namespace protocol {

    void worker_proposal_operation::validate() const {
        GOLOS_CHECK_PARAM_ACCOUNT(author);
        GOLOS_CHECK_PARAM(permlink, validate_permlink(permlink));
    }

    void worker_proposal_delete_operation::validate() const {
        GOLOS_CHECK_PARAM_ACCOUNT(author);
        GOLOS_CHECK_PARAM(permlink, validate_permlink(permlink));
    }

} } // golos::protocol
