#pragma once

#include <appbase/plugin.hpp>
#include <golos/chain/database.hpp>

#include <golos/plugins/json_rpc/plugin.hpp>

namespace golos { namespace plugins { namespace worker_api {

    using namespace golos::chain;

    // TODO: DEFINE_API_ARGS

    class worker_api_plugin final : public appbase::plugin<worker_api_plugin> {
    public:
        APPBASE_PLUGIN_REQUIRES((json_rpc::plugin))

        worker_api_plugin();

        ~worker_api_plugin();

        void set_program_options(
            boost::program_options::options_description& cli,
            boost::program_options::options_description& cfg) override;

        void plugin_initialize(const boost::program_options::variables_map& options) override;

        void plugin_startup() override;

        void plugin_shutdown() override;

        static const std::string& name();

        // TODO: DECLARE_API

    private:
        class worker_api_plugin_impl;

        std::unique_ptr<worker_api_plugin_impl> my;
    };

} } } //golos::plugins::worker_api
