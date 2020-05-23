#include <Tests.h>
#include <Sortings.h>

#include <algorithm>
#include <numeric>
#include <cassert>
#include <vector>
#include <sstream>
#include <random>
#include <future>
#include <map>
#include <iomanip>

namespace std
{
    template <>
    struct less<sg::TestConfig>
    {
        bool operator()(const sg::TestConfig& a, const sg::TestConfig& b) const
        {
            using DistType = std::underlying_type_t<sg::Distribution>;
            return a.length < b.length ||
                (!(a.length < b.length) && a.upper < b.upper) ||
                (!(a.length < b.length) && !(a.upper < b.upper) && static_cast<DistType>(a.distribution) < static_cast<DistType>(b.distribution));
        }
    };
}

namespace sg
{
    static constexpr std::size_t BENCHMARK_NUMBER = 1000;

    struct ScopedTimer
    {
        ScopedTimer(SortTestDration& out_duration) : out_duration(out_duration)
        {
            start = std::chrono::high_resolution_clock::now();
        }
        ~ScopedTimer()
        {
            out_duration = std::chrono::duration_cast<SortTestDration>(std::chrono::high_resolution_clock::now() - start);
        }

    private:
        std::chrono::high_resolution_clock::time_point start;
        SortTestDration& out_duration;
    };

    template <class Iterator>
    void generate_uniform_distributed(Iterator first, Iterator last, it_value_t<Iterator> upper);

    template<class Iterator>
    void generate_normal_distributed(Iterator first, Iterator last, it_value_t<Iterator> upper);

    SortTestDration measure_introsort(std::size_t N, std::uint32_t upper, Distribution dist)
    {
        std::vector<std::uint32_t> vec(N);
        switch (dist)
        {
        case Distribution::Normal: generate_normal_distributed(vec.begin(), vec.end(), upper); break;
        case Distribution::Uniform: generate_uniform_distributed(vec.begin(), vec.end(), upper); break;
        default: assert(false); break;
        }

        SortTestDration res;
        {
            ScopedTimer timer(res);
            introsort(vec.begin(), vec.end());
        }
        assert(std::is_sorted(vec.begin(), vec.end()));

        return res;
    }

    SortTestDration measure_bucket_sort(std::size_t N, std::uint32_t upper, Distribution dist)
    {
        std::vector<std::uint32_t> vec(N);
        switch (dist)
        {
        case Distribution::Normal: generate_normal_distributed(vec.begin(), vec.end(), upper); break;
        case Distribution::Uniform: generate_uniform_distributed(vec.begin(), vec.end(), upper); break;
        default: assert(false); break;
        }

        SortTestDration res;
        {
            ScopedTimer timer(res);
            bucket_sort(vec.begin(), vec.end());
        }
        assert(std::is_sorted(vec.begin(), vec.end()));

        return res;
    }

    SortTestDration measure_block_qsort(std::size_t N, std::uint32_t upper, Distribution dist)
    {
        std::vector<std::uint32_t> vec(N);
        switch (dist)
        {
        case Distribution::Normal: generate_normal_distributed(vec.begin(), vec.end(), upper); break;
        case Distribution::Uniform: generate_uniform_distributed(vec.begin(), vec.end(), upper); break;
        default: assert(false); break;
        }

        SortTestDration res;
        {
            ScopedTimer timer(res);
            block_qsort(vec.begin(), vec.end());
        }
        assert(std::is_sorted(vec.begin(), vec.end()));

        return res;
    }

    SortTestDration measure_library_sort(std::size_t N, std::uint32_t upper, Distribution dist)
    {
        std::vector<std::uint32_t> vec(N);
        switch (dist)
        {
        case Distribution::Normal: generate_normal_distributed(vec.begin(), vec.end(), upper); break;
        case Distribution::Uniform: generate_uniform_distributed(vec.begin(), vec.end(), upper); break;
        default: assert(false); break;
        }

        SortTestDration res;
        {
            ScopedTimer timer(res);
            library_sort(vec.begin(), vec.end());
        }
        assert(std::is_sorted(vec.begin(), vec.end()));

        return res;
    }

    SortTestDration measure_std_stable_sort(std::size_t N, std::uint32_t upper, Distribution dist)
    {
        std::vector<std::uint32_t> vec(N);
        switch (dist)
        {
        case Distribution::Normal: generate_normal_distributed(vec.begin(), vec.end(), upper); break;
        case Distribution::Uniform: generate_uniform_distributed(vec.begin(), vec.end(), upper); break;
        default: assert(false); break;
        }

        SortTestDration res;
        {
            ScopedTimer timer(res);
            std::stable_sort(vec.begin(), vec.end());
        }
        assert(std::is_sorted(vec.begin(), vec.end()));

        return res;
    }

    SortTestDration measure_std_sort(std::size_t N, std::uint32_t upper, Distribution dist)
    {
        std::vector<std::uint32_t> vec(N);
        switch (dist)
        {
        case Distribution::Normal: generate_normal_distributed(vec.begin(), vec.end(), upper); break;
        case Distribution::Uniform: generate_uniform_distributed(vec.begin(), vec.end(), upper); break;
        default: assert(false); break;
        }

        SortTestDration res;
        {
            ScopedTimer timer(res);
            std::sort(vec.begin(), vec.end());
        }
        assert(std::is_sorted(vec.begin(), vec.end()));

        return res;
    }

    std::array<ConfigCluster, 5> GenerateTestConfigClusters()
    {
        std::array<ConfigCluster, 5> res;

        for (std::size_t i = 0; i < res.size(); ++i)
        {
            std::uint32_t upper_src = 1;
            {
                auto upper = upper_src << 31;
                res[i].n30K[0] = { 30'000, upper, Distribution::Uniform };
                res[i].n100K[0] = { 100'000, upper, Distribution::Uniform };
                res[i].n300K[0] = { 300'000, upper, Distribution::Uniform };
                res[i].n1000K[0] = { 1'000'000, upper, Distribution::Uniform };
            }
            {
                res[i].n30K[1] = { 30'000, 30'000 - 1, Distribution::Uniform };
                res[i].n100K[1] = { 100'000, 100'000 - 1, Distribution::Uniform };
                res[i].n300K[1] = { 300'000, 300'000 - 1, Distribution::Uniform };
                res[i].n1000K[1] = { 1'000'000, 1'000'000 - 1, Distribution::Uniform };
            }
            {
                auto upper = upper_src << 15;
                res[i].n30K[2] = { 30'000, upper, Distribution::Uniform };
                res[i].n100K[2] = { 100'000, upper, Distribution::Uniform };
                res[i].n300K[2] = { 300'000, upper, Distribution::Uniform };
                res[i].n1000K[2] = { 1'000'000, upper, Distribution::Uniform };
            }
            {
                auto upper = upper_src << 31;
                res[i].n30K[3] = { 30'000, upper, Distribution::Normal };
                res[i].n100K[3] = { 100'000, upper, Distribution::Normal };
                res[i].n300K[3] = { 300'000, upper, Distribution::Normal };
                res[i].n1000K[3] = { 1'000'000, upper, Distribution::Normal };
            }
        }
        return res;

    }

    std::array<TestClusterResult, 5> RunTests(const std::array<ConfigCluster, 5>& config)
    {
        std::array<TestClusterResult, 5> res;
        std::vector<std::future<void>> futures;
        for (std::size_t i = 0; i < std::min(res.size(), config.size()); ++i)
        {
            futures.emplace_back(
                std::async(std::launch::async,
                           [&res, config, i]()
                           {
                               for (std::size_t j = 0; j < res[i].n30K.size(); ++j)
                               {
                                   auto N = res[i].n30K[j].length = config[i].n30K[j].length;
                                   auto upper = res[i].n30K[j].upper = config[i].n30K[j].upper;
                                   auto dist = res[i].n30K[j].distribution = config[i].n30K[j].distribution;

                                   res[i].n30K[j].introsort_time = measure_introsort(N, upper, dist);
                                   res[i].n30K[j].bucket_sort_time = measure_bucket_sort(N, upper, dist);
                                   res[i].n30K[j].block_qsort_time = measure_block_qsort(N, upper, dist);
                                   res[i].n30K[j].library_sort_time = measure_library_sort(N, upper, dist);
                                   res[i].n30K[j].std_stable_sort_time = measure_std_stable_sort(N, upper, dist);
                                   res[i].n30K[j].std_sort_time = measure_std_sort(N, upper, dist);
                               }
                               for (std::size_t j = 0; j < res[i].n100K.size(); ++j)
                               {
                                   auto N = res[i].n100K[j].length = config[i].n100K[j].length;
                                   auto upper = res[i].n100K[j].upper = config[i].n100K[j].upper;
                                   auto dist = res[i].n100K[j].distribution = config[i].n100K[j].distribution;

                                   res[i].n100K[j].introsort_time = measure_introsort(N, upper, dist);
                                   res[i].n100K[j].bucket_sort_time = measure_bucket_sort(N, upper, dist);
                                   res[i].n100K[j].block_qsort_time = measure_block_qsort(N, upper, dist);
                                   res[i].n100K[j].library_sort_time = measure_library_sort(N, upper, dist);
                                   res[i].n100K[j].std_stable_sort_time = measure_std_stable_sort(N, upper, dist);
                                   res[i].n100K[j].std_sort_time = measure_std_sort(N, upper, dist);
                               }
                               for (std::size_t j = 0; j < res[i].n300K.size(); ++j)
                               {
                                   auto N = res[i].n300K[j].length = config[i].n300K[j].length;
                                   auto upper = res[i].n300K[j].upper = config[i].n300K[j].upper;
                                   auto dist = res[i].n300K[j].distribution = config[i].n300K[j].distribution;

                                   res[i].n300K[j].introsort_time = measure_introsort(N, upper, dist);
                                   res[i].n300K[j].bucket_sort_time = measure_bucket_sort(N, upper, dist);
                                   res[i].n300K[j].block_qsort_time = measure_block_qsort(N, upper, dist);
                                   res[i].n300K[j].library_sort_time = measure_library_sort(N, upper, dist);
                                   res[i].n300K[j].std_stable_sort_time = measure_std_stable_sort(N, upper, dist);
                                   res[i].n300K[j].std_sort_time = measure_std_sort(N, upper, dist);

                               }
                               for (std::size_t j = 0; j < res[i].n1000K.size(); ++j)
                               {
                                   auto N = res[i].n1000K[j].length = config[i].n1000K[j].length;
                                   auto upper = res[i].n1000K[j].upper = config[i].n1000K[j].upper;
                                   auto dist = res[i].n1000K[j].distribution = config[i].n1000K[j].distribution;

                                   res[i].n1000K[j].introsort_time = measure_introsort(N, upper, dist);
                                   res[i].n1000K[j].bucket_sort_time = measure_bucket_sort(N, upper, dist);
                                   res[i].n1000K[j].block_qsort_time = measure_block_qsort(N, upper, dist);
                                   res[i].n1000K[j].library_sort_time = measure_library_sort(N, upper, dist);
                                   res[i].n1000K[j].std_stable_sort_time = measure_std_stable_sort(N, upper, dist);
                                   res[i].n1000K[j].std_sort_time = measure_std_sort(N, upper, dist);
                               }
                           }));
        }

        for (auto& future : futures)
        {
            future.wait();
        }

        return res;
    }

    std::string GetTestResultsAsString(const std::array<TestClusterResult, 5>& results)
    {
        std::stringstream ss;

        std::map<TestConfig, double> introsort_time;
        std::map<TestConfig, double> bucket_sort_time;
        std::map<TestConfig, double> block_sort_time;
        std::map<TestConfig, double> library_sort_time;
        std::map<TestConfig, double> std_stable_sort_time;
        std::map<TestConfig, double> std_sort_time;

        for (const auto& result : results)
        {
            for (const auto& n30K : result.n30K)
            {
                TestConfig key{ n30K.length, n30K.upper, n30K.distribution };
                introsort_time[key] += n30K.introsort_time.count();
                bucket_sort_time[key] += n30K.bucket_sort_time.count();
                block_sort_time[key] += n30K.block_qsort_time.count();
                library_sort_time[key] += n30K.library_sort_time.count();
                std_stable_sort_time[key] += n30K.std_stable_sort_time.count();
                std_sort_time[key] += n30K.std_sort_time.count();
            }

            for (const auto& n100K : result.n100K)
            {
                TestConfig key{ n100K.length, n100K.upper, n100K.distribution };
                introsort_time[key] += n100K.introsort_time.count();
                bucket_sort_time[key] += n100K.bucket_sort_time.count();
                block_sort_time[key] += n100K.block_qsort_time.count();
                library_sort_time[key] += n100K.library_sort_time.count();
                std_stable_sort_time[key] += n100K.std_stable_sort_time.count();
                std_sort_time[key] += n100K.std_sort_time.count();
            }

            for (const auto& n300K : result.n300K)
            {
                TestConfig key{ n300K.length, n300K.upper, n300K.distribution };
                introsort_time[key] += n300K.introsort_time.count();
                bucket_sort_time[key] += n300K.bucket_sort_time.count();
                block_sort_time[key] += n300K.block_qsort_time.count();
                library_sort_time[key] += n300K.library_sort_time.count();
                std_stable_sort_time[key] += n300K.std_stable_sort_time.count();
                std_sort_time[key] += n300K.std_sort_time.count();
            }

            for (const auto& n1000K : result.n1000K)
            {
                TestConfig key{ n1000K.length, n1000K.upper, n1000K.distribution };
                introsort_time[key] += n1000K.introsort_time.count();
                bucket_sort_time[key] += n1000K.bucket_sort_time.count();
                block_sort_time[key] += n1000K.block_qsort_time.count();
                library_sort_time[key] += n1000K.library_sort_time.count();
                std_stable_sort_time[key] += n1000K.std_stable_sort_time.count();
                std_sort_time[key] += n1000K.std_sort_time.count();
            }
        }

        introsort_time[{results[0].n30K[0].length, results[0].n30K[0].upper, results[0].n30K[0].distribution}] /= results.size();
        bucket_sort_time[{results[0].n30K[0].length, results[0].n30K[0].upper, results[0].n30K[0].distribution}] /= results.size();
        block_sort_time[{results[0].n30K[0].length, results[0].n30K[0].upper, results[0].n30K[0].distribution}] /= results.size();
        library_sort_time[{results[0].n30K[0].length, results[0].n30K[0].upper, results[0].n30K[0].distribution}] /= results.size();
        std_stable_sort_time[{results[0].n30K[0].length, results[0].n30K[0].upper, results[0].n30K[0].distribution}] /= results.size();
        std_sort_time[{results[0].n30K[0].length, results[0].n30K[0].upper, results[0].n30K[0].distribution}] /= results.size();

        introsort_time[{results[0].n100K[0].length, results[0].n100K[0].upper, results[0].n100K[0].distribution}] /= results.size();
        bucket_sort_time[{results[0].n100K[0].length, results[0].n100K[0].upper, results[0].n100K[0].distribution}] /= results.size();
        block_sort_time[{results[0].n100K[0].length, results[0].n100K[0].upper, results[0].n100K[0].distribution}] /= results.size();
        library_sort_time[{results[0].n100K[0].length, results[0].n100K[0].upper, results[0].n100K[0].distribution}] /= results.size();
        std_stable_sort_time[{results[0].n100K[0].length, results[0].n100K[0].upper, results[0].n100K[0].distribution}] /= results.size();
        std_sort_time[{results[0].n100K[0].length, results[0].n100K[0].upper, results[0].n100K[0].distribution}] /= results.size();

        introsort_time[{results[0].n300K[0].length, results[0].n300K[0].upper, results[0].n300K[0].distribution}] /= results.size();
        bucket_sort_time[{results[0].n300K[0].length, results[0].n300K[0].upper, results[0].n300K[0].distribution}] /= results.size();
        block_sort_time[{results[0].n300K[0].length, results[0].n300K[0].upper, results[0].n300K[0].distribution}] /= results.size();
        library_sort_time[{results[0].n300K[0].length, results[0].n300K[0].upper, results[0].n300K[0].distribution}] /= results.size();
        std_stable_sort_time[{results[0].n300K[0].length, results[0].n300K[0].upper, results[0].n300K[0].distribution}] /= results.size();
        std_sort_time[{results[0].n300K[0].length, results[0].n300K[0].upper, results[0].n300K[0].distribution}] /= results.size();

        introsort_time[{results[0].n1000K[0].length, results[0].n1000K[0].upper, results[0].n1000K[0].distribution}] /= results.size();
        bucket_sort_time[{results[0].n1000K[0].length, results[0].n1000K[0].upper, results[0].n1000K[0].distribution}] /= results.size();
        block_sort_time[{results[0].n1000K[0].length, results[0].n1000K[0].upper, results[0].n1000K[0].distribution}] /= results.size();
        library_sort_time[{results[0].n1000K[0].length, results[0].n1000K[0].upper, results[0].n1000K[0].distribution}] /= results.size();
        std_stable_sort_time[{results[0].n1000K[0].length, results[0].n1000K[0].upper, results[0].n1000K[0].distribution}] /= results.size();
        std_sort_time[{results[0].n1000K[0].length, results[0].n1000K[0].upper, results[0].n1000K[0].distribution}] /= results.size();

        auto getDistStr = [](Distribution d)
        {
            switch (d)
            {
            case Distribution::Normal: return "Normal";
            case Distribution::Uniform: return "Uniform";
            default: return "UNKNOWN";
            }
        };

        auto printRes = [&](const std::map<TestConfig, double>& data)
        {
            for (const auto& d : data)
            {
                ss << "\tLength - " << std::setw(12) << std::left << d.first.length << ", ";
                ss << "Upper - " << std::setw(12) << std::left << d.first.upper << ", ";
                ss << "Distribution - " << std::setw(12) << std::left << getDistStr(d.first.distribution) << ": ";
                ss << d.second << " ms\n";
            }
        };

        ss << "Introsort:\n";
        printRes(introsort_time);

        ss << "Bucket sort:\n";
        printRes(bucket_sort_time);

        ss << "Block quick sort:\n";
        printRes(block_sort_time);

        ss << "Library sort:\n";
        printRes(library_sort_time);

        ss << "std::stable_sort:\n";
        printRes(std_stable_sort_time);

        ss << "std::sort:\n";
        printRes(std_sort_time);

        return ss.str();
    }


    template<class Iterator>
    void generate_uniform_distributed(Iterator first, Iterator last, it_value_t<Iterator> upper)
    {
        using value_type = it_value_t<Iterator>;

        std::random_device r;
        std::mt19937_64 re(r());

        auto min = value_type{ 0 };
        auto max = upper;

        if constexpr (std::is_integral_v<value_type>)
        {
            std::uniform_int_distribution<value_type> ud(min, max);
            std::generate(first, last, [&]() { return ud(re); });
        }
        else
        {
            std::uniform_real_distribution<value_type> ud(min, max);
            std::generate(first, last, [&]() { return ud(re); });
        }
    }

    template<class Iterator>
    void generate_normal_distributed(Iterator first, Iterator last, it_value_t<Iterator> upper)
    {
        using value_type = it_value_t<Iterator>;

        std::random_device r;
        std::mt19937_64 re(r());

        auto min = value_type{ 0 };
        auto max = upper;

        std::normal_distribution nd(0.0, 1.0);
        std::generate(first, last,
                      [&]()
                      {
                          double d = nd(re);
                          auto res = map_and_clamp(-5.0, 5.0, 0.0, static_cast<double>(upper), d);
                          return static_cast<value_type>(res);
                      });
    }
}