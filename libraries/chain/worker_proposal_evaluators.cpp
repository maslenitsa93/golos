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

        _db.remove(*wpo_itr);
    }

} } // golos::chain