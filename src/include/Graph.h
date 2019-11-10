//
// Created by junior on 2019/11/5.
//

#ifndef NETWORKS_CPP_GRAPH_H
#define NETWORKS_CPP_GRAPH_H

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
        /**
         * 这里用的是HashTable存储(HTS hash table storage format);
         * 并且提供了接口支持 HTS -> CSR (compressed sparse row) 的切换.
         *   TODO:后续可能提供 template argument 来控制 HTS/CSR/CSC/COO 的自由切换
         */
        using Adjacency_list_type = HashMap<Vertex, WeightType>;
        using iterator = typename Adjacency_list_type::iterator;
        using const_iterator = typename Adjacency_list_type::const_iterator;
        using VertexIdType = uint32_t;

        /**
         * 获得 adjacency_matrix[begin][end] 的迭代器
         */
        inline std::optional<iterator> get_iterator(VertexIdType begin_id, const Vertex &end) {
            if (begin_id < adjacency_matrix.size()) {
                if (iterator itr = adjacency_matrix[begin_id].find(end); itr != adjacency_matrix[begin_id].end()) {
                    return itr;
                }
                return std::nullopt;
            }
            return std::nullopt;
        }

        /**
         *  获得 adjacency_matrix[begin][end] 的只读迭代器
         */
        inline std::optional<const_iterator> get_iterator(VertexIdType begin_id, const Vertex &end) const {
            if (begin_id < adjacency_matrix.size()) {
                if (const_iterator c_itr = adjacency_matrix[begin_id].find(end);
                        c_itr != adjacency_matrix[begin_id].end()) {
                    return c_itr;
                }
                return std::nullopt;
            }
            return std::nullopt;
        }


        /**
         * 获得权重值,但是不做任何检查,仅用于访问已确定的邻居权重的时候! 得到的权重仅是单方向的!
         * 对于无向图,获得该值后还需要注意: 1. 自环边权重要乘以二; 2. 另一个方向的权重值也是存在的.
         * @return
         */
        inline WeightType get_weight_arc_without_check(VertexIdType begin_id, const Vertex &end) const {
            return adjacency_matrix[begin_id][end];
        }

        /**
         * 获得权重,会检查无效边,但依旧只获得单方向的权重.
         * 对于无向图,需要注意:
         * 1.获得该值且该值非0时,如果是自环边权重要乘以二;
         * 2.如果获得值为0,可能要考虑得到另一个方向的权重值是否非0(因为无向图也只保留了一边的信息).
         * @return
         */
        inline WeightType get_weight_arc_with_check(VertexIdType begin_id, const Vertex &end) const {
            if (auto itr = get_iterator(begin_id, end);itr != std::nullopt) return (*(itr.value())).second;
            return 0;
        }

        /**
         * insert_edge_arc 仅插入单方向的权重值.
         * 1.对于 std::map,lower_bound find pos + emplace_hint(pos,args..)/try_emplace(pos,key,value)
         *   才有作用(lower_bound在红黑树里面查找插入位置,emplace_hint加速插入);
         *   而对 std::unordered_map, emplace_hint(pos,args..)/try_emplace(pos,key,value)中的 pos参数 只是作为接口兼容,
         *   没有任何加速作用(unordered_map是根据Hash value来插入元素的).
         * 2.对于try_emplace(key,value),返回 pair<iterator,bool>.若之前已经插入相同的key,那么bool为false;
         *   如果是第一次插入key,bool为true.
         */
        inline void insert_edge_arc(VertexIdType begin_id, const Vertex &end, WeightType weight) {
            if (auto ret = adjacency_matrix[begin_id].try_emplace(end, weight);ret.second == false) {
                (*ret.first).second += weight;
            }
        }

        /**
         * insert_edge: 无论有向图/无向图都只保留单方向的信息.因为已经存在有向图,无向图的类型了,所以无向图的邻接矩阵不需要存两个方向的信息.
         * 而在获取实际的权重时,需要针对两种类型写不同的处理代码!!!
         * TODO: On Networks 3rd book, author define directed edge A[i][j]= 1 when j -> i.
         */
        inline void insert_edge(const Vertex &begin, const Vertex &end,
                                VertexIdType begin_id, VertexIdType end_id, WeightType weight) {
            if constexpr (type == GraphType::directed) {
                insert_edge_arc(begin_id, end, weight);
            } else {
                if (begin_id == end_id) { // 自环边不用查询
                    insert_edge_arc(begin_id, end, weight);
                } else {
                    // 对于重边(非自环),如果是无向图,先出现 A - B, 后面出现 B - A, 需要先查询有没有 A - B,
                    // 然后把 B - A 的权重加到 A - B 上(也就是说权重只加到一个方向上).
                    if (auto itr = get_iterator(end_id, begin); itr != std::nullopt) {
                        (*(itr.value())).second += weight;
                    } else {
                        insert_edge_arc(begin_id, end, weight);
                    }
                }
            }
        }

        /**
         * 添加顶点并返回顶点的编号
         */
        inline VertexIdType add_vertex(const Vertex &v) {
            auto ret = vertices.try_emplace(v, global_vertex_id);
            if (ret.second == true) { // 插入新顶点,需要创建新的Adjacency list,同时更新global_vertex_id;
                adjacency_matrix.emplace_back(Adjacency_list_type{});
                global_vertex_id++;
            }
            return (*ret.first).second;
        }

    public:
        /**
         * 创建图
         * @param vertex_reserve_number 预设的顶点数量大小,用于提前分配内存,避免后期频繁扩容.
         * @param graph_name
         */
        explicit Graph(std::string graph_name = "anonymous_graph", size_t vertex_reserve_number = 20)
                : n(0), m(0), graph_name(std::move(graph_name)), global_vertex_id(0) {
            vertices.reserve(vertex_reserve_number);
            adjacency_matrix.reserve(vertex_reserve_number);
        }

        void add_edge(const Vertex &begin, const Vertex &end, WeightType weight) {
            auto begin_id = add_vertex(begin);
            auto end_id = add_vertex(end);
            m++;
            n = vertices.size();
            insert_edge(begin, end, begin_id, end_id, weight);
        }

        WeightType get_weight(const Vertex &begin, const Vertex &end) const {
            if constexpr (type == GraphType::directed) {
                if (auto begin_v = vertices.find(begin);begin_v != vertices.end()) {
                    return get_weight_arc_with_check((*begin_v).second, end);
                }
                return 0;
            } else {
                if (auto begin_v = vertices.find(begin);begin_v != vertices.end()) {
                    auto ret = get_weight_arc_with_check((*begin_v).second, end);
                    if (ret == 0) { // 返回0也可能是因为这里访问的是无向图的另一边
                        if (auto end_v = vertices.find(end);end_v != vertices.end()) {
                            return get_weight_arc_with_check((*end_v).second, begin);
                        }
                        return 0;
                    }
                    return (begin == end) ? ret * 2 : ret; // 自环边处理
                }
                return 0;
            }
        }

        [[nodiscard]] inline constexpr size_t vertex_number() const { return n; }

        [[nodiscard]] inline constexpr size_t edge_number() const { return m; }

        friend std::ostream &operator<<(std::ostream &out, const Graph<Vertex, WeightType, type> &g) {
            if (g.n > 20) {
                std::cerr << "print() only for small graph. Your graph is too large.\n";
                std::exit(-1);
            }
            out << "{\n";
            out << "\tname: " << g.graph_name << "\n";
            out << "\tvertex number: " << g.n << "\n";
            out << "\tedge number: " << g.m << "\n";

            if constexpr (type == GraphType::undirected) out << "\ttype: undirected graph\n";
            else out << "\ttype: directed graph\n";

            out << "\tedges: \n";
            std::for_each(g.vertices.begin(), g.vertices.end(), [&](const auto &begin_v) {
                const auto &adjacency_list = g.adjacency_matrix[begin_v.second];
                std::for_each(adjacency_list.begin(), adjacency_list.end(), [&](const auto &end_v) {
                    if constexpr (type == GraphType::directed) {
                        out << "\t\t" << begin_v.first << "->" << end_v.first << " weight:" << end_v.second << "\n";
                    } else {
                        out << "\t\t" << begin_v.first << "<->" << end_v.first << " weight:" << end_v.second << "\n";
                    }
                });
            });

            out << "\tadjacency_matrix: \n";
            out << "\t\t\t";
            std::for_each(g.vertices.begin(), g.vertices.end(), [&](const auto &v) { out << v.first << "\t"; });
            out << "\n";
            std::for_each(g.vertices.begin(), g.vertices.end(), [&](const auto &begin) {
                out << "\t\t" << begin.first << " ";
                std::for_each(g.vertices.begin(), g.vertices.end(), [&](const auto &end) {
                    out << g.get_weight(begin.first, end.first) << "\t";
                });
                out << "\n";
            });
            out << "}\n";
            return out;
        }

        // Transfer internal Hash Table Storage to array fire sparse matrix
        // (use cuda/opencl backend)
        // 参考: https://stackoverflow.com/questions/49867065/arrayfire-sparse-matrix-issues
        // TODO: 这里只针对HTS存储的单向边数据转成CSR,没有针对无向图的处理.
        //      后续有可能添加,但要看后续的算法是否需要对完整的邻接矩阵进行复杂计算(计算需要考虑对称的部分)
        af::array convert_to_csr_storage() {
            Vector<int> rows;
            Vector<int> cols;
            Vector<float> weights;
            rows.reserve(n + 1);
            cols.reserve(m);
            weights.reserve(m);
            int step = 0;
            for (size_t row = 0; row < adjacency_matrix.size(); ++row) {
                const auto &adj = adjacency_matrix[row];
                rows.emplace_back(step);
                step += adj.size();
                std::for_each(adj.begin(), adj.end(), [&](const auto &item) {
                    cols.emplace_back(vertices[item.first]);
                    weights.emplace_back(item.second);
                });
            }
            rows.emplace_back(step);
            return af::sparse(n, n, af::array(weights.size(), weights.data()),
                              af::array(rows.size(), rows.data()),
                              af::array(cols.size(), cols.data()),
                              af::storage::AF_STORAGE_CSR);
        }

    private:
        size_t n, m; // n: vertex number; m: edge number.
        std::string graph_name;
        VertexIdType global_vertex_id;
        HashMap <Vertex, VertexIdType> vertices;
        Vector <Adjacency_list_type> adjacency_matrix;
    };
}
#endif //NETWORKS_CPP_GRAPH_H
