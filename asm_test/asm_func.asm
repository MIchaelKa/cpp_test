
.globl _sum_two
.globl _compare_exchange
.globl _exchange

_sum_two:
    movq %rdi, %rax
    addq %rsi, %rax
    retq

_compare_exchange:
    # cmpxchg (%rdi), %rsi
    # cmpxchg $5, %rsi
    movq %rdi, %rax
    cmpxchg %eax, %esi
    retq

_exchange:
    movq (%rdi), %rax
    xchg %rsi, (%rdi)
    retq