#include <cstdio>

#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <iostream>
#include <unordered_map>
#include <set>
#include <functional>
#include <cassert>
#include <iterator>

#include <boost/optional.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>


using namespace std;

namespace detail {
    string gen_random_string(int length = 10) {
        static const string abc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXWZ0123456789";

        string ret;
        random_device rd;
        std::uniform_int_distribution<int> dist(0, abc.size() - 1);
        for (int i = 0; i < length; ++i) {
            ret += abc[dist(rd)];
        }

        return ret;
    }

    int power_low_rand(double y, int a, int b) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> d(0, 1);

        auto x1 = pow(b, y + 1);
        auto x0 = pow(a, y + 1);
        auto ex = (double)1 / (double)(y + 1);
        auto ret = pow((x1 - x0)*d(gen) + x0, ex);

        return b - round(ret) + a;
    }
};

class node {
public:
    explicit node(int size) :
        id_(detail::gen_random_string()),
        memory_size_(size)
    {
        assert(size > 0);
    }

    string id() const {
        return id_;
    }

    int size() const {
        return memory_size_;
    }

    using id_t = string;
    using timestamp_t = chrono::time_point<chrono::steady_clock>;

    struct connection_t {
        id_t id;
        timestamp_t timestamp;
    };

    using connected_nodes_t = boost::multi_index_container<
        connection_t,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<BOOST_MULTI_INDEX_MEMBER(connection_t, id_t, id)>,
            boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(connection_t, timestamp_t, timestamp)>
        >
    >;

    //using nodes_list_t = set<connection_t>;

    connected_nodes_t nodes() const {
        auto ret = nodes_;
        ret.insert({ id_, chrono::steady_clock::now() }); //always add self
        return ret;
    }

    void disconnect_from(node::id_t const& cn) {
        auto it = nodes_.erase(cn);
    }

    boost::optional<connection_t> connect_to(node::id_t const& other) {
        boost::optional<connection_t> ret = boost::none;
        if (nodes_.size() == memory_size_) {
            auto& timestamp_index = nodes_.get<1>();
            ret = boost::make_optional(*timestamp_index.begin());
            timestamp_index.erase(timestamp_index.begin());
        }
        nodes_.insert({ other, chrono::steady_clock::now()});
        return ret;
    }

    void sync_with(node const& other) {
        sync_with(other.nodes());
    }

    void sync_with(connected_nodes_t const& other_nodes) {
        //unite

        connected_nodes_t un(nodes_.begin(), nodes_.end());
        for (auto& node : other_nodes) {
            if(node.id != id()) //don't add self
                un.insert(node);
        }

        //cut first N
        connected_nodes_t ret;
        int cnt = 0;
        for (auto& n : un.get<1>()) {
            ret.insert({n.id, n.timestamp});
            cnt++;
            if (cnt == memory_size_)
                break;
        }

        nodes_.swap(ret);
    }

    int memory_size() const {
        return memory_size_;
    }

private:
    id_t id_;
    int memory_size_;
    connected_nodes_t nodes_;
};

class universe {
private:
    using nodes_index_t = unordered_map<node::id_t, int>;
    nodes_index_t nodes_index_;
public:
    using nodes_list_t = vector<node>;

    class graph_metrics {
    public:
        graph_metrics(nodes_list_t const& nodes) :
            cc_(vector<pair<node::id_t, int>>{}),
            ccc_(calc_connected_components(nodes)),
            //conn_to_all_(calc_conn_to_all(nodes)),
            dd_(calc_degee_distribution(nodes))
            //,dia_(calc_diameter(nodes))
        {
        }

        void print(bool print_cc = false) const {
            cout << "Connected componentes number: " << ccc_ << endl;
            if (print_cc) {
                cout << "Connected components: " << endl;
                for (int i = 0; i < cc_.size(); ++i) {
                    cout << cc_[i].first << " " << cc_[i].second << endl;
                }
                cout << endl;
            }
            //cout << "Connected to all: " << conn_to_all_ << endl;
            //cout << "Diameter: " << dia_ << endl;
            cout << "First 1000 of degree distribution:" << endl;
            for (int i = 0; i < min<int>(1000, dd_.size()); ++i) {
                cout << dd_[i] << " ";
            }
            cout << endl;
        }

    private:
        int calc_connected_components(nodes_list_t const& nodes) {
            unordered_map<node::id_t, bool> used;
            unordered_map<node::id_t, const node&> nd;
            for (auto& n : nodes) {
                used[n.id()] = false;
                nd.insert({ n.id(), n });
            }

            function<void(node const& n, int i)> dfs = [&used, &dfs, &nd, this](node const& n, int i) -> void {
                used[n.id()] = true;
                cc_.emplace_back(n.id(), i);
                for (auto& unode : n.nodes()) {
                    if (!used[unode.id]) {
                        dfs(nd.find(unode.id)->second, i);
                    }
                }
            };

            int cnt = 0;
            for (auto& n : nodes) {
                if (!used[n.id()]) {
                    dfs(n, cnt);
                    cnt++;
                }
            }

            return cnt;
        }

        //int calc_conn_to_all(nodes_list_t const& nodes) {
        //    unordered_map<node::id_t, int> conn_cnt;
        //    unordered_map<node::id_t, bool> used;
        //    unordered_map<node::id_t, const node&> nd;
        //    for (auto& n : nodes) {
        //        used[n.id()] = false;
        //        conn_cnt[n.id()] = 0;
        //        nd.insert({ n.id(), n });
        //    }

        //    function<int (node const& n)> dfs = [&conn_cnt, &dfs, &nd, &used](node const& n) -> int {
        //        used[n.id()] = true;
        //        int ret = 0;
        //        for (auto& unode : n.nodes()) {
        //            if (!used[unode.first]) {
        //                ret += dfs(nd.find(unode.first)->second);
        //            }
        //        }

        //        return ret + 1;
        //    };

        //    //for (auto& n : nodes) {
        //    //    conn_cnt[n.id()] = dfs(n);
        //    //    cout << "Not connected to " << n.id() << endl;
        //    //    for (auto& m : nodes) {
        //    //        if (!used[m.id()])
        //    //            cout << m.id() << " ";
        //    //        used[m.id()] = false;
        //    //        cout << endl;
        //    //    }
        //    //}

        //    int ret = 0;
        //    for (auto& cc: conn_cnt) {
        //        assert(cc.second <= nodes.size());
        //        if (cc.second == nodes.size()) {
        //            ret++;
        //        }
        //    }

        //    //int cnt = 0;
        //    //for (auto& c : conn_cnt) {
        //    //    //if (cnt++ == 20)
        //    //    //    break;
        //    //    cout << c.second << " ";
        //    //}
        //    //cout << endl;

        //    return ret;
        //}

        vector<int> calc_degee_distribution(nodes_list_t const& nodes) {
            vector<int> ret(nodes.size(), 0);
            for (auto& n : nodes) {
                ret[n.nodes().size()]++;
            }
            return ret;
        }

        long long calc_diameter(nodes_list_t const& nodes) {
            unordered_map<node::id_t, int> node_num;
            
            const auto INF = numeric_limits<long long>::max() / 3;
            int cnt = 0;
            vector<vector<long long> > g(nodes.size());
            for (auto& v : nodes) {
                node_num[v.id()] = cnt++;
                g[node_num[v.id()]].resize(nodes.size(), INF);
            }

            for (auto& v : nodes) {
                for (auto& u : v.nodes()) {
                    g[node_num[v.id()]][node_num[u.id]] = 1;
                    g[node_num[u.id]][node_num[v.id()]] = 1;
                }
            }

            for (int k = 0; k < nodes.size(); ++k) {
                for (int i = 0; i < nodes.size(); ++i) {
                    for (int j = 0; j < nodes.size(); ++j) {
                        g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
                    }
                }
            }

            long long mx = 0;

            for (int k = 0; k < nodes.size(); ++k) {
                for (int i = 0; i < nodes.size(); ++i) {
                    mx = max(mx, g[k][i]);
                }
            }
            return mx;
        }

        vector<pair<node::id_t, int>> cc_;
        int ccc_;
        //int conn_to_all_;
        vector<int> dd_;
        //long long dia_;
    };

    void add_node(int memory_size) {
        auto n = node{ memory_size };
        nodes_index_[n.id()] = nodes_.size();
        nodes_.push_back(move(n));
    }

    node& get_random_node() {
        std::random_device rd;

        std::uniform_int_distribution<int> dist_v(0, nodes_.size() - 1);
        return nodes_[dist_v(rd)];
    }

    node& get_node(node::id_t const& id) {
        return nodes_[nodes_index_[id]];
    }

    graph_metrics get_metrics() const {
        return graph_metrics(nodes_);
    }

    void print() const {
        for (auto& node : nodes_) {
            cout << node.id() << " ";
            for (auto& unode : node.nodes()) {
                cout << unode.id << " ";
            }
            cout << endl;
        }
    }

    nodes_list_t& nodes() {
        return nodes_;
    }

    pair<node, node> get_random_connected_pair() {
        std::random_device rd;

        std::uniform_int_distribution<int> dist_v(0, graph_.size() - 1);
        auto vi = dist_v(rd);

        std::uniform_int_distribution<int> dist_u(0, graph_[vi].size() - 1);
        auto ui = dist_u(rd);

        return { nodes_[vi], nodes_[graph_[vi][ui]] };
    }

    void check_symmetry() {
        for (auto& v : nodes_) {
            for (auto& u : v.nodes()) {
                bool f = false;
                for (auto& mv : get_node(u.id).nodes()) {
                    if (mv.id == v.id())
                        f = true;
                }

                assert(f);
            }
        }
    }

    bool is_directly_connected(node const& u, node const& v) {
        return true;
    }
protected:
    nodes_list_t nodes_;

    vector<vector<int>> graph_;
};

class world {
    universe un_;

    //const int NODE_NUM = 100'000;
    //const int MEMORY_SIZE = 1'000;

    //const int ITERATION_NUM = 10'000;

    int NODE_NUM = 1'000;
    int MIN_MEMORY_SIZE = 5;
    int MAX_MEMORY_SIZE = 50;
    int ITERATION_NUM = 100;

public:
    void init(int node_num, int min_mem_size, int max_mem_size, int iter_num) {
        NODE_NUM = node_num;
        MIN_MEMORY_SIZE = min_mem_size;
        MAX_MEMORY_SIZE = max_mem_size;
        ITERATION_NUM = iter_num;

        for (int i = 0; i < NODE_NUM; ++i) {
            //un_.add_node(MEMORY_SIZE);
            un_.add_node(detail::power_low_rand(2.3, MIN_MEMORY_SIZE, MAX_MEMORY_SIZE));
        }
    }

    void run() {
        //un_.print();
        cout << "Degree distribution (2.3) of memory size" << endl;
        //cout << "Local node sync with 0.1 random" << endl;
        cout << "Random node sync" << endl;
        un_.get_metrics().print();
        for (int i = 0; i < ITERATION_NUM; ++i) {
            cout << "Iteration #" << i << endl;
            for (auto&& n : un_.nodes()) {
                run_one(ref(n), un_);

                //check invariants
                //un_.check_symmetry();
            }

            //un_.print();
            un_.get_metrics().print();
        }
    }
protected:
    virtual void run_one(node& n, universe & un) = 0;
};

class simple_world : public world {
protected:
    virtual void run_one(node& v, universe & univ) {
        auto find_random = [&v, &univ] () -> node& {
            while (true) {
                auto& u = univ.get_random_node();
                if (u.id() != v.id()) {
                    return u;
                }
            }
        };

        auto findu = [&v, &univ, &find_random]() -> node& {
            const auto& vnodes = v.nodes();

            //if (vnodes.size() < 2)
                return find_random();



            std::random_device rd;
            //std::uniform_int_distribution<int> dist_fr(0, 9);

            //if (dist_fr(rd) == 0) {
            //    return find_random();
            //}

            std::uniform_int_distribution<int> dist_v(0, vnodes.size() - 1);
            auto it = vnodes.begin();
            advance(it, dist_v(rd));

            return univ.get_node(it->id);
        };

        auto& u = [&v, findu, &univ]() -> node& {
            while (true) {
                auto& u = findu();
                if (u.id() != v.id() && univ.is_directly_connected(v, u))
                    return u;
            }
        }();

        //cout << "Sync: " << v.id() << " " << u.id() << endl;
        auto vn = v.nodes();
        auto un = u.nodes();

        v.sync_with(un);
        u.sync_with(vn);

        //disconnect removed
        auto calc_diff = [](node::connected_nodes_t const& orig, node::connected_nodes_t const& ne) {
            set<node::id_t> n;

            for (auto& nd : ne) {
                n.insert(nd.id);
            }

            node::connected_nodes_t ret;
            for (auto& nd : orig) {
                if (n.find(nd.id) == n.end()) {
                    ret.insert(nd);
                }
            }

            return ret;
        };

        for (auto& n : calc_diff(vn, v.nodes())) {
            if (n.id != v.id()) {
                //cout << "Removed from " << v.id() << " " << n.id << endl;
                univ.get_node(n.id).disconnect_from(v.id());
            }
        }

        for (auto& n : calc_diff(un, u.nodes())) {
            if (n.id != u.id()) {
                //cout << "Removed from " << u.id() << " " << n.id << endl;
                univ.get_node(n.id).disconnect_from(u.id());
            }
        }
        
        //conect newly added

        //cout << "Add: ";
        for (auto& n : calc_diff(v.nodes(), vn)) {
            //cout << n.second << " ";
            if (n.id != u.id() ) {
                //cout << "Connect to " << v.id() << " " << n.id << endl;
                auto on = univ.get_node(n.id).connect_to(v.id());
                if (on) {
                    //cout << "Removed from " << on->id << " " << n.id << endl;
                    univ.get_node(on->id).disconnect_from(n.id);
                }
            }
        }
        //cout << endl;

        //cout << "Add: ";
        for (auto& n : calc_diff(u.nodes(), un)) {
            //cout << n.id << " ";
            if (n.id != v.id()) {
                //cout << "Connect to " << u.id() << " " << n.id << endl;
                auto on = univ.get_node(n.id).connect_to(u.id());
                if (on) {
                    //cout << "Removed from " << on->id << " " << n.id << endl;
                    univ.get_node(on->id).disconnect_from(n.id);
                }
            }
        }
        //cout << endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cout << "Usage: " << endl;
        cout << "noma_peer_sampling <node_num> <min_memory_size> <max_memory_size> <iteration_num>" << endl;
        return -1;
    }

    cout
        << "Nodes num: " << atoll(argv[1]) << endl
        << "Memory min size: " << atoll(argv[2]) << endl
        << "Memory max size: " << atoll(argv[3]) << endl
        << "Iteration num: " << atoll(argv[4]) << endl;
    
    simple_world w;
    w.init(atoll(argv[1]), atoll(argv[2]), atoll(argv[3]), atoll(argv[4]));
    w.run();

    return 0;
}
