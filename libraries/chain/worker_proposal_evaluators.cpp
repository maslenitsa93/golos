//TODO: rename me to worker_proposal_evaluators.hpp
#include <golos/protocol/worker_proposal_operations.hpp>
#include <golos/protocol/exceptions.hpp>
#include <golos/chain/steem_evaluator.hpp>
#include <golos/chain/database.hpp>
#include <golos/chain/steem_objects.hpp>
#include <golos/chain/worker_proposal_objects.hpp>

namespace golos { namespace chain {

    void worker_proposal_evaluator::do_apply(const worker_proposal_operation& o) {
        ASSERT_REQ_HF(STEEMIT_HARDFORK_0_20__1013, "worker_proposal_operation");

        const auto& comment = _db.get_comment(o.author, o.permlink);

        GOLOS_CHECK_LOGIC(comment.parent_author == STEEMIT_ROOT_POST_PARENT,
            logic_exception::worker_proposal_can_be_created_only_on_post,
            "Worker proposal can be created only on post");

        const auto now = _db.head_block_time();

        const auto& wpo_idx = _db.get_index<worker_proposal_index, by_permlink>();
        auto wpo_itr = wpo_idx.find(std::make_tuple(o.author, o.permlink));

        if (wpo_itr != wpo_idx.end()) {
            _db.modify(*wpo_itr, [&](worker_proposal_object& wpo) {
                wpo.type = o.type;
                wpo.modified = now;
            });
            return;
        }

        _db.create<worker_proposal_object>([&](worker_proposal_object& wpo) {
            wpo.author = o.author;
            wpo.permlink = comment.permlink;
            wpo.type = o.type;
            wpo.state = created;
            wpo.created = now;
        });
    }

    void worker_proposal_delete_evaluator::do_apply(const worker_proposal_delete_operation& o) {
        ASSERT_REQ_HF(STEEMIT_HARDFORK_0_20__1013, "worker_proposal_delete_operation");

        const auto& wpo_idx = _db.get_index<worker_proposal_index, by_permlink>();
        auto wpo_itr = wpo_idx.find(std::make_tuple(o.author, o.permlink));
        if (wpo_itr == wpo_idx.end()) {
            GOLOS_THROW_MISSING_OBJECT("worker_proposal_object", fc::mutable_variant_object()("author",o.author)("permlink",o.permlink));
        }

        GOLOS_CHECK_LOGIC(wpo_itr->state == created,
            logic_exception::cannot_delete_worker_proposal_with_approved_techspec,
            "Cannot delete worker proposal with approved techspec");

        GOLOS_CHECK_LOGIC(wpo_itr->type == task,
            logic_exception::cannot_delete_worker_proposal_with_premade_work,
            "Cannot delete worker proposal with premade work");

        const auto& wto_idx = _db.get_index<worker_techspec_index, by_worker_proposal>();
        auto wto_itr = wto_idx.find(std::make_tuple(o.author, o.permlink));
        while (wto_itr != wto_idx.end()) {
            const auto& cur_wto = *wto_itr;
            ++wto_itr;
            _db.remove(cur_wto);
        }

        _db.remove(*wpo_itr);
    }

    void worker_techspec_evaluator::do_apply(const worker_techspec_operation& o) {
        ASSERT_REQ_HF(STEEMIT_HARDFORK_0_20__1013, "worker_techspec_operation");

        const auto now = _db.head_block_time();

        const auto& comment = _db.get_comment(o.author, o.permlink);

        GOLOS_CHECK_LOGIC(comment.parent_author == STEEMIT_ROOT_POST_PARENT,
            logic_exception::worker_techspec_can_be_created_only_on_post,
            "Worker techspec can be created only on post");

        const auto& wpo_idx = _db.get_index<worker_proposal_index, by_permlink>();
        auto wpo_itr = wpo_idx.find(std::make_tuple(o.worker_proposal_author, o.worker_proposal_permlink));

        GOLOS_CHECK_LOGIC(wpo_itr != wpo_idx.end(),
            logic_exception::worker_techspec_can_be_created_only_for_existing_proposal,
            "Worker techspec can be created only for existing proposal");
        GOLOS_CHECK_LOGIC(wpo_itr->state == created,
            logic_exception::this_worker_proposal_already_has_approved_techspec,
            "This worker proposal already has approved techspec");
        GOLOS_CHECK_LOGIC(wpo_itr->type == task,
            logic_exception::this_worker_proposal_has_premade_work,
            "This worker proposal has premade work");

        const auto& wto_idx = _db.get_index<worker_techspec_index, by_worker_proposal>();
        auto wto_itr = wto_idx.find(std::make_tuple(o.worker_proposal_author, o.worker_proposal_permlink));
        if (wto_itr != wto_idx.end()) {
            GOLOS_CHECK_LOGIC(o.specification_cost.symbol == wto_itr->specification_cost.symbol,
                logic_exception::cannot_change_cost_symbol,
                "Cannot change cost symbol");
            GOLOS_CHECK_LOGIC(o.development_cost.symbol == wto_itr->development_cost.symbol,
                logic_exception::cannot_change_cost_symbol,
                "Cannot change cost symbol");

            _db.modify(*wto_itr, [&](worker_techspec_object& wto) {
                wto.modified = now;
                wto.specification_cost = o.specification_cost;
                wto.specification_eta = o.specification_eta;
                wto.development_cost = o.development_cost;
                wto.development_eta = o.development_eta;
                wto.payments_count = o.payments_count;
                wto.payments_interval = o.payments_interval;
            });

            return;
        }

        _db.create<worker_techspec_object>([&](worker_techspec_object& wto) {
            wto.author = o.author;
            wto.permlink = comment.permlink;
            wto.worker_proposal_author = o.worker_proposal_author;
            from_string(wto.worker_proposal_permlink, o.worker_proposal_permlink);
            wto.created = now;
            wto.specification_cost = o.specification_cost;
            wto.specification_eta = o.specification_eta;
            wto.development_cost = o.development_cost;
            wto.development_eta = o.development_eta;
            wto.payments_count = o.payments_count;
            wto.payments_interval = o.payments_interval;
        });
    }

    void worker_techspec_delete_evaluator::do_apply(const worker_techspec_delete_operation& o) {
        ASSERT_REQ_HF(STEEMIT_HARDFORK_0_20__1013, "worker_techspec_delete_operation");

        const auto& wto_idx = _db.get_index<worker_techspec_index, by_permlink>();
        auto wto_itr = wto_idx.find(std::make_tuple(o.author, o.permlink));
        if (wto_itr == wto_idx.end()) {
            GOLOS_THROW_MISSING_OBJECT("worker_techspec_object", fc::mutable_variant_object()("author",o.author)("permlink",o.permlink));
        }

        _db.remove(*wto_itr);
    }

} } // golos::chain
