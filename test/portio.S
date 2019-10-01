    .global main
    .type main, @function
main:
    mov $0x00, %al
    mov $0x3fb, %edx
    out %al, (%dx)
    mov $0x41, %al
    mov $0x3f8, %edx
    out %al, (%dx)
    mov $0x3f8, %edx
    add $5, %edx
    in  (%dx), %al
    mov $0x20, %bl
    cmp %al, %bl
    jne .fail
    mov $0x41, %al
    mov $0x3f8, %edx
    out %al, (%dx)
.fail:
    hlt
