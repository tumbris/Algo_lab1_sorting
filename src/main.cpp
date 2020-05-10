#include <Sortings.h>
#include <iostream>
#include <iterator>
#include <random>
#include <cassert>

int main(int argc, char **argv)
{
    std::vector<std::int32_t> vec;
    vec.reserve(100000);

    std::random_device r;
    std::default_random_engine dre(r());
    std::uniform_int_distribution<std::int32_t> uid(std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max());

    auto generator = [&]()
    {
        return uid(dre);
    };

    std::generate_n(std::back_inserter(vec), vec.capacity(), std::move(generator));
    sg::bucket_sort(vec.begin(), vec.end());

    assert(std::is_sorted(vec.begin(), vec.end()));

    return EXIT_SUCCESS;
}