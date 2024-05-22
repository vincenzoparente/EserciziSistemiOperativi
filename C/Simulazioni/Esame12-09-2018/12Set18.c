#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];         /* definizione del TIPO pipe_t come array di 2 interi */

int main(int argc, char** argv)
{
    /* ------------- Variabili locali ------------- */
    
    int pid;				/* process identifier per le fork() */
    int N;					/* numero di file passati sulla riga di comando */
    int i,j;				/* indici per i cicli */
    pipe_t* pipedFigli;     /* pipe per la comunicazione tra padre e figli */
    pipe_t* pipedNipoti;    /* pipe per la comunicazione tra padre e nipoti */
    int status;				/* variabile di stato per la wait */
    int ritorno;			/* variabile usata dal padre per recuperare valore di ritorno di ogni figlio */
    
    /* -------------------------------------------- */
    
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }
    
    /* Numero di parametri passati da linea di comando */
    N = argc - 1;
    
    /* Creo le pipe per consentire la comunicazione tra padre e figli verificando se l'operazione va a buon fine */
    for (i = 0; i < N; i++)
    {
        /* Creazione della pipe */
        if (pipe(pipedFigli[i]) < 0)
        {
            /* La creazione della pipe ha fallito, stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
            printf("Errore nel piping.\n");
            exit(3);
        }
    }

    /* Creo le pipe per consentire la comunicazione tra padre e nipoti verificando se l'operazione va a buon fine */
    for (i = 0; i < N; i++)
    {
        /* Creazione della pipe */
        if (pipe(pipedNipoti[i]) < 0)
        {
            /* La creazione della pipe ha fallito, stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
            printf("Errore nel piping.\n");
            exit(4);
        }
    }

    /* Creo le N fork */
    for (i = 0; i < N; i++)
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
            
            /* Genero un processo nipote */
            /* Controllo che la fork() abbia successo */
            if ((pid = fork()) < 0)
            {
                /* La fork() ha fallito, dunque stampo un messaggio d'errore e ritorno un valore intero d'errore */
                printf("Errore nella fork.\n");
                exit(4);
            }
            
            /* Se pid == 0, allora la fork() ha avuto successo e possiamo eseguire il codice del nipote */
            if (pid == 0)
            {
                /* Codice del nipote */
                printf("DEBUG-Esecuzione del processo nipote %d\n", getpid());
                
                /* Chiudo i file descriptors non necessari */
                for (j = 0; j < N; j++)
                {
                    close(pipedNipoti[j][0]);
                    if (j != i)
                    {
                        close(pipedNipoti[j][1]);
                    }
                    close(pipedFigli[j][0]);
                    close(pipedFigli[j][1]);
                }

                /* Apertura del file e controllo esistenza */
                int fd;
                if((fd = open(argv[i+1], O_RDONLY)) < 0){
                    printf("Errore nell'apertura del file '%s'.\n", argv[i+1]);
                    exit(-1);
                }
                
                
            }

            /* Chiudo i file descriptors non necessari */
            for (j = 0; j < N; j++)
            {
                close(pipedFigli[j][0]);
                if (j != i)
                {
                    close(pipedFigli[j][1]);
                }
                close(pipedNipoti[j][0]);
                close(pipedNipoti[j][1]);
            }
        }
    }

    /* Codice del padre */

    /* Chiudo i lati di pipe inutilizzati */
    for ( i = 0; i < N; i++)
    {
        close(pipedFigli[i][1]);
        close(pipedNipoti[i][1]);
    }
    
    
    
    exit(0);
}