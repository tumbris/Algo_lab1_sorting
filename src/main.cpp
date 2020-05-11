#include <Sortings.h>
#include <iostream>
#include <iterator>
#include <random>
#include <cassert>

int main(int argc, char **argv)
{
#if 1
    std::vector<std::int32_t> vec;
    vec.reserve(1000);

    std::random_device r;
    std::default_random_engine dre(r());
    std::uniform_int_distribution<std::int32_t> uid(std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max());

    auto generator = [&]()
    {
        return uid(dre);
    };

    std::generate_n(std::back_inserter(vec), vec.capacity(), std::move(generator));
#else 
    std::vector<std::int32_t> vec{ 1, 2, 3, 5, 4, 7, 6, 8, 10, 9, 16, 11, 17, 13, 12, 14};
#endif
    sg::introsort(vec.begin(), vec.end());

    assert(std::is_sorted(vec.begin(), vec.end()));

    return EXIT_SUCCESS;
}