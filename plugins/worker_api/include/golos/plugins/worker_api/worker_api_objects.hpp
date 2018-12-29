#pragma once

#include <golos/chain/worker_proposal_objects.hpp>

namespace golos { namespace plugins { namespace worker_api {

    using namespace golos::chain;

    struct worker_proposal_api_object {
        worker_proposal_api_object(const worker_proposal_object& o)
            : author(o.author),
              permlink(to_string(o.permlink)),
              type(o.type),
              state(o.state),
              deposit(o.deposit),
              worker(o.worker),
              work_beginning_time(o.work_beginning_time),
              worker_payments_count(o.worker_payments_count),
              payment_beginning_time(o.payment_beginning_time),
              created(o.created),
              modified(o.modified) {
        }

        worker_proposal_api_object() {
        }

        account_name_type author;
        std::string permlink;
        worker_proposal_type type;
        worker_proposal_state state;
        asset deposit;
        // TODO techspec id
        account_name_type worker;
        time_point_sec work_beginning_time;
        uint8_t worker_payments_count = 0;
        time_point_sec payment_beginning_time;
        time_point_sec created;
        time_point_sec modified;
    };

} } } // golos::plugins::worker_api

FC_REFLECT((golos::plugins::worker_api::worker_proposal_api_object),
    (author)(permlink)(type)(state)(deposit)(worker)(work_beginning_time)(worker_payments_count)
    (payment_beginning_time)(created)(modified)
)
