# CppExercises

## fork vs thread

Create comparison of performance between forking and threading.  

Results from an example linux PC:

```
thread_and_join            1000 times:        42464524ns (4.246us each)
fork_and_wait              1000 times:       771957732ns (77.196us each)
thread_work_and_join       1000 times:        63978101ns (6.398us each)
fork_work_and_wait         1000 times:       938022697ns (93.802us each)
fork_exec_and_wait         1000 times:      2279183345ns (227.918us each)
```