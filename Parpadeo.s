.global parpadeo

.data
speed: .word 500000

.text
parpadeo:
    @ Guardar registros necesarios
    push {r4, r5, r6, r7, lr}

    @ Inicializar variables
    ldr r0, =speed
    ldr r0, [r0]       @ speed
    mov r1, #1         @ on_time
    mov r2, #0xAA      @ output (posiciones impares)
    mov r3, #0x55      @ output (posiciones pares)

loop
    @ Salir del bucle si kbhit() devuelve verdadero
    bl kbhit
    cmp r0, #0
    beq continue_loop

    @ Leer la tecla presionada
    bl my_getch
    cmp r0, #'e'
    beq end_loop

continue_loop
    @ Mostrar posiciones impares (0xAA)
    mov r0, r2
    bl disp_binary
    ldr r0, =speed
    ldr r0, [r0]
    bl delay

    @ Mostrar posiciones pares (0x55)
    mov r0, r3
    bl disp_binary
    ldr r0, =speed
    ldr r0, [r0]
    bl delay

    b loop

end_loop
    @ Apagar todos los LEDs (output = 0x00)
    mov r0, #0x00
    bl disp_binary
    @ Mostrar mensaje de despedida (opcional, si es necesario en pantalla)
    @ printw("Chau...\n");

    @ Restaurar registros y salir
    pop {r4, r5, r6, r7, lr}
    bx lr
