# CReference

## Descrittore di processo

I singoli elementi della tabella dei processi sono delle strutture dati definite nel file `/usr/include/linux/sched.h`:
\
`extern struct task_struct *task[NR_TASKS];`
\
L'implementazione della struttura dati è la seguente (nel file `tasks.h`):
\
```c
struct task_struct {
    /* stato del processo */
    volatile long state;

    /* identificatore del processo */
    pid_t pid;

    /* used id, effective user id, saved user id */
    uid_t uid, euid, suid;

    /* group id, effective group id, saved group id */
    gid_t gid, egid, sgid;

    /* tempo di esecuzione in tick (priorità dinamica) */
    long counter;

    /* priorità statica */
    long priority; 

    /* maschera dei segnali ricevuti non ancora serviti */
    int sigpending;

    /* codice di terminazione e segnale che ha causato la terminazione */
    int exit_code, exit_signal;

    /* task seguente e precedente nella lista dei task */
    struct task_struct *next_run;
    struct task_struct *prev_run;

    /* padre originale */
    struct task_struct *p_opptr;

    /* padre attuale */
    struct task_struct *p_pptr;

    /* informazioni sui file aperti */
    struct files_struct *files;

    /* ... */
}
```