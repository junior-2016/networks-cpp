## networks-cpp
### build & run
```bash
$ git clone https://github.com/junior-2016/networks-cpp.git
$ cd networks-cpp
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make -j n 
(n is thread number)
$ ./networks_cpp_test
```

### example
```c++
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
```
result:(memory usage about 150MB ~ 200MB)
```
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from GraphTest
[ RUN      ] GraphTest.lemon_graph_create
vertex_number:1000000
edge_number:10000000
[       OK ] GraphTest.lemon_graph_create (4900 ms)
[----------] 1 test from GraphTest (4900 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (4900 ms total)
[  PASSED  ] 1 test.
```