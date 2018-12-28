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

    void worker_techspec_operation::validate() const {
        GOLOS_CHECK_PARAM_ACCOUNT(author);
        GOLOS_CHECK_PARAM(permlink, validate_permlink(permlink));
        GOLOS_CHECK_PARAM_ACCOUNT(worker_proposal_author);
        GOLOS_CHECK_PARAM(worker_proposal_permlink, validate_permlink(worker_proposal_permlink));

        GOLOS_CHECK_PARAM(specification_cost, {
            GOLOS_CHECK_VALUE_GE(specification_cost.amount, 0);
        });
        GOLOS_CHECK_PARAM(development_cost, {
            GOLOS_CHECK_VALUE_GE(development_cost.amount, 0);
        });
    }

    void worker_techspec_delete_operation::validate() const {
        GOLOS_CHECK_PARAM_ACCOUNT(author);
        GOLOS_CHECK_PARAM(permlink, validate_permlink(permlink));
    }

} } // golos::protocol
