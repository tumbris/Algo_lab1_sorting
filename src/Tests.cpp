#include <Tests.h>
#include <Sortings.h>

#include <algorithm>
#include <numeric>
#include <cassert>
#include <vector>
#include <iostream>
#include <random>

namespace sg
{
    static constexpr std::size_t BENCHMARK_NUMBER = 1000;

    struct ScopedTimer
    {
        ScopedTimer(std::chrono::high_resolution_clock::duration& out_duration) : out_duration(out_duration)
        {
            start = std::chrono::high_resolution_clock::now();
        }
        ~ScopedTimer()
        {
            out_duration = std::chrono::high_resolution_clock::now() - start;
        }

    private:
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::duration& out_duration;
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