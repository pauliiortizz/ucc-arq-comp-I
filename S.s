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

.data
array: 
  .byte 0x00, 0x80, 0xC0, 0XE0, 0XF0, 0XF8, 0XFC, 0XFE, 0XFF, 0X7F, 0X3F, 0X1F, 0X0F, 0X07, 0X03, 0X01
.text

olaOceanica:

      PUSH {R4, R5, R6,R7, LR}

start: 
      LDR R4,=array
      MOV R7, #16       

loop: 
  LDRB R5,[R4], #1
MOV R0, R5  
          BL disp_binaryAssembly
          BL delayAssembly
              
          SUBS R7, R7, #1   
          BNE loop    

      b olaOceanica    

 TERMINO: 
 POP {R4, R5, R6,R7, PC}
 
 .data

.end
