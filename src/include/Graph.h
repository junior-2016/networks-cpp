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

    template<typename Vertex,
            typename WeightType,
            auto type>
    class Graph {
        static_assert(std::is_same_v<decltype(type), GraphType>);
        static_assert((type == GraphType::undirected) || (type == GraphType::directed));
    private:
        using iterator = typename HashMap<Vertex, WeightType>::iterator;
        using const_iterator = typename HashMap<Vertex, WeightType>::const_iterator;

        /**
         * 获得 adjacency_matrix[begin][end] 的迭代器
         */
        inline std::optional<iterator> get_iterator(const Vertex &begin, const Vertex &end) {
            if (auto pos = adjacency_matrix.find(begin); pos != adjacency_matrix.end()) {
                if (iterator pos_ = ((*pos).second).find(end); pos_ != ((*pos).second).end()) {
                    return pos_;
                }
                return std::nullopt;
            }
            return std::nullopt;
        }

        /**
         *  获得 adjacency_matrix[begin][end] 的只读迭代器
         */
        inline std::optional<const_iterator> get_iterator(const Vertex &begin, const Vertex &end) const {
            if (auto pos = adjacency_matrix.find(begin); pos != adjacency_matrix.end()) {
                if (const_iterator pos_ = ((*pos).second).find(end); pos_ != ((*pos).second).end()) {
                    return pos_;
                }
                return std::nullopt;
            }
            return std::nullopt;
        }

        /**
         * insert_edge_arc 仅插入单方向的权重值.
         * 优化思路:
         * 1.不应使用 HashMap.operator[] 访问或者创建元素,因为[]访问的同时也是在创建对象,导致内存优化无效.
         * 2.对于 std::map,lower_bound find pos + emplace_hint(pos,args..)/try_emplace(pos,key,value)
         *   才有作用(lower_bound在红黑树里面查找插入位置,emplace_hint加速插入);
         *   而对 std::unordered_map, emplace_hint(pos,args..)/try_emplace(pos,key,value)只是作为接口兼容,
         *   没有任何作用(unordered_map是根据Hash value来插入元素的).
         * 3.对于try_emplace(key,value),返回 pair<iterator,bool>.若之前已经插入相同的key,那么bool为false;
         *   如果是第一次插入key,bool为true.
         */
        inline void insert_edge_arc(const Vertex &begin, const Vertex &end, WeightType weight) {
            auto pos = adjacency_matrix.try_emplace(begin, HashMap<Vertex, WeightType>{});
            auto pos_ = ((*pos.first).second).try_emplace(end, weight);
            if (pos_.second == false) (*pos_.first).second += weight;
        }

        /**
         * insert_edge: 无论有向图/无向图都只保留单方向的信息.因为已经存在有向图,无向图的类型了,所以无向图的邻接矩阵不需要存两个方向的信息.
         * 而在获取实际的权重时,需要针对两种类型写不同的处理代码!!!
         * TODO: On Networks 3rd book, author define directed edge A[i][j]= 1 when j -> i.
         */
        inline void insert_edge(const Vertex &begin, const Vertex &end, WeightType weight) {
            if constexpr (type == GraphType::directed) {
                insert_edge_arc(begin, end, weight);
            } else {
                // 自环边不用查询
                if (begin == end) {
                    insert_edge_arc(begin, end, weight);
                } else {
                    // 对于重边(非自环),如果是无向图,先出现 A - B, 后面出现 B - A, 需要先查询有没有 A - B,
                    // 然后把 B - A 的权重加到 A - B 上(也就是说权重只加到一个方向上).
                    if (std::optional<iterator> itr = get_iterator(end, begin); itr != std::nullopt) {
                        (*(itr.value())).second += weight;
                    } else {
                        insert_edge_arc(begin, end, weight);
                    }
                }
            }
        }

    public:
        explicit Graph(std::string graph_name = "anonymous_graph") : n(0), m(0), graph_name(std::move(graph_name)) {}

        void add_edge(const Vertex &begin, const Vertex &end, WeightType weight) {
            vertices.insert(begin);
            vertices.insert(end);
            m++;
            n = vertices.size();
            insert_edge(begin, end, weight);
        }

        void print() const {
            if (n > 20) {
                std::cerr << "print() only for small graph. Your graph is too large.\n";
                std::exit(-1);
            }
            std::cout << "{\n";
            std::cout << "\tname: " << graph_name << "\n";
            std::cout << "\tn: " << n << "\n";
            std::cout << "\tm: " << m << "\n";
            if constexpr (type == GraphType::undirected) { std::cout << "\ttype: undirected graph\n"; }
            else { std::cout << "\ttype: directed graph\n"; }
            std::cout << "\tedges: \n";
            std::for_each(adjacency_matrix.begin(), adjacency_matrix.end(), [](const auto &item) {
                std::for_each(item.second.begin(), item.second.end(), [&](const auto &item_) {
                    if constexpr (type == GraphType::directed) {
                        std::cout << "\t\t" << item.first << "->" << item_.first << " weight:" << item_.second << "\n";
                    } else {
                        std::cout << "\t\t" << item.first << "<->" << item_.first << " weight:" << item_.second << "\n";
                    }
                });
            });
            std::cout << "\tadjacency_matrix: \n";
            std::cout << "\t\t\t";
            std::for_each(vertices.begin(), vertices.end(), [](const auto &v) { std::cout << v << "\t"; });
            std::cout << "\n";
            std::for_each(vertices.begin(), vertices.end(), [&](const auto &begin) {
                std::cout << "\t\t" << begin << " ";
                std::for_each(vertices.begin(), vertices.end(), [&](const auto &end) {
                    if constexpr (type == GraphType::directed) {
                        if (std::optional<const_iterator> c_itr = get_iterator(begin, end); c_itr != std::nullopt) {
                            std::cout << (*(c_itr.value())).second << "\t";
                        } else {
                            std::cout << 0 << "\t";
                        }
                    } else {
                        if (std::optional<const_iterator> c_itr = get_iterator(begin, end); c_itr != std::nullopt) {
                            std::cout << ((begin == end) ? (*(c_itr.value())).second * 2 : (*(c_itr.value())).second)
                                      << "\t";
                        } else if (std::optional<const_iterator> c_itr_ = get_iterator(end, begin);
                                c_itr_ != std::nullopt) {
                            std::cout << ((begin == end) ? (*(c_itr_.value())).second * 2 : (*(c_itr_.value())).second)
                                      << "\t";
                        } else {
                            std::cout << 0 << "\t";
                        }
                    }
                });
                std::cout << "\n";
            });
            std::cout << "}\n";
        }

    private:
        size_t n, m; // n: vertex number; m: edge number.
        std::string graph_name;
        HashSet <Vertex> vertices;
        HashMap <Vertex, HashMap<Vertex, WeightType>> adjacency_matrix;
    };
}
#endif //NETWORKS_CPP_GRAPH_H
