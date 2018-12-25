#pragma once

#include <golos/protocol/authority.hpp>
#include <golos/protocol/base.hpp>

namespace golos { namespace protocol {

    enum worker_proposal_type {
        task,
        premade_work,
        _wpt_size
    };

    struct worker_proposal_operation : public base_operation {
        account_name_type author;
        std::string permlink;
        worker_proposal_type type = task;

        extensions_type extensions;

        void validate() const;

        void get_required_posting_authorities(flat_set<account_name_type>& a) const {
            a.insert(author);
        }
    };

    struct worker_proposal_delete_operation : public base_operation {
        account_name_type author;
        std::string permlink;

        extensions_type extensions;

        void validate() const;

        void get_required_posting_authorities(flat_set<account_name_type>& a) const {
            a.insert(author);
        }
    };

} } // golos::protocol

FC_REFLECT_ENUM(golos::protocol::worker_proposal_type, (task)(premade_work)(_wpt_size))
FC_REFLECT(
    (golos::protocol::worker_proposal_operation),
    (author)(permlink)(type)(extensions))

FC_REFLECT(
    (golos::protocol::worker_proposal_delete_operation),
    (author)(permlink)(extensions))
