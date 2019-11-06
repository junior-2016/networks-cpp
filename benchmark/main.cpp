//
// Created by junior on 2019/11/5.
//

#include "../src/include/networks-cpp.h"
#include "../third-party/google/benchmark/include/benchmark/benchmark.h"
static void BM_memcpy(benchmark::State& state) {
    char* src = new char[state.range(0)];
    char* dst = new char[state.range(0)];
    memset(src, 'x', state.range(0));
    for (auto _ : state)
        memcpy(dst, src, state.range(0));
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
    delete[] src;
    delete[] dst;
}
BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(1<<10)->Arg(8<<10);

static void BM_DenseRange(benchmark::State& state) {
    for(auto _ : state) {
        std::vector<int> v(state.range(0), state.range(0));
        benchmark::DoNotOptimize(v.data());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DenseRange)->DenseRange(0, 1024, 128);

BENCHMARK_MAIN();