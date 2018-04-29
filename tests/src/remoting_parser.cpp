#include <boost/test/unit_test.hpp>

#include <caf/all.hpp>

#include <protocol.hpp>
#include <remoting.hpp>
#include <utils.hpp>


BOOST_AUTO_TEST_CASE(core_parser_message_same_size) {

    using namespace caf;

    for(int i = 0; i < 100; ++i)
    {
        actor_system_config conf{};
        actor_system system{ conf };

        auto data = utils::random_string(666);

        auto env = remoting::envelope{ actor_id{666}, make_message(data) }.to_message(remoting::protocol::message_to, system);

        BOOST_CHECK(!env != true);

        auto buf = env->to_buffer();

        remoting::parser parser;

        remoting::protocol::message msg{remoting::protocol::discover, 0, ""};
        auto res = parser.parse(msg, buf.begin(), buf.end());

        auto state_res = res.state == remoting::parser::result_state::parsed;
        BOOST_CHECK(state_res);

        BOOST_CHECK(msg.action == env->action);
        BOOST_CHECK(msg.size == env->size);
        BOOST_CHECK(msg.body == env->body);

        auto e = remoting::envelope::from_message(msg, system);

        BOOST_CHECK(!e != true);

        BOOST_CHECK(e->id == 666);
    }
}
