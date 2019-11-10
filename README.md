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
```
result:(memory usage about 340MB ~ 400MB)
```
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from GraphTest
[ RUN      ] GraphTest.GraphCreate
[       OK ] GraphTest.GraphCreate (10023 ms)
[ RUN      ] GraphTest.GraphToCSR
1000000 1000000 1 1
[       OK ] GraphTest.GraphToCSR (1236 ms)
[----------] 2 tests from GraphTest (11259 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (11259 ms total)
[  PASSED  ] 2 tests.
```