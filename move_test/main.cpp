//
//  main.cpp
//  test_cpp
//
//  Created by Калинин Михаил Андреевич on 30.11.2022.
//

#include <iostream>
#include <vector>

//std::string& test_func(std::string&& str) {
//    if (str.size())
//        str[0] = std::toupper(str[0]);
//    return str;
//}

std::string test_func(std::string str) {
    if (str.size())
        str[0] = std::toupper(str[0]);
    return str;
}

void test_move() {
    std::string test_str {"ert"};
    
//    std::string result = test_func("test_str");
    std::string result = test_func(std::move(test_str));
//    std::string result = test_func("world");
    
    std::cout << test_str << std::endl;
    std::cout << result << std::endl;
}

int main(int argc, const char * argv[]) {
    
    int array[5] = {1,2,3,4,5};
    
    array[1] = 8;
    
    auto vec = std::vector<int>(array, array + 3);
    
    for (auto e : vec) {
        std::cout << e << std::endl;
    }

    return 0;
}
