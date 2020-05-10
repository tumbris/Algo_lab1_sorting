#include <Utility.h>

#include <vector>
#include <array>
#include <type_traits>
#include <algorithm>
#include <limits>
#include <future>

namespace sg
{
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