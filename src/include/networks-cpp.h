//
// Created by junior on 2019/11/5.
//

#ifndef NETWORKS_CPP_NETWORKS_CPP_H
#define NETWORKS_CPP_NETWORKS_CPP_H

// IO include
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

// C++ std include
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <bitset>
#include <algorithm>
#include <utility>
#include <memory>
#include <functional>
#include <chrono>
#include <random>
#include <limits>

// TODO: use parallelism-ts to accelerate some algorithm (such as std::for_each).
//  GCC had implemented it by TBB backend.
#ifdef USE_CXX_PARALLISM_TS

#include <execution> // C++17 parallelism TS header,only be implemented in libstdc++(GCC 9)

#endif

// C std include
#include <cstdint>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <cmath>
#include <cassert>

// third-party include
#include <arrayfire.h>

namespace networks_cpp {
    // forward declaration
    template<typename Vertex,
            typename WeightType,
            auto type>
    class Graph;

    enum class GraphType : uint32_t;

    template<typename T>
    using Vector = std::vector<T>;
    template<typename T, size_t num>
    using Array = std::array<T, num>;
    template<typename T>
    using List = std::list<T>;

    template<typename K,
            typename V,
            typename Hash = std::hash<K>,
            typename Pred = std::equal_to<K>,
            typename Alloc = std::allocator<std::pair<const K, V>>>
    using HashMap = std::unordered_map<K, V, Hash, Pred, Alloc>;
    template<typename K,
            typename V,
            typename Compare = std::less<K>,
            typename Alloc = std::allocator<std::pair<const K, V>>>
    using TreeMap = std::map<K, V, Compare, Alloc>;

    template<typename K,
            typename Hash = std::hash<K>,
            typename Pred = std::equal_to<K>,
            typename Alloc = std::allocator<K>>
    using HashSet = std::unordered_set<K, Hash, Pred, Alloc>;

    template<typename K,
            typename Compare = std::less<K>,
            typename Alloc = std::allocator<K> >
    using TreeSet = std::set<K, Compare, Alloc>;
}

#endif //NETWORKS_CPP_NETWORKS_CPP_H
