enable_language(ASM)

#set(ASM_SOURCES
#    asm_func.asm
#)

add_executable(asm_test asm_func.asm main.cpp)