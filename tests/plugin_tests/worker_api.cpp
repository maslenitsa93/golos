#include <boost/test/unit_test.hpp>

#include "database_fixture.hpp"
#include "helpers.hpp"

#include <golos/plugins/worker_api/worker_api_plugin.hpp>

using namespace golos::plugins::worker_api;

struct worker_api_fixture : public golos::chain::database_fixture {
    worker_api_fixture() : golos::chain::database_fixture() {
        initialize<golos::plugins::worker_api::worker_api_plugin>();
        open_database();
        startup();
    }
};

BOOST_FIXTURE_TEST_SUITE(worker_api_plugin, worker_api_fixture)

// TODO: Start write tests in #1025 issue

BOOST_AUTO_TEST_SUITE_END()
