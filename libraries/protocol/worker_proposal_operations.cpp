#include <golos/protocol/worker_proposal_operations.hpp>
#include <golos/protocol/exceptions.hpp>
#include <golos/protocol/validate_helper.hpp>

namespace golos { namespace protocol {

    static inline void validate_account_name(const string& name) {
        GOLOS_CHECK_VALUE(is_valid_account_name(name), "Account name ${name} is invalid", ("name", name));
    }

    /// TODO: after the hardfork, we can rename this method validate_permlink because it is strictily less restrictive than before
    ///  Issue #56 contains the justificiation for allowing any UTF-8 string to serve as a permlink, content will be grouped by tags
    ///  going forward.
    inline void validate_permlink(const string& permlink) {
        GOLOS_CHECK_VALUE(permlink.size() < STEEMIT_MAX_PERMLINK_LENGTH, "permlink is too long");
        GOLOS_CHECK_VALUE(fc::is_utf8(permlink), "permlink not formatted in UTF8");
    }

    void worker_proposal_operation::validate() const {
        GOLOS_CHECK_PARAM_ACCOUNT(author);
        GOLOS_CHECK_PARAM(permlink, validate_permlink(permlink));
    }

    void worker_proposal_delete_operation::validate() const {
        GOLOS_CHECK_PARAM_ACCOUNT(author);
        GOLOS_CHECK_PARAM(permlink, validate_permlink(permlink));
    }

} } // golos::protocol
