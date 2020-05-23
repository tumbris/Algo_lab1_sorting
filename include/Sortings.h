#pragma once

#include <Utility.h>

#include <vector>
#include <array>
#include <type_traits>
#include <algorithm>
#include <limits>
#include <cmath>
#include <optional>
#include <cassert>

namespace sg
{
    namespace details
    {
        template <class RandomAccessIt>
        void insertion_sort(RandomAccessIt first, RandomAccessIt last)
        {
            auto size = last - first;

            for (decltype(size) i = 1; i <= size; ++i)
            {
                auto key = *(first + i);
                auto j = i - 1;

                while (j >= 0 && *(first + j) > key)
                {
                    *(first + (j + 1)) = *(first + j);
                    --j;
                }
                *(first + (j + 1)) = key;
            }
        }

        namespace introsort
        {
            template <class RandomAccessIt>
            RandomAccessIt partition(RandomAccessIt first, RandomAccessIt last)
            {
                auto partition_it = first;
                for (auto head = first; head != last; ++head)
                {
                    if (*head <= *last)
                    {
                        std::swap(*head, *partition_it);
                        ++partition_it;
                    }
                }
                std::swap(*partition_it, *last);
                return partition_it;
            }

            template <class RandomAccessIt>
            void introsort_impl(RandomAccessIt first, RandomAccessIt last, std::size_t depth_limit)
            {
                auto size = std::distance(first, last) + 1;

                if (size < details::introsort::INTROSORT_THRESHOLD)
                {
                    insertion_sort(first, last);
                    return;
                }

                if (depth_limit == 0)
                {
                    std::make_heap(first, last + 1);
                    std::sort_heap(first, last + 1);
                    return;
                }

                auto pivot = first + (size / 2);

                std::swap(*pivot, *last);

                auto partition_point = partition(first, last);
                if (partition_point != first)
                {
                    introsort_impl(first, partition_point - 1, depth_limit - 1);
                }
                if (partition_point != last)
                {
                    introsort_impl(partition_point + 1, last, depth_limit - 1);
                }
            }
        }

        namespace block_qsort
        {
            template <class RandomAccessIt>
            RandomAccessIt block_partition(RandomAccessIt first, RandomAccessIt last, std::remove_reference_t<decltype(*first)> pivot)
            {
                std::array<std::size_t, BLOCK_SIZE> offset_l{ 0 };
                std::array<std::size_t, BLOCK_SIZE> offset_r{ 0 };

                std::size_t start_l = 0;
                std::size_t start_r = 0;
                std::size_t num_l = 0;
                std::size_t num_r = 0;

                auto L = first;
                auto R = last - 1;

                while (/*R - L + 1 > 2 * BLOCK_SIZE*/ false)
                {
                    if (num_l == 0)
                    {
                        start_l = 0;
                        for (std::size_t i = 0; i < BLOCK_SIZE; ++i)
                        {
                            offset_l[num_l] = i;
                            num_l += (pivot <= *(L + i));
                        }
                    }
                    if (num_r == 0)
                    {
                        start_r = 0;
                        for (std::size_t i = 0; i < BLOCK_SIZE; ++i)
                        {
                            offset_r[num_r] = i;
                            num_r += (pivot >= *(R - i));
                        }
                    }
                    auto num = std::min(num_l, num_r);
                    for (std::size_t i = 0; i < num; ++i)
                    {
                        std::swap(*(L + offset_l[start_l + i]), *(R - offset_r[start_r + i]));
                    }
                    num_l -= num;
                    num_r -= num;
                    start_l += num;
                    start_r += num;
                    if (num_l == 0) L += BLOCK_SIZE;
                    if (num_r == 0) R -= BLOCK_SIZE;
                }
                if (L > first) L -= BLOCK_SIZE;
                if (R < last - 1) R += BLOCK_SIZE;

                auto res = std::partition(L, R + 1, [pivot](const auto& em) { return em < pivot; });
                std::partition(res, R + 1, [pivot](const auto& em) { return em == pivot; });
                assert(std::is_partitioned(L, res, [pivot](const auto& em) { return em <= pivot; }));
                return res;
                //return hoares_partition(L, R, pivot);
            }
        }

        namespace lib_sort
        {
            template <class T>
            std::vector<std::optional<T>> prepare_lib_sort(std::size_t epsilon, std::size_t n)
            {
                return std::vector<std::optional<T>>((1 + epsilon) * n, std::nullopt);
            }

            template <class T>
            std::size_t search_free(const std::vector<std::optional<T>>& sorted, const T& element, std::int64_t last)
            {
                std::int64_t first = 0;

                while (last >= 0 && !(sorted[last].has_value()))
                {
                    last--;
                }
                while (first <= last && !(sorted[first].has_value()))
                {
                    first++;
                }

                while (first <= last)
                {
                    std::int64_t middle = (first + last) / 2;
                    if (!(sorted[middle].has_value()))
                    {
                        auto tmp = middle + 1;
                        while (tmp < last && !(sorted[tmp].has_value())) tmp++;
                        if (sorted[tmp].has_value() && sorted[tmp].value() > element)
                        {
                            tmp = middle - 1;
                            while (middle > first && !(sorted[middle].has_value())) middle--;
                            if (!(sorted[middle].has_value()) || sorted[middle] < element)
                            {
                                return middle;
                            }
                            last = middle - 1;
                        }
                        else first = tmp + 1;
                    }
                    else if (sorted[middle].value() < element)
                    {
                        first = middle + 1;
                    }
                    else
                    {
                        last = middle - 1;
                    }
                }
                if (last >= 0 && !(sorted[last].has_value())) last--;
                return last;
            }

            template <class RandomAccessIt>
            void lib_sort(RandomAccessIt first, RandomAccessIt last, std::vector<std::optional<it_value_t<RandomAccessIt>>>& S)
            {
                if (last <= first) return;

                std::int64_t size = last - first;


                std::int64_t goal = 1;
                std::int64_t pos = 1;

                S[0] = *first;

                std::int64_t sLen = std::max(static_cast<std::int64_t>(1 + EPSILON), goal + 1);

                while (pos < size)
                {
                    for (std::int64_t j = 0; j < goal; j++)
                    {
                        std::int64_t insPos = search_free(S, *(first + pos), sLen - 1);
                        insPos++;

                        if (S[insPos].has_value())
                        {
                            std::int64_t nextFree = insPos + 1;
                            while (S[nextFree].has_value()) nextFree++;
                            if (nextFree >= sLen)
                            {
                                insPos--;
                                if (S[insPos].has_value())
                                {
                                    nextFree = insPos - 1;
                                    while (S[nextFree].has_value()) nextFree--;
                                    while (nextFree < insPos)
                                    {
                                        S[nextFree] = S[nextFree + 1];
                                        nextFree++;
                                    }
                                }
                            }
                            else
                            {
                                while (nextFree > insPos)
                                {
                                    S[nextFree] = S[nextFree - 1];
                                    nextFree--;
                                }
                            }
                        }
                        else if (insPos >= sLen)
                        {
                            insPos--;
                            std::int64_t nextFree = insPos - 1;
                            while (S[nextFree].has_value()) nextFree--;
                            while (nextFree < insPos)
                            {
                                S[nextFree] = S[nextFree + 1];
                                nextFree++;
                            }
                        }

                        S[insPos] = *(first + pos++);

                        if (pos >= size)
                            return;
                    }

                    for (std::int64_t j = sLen - 1, k = std::min(goal * (2 + 2 * EPSILON), (1 + EPSILON) * size) - 1,
                         step = (k + 1) / (j + 1); j >= 0; j--, k -= step)
                    {
                        S[k] = S[j];
                        S[j] = std::nullopt;
                    }

                    sLen = std::min(goal * (2 + 2 * EPSILON), size * (1 + EPSILON));
                    goal <<= 1;
                }
            }
        }
    }

    template <class RandomAccessIt>
    void introsort(RandomAccessIt first, RandomAccessIt last)
    {
        std::size_t size = std::distance(first, last);
        if (size > 0)
        {
            std::size_t depth_limit = 2 * static_cast<std::size_t>(std::log(static_cast<double>(size)));
            details::introsort::introsort_impl(first, last - 1, depth_limit);
        }
    }

    template <class ForwardIt>
    void bucket_sort(ForwardIt first, ForwardIt last)
    {
        using namespace details::bucket_sort;

        using value_type = std::remove_reference_t<decltype(*first)>;
        using bucket_t = std::vector<value_type>;

        static_assert(std::is_integral_v<value_type>, "This realization of bucket_sort supports only integer types!");

        static constexpr auto max_value = std::numeric_limits<value_type>::max();
        static constexpr std::int64_t interval = max_value / INTEGER_BASE;

        std::array<bucket_t, INTEGER_BASE * 2 + 1 /*for negative numbers too*/> buckets;

        for (auto head = first; head != last; ++head)
        {
            value_type temp = *head;
            std::int64_t temp_index = temp / interval;
            std::size_t index = temp_index + INTEGER_BASE;
            buckets[index].push_back(*head);
        }

        for (auto& bucket : buckets)
        {
            std::sort(bucket.begin(), bucket.end());
        }

        for (const auto& bucket : buckets)
        {
            for (auto e : bucket)
            {
                *(first++) = e;
            }
        }
    }

    template <class RandomAccessIt>
    void block_qsort(RandomAccessIt first, RandomAccessIt last)
    {
        using namespace details::block_qsort;

        const auto size = last - first;

        if (size > 1)
        {
            if (size < BQSORT_THRESHOLD)
            {
                details::insertion_sort(first, last - 1);
            }
            else
            {
                auto pivot = *(first + (size / 2));
                auto cut = block_partition(first, last, pivot);
                if (cut != first)
                {
                    block_qsort(first, cut);
                }
                if (cut != last)
                {
                    block_qsort(cut + 1, last);
                }
            }
        }
    }

    template <class RandomAccessIt>
    void library_sort(RandomAccessIt first, RandomAccessIt last)
    {
        using namespace details::lib_sort;
        using value_type = std::remove_reference_t<decltype(*first)>;

        if (first < last)
        {
            auto S = prepare_lib_sort<value_type>(EPSILON, last - first);
            lib_sort(first, last, S);

            auto head = first;
            for (auto it = S.begin(); it != S.end() && head != last; ++it)
            {
                if (it->has_value())
                {
                    *(head++) = it->value();
                }
            }
        }
    }
}