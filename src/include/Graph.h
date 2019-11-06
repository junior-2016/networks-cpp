//
// Created by junior on 2019/11/5.
//

#ifndef NETWORKS_CPP_GRAPH_H
#define NETWORKS_CPP_GRAPH_H

#include <utility>

#include "networks-cpp.h"

namespace networks_cpp {
    enum class GraphType : uint32_t {
        undirected = 1,
        directed = 2,
    };

    // TODO: after concept-Ts, add constrain to Edge class => begin()/end()/weight().
    template<auto type,
            typename Vertex,
            template<typename/*Vertex*/, typename WeightType> typename Edge,
            typename WeightType>
    class Graph {
        static_assert(std::is_same_v<decltype(type), GraphType>);
        static_assert((type == GraphType::undirected) || (type == GraphType::directed));
    public:
        Graph(const HashSet <Vertex> &vertices,
              const Vector <Edge<Vertex, WeightType>> &edges,
              std::string graph_name = "anonymous_graph")
                : vertices(vertices), graph_name(std::move(graph_name)) {
            adjacency_matrix.reserve(vertices.size());
            for (const auto &edge : edges) {
                // 下面的代码可以处理: 简单图/带重边/带自环边/带权图/有向图/无向图
                if constexpr (type == GraphType::directed) {
                    adjacency_matrix[edge.begin()][edge.end()] += edge.weight();
                } else {
                    adjacency_matrix[edge.begin()][edge.end()] += edge.weight();
                    adjacency_matrix[edge.end()][edge.begin()] += edge.weight();
                }
            }
        }

        void print() const {
            std::cout << "{\n";
            std::cout << "\tname: " << graph_name << "\n";
            if constexpr (type == GraphType::undirected) { std::cout << "\ttype: undirected graph\n"; }
            else { std::cout << "\ttype: directed graph\n"; }
            std::cout << "\tedges: \n";
            std::for_each(adjacency_matrix.begin(), adjacency_matrix.end(), [](const auto &item) {
                std::for_each(item.second.begin(), item.second.end(), [&](const auto &item_) {
                    std::cout << "\t\t" << item.first << "," << item_.first << "," << item_.second << "\n";
                });
            });
            std::cout << "}\n";
        }

    private:
        std::string graph_name;
        HashSet <Vertex> vertices;
        HashMap <Vertex, HashMap<Vertex, WeightType>> adjacency_matrix;
    };
}
#endif //NETWORKS_CPP_GRAPH_H
