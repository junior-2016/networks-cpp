//
// Created by junior on 2019/11/5.
//
#include "../src/include/networks-cpp.h"
#include "../third-party/google/googletest/googletest/include/gtest/gtest.h"
#include "../src/include/Graph.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
//template<auto type>
//void set_edge(networks_cpp::Graph<std::string, int, type> &graph) {
//    graph.add_edge("v1", "v2", 1);
//    graph.add_edge("v1", "v2", 10);
//    graph.add_edge("v1", "v3", 2);
//    graph.add_edge("v3", "v1", 4);
//    graph.add_edge("v2", "v2", 7);
//    graph.add_edge("v2", "v2", 17);
//    graph.add_edge("v2", "v3", 5);
//}
//
//TEST(GraphTest, GraphPrint) {
//    using namespace networks_cpp;
//    Graph<std::string, int, GraphType::directed> graph1("graph1");
//    Graph<std::string, int, GraphType::undirected> graph2("graph2");
//    set_edge(graph1);
//    set_edge(graph2);
//    std::cout << graph1;
//    std::cout << graph2;
//    graph1.remove_edge("v1", "v2");
//    graph2.remove_edge("v3", "v1");
//    std::cout << graph1;
//    std::cout << graph2;
//    graph1.clear_vertex_data("v1");
//    graph2.clear_vertex_data("v1");
//    std::cout << graph1;
//    std::cout << graph2;
//}

constexpr size_t v_size = 1000000, e_size = v_size * 10;
using Vertex = int_fast32_t;
using WeightType = int_fast8_t;
networks_cpp::Graph<Vertex, WeightType, networks_cpp::GraphType::undirected> g("large_graph", v_size);
TEST(GraphTest, GraphCreate) {
    using namespace networks_cpp;
    std::uniform_int_distribution<Vertex> dis(0, v_size - 1);  // random select vertex
    std::uniform_int_distribution<WeightType> dis_w(1, 100);        // weights
    std::random_device device;
    for (size_t i = 0; i < e_size; i++) {
        auto start = dis(device), end = dis(device);
        g.add_edge(start, end, dis_w(device));
    }
}

TEST(GraphTest, GraphToCSR) {
    af::array csr_matrix = g.convert_to_csr_storage();
    std::cout << csr_matrix.dims() << "\n";
}

TEST(GraphTest, TestBoostGraph) {
    using namespace boost;
    using edge_weight_property = property<edge_weight_t, WeightType>;
    using graph = adjacency_list<multisetS, vecS, directedS, no_property, edge_weight_property>;
    std::uniform_int_distribution<Vertex> dis(0, v_size - 1);  // random select vertex
    std::uniform_int_distribution<WeightType> dis_w(1, 100);        // weights
    std::random_device device;
    graph g_;
    for (size_t i = 0; i < e_size; i++) {
        auto start = dis(device), end = dis(device);
        add_edge(start, end, dis_w(device), g_);
    }
}

//TEST(ArrayFireTest, ArrayFireBackendInfo) {
//    af::info(); // print backend information
//    af_print(af::randu(5, 4));
//}
//
//TEST(ArrayFireTest, SomeApi) {
//    af::array a = af::randu(1000000, af::dtype::f32);
//    gfor (af::seq i, a.dims(0)) a(i) = a(i) + 1;
//    std::cout << af::sum<float>(a) << "\n";
//}
//
//TEST(ArrayFireTest, ArrayFireSpareMatrix) {
//    int row[] = {0, 3, 5, 7, 7};
//    int col[] = {0, 1, 2, 0, 1, 0, 2};
//    float values[] = {3, 3, 4, 3, 10, 4, 3};
//    af::array rr = sparse(4, 3, af::array(7, values),
//                          af::array(5, row), af::array(7, col));
//    af_print(rr);
//    af_print(dense(rr));
//
//}

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
