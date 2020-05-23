#pragma once

#include <ostream>
#include <chrono>
#include <array>

namespace sg
{
    using SortTestDration = std::chrono::duration<double, std::milli>;

    enum class Distribution : std::uint8_t
    {
        Normal,
        Uniform
    };

    struct TestConfig
    {
        std::size_t length = 0;
        std::uint32_t upper = 0;

        Distribution distribution = Distribution::Uniform;
    };

    struct ConfigCluster
    {
        std::array<TestConfig, 4> n30K;
        std::array<TestConfig, 4> n100K;
        std::array<TestConfig, 4> n300K;
        std::array<TestConfig, 4> n1000K;
    };

    struct TestResult
    {
        SortTestDration introsort_time;
        SortTestDration bucket_sort_time;
        SortTestDration block_qsort_time;
        SortTestDration library_sort_time;
        SortTestDration std_stable_sort_time;
        SortTestDration std_sort_time;

        std::size_t length = 0;
        std::uint32_t upper = 0;

        Distribution distribution = Distribution::Uniform;
    };

    struct TestClusterResult
    {
        //Time in ms
        std::array<TestResult, 4> n30K;
        std::array<TestResult, 4> n100K;
        std::array<TestResult, 4> n300K;
        std::array<TestResult, 4> n1000K;
    };

    SortTestDration measure_introsort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_bucket_sort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_block_qsort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_library_sort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_std_stable_sort(std::size_t N, std::uint32_t upper, Distribution dist);
    SortTestDration measure_std_sort(std::size_t N, std::uint32_t upper, Distribution dist);


    std::array<ConfigCluster, 5> GenerateTestConfigClusters();
    std::array<TestClusterResult, 5> RunTests(const std::array<ConfigCluster, 5>& config);

    std::string GetTestResultsAsString(const std::array<TestClusterResult, 5>& results);
}