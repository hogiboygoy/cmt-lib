;==============================================================================
;              Cooperative MultiTasking system
;                    CMT system
; By Alexander Sibilev
;==============================================================================
#include <vregs.inc>
            module  taskSwitch

            section `.near_func.text`:CODE:NOROOT(0)

            extern  cmtCurTask,cmtNextTask,cmtTaskAddProc
            public  cmtPortSwitchTask

cmtPortSwitchTask:
            LDW     X,?w4
            PUSHW   X
            LDW     X,?w5
            PUSHW   X
            LDW     X,?w6
            PUSHW   X
            LDW     X,?w7
            PUSHW   X
            LDW     X,cmtCurTask   ;x = cmtCurTask
            LDW     Y,SP           ;y = sp
            LDW     (X),Y          ;*cmtCurTask = sp
            LDW     X,cmtNextTask  ;x = cmtNextTask
            LDW     cmtCurTask,X   ;cmtCurTask = cmtNextTask
            LDW     X,(X)          ;y = *cmtNextTask
            LDW     SP,X           ;sp = *cmtNextTask
            POPW    X
            LDW     ?w7,X
            POPW    X
            LDW     ?w6,X
            POPW    X
            LDW     ?w5,X
            POPW    X
            LDW     ?w4,X
            RET

            public  cmtTaskAdd32Asm
cmtTaskAdd32Asm:
            LDW     X,cmtCurTask   ;x = cmtCurTask
            LDW     Y,SP           ;y = sp
            LDW     (X),Y          ;*cmtCurTask = sp
            LDW     X,cmtNextTask  ;x = cmtNextTask
            LDW     X,(X)          ;x = *cmtNextTask
            LDW     SP,X           ;sp = *cmtNextTask
            LDW     Y,cmtTaskAddProc
            PUSHW   Y              ;return addr
            LDW     X,?w4
            PUSHW   X
            LDW     X,?w5
            PUSHW   X
            LDW     X,?w6
            PUSHW   X
            LDW     X,?w7
            PUSHW   X

            LDW     X,cmtNextTask  ;x = cmtNextTask
            LDW     Y,SP
            LDW     (X),Y

            LDW     X,cmtCurTask   ;x = cmtCurTask
            LDW     X,(X)          ;x = *cmtCurTask
            LDW     SP,X           ;sp = *cmtCurTask
            RET
            end
