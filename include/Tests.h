#pragma once

#include <ostream>
#include <chrono>

namespace sg
{
    using SortTestDration = std::chrono::high_resolution_clock::duration;

    enum class Distribution : std::uint8_t
    {
        Normal,
        Uniform
    };

    struct TestConfig
    {
        std::uint32_t introsort_upper = 0;
        std::uint32_t bucket_sort_upper = 0;
        std::uint32_t block_qsort_upper = 0;
        std::uint32_t library_sort_upper = 0;
        std::uint32_t std_stable_sort_upper = 0;
        std::uint32_t std_sort_upper = 0;

        std::size_t length = 0;

        Distribution distribution = Distribution::Uniform;
    };
    struct TestResult
    {
        SortTestDration introsort_time;
        SortTestDration bucket_sort_time;
        SortTestDration block_qsort_time;
        SortTestDration library_sort_time;
        SortTestDration std_stable_sort_time;
        SortTestDration std_sort_time;
    };

    SortTestDration measure_introsort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_bucket_sort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_block_qsort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_library_sort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_std_stable_sort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_std_sort(std::size_t N, std::uint32_t upper, Distribution dist);
}