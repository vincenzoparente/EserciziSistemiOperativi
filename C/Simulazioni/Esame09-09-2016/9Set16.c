#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];         /* definizione del TIPO pipe_t come array di 2 interi */

typedef struct caratteriLetti {
    char v1;        // Carattere cercato
    long int v2;    // Numero di occorrenze
} caratteriLetti;

int main(int argc, char** argv)
{
    /* ------------- Variabili locali ------------- */
    
    int pid;				/* process identifier per le fork() */
    int i, j;				/* indici per i cicli */
    int fd;                 /* file descriptor dell'unico file passato */
    pipe_t piped[26];		/* array statico di pipe descriptors */
    caratteriLetti cl[26];  /* array statico di strutture per salvare il carattere letto e le sue occorrenze */
    int status;				/* variabile di stato per la wait */
    int ritorno;			/* variabile usata dal padre per recuperare valore di ritorno di ogni figlio */
    
    /* -------------------------------------------- */
    
    /* Creo una pipe per consentire la comunicazione tra figli verificando se l'operazione va a buon fine */
    for (i = 0; i < 26; i++)
    {
        /* Creazione della pipe */
        if (pipe(piped[i]) < 0)
        {
            /* La creazione della pipe ha fallito, stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
            printf("Errore nel piping.\n");
            exit(3);
        }
    }

    for (i = 0; i < 26; i++)
    {
        /* Genero un processo figlio */
        /* Controllo che la fork() abbia successo */
        if ((pid = fork()) < 0)
        {
            /* La fork() ha fallito, dunque stampo un messaggio d'errore e ritorno un valore intero d'errore */
            printf("Errore nella fork.\n");
            exit(4);
        }
        
        /* Se pid == 0, allora la fork() ha avuto successo e possiamo eseguire il codice del figlio */
        if (pid == 0)
        {
            /* Codice del figlio */
            printf("DEBUG-Esecuzione del processo figlio %d\n", getpid());

            /* Assegno alla struttura il carattere corrispondente */
            cl[i].v1 = 'a' + i;
            /* Faccio iniziare da 0 v2 */
            cl[i].v2 = 0;

            /* Apertura del file e controllo esistenza */
            if((fd = open(argv[2], O_RDWR)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[2]);
                exit(-1);
            }
            
            /* Ciclo di lettura dei caratteri */
            char tmp;
            while(read(fd, &tmp, 1))
            {
                if (cl[i].v1 == tmp)
                {
                    cl[i].v2++;
                }
            }
        }
    }
    
    /* Codice del padre (fuori dal for perche' mi interessa eseguirlo solo quando il ciclo e' terminato) */
    
    exit(0);
}