#include <cstdint>
#include <cstddef>

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
            static constexpr std::size_t BLOCK_SIZE = 5; //https://dl.acm.org/doi/fullHtml/10.1145/3274660
            static constexpr std::size_t BQSORT_THRESHOLD = 5;
        }
    }

}