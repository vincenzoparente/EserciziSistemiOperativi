#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Contiene le funzioni per la gestione dei file (open, read, write, close)
#include <fcntl.h>      // Contiene le costanti per la gestione dei file (O_RDONLY, O_WRONLY, O_RDWR)

#define PERM 0666       // Definisco i permessi del file in ottale

// RICORDA: I file vengono chiusi automaticamente quando si chiude il programma con exit()

int main (int argc, char *argv[]) {
    // Dichiaro le variabili per i file da leggere e scrivere e una per il numero di byte letti
    int infile, outfile, nread;
    char buffer[BUFSIZ];

    // Controllo che il numero di argomenti sia corretto
    if (argc != 3) {
        // Stampo un messaggio di errore se il numero di argomenti non è corretto
        printf("Usage: %s file\n", argv[0]);
        exit(1);
    }

    // Apro il file in sola lettura
    if ((infile = open(argv[1], O_RDONLY)) < 0) {
        // Stampo un messaggio di errore se non sono riuscito ad aprire il file in lettura
        printf("Unable to open file %s", argv[1]);
        exit(2);
    }

    // Creo un file con i diritti 0666
    if ((outfile = creat(argv[1], PERM)) < 0) {
        // Stampo un messaggio di errore se non sono riuscito a creare infile
        printf("Unable to create file %s", argv[1]);
        exit(3);
    }

    // Leggo dal file fino a che nread è maggiore di 0
    while ((nread = read(infile, buffer, BUFSIZ)) > 0) {
        if (write(outfile, buffer, nread) != nread) {
            // Stampo un messaggio di errore se non sono riuscito a scrivere su outfile
            printf("Unable to write string \"%s\" on file %s", argv[2], argv[1]);
            exit(4);
        }
    }

    exit(0);
}