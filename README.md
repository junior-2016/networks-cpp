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
TEST(GraphTest, GraphCreate) {
    using namespace networks_cpp;
    constexpr size_t v_size = 1000000, e_size = v_size * 10;
    using Vertex = int_fast32_t;
    using WeightType = int_fast8_t;
    std::uniform_int_distribution<Vertex> dis(0, v_size - 1);  // random select vertex
    std::uniform_int_distribution<WeightType> dis_w(1, 100);        // weights
    std::random_device device;
    Graph<Vertex, WeightType, GraphType::undirected> g("large_graph", v_size);
    for (size_t i = 0; i < e_size; i++) {
        auto start = dis(device), end = dis(device);
        g.add_edge(start, end, dis_w(device));
    }
}
```
result:(memory usage about 520MB)
```
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from GraphTest
[ RUN      ] GraphTest.GraphCreate
[       OK ] GraphTest.GraphCreate (16878 ms)
[----------] 1 test from GraphTest (16878 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (16878 ms total)
[  PASSED  ] 1 test.
```