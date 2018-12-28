//TODO: rename me to worker_objects.hpp
#pragma once

#include <golos/chain/database.hpp>

namespace golos { namespace chain {

    enum worker_proposal_state {
        created,
        techspec,
        work,
        witnesses_review,
        payment,
        closed
    };

    class worker_proposal_object : public object<worker_proposal_object_type, worker_proposal_object> {
    public:
        worker_proposal_object() = delete;

        template <typename Constructor, typename Allocator>
        worker_proposal_object(Constructor&& c, allocator <Allocator> a)
            : permlink(a) {
            c(*this);
        };

        id_type id;

        account_name_type author;
        shared_string permlink;
        worker_proposal_type type;
        worker_proposal_state state;
        asset deposit;
        id_type techspec_id; // TODO
        account_name_type worker;
        time_point_sec work_beginning_time;
        uint8_t worker_payments_count;
        time_point_sec payment_beginning_time;
        time_point_sec created;
        time_point_sec modified;
    };

    class worker_techspec_object : public object<worker_techspec_object_type, worker_techspec_object> {
    public:
        worker_techspec_object() = delete;

        template <typename Constructor, typename Allocator>
        worker_techspec_object(Constructor&& c, allocator <Allocator> a)
            : permlink(a), worker_proposal_permlink(a) {
            c(*this);
        };

        id_type id;

        account_name_type author;
        shared_string permlink;
        account_name_type worker_proposal_author;
        shared_string worker_proposal_permlink;
        time_point_sec created;
        time_point_sec modified;
        asset specification_cost;
        uint32_t specification_eta;
        asset development_cost;
        uint32_t development_eta;
        uint16_t payments_count;
        uint32_t payments_interval;
    };

    struct by_permlink;

    using worker_proposal_index = multi_index_container<
        worker_proposal_object,
        indexed_by<
            ordered_unique<
                tag<by_id>,
                member<worker_proposal_object, worker_proposal_object_id_type, &worker_proposal_object::id>>,
            ordered_unique<
                tag<by_permlink>,
                composite_key<
                    worker_proposal_object,
                    member<worker_proposal_object, account_name_type, &worker_proposal_object::author>,
                    member<worker_proposal_object, shared_string, &worker_proposal_object::permlink>>,
                composite_key_compare<
                    std::less<account_name_type>,
                    chainbase::strcmp_less>>>,
        allocator<worker_proposal_object>>;

    struct by_worker_proposal;

    using worker_techspec_index = multi_index_container<
        worker_techspec_object,
        indexed_by<
            ordered_unique<
                tag<by_id>,
                member<worker_techspec_object, worker_techspec_object_id_type, &worker_techspec_object::id>>,
            ordered_unique<
                tag<by_permlink>,
                composite_key<
                    worker_techspec_object,
                    member<worker_techspec_object, account_name_type, &worker_techspec_object::author>,
                    member<worker_techspec_object, shared_string, &worker_techspec_object::permlink>>,
                composite_key_compare<
                    std::less<account_name_type>,
                    chainbase::strcmp_less>>,
            ordered_unique<
                tag<by_worker_proposal>,
                composite_key<
                    worker_techspec_object,
                    member<worker_techspec_object, account_name_type, &worker_techspec_object::worker_proposal_author>,
                    member<worker_techspec_object, shared_string, &worker_techspec_object::worker_proposal_permlink>>,
                composite_key_compare<
                    std::less<account_name_type>,
                    chainbase::strcmp_less>>>,
        allocator<worker_techspec_object>>;

} } // golos::chain

CHAINBASE_SET_INDEX_TYPE(
    golos::chain::worker_proposal_object,
    golos::chain::worker_proposal_index);

CHAINBASE_SET_INDEX_TYPE(
    golos::chain::worker_techspec_object,
    golos::chain::worker_techspec_index);
