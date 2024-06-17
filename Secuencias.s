.text 
.global parpadeo

.data
speed: .word 500000

.text
parpadeo:
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




    

    .global ola_oceanica

.data
tabla: .byte 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
speed4: .word 500000

.text
ola_oceanica:
    @ Guardar registros necesarios
    PUSH {R4, R5, R6, R7, LR}

resetO:
    @ Inicializar variables
    LDR R4, =tabla      @ Cargar la dirección de la tabla en R4
    MOV R5, #0          @ Índice inicial
    MOV R6, #16         @ Tamaño de la tabla
    LDR R0, =speed4     @ Cargar la dirección de speed4 en R0
    LDR R1, [R0]        @ Cargar el valor de speed4 en R1 (velocidad)
    MOV R7, #5          @ Repetir 5 veces

loopO:
    @ Comprobar si se ha presionado alguna tecla
    BL kbhit
    CMP R0, #0          @ Comprobar si kbhit devuelve 0 (no se ha presionado ninguna tecla)
    BEQ no_key_pressed

    @ Leer la tecla presionada
    BL my_getch
    CMP R0, #'e'        @ Comprobar si la tecla presionada es 'e'
    BEQ end_loop        @ Si la tecla presionada es 'e', salir del loop

no_key_pressed:
    @ Recorrer la tabla de patrones de olas
    LDRB R0, [R4, R5]   @ Cargar el valor de la tabla en R0
    BL disp_binary      @ Mostrar el valor en binario
    BL ledShow          @ Mostrar el valor en los LEDs
    MOV R0, R1          @ Pasar el valor de speed4 a R0 para el retardo
    BL delay            @ Llamar a delay

    ADD R5, R5, #1      @ Incrementar el índice
    CMP R5, R6          @ Comparar el índice con el tamaño de la tabla
    BEQ resetO          @ Si el índice es igual al tamaño, reiniciar

    SUBS R7, R7, #1
    BEQ resetO

    B loopO             @ Volver al inicio del bucle

end_loop:
    @ Apagar todos los LEDs (output = 0x00)
    MOV R0, #0x00
    BL disp_binary
    BL ledShow

    @ Restaurar registros y salir
    POP {R4, R5, R6, R7, PC}

