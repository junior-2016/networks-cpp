//
// Created by junior on 2019/11/5.
//
#include <utility>

#include "../src/include/networks-cpp.h"
#include "../third-party/google/googletest/googletest/include/gtest/gtest.h"
#include "../src/include/Graph.h"

template<auto type>
void set_edge(networks_cpp::Graph<std::string, int, type> &graph) {
    graph.add_edge("v1", "v2", 1);
    graph.add_edge("v1", "v2", 10);
    graph.add_edge("v1", "v3", 2);
    graph.add_edge("v3", "v1", 4);
    graph.add_edge("v2", "v2", 7);
    graph.add_edge("v2", "v2", 17);
    graph.add_edge("v2", "v3", 5);
}

TEST(GraphTest, GraphPrint) {
    using namespace networks_cpp;
    Graph<std::string, int, GraphType::directed> graph1("one");
    Graph<std::string, int, GraphType::undirected> graph2("second");
    set_edge(graph1);
    set_edge(graph2);
    graph1.print();
    graph2.print();
}

TEST(GraphTest, GraphCreate) {
    using namespace networks_cpp;
    constexpr size_t v_size = 1000000, e_size = v_size * 10;
    using Vertex = int_fast32_t;
    using WeightType = int_fast8_t;
    std::uniform_int_distribution<Vertex> dis(0, v_size - 1);  // random select vertex
    std::uniform_int_distribution<WeightType> dis_w(1, 100);        // weights
    std::random_device device;
    Graph<Vertex, WeightType, GraphType::undirected> g;
    for (size_t i = 0; i < e_size; i++) {
        auto start = dis(device), end = dis(device);
        g.add_edge(start, end, dis_w(device));
    }
}

//TEST(Temp, HashMap) {
//    using namespace networks_cpp;
//    HashMap<Vertex, HashMap<Vertex, size_t>> map;
//    Vertex v("v"), k("k"), m("m");
//
//    auto pos = map.try_emplace(v, HashMap<Vertex, size_t>{});
//    std::cout << std::boolalpha << pos.second << "\n";
//    ((*pos.first).second).try_emplace(k, 1);
//    std::cout << "Value:" << map[v][k] << "\n\n";
//
//    pos = map.try_emplace(v, HashMap<Vertex, size_t>{});
//    std::cout << std::boolalpha << pos.second << "\n";
//    ((*pos.first).second).try_emplace(m, 2);
//    std::cout << "Value:" << map[v][k] << " " << map[v][m] << "\n\n";
//
//    pos = map.try_emplace(v, HashMap<Vertex, size_t>{});
//    auto pos_ = ((*pos.first).second).try_emplace(k, 3);
//    if (pos_.second == false) {
//        (*pos_.first).second += 3;
//    }
//    std::cout << "Value:" << map[v][k] << " " << map[v][m] << "\n\n";
//}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
