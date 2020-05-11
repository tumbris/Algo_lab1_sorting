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
        RandomAccessIt partition(RandomAccessIt first, RandomAccessIt last)
        {
            auto index = [=](RandomAccessIt it) -> std::int64_t { return std::distance(first, it); };
            auto element = [=](std::size_t index) -> decltype(*first) { return *std::next(first, index); };
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

        template <class RandomAccessIt>
        void introsort_impl(RandomAccessIt first, RandomAccessIt last, std::size_t depth_limit)
        {
            auto size = std::distance(first, last) + 1;

            if (size < details::INTROSORT_THRESHOLD)
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

    template <class RandomAccessIt>
    void introsort(RandomAccessIt begin, RandomAccessIt end)
    {
        std::size_t depth_limit = 2 * static_cast<std::size_t>(std::log(static_cast<double>(std::distance(begin, end))));
        details::introsort_impl(begin, std::prev(end), depth_limit);
    }

    template <class ForwardIt>
    void bucket_sort(ForwardIt first, ForwardIt last)
    {
        using value_type = std::remove_reference_t<decltype(*first)>;
        using bucket_t = std::vector<value_type>;

        static_assert(std::is_integral_v<value_type>, "This realization of bucket_sort supports only integer types!");

        static constexpr auto max_value = std::numeric_limits<value_type>::max();
        static constexpr std::int64_t interval = max_value / details::INTEGER_BASE;

        std::array<bucket_t, details::INTEGER_BASE * 2 + 1 /*for negative numbers too*/> buckets;

        for (auto head = first; head != last; ++head)
        {
            value_type temp = *head;
            std::int64_t temp_index = temp / interval;
            std::size_t index = temp_index + details::INTEGER_BASE;
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
}