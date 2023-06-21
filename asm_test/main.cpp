
#include <iostream>

extern "C" int64_t sum_two(int64_t a, int64_t b);

extern "C" int64_t compare_exchange(int64_t a, int64_t b);

int main() {
    int64_t result = sum_two(5, 6);
    std::cout << "sum_two: " << result << std::endl;

    int64_t first = 11;
    int64_t a = compare_exchange(first, 11);
    std::cout << "compare_exchange: " << a << std::endl;

    return 0;
}