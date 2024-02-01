# Projecto 2 - IAED 2022/2023

## Enunciado

O enunciado do projecto está disponível em [enunciado.md](enunciado.md). 

## Data de entrega: 14 de abril de 2023, às 19h59

Informações detalhadas sobre o processo de submissão estão disponíveis em [informacoes.md](informacoes.md).



# Evaluation

2023-04-14T18:25:28

## Test 1: [+ Accepted+].
## Test 2: [+ Accepted+].
## Test 3: [+ Accepted+].
## Test 4: [+ Accepted+].
## Test 5: [- Command terminated by signal (11: SIGSEGV)-].

## Test 6: [+ Accepted+].
## Test 7: [+ Accepted+].
## Test 8: [- Command terminated by signal (11: SIGSEGV)-].

## Test 9: [+ Accepted+].
## Test 10: [+ Accepted+].
## Test 11: [+ Accepted+].
## Test 12: [+ Accepted+].
## Test 13: [+ Accepted+].
## Test 14: [+ Accepted+].
## Test 15: [+ Accepted+].
## Test 16: [+ Accepted+].
## Test 17: [+ Accepted+].
## Test 18: [+ Accepted+].
## Test 19: [- Wrong Answer-].


## Test 20: [- Command terminated by signal (11: SIGSEGV)-].

## Test 21: [- Command terminated by signal (11: SIGSEGV)-].

## Test 22: [+ Accepted+].
## Test 23: [+ Accepted+].
## Test 24: [- Command terminated by signal (11: SIGSEGV)-].

## Test 25: [- Command terminated by signal (11: SIGSEGV)-].

## Test 26: [- Command terminated by signal (11: SIGSEGV)-].

## Test 27: [+ Accepted+].
## Test 28: [+ Accepted+].
## Test 29: [+ Accepted+].
## Test 30: [- Command terminated by signal (6: SIGABRT)-].

## Test 31: [- Command terminated by signal (6: SIGABRT)-].

## Test 32: [- Wrong Answer-].




## Number of passed tests: 21/32


## Valgrind Output:


```
==23674== Memcheck, a memory error detector
==23674== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==23674== Using Valgrind-3.14.0 and LibVEX; rerun with -h for copyright info
==23674== Command: ./proj.out
==23674== 
==23674== Invalid read of size 8
==23674==    at 0x10B08D: listRoutesNoArguments (project2.c:874)
==23674==    by 0x10B458: listRoutes (project2.c:138)
==23674==    by 0x1092D5: command_c (project2.c:85)
==23674==    by 0x1092D5: main (project2.c:41)
==23674==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==23674== 
==23674== 
==23674== Process terminating with default action of signal 11 (SIGSEGV)
==23674==  Access not within mapped region at address 0x0
==23674==    at 0x10B08D: listRoutesNoArguments (project2.c:874)
==23674==    by 0x10B458: listRoutes (project2.c:138)
==23674==    by 0x1092D5: command_c (project2.c:85)
==23674==    by 0x1092D5: main (project2.c:41)
==23674==  If you believe this happened as a result of a stack
==23674==  overflow in your program's main thread (unlikely but
==23674==  possible), you can try to increase the size of the
==23674==  main thread stack using the --main-stacksize= flag.
==23674==  The main thread stack size used in this run was 8388608.
==23674== 
==23674== HEAP SUMMARY:
==23674==     in use at exit: 842 bytes in 27 blocks
==23674==   total heap usage: 213 allocs, 186 frees, 14,018 bytes allocated
==23674== 
==23674== LEAK SUMMARY:
==23674==    definitely lost: 162 bytes in 5 blocks
==23674==    indirectly lost: 320 bytes in 8 blocks
==23674==      possibly lost: 0 bytes in 0 blocks
==23674==    still reachable: 360 bytes in 14 blocks
==23674==         suppressed: 0 bytes in 0 blocks
==23674== Rerun with --leak-check=full to see details of leaked memory
==23674== 
==23674== For counts of detected and suppressed errors, rerun with: -v
==23674== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

```


Your code will not be reevaluated if you submit before 2023-04-14 18:35:28 @ Lisbon, Portugal. You need to wait 10 minute(s).

