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
      
start: MOV R5, #0x01        // Iniciar con un LED encendido
      MOV R7, #8           // Número de LEDs (o pasos) hacia un lado
      MOV R4, #255
      ADD R4, R4, R4

// Movimiento de ida
loop1: MOV R6, R5
      ORR R5, R5, R6       // Conservar los LEDs encendidos previamente
      MOV R0, R5
      MOV R1, #5
      BL disp_binaryAssembly
      MOV R0, R4
      BL delayAssembly
      CMP R0, #0
      BEQ TERMINO
      MOV R4, R0

      SUBS R7, R7, #1
      BNE loop1

      MOV R7, #8           // Reiniciar el contador de pasos
      MOV R5, #0xFF        // Iniciar con todos los LEDs encendidos para la vuelta

// Movimiento de regreso
loop2: MOV R6, R5
      BIC R5, R5, R6       // Apagar LEDs uno por uno
      MOV R0, R5
      MOV R1, #5
      BL disp_binaryAssembly
      MOV R0, R4
      BL delayAssembly
      CMP R0, #0
      BEQ TERMINO
      MOV R4, R0

      SUBS R7, R7, #1
      BNE loop2

      BL delayAssembly
      CMP R0, #0
      BNE start

 TERMINO: POP {R4, R5, R6, R7, PC}
 

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
