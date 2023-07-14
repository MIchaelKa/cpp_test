
#include <iostream>

template <typename T>
class BaseClass {
public:

    void set_value(T value) {
        value_ = value;
    }

    T get_value() {
        return value_;
    }

    virtual void update_value(T value) = 0;

protected:
    T value_;
};

template <typename T>
class BaseClassA: public BaseClass<T> {

    void update_value(T value) {
        std::cout << "update_value" << std::endl;
    }
};


class BaseClassB: public BaseClass<std::string> {

    void update_value(std::string value) {
        std::cout << "update_value" << std::endl;
        value_ = value;
    }
};


int main() {

    BaseClass<int>* a = new BaseClassA<int>();
    a->set_value(5);
    std::cout << "get_value: " << a->get_value() << std::endl;

    // Cannot do this
    // BaseClass<T>* b = new BaseClassB();

    BaseClass<std::string>* b = new BaseClassB();

    b->set_value("Hello");
    std::cout << "get_value: " << b->get_value() << std::endl;
    b->update_value("World");
    std::cout << "get_value: " << b->get_value() << std::endl;

    return 0;
}