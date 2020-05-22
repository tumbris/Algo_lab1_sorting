#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <ostream>
#include <fstream>
#include <memory>
#include <cassert>

namespace sg
{
    namespace details
    {
        namespace bucket_sort
        {
            static constexpr std::size_t INTEGER_BASE = 10;
        }
        namespace introsort
        {
            static constexpr std::size_t INTROSORT_THRESHOLD = 16;
        }
        namespace block_qsort
        {
            static constexpr std::size_t BLOCK_SIZE = 128; //https://dl.acm.org/doi/fullHtml/10.1145/3274660
            static constexpr std::size_t BQSORT_THRESHOLD = 16;
        }
        namespace lib_sort
        {
            static constexpr std::size_t EPSILON = 1;
        }
    }

    template <class Iterator>
    using it_value_t = std::remove_reference_t<decltype(std::declval<Iterator>().operator*())>;

    std::ostream& GetOutputStream(char** argv, std::ofstream& fileStream);


    template <class T, class Y>
    constexpr T map_and_clamp(T start, T end, Y to_start, Y to_end, std::common_type_t<T, Y> value)
    {
        assert(start != end);
        auto slope = (to_end - to_start) / (end - start);
        auto res = to_start + slope * (value - start);
        if (res > to_end) res = to_end;
        else if (res < to_start) res = to_start;
        return res;
    }
}