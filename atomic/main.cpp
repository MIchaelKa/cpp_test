#include <atomic>

std::atomic<int> x;

// store
void store_release() {
    x.store(42, std::memory_order_release);
}

void store_release(int y) {
    x.store(y, std::memory_order_release);
}

void store_seq_cst(int y) {
    x.store(y, std::memory_order_seq_cst);
}

// exchange
void exchange_release() {
    x.exchange(42, std::memory_order_release);
}

void exchange_release(int y) {
    x.exchange(y, std::memory_order_release);
}

int exchange_ret_release(int y) {
    return x.exchange(y, std::memory_order_release);
}

void exchange_seq_cst() {
    x.exchange(42, std::memory_order_seq_cst);
}

int main() {
    std::atomic<int> z;
    int y;
    z.store(y, std::memory_order_release);
    if (z.load() == 41) {
        exchange_seq_cst();
    }
    return 0;
}