#include <Utility.h>

#include <vector>
#include <array>
#include <type_traits>
#include <algorithm>
#include <limits>
#include <future>
#include <cmath>
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

                if (size < details::INTROSORT_THRESHOLD)
                {
                    ::insertion_sort(first, last);
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
            RandomAccessIt hoares_partition(RandomAccessIt first, RandomAccessIt last, std::remove_reference_t<decltype(*first)> pivot)
            {
                while (last - first > 0)
                {
                    while (*first < pivot) ++first;
                    while (*last > pivot) --last;
                    if (last - first > 0) std::swap(*first, *last);
                }
                return first;
            }

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

                while (R - L + 1 > 2 * BLOCK_SIZE)
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
                return hoares_partition(L, R, pivot);
            }
        }
    }

    template <class RandomAccessIt>
    void introsort(RandomAccessIt first, RandomAccessIt last)
    {
        std::size_t depth_limit = 2 * static_cast<std::size_t>(std::log(static_cast<double>(std::distance(first, last))));
        details::introsort_impl(first, last - 1, depth_limit);
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

        if (size > 0)
        {
            if (size < BQSORT_THRESHOLD)
            {
                details::insertion_sort(first, last - 1);
            }
            else
            {
                auto pivot = *(first + (size / 2));
                auto cut = block_partition(first, last, pivot);
                block_qsort(first, cut);
                block_qsort(cut, last);
            }
        }
    }
}