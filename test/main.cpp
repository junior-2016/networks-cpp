//
// Created by junior on 2019/11/5.
//
#include <utility>

#include "../src/include/networks-cpp.h"
#include "../third-party/google/googletest/googletest/include/gtest/gtest.h"
#include "../src/include/Graph.h"

struct Vertex {
    std::string name;

    explicit Vertex(std::string name) : name(std::move(name)) {}

    bool operator==(const Vertex &other) const {
        return name == other.name;
    }

    friend std::ostream &operator<<(std::ostream &out, const Vertex &v) {
        out << v.name;
        return out;
    }
};
namespace std {
    template<>
    struct hash<Vertex> {
        size_t operator()(const Vertex &vertex) const {
            return std::hash<std::string>{}(vertex.name);
        }
    };
}

template<typename V, typename WeightType>
struct Edge {
private:
    V v1, v2;
    WeightType w;
public:

    Edge(V v1, V v2, WeightType w) : v1(std::move(v1)), v2(std::move(v2)), w(w) {}

    Edge(std::pair<V, V> &vs, WeightType w) : v1(vs.first), v2(vs.second), w(w) {}

    [[nodiscard]] V begin() const { return v1; }

    [[nodiscard]] V end() const { return v2; }

    [[nodiscard]] constexpr WeightType weight() const { return w; }
};

TEST(GraphTest, GraphCreate) {
    using namespace networks_cpp;
    using E = Edge<Vertex, size_t>;
    Vertex v1{"v1"}, v2{"v2"}, v3{"v3"};
    HashSet<Vertex> vertices = {v1, v2, v3};
    Vector<E> edges = {
            E{v1, v2, 1},
            E{v1, v3, 2},
            E{v2, v3, 5},
            E{v1, v2, 10},
            E{v2, v2, 17}
    };
    Graph<GraphType::directed, Vertex, Edge, size_t> graph(vertices, edges, "graph_one");
    graph.print();

    Graph<GraphType::undirected, Vertex, Edge, size_t> graph_(vertices, edges, "graph_second");
    graph_.print();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
