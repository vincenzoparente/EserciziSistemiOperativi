// Creazione di un file
// int fd = file descriptor
int fd = creat("file", mode);
// Argomenti della funzione (da sinistra):
//              char *name = nome del file
//              int mode = attributi (diritti) del file in ottale (es. 0666)

// Apertura di un file
int fd = open("file", flag);
// Argomenti della funzione (da sinistra):
//              char *name = nome del file
//              int flag = modalità di apertura del file
// Le flag sono definite nella libreria <fcntl.h>. Le principali sono:
//              O_RDONLY = apertura in sola lettura
//              O_WRONLY = apertura in sola scrittura
//              O_RDWR = apertura in lettura/scrittura

// Chiusura di un file
// retval = valore di ritorno della funzione per verificare la corretta chiusura del file
int retval = close(fd);