#include <golos/plugins/worker_api/worker_api_plugin.hpp>
#include <golos/plugins/json_rpc/api_helper.hpp>
#include <golos/plugins/chain/plugin.hpp>
#include <golos/chain/comment_object.hpp>
#include <appbase/application.hpp>

namespace golos { namespace plugins { namespace worker_api {

namespace bpo = boost::program_options;

class worker_api_plugin::worker_api_plugin_impl final {
public:
    worker_api_plugin_impl(worker_api_plugin& plugin)
            : _db(appbase::app().get_plugin<golos::plugins::chain::plugin>().db()) {
    }

    void select_unordered_worker_proposals(const worker_proposal_query& query, std::vector<worker_proposal_api_object>& result);

    ~worker_api_plugin_impl() = default;

    golos::chain::database& _db;
};

worker_api_plugin::worker_api_plugin() = default;

worker_api_plugin::~worker_api_plugin() = default;

const std::string& worker_api_plugin::name() {
    static std::string name = "worker_api";
    return name;
}

void worker_api_plugin::set_program_options(
    bpo::options_description& cli,
    bpo::options_description& cfg
) {
}

void worker_api_plugin::plugin_initialize(const boost::program_options::variables_map &options) {
    ilog("Intializing account notes plugin");

    my = std::make_unique<worker_api_plugin::worker_api_plugin_impl>(*this);

    JSON_RPC_REGISTER_API(name())
}

void worker_api_plugin::plugin_startup() {
    ilog("Starting up worker api plugin");
}

void worker_api_plugin::plugin_shutdown() {
    ilog("Shutting down worker api plugin");
}

void worker_api_plugin::worker_api_plugin_impl::select_unordered_worker_proposals(const worker_proposal_query& query,
    std::vector<worker_proposal_api_object>& result) {

    if (!_db.has_index<worker_proposal_index>()) {
        return;
    }

    _db.with_weak_read_lock([&]() {
        const auto& wpo_idx = _db.get_index<worker_proposal_index, by_permlink>();
        auto permlink = query.start_permlink ? *query.start_permlink : std::string();
        auto wpo_itr = wpo_idx.find(std::make_tuple(*query.start_author, permlink));
        for (; wpo_itr != wpo_idx.end() && result.size() < query.limit && wpo_itr->author == *query.start_author; ++wpo_itr) {
            if (!permlink.empty() && to_string(wpo_itr->permlink) != permlink) {
                break;
            }
            worker_proposal_api_object obj(*wpo_itr);
            result.push_back(obj);
        }
    });
}

template <class Order>
std::vector<worker_proposal_api_object> worker_api_plugin::get_worker_proposals(const worker_proposal_query& query, Order&& order) {
    std::vector<worker_proposal_api_object> result;

    query.validate();

    GOLOS_CHECK_PARAM(query.start_permlink.valid() ? query.start_author.valid() : true,
        GOLOS_CHECK_VALUE(query.start_permlink, "start_permlink without start_author is useless"));

    result.reserve(query.limit);

    my->select_unordered_worker_proposals(query, result);

    if (result.empty()) {
        return result;
    }

    auto it = result.begin();
    const auto et = result.end();
    std::sort(it, et, order);

    return result;
}

// Api Defines

DEFINE_API(worker_api_plugin, get_worker_proposals_by_created) {
    PLUGIN_API_VALIDATE_ARGS(
        (worker_proposal_query, query)
    )
    return get_worker_proposals(query, [&](const auto& lhs, const auto& rhs) -> bool {
        return lhs.created > rhs.created;
    });
}

DEFINE_API(worker_api_plugin, get_worker_proposals_by_rshares) {
    PLUGIN_API_VALIDATE_ARGS(
        (worker_proposal_query, query)
    )
    return get_worker_proposals(query, [&](const auto& lhs, const auto& rhs) -> bool {
        auto& lhs_post = my->_db.get_comment(lhs.author, lhs.permlink);
        auto& rhs_post = my->_db.get_comment(rhs.author, rhs.permlink);
        return lhs_post.net_rshares > rhs_post.net_rshares;
    });
}

} } } // golos::plugins::worker_api
