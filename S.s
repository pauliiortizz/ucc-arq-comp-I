.text
.extern delayAssembly
.extern disp_binaryAssembly

.global parpadeoAlternado
parpadeoAlternado:
      STMFD SP!, {R4,R5,R6,R7,LR}

parpadeo_alternado_start:
      MOV R4, #0
      MOV R7, #170  
      LDR R5, =array 
;
loop_alternado:
      MOV R0, R7
      BL disp_binaryAssembly
      BL delayAssembly

      EOR R7, R7, #255 

      CMP R4, #9
      BEQ parpadeo_alternado_start
      ADD R4, R4, #1
      B loop_alternado

FINAL:     
      POP {R4,R5,R6,R7,PC}


.global olaOceanica
olaOceanica:
      PUSH {R4, R5, R6, R7, LR}
      
start: MOV R5, #0x01  ; Inicializa R5 con el bit más bajo
      MOV R7, #7      ; Inicializa R7 con el contador para 7 iteraciones
      MOV R4, #255    ; Inicializa R4 con 255
      ADD R4, R4, R4  ; Duplica R4, ahora es 510
      MOV R3, #0      ; Inicializa R3 como acumulador de los 1s

loop1: MOV R6, R5
      LSL R5, R5, #1  ; Desplazamiento lógico a la izquierda
      ORR R3, R3, R6  ; Acumula los 1s en R3
      MOV R0, R3
      BL disp_binary  ; Muestra el valor en binario
      MOV R0, R4
      BL delayAssembly ; Retraso
      CMP R0, #0
      BEQ TERMINO
      MOV R4, R0

      SUBS R7,R7,#1
      BNE loop1

      MOV R7, #7
      MOV R5, #0x80  ; Inicializa R5 con el bit más alto

loop2: MOV R6, R5
      LSR R5, R5, #1  ; Desplazamiento lógico a la derecha
      ORR R3, R3, R6  ; Acumula los 1s en R3
      MOV R0, R3
      MOV R1,#5
      BL disp_binaryAssembly ; Muestra el valor en binario
      MOV R0, R4
      BL delayAssembly ; Retraso
      CMP R0, #0
      BEQ TERMINO
      MOV R4, R0

      SUBS R7,R7,#1
      BNE loop2

      MOV R7, #7
      MOV R5, #0x01  ; Reinicia el ciclo con el bit más bajo

      B start        ; Vuelve al inicio para crear un bucle infinito

TERMINO: 
      POP {R4, R5, R6, R7, PC}

.data
array:
      .byte 0x80
      .byte 0x81
      .byte 0x82
      .byte 0x84
      .byte 0x88
      .byte 0x90
      .byte 0xA0
      .byte 0xC0
      .byte 0xFF
.end
