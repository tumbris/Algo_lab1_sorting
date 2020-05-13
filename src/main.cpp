#include <Sortings.h>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <fstream>
#include <cassert>

int main(int argc, char **argv)
{
    std::random_device r;
    std::default_random_engine dre(r());
#if 1
    std::vector<std::int32_t> vec;
    vec.reserve(100000);

    std::uniform_int_distribution<std::int32_t> uid(std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max());

    auto generator = [&]()
    {
        return uid(dre);
    };

    std::generate_n(std::back_inserter(vec), vec.capacity(), std::move(generator));
#elif 0
    std::vector<std::int32_t> vec{ 1, 2, 3, 5, 4, 7, 6, 8, 10, 9, 16, 11, 17, 13, 12, 14};

#elif 1
    std::vector<std::int32_t> vec(50);
    std::iota(vec.begin(), vec.end(), 0);
    //std::reverse(vec.begin(), vec.end());
    std::ofstream f("arr.txt");

    std::shuffle(vec.begin(), vec.end(), dre);
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<decltype(vec)::value_type>(f, ", "));
    f.close();
#elif 1
    std::vector vec{ 16, 28, 8, 7, 1, 49, 17, 0, 15, 14, 41, 5, 43, 11, 37, 44, 12, 33, 40, 24, 20, 10, 45, 3, 35, 39, 2, 34, 13, 46, 26, 47, 27, 36, 6, 29, 38, 21, 42, 32, 23, 18, 9, 31, 4, 19, 25, 30, 48, 22 };

#endif
    sg::block_qsort(vec.begin(), vec.end());

    assert(std::is_sorted(vec.begin(), vec.end()));

    return EXIT_SUCCESS;
}