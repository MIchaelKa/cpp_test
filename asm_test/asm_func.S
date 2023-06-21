
.globl _sum_two

_sum_two:
    movq %rdi, %rax
    addq %rsi, %rax
    retq