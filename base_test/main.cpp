#include <iostream>


class Wrapper {
public:
    Wrapper(int value)
        : pointer_(std::make_shared<int>(value))
    {}

    void print_use_count() {
        std::cout << "use_count: " << pointer_.use_count() << std::endl;
    }

    std::shared_ptr<int> get_pointer() {
        return pointer_;
    }

    int* get_reference() {
        return pointer_.get();
    }

private:
    std::shared_ptr<int> pointer_;
};

int main() {

    Wrapper wrapper(5);
    wrapper.print_use_count();

    {
        auto a = wrapper.get_pointer();
        auto b = wrapper.get_reference();
        wrapper.print_use_count();
    }

    wrapper.print_use_count();

    return 0;
}