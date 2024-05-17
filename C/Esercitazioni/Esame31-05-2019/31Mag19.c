#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];         /* definizione del TIPO pipe_t come array di 2 interi */

int main(int argc, char** argv)
{
    /* ------------- Variabili locali ------------- */
    
    int pid;				/* process identifier per le fork() */
    int N;					/* numero di file passati sulla riga di comando */
    int i,j;				/* indici per i cicli */
    pipe_t *piped;			/* array dinamico di pipe descriptors */
    pipe_t pipednipote;     /* pipe per la comunicazione tra figlio e nipote */
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
    
    /* Allocazione dell'array di pipe */
    piped = (pipe_t*)malloc(N * sizeof(pipe_t));
    /* Controllo che l'allocazione di memoria sia andaata a buon fine */
    if (piped == NULL)
    {
        /* In tal caso si e' verificato un errore nell'allocazione: stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
        printf("Errore nell'allocazione della memoria per il vettore di pipe.\n");
        exit(2);
    }
    
    /* Creazione delle N pipe */
    for (i = 0; i < N; i++)
    {
        /* Controllo che ogni creazione vada a buon fine */
        if (pipe(piped[i]) < 0)
        {
            /* Non e' stato possibile creare la pipe con successo: stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
            printf("Errore nel piping.\n");
            exit(3);
        }
    }
    
    /* Generazione degli N figli */
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

            /* Chiudo i lati di pipe non utilizzati dal figlio */
            for (j = 0; j < N; j++)
            {
                close(piped[i][0]);
                if (i != j) 
                {
                    close(piped[i][1]);
                }
            }
            
            /* Creo una pipe per consentire la comunicazione figlio-nipote verificando se l'operazione va a buon fine */
            if ((pipe(pipednipote)) < 0)
            {
                /* La creazione della pipe ha fallito, stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
                printf("Errore nel piping.\n");
                exit(3);
            }
            
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
                
                /* Chiudo il lato di pipe non usato dal nipote */
                close(pipednipote[0]);
                /* Chiudo il lato di pipe ereditato dal figlio che lo utilizza per comunicare al padre */
                close(piped[i][1]);

                /* Eseguo il sort del file corrispondente */
                execlp("sort", "sort", "-f", argv[i +1], (char*)0);

                /* Se torno qui ho avuto un errore con la exec */
                printf("Errore nella exec.\n");
                exit(-1);
            }
            
            /* Codice del figlio */
            /* Chiudo il lato di pipe non usato dal figlio */
            close(pipednipote[1]);
        }
    }
    
    /* Codice del padre */
    /* Chiudo i lati di pipe non utilizzati dal padre */
    for (i = 0; i < N; i++)
    {
        close(piped[i][1]);
    }
    
    
    exit(0);
}