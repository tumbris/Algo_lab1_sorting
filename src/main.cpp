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
    vec.reserve(1000);

    std::uniform_int_distribution<std::int32_t> uid(std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max());

    auto generator = [&]()
    {
        return uid(dre);
    };

    std::generate_n(std::back_inserter(vec), vec.capacity(), std::move(generator));
#elif 0
    std::vector<std::int32_t> vec{ 1, 2, 3, 5, 4, 7, 6, 8, 10, 9, 16, 11, 17, 13, 12, 14};
#elif 0
    std::vector<std::int32_t> vec(20);
    std::iota(vec.begin(), vec.end(), 0);
    //std::reverse(vec.begin(), vec.end());
    std::ofstream f("arr.txt");

    std::shuffle(vec.begin(), vec.end(), dre);
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<decltype(vec)::value_type>(f, ", "));
    f.close();
#elif 1
    std::vector vec{ 1, 7, 17, 18, 12, 0, 5, 4, 6, 3, 8, 16, 10, 9, 14, 15, 2, 13, 19, 11 };

#endif
    sg::library_sort(vec.begin(), vec.end());

    assert(std::is_sorted(vec.begin(), vec.end()));

    return EXIT_SUCCESS;
}