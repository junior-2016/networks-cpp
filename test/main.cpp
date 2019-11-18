//
// Created by junior on 2019/11/5.
//
#include "../src/include/networks-cpp.h"
#include "../third-party/google/googletest/googletest/include/gtest/gtest.h"

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
//}

TEST(GraphTest, lemon_graph_create) {
    using namespace lemon;
    constexpr size_t v_size = 1000000, e_size = v_size * 10;
    using VertexId   = int;
    using WeightType = int;
    using Graph = SmartGraph;
    using Node = Graph::Node;
    using WeightMap = Graph::EdgeMap<WeightType>;

    Graph g;
    WeightMap weights(g);
    std::vector<Node> vertices;

    std::uniform_int_distribution<VertexId> dis(0, v_size - 1);  // random select vertex
    std::uniform_int_distribution<WeightType> dis_w(1, 100);        // weights
    std::random_device device;

    vertices.reserve(v_size);
    for (size_t i = 0; i < v_size; i++) {
        vertices.emplace_back(g.addNode());
    }

    for (size_t i = 0; i < e_size; i++) {
        auto start = dis(device), end = dis(device);
        auto e = g.addEdge(vertices[start], vertices[end]);
        weights[e] = dis_w(device);
    }
    std::cout << "vertex_number:" << countNodes(g) << "\nedge_number:" << countEdges(g) << "\n";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
