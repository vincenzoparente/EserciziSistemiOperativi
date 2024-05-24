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
    pipe_t* piped;          /* pipe per la comunicazione padre-figlio */
    char CF, CP;            /* rispettivamente, carattere letto dal figlio e carattere letto dal padre */
    int status;				/* variabile di stato per la wait */
    int ritorno;			/* variabile usata dal padre per recuperare valore di ritorno di ogni figlio */
    
    /* -------------------------------------------- */

    /* Controllo se il numero di parametri passati da linea di comando è corretto */
    if (argc < 4)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 4\n", argc);
        exit(1);
    }
    
    /* Numero di parametri passati da linea di comando */
    N = argc - 2;
    
    /* Creo un vettore di pipe */
    pipe_t* piped = (pipe_t*)malloc(N * sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore nell'allocazione della memoria per la pipe padre-figli\n");
        exit(2);
    }
    
    /* Creo le pipe per consentire la comunicazione tra padre e figli */
    for (i = 0; i < N; i++)
    {
        /* Creazione della pipe */
        if (pipe(piped[i]) < 0)
        {
            /* La creazione della pipe ha fallito, stampo un messaggio d'errore ed esco specificando un valore intero d'errore */
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
            
            /* Chiudo i file descriptors non necessari */
            for (j = 0; j < N; j++)
            {
                close(piped[i][0]);
                if (i != j)
                {
                    close(piped[i][1]);
                }
            }
            
            
            
            exit(0);
        }
    }
    
    /* Codice del padre */

    /* Chiudo i file descriptors non necessari */
    for (i = 0; i < N; i++)
    {
        close(piped[i][1]);
    }

    /* Apertura del file*/
    int fd;
    if((fd = open(argv[argc - 1], O_RDONLY)) < 0){
        printf("Errore nell'apertura del file '%s'.\n", argv[argc - 1]);
        exit(5);
    }
        
    /* Leggo un carattere per volta dal file AF */
    while (read(fd, &CP, 1))
    {
        /* Lo confronto con ognuno dei caratteri letti dai figli */
        for (i = 0; i < N; i++)
        {
            /* Leggo un carattere dal figlio di indice i */
            if (read(piped[i][0], &CF, 1) == 0)
            {
                printf("Errore: fallita la read dalla piped del figlio di indice %d.\n", i);
                exit(6);
            }
            
            if (CF == CP)
            {

            }
        }
    }

    /* Invio il segnale di terminazione forzata a tutti i figli */
    for (i = 0; i < N; i++)
    {
        
    }
        
    /* Aspetto i figli */
    int pidFiglio;
    for (i = 0; i < N; i++)
    {
        if ((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore del padre in wait.\n");
            exit(7);
        }
        if ((status & 0xFF) != 0)
        {
            printf("Processo figlio %d terminato in modo anomalo.\n", pidFiglio);
        }
        else
        {
            ritorno = (int)((status >> 8) & 0xFF);
            printf("Il processo figlio %d ha ritornato %d.\n", pidFiglio, ritorno);
        }
    }
    
    exit(0);
}