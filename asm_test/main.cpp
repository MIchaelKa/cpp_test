
#include <iostream>

extern "C" int64_t sum_two(int64_t a, int64_t b);

int main() {
    int64_t result = sum_two(5, 6);
    std::cout << "sum_two: " << result << std::endl;
    return 0;
}