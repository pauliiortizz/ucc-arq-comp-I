.global ola_oceanica
.global disp_binary
.global delay
.global GetAsyncKeyState

.data
tabla: .byte 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
speed4: .word 500000

.text
ola_oceanica:
    @ Guardar registros necesarios
    push {r4, r5, r6, r7, lr}

    @ Inicializar variables
    ldr r0, =speed4     @ Cargar la dirección de speed4 en r0
    ldr r0, [r0]        @ Cargar el valor de speed4 en r0
    mov r1, r0          @ Copiar speed4 a r1 (velocidad)
    ldr r2, =tabla      @ Cargar la dirección de la tabla en r2
    mov r3, #16         @ Tamaño de la tabla (16 bytes)

loop:
    @ Recorrer la tabla
    mov r4, #0          @ Inicializar el índice a 0

loop_inner:
    cmp r4, r3          @ Comparar el índice con el tamaño de la tabla
    bge check_key       @ Si el índice es mayor o igual, saltar a check_key

    ldrb r0, [r2, r4]   @ Cargar el valor de la tabla en r0
    bl disp_binary      @ Llamar a disp_binary para mostrar el valor
    ldr r0, =speed4     @ Cargar la dirección de speed4 en r0
    ldr r0, [r0]        @ Cargar el valor de speed4 en r0
    bl delay            @ Llamar a delay con speed4

    add r4, r4, #1      @ Incrementar el índice
    b loop_inner        @ Volver al inicio del bucle interno

check_key:
    @ Comprobar si se ha presionado la tecla de escape
    ldr r0, =VK_ESCAPE
    bl GetAsyncKeyState
    cmp r0, #0
    beq loop            @ Si no se ha presionado, volver al inicio del bucle

exit:
    @ Apagar todos los LEDs (output = 0x00)
    mov r0, #0x00
    bl disp_binary

    @ Restaurar registros y salir
    pop {r4, r5, r6, r7, lr}
    bx lr
