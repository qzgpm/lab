ASSUME CS:CODE,DS:DATA
DATA SEGMENT
        STR1 DB "HELLO$"
        STR2 DB "WORLD$"
        STR3 DB 50 DUP('$')
DATA ENDS
CODE SEGMENT
START:
        MOV AX,DATA
        MOV DS,AX
        XOR CL,CL
        LEA SI,STR1
        LEA DI,STR3
LOOP1:
        MOV AL,[SI]
        CMP AL,'$'
        JZ STOP1
        MOV [DI],AL
        INC DI
        INC SI
        JMP LOOP1
STOP1:
        LEA SI,STR2
LOOP2:
        MOV AL,[SI]
        CMP AL,'$'
        JZ STOP2
        MOV [DI],AL
        INC DI
        INC SI
        JMP LOOP2
STOP2:
        LEA DX,STR3
        MOV AH,09H
        INT 21H
        MOV AH,4CH
        INT 21H
CODE ENDS
END START
