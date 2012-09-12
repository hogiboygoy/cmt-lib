           .text
           .state32
           .global cmtPortSwitchTask
           .global cmtCurTask,cmtNextTask,cmtTaskAddProc
           .asmfunc
cmtPortSwitchTask:
           push {r4,r5,r6,r7,r8,r9,r10,r11,r12}
           push {lr}
           ldr  r0,adrOfCurTask ; r0 = &cmtCurTask
           ldr  r4,[r0]         ; r4 = cmtCurTask
           str  sp,[r4]         ; *cmtCurTask = sp
           ldr  r5,adrOfNextTask ; r5 = cmtNextTask;
           ldr  r6,[r5]         ; r6 = cmtNextTask
           str  r6,[r0]         ; cmtCurTask =  cmtNextTask
           ldr  sp,[r6]         ; sp = *cmtNextTask
           pop  {lr}
           pop  {r4,r5,r6,r7,r8,r9,r10,r11,r12}
           bx   lr
adrOfCurTask: .word cmtCurTask
adrOfNextTask: .word cmtNextTask           
           .endasmfunc

           .global cmtTaskAdd32Asm
           .asmfunc
cmtTaskAdd32Asm:
           ldr  r1,adrOfTaskAddProc
           ldr  r1,[r1]         ; r1 = proc
           mov  r0,sp           ; r0 = sp
           ldr  r2,adrOfNextTaskA ; r2 = &cmtNextTask
           ldr  r2,[r2]         ; r2 = cmtNextTask
           ldr  sp,[r2]         ; sp = *cmtNextTask
           push {r4,r5,r6,r7,r8,r9,r10,r11,r12}
           push {r1}
           str  sp,[r2]         ; *cmtNextTask = sp
;—формировать стек
           mov  sp,r0           ; sp = r0
           bx   lr
adrOfTaskAddProc: .word cmtTaskAddProc
adrOfNextTaskA: .word cmtNextTask           
           .endasmfunc
           .end
           
           