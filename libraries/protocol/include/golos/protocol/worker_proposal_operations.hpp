//TODO: rename me to worker_operations.hpp
#pragma once

#include <golos/protocol/asset.hpp>
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

    struct techspec_data {
        asset specification_cost;
        time_point_sec specification_deadline;
        asset development_cost;
        time_point_sec development_deadline;
        uint16_t payments_count;
        uint32_t payments_interval;
    };

    struct worker_techspec_operation : public base_operation {
        account_name_type author;
        std::string permlink;
        techspec_data data;

        extensions_type extensions;

        void validate() const;

        void get_required_posting_authorities(flat_set<account_name_type>& a) const {
            a.insert(author);
        }
    };

    struct worker_techspec_delete_operation : public base_operation {
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

FC_REFLECT((golos::protocol::techspec_data),
    (specification_cost)(specification_deadline)(development_cost)(development_deadline)(payments_count)(payments_interval))
FC_REFLECT(
    (golos::protocol::worker_techspec_operation),
    (author)(permlink)(data)(extensions))

FC_REFLECT(
    (golos::protocol::worker_techspec_delete_operation),
    (author)(permlink)(extensions))
