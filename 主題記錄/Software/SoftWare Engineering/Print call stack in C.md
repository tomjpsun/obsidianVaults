#callstack  #backtrace

用 libunwind LIB 可以在 C function 裏面印出 call stack
# 安裝 libunwind
**Ubuntu**: `sudo apt install libunwind-dev`
# Include header 

範例如下:
``` c
#include <stdio.h>
#include <stdlib.h>
#include <libunwind.h>

void print_stack_trace() {
    unw_cursor_t cursor;
    unw_context_t context;
    
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);
    
    printf("Call stack:\n");
    while (unw_step(&cursor) > 0) {
        unw_word_t offset, pc;
        char function_name[256];

        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        if (unw_get_proc_name(&cursor, function_name, sizeof(function_name), &offset) == 0) {
            printf("%s+0x%lx [%p]\n", function_name, offset, (void *)pc);
        } else {
            printf("?? [%p]\n", (void *)pc);
        }
    }
}

void func3() { print_stack_trace(); }
void func2() { func3(); }
void func1() { func2(); }

int main() {
    func1();
    return 0;
}
```
# Add link library
它可以支援多架構（x86, ARM, RISC-V）
對於 ARM 平臺例如樹莓派, link 命令加上 
`-lunwind -lunwind-aarch64`
即可
對於 x86 平臺例如pc, link 命令加上 
`-lunwind`
即可