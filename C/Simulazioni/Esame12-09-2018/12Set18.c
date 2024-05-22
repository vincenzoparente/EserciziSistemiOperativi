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
    
    exit(0);
}