.text 
.global parpadeoAlternado

.data
speed: .word 500000

.text
parpadeoAlternado:
    @ Guardar registros necesarios
    push {r4, r5, r6, r7, lr}

    @ Inicializar variables
    ldr r0, =speed
    ldr r1, [r0]       @ Cargar valor de speed en r1
    mov r2, #0xAA      @ Patrón para posiciones impares
    mov r3, #0x55      @ Patrón para posiciones pares

loop:
    @ Comprobar si una tecla ha sido presionada con kbhit()
    bl kbhit
    cmp r0, #0
    bne continue_loop

    @ Leer la tecla presionada con getch()
    bl getch
    cmp r0, #'e'
    beq end_loop

continue_loop:
    @ Mostrar posiciones impares (0xAA)
    mov r0, r2
    bl disp_binary
    mov r0, r1
    bl delay

    @ Mostrar posiciones pares (0x55)
    mov r0, r3
    bl disp_binary
    mov r0, r1
    bl delay

    b loop

end_loop:
    @ Apagar todos los LEDs (output = 0x00)
    mov r0, #0x00
    bl disp_binary

    @ Restaurar registros y salir
    pop {r4, r5, r6, r7, lr}
    bx lr
