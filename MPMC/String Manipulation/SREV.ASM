ASSUME CS:CODE,DS:DATA
DATA SEGMENT
        STR1 DB "RATS$"
        STR2 DB 50 DUP('$')
DATA ENDS
CODE SEGMENT
START:
        MOV AX,DATA
        MOV DS,AX
        XOR CL,CL
        LEA SI,STR1
        LEA DI,STR2
LOOP1:
        MOV AL,[SI]
        CMP AL,'$'
        JZ REV
        INC CL

        PUSH AX
        INC SI
        JMP LOOP1
REV:
        POP AX
        MOV [DI],AL
        INC DI
        DEC CL
        JNZ REV
        LEA DX,STR2
        MOV AH,09H
        INT 21H
        MOV AH,4CH
        INT 21H
CODE ENDS
END START
