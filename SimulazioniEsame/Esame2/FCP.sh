#!/bin/bash

# Controllo lasco del numero di parametri inseriti
if [[ $# -lt 3 ]]; then
    echo "Numero parametri errato."
    exit 1
fi

# Controllo che il primo parametro sia un numero intero
case "$1" in
*[!0-9]*)
    echo "Il parametro $1 non e' un numero intero."
    exit 2
    ;;
*)
    echo "Il parametro $1 e' un numero intero."
    # Controllo che il numero sia positivo
    if test "$1" -le 0; then
        echo "Il numero $1 non e' positivo."
    else
        echo "Il numero $1 e' positivo."
    fi
    ;;
esac

# Assegno a una variabile Y il primo parametro
Y=$1

# Creo una lista vuota per conservarvi i parametri
dirList=""

# Contatore per il ciclo
N=1
# Ciclo per scorrere tutti i parametri
for i; do
    # Salto il primo parametro
    if [[ "$N" -eq 1 ]]; then
        break
    fi
    # Controllo che il path sia assoluto
    case "$i" in
    /*)
        echo "Il parametro $i e' un path assoluto."
        # Controllo che la directory esista e sia traversabile
        if [[ -d "$i" && -x "$i" ]]; then
            echo "Il parametro $i e' una directory."
            dirList="$dirList $i"
        else
            echo "Il parametro $i non e' una directory."
            exit 3
        fi
        ;;
    *)
        echo "Il parametro $i non e' un path assoluto."
        exit 4
        ;;
    esac
    N=$(("$N" + 1))
done

# Creo il file temporaneo dove salvare le occorrenze
touch /tmp/occurrences

# Aggiorno la variabile d'ambiente PATH
PATH=$(pwd):$PATH
export PATH

# Ciclo che scorre la lista di parametri
for i in $dirList; do
    FCR.sh $i $Y
done

# Conto le occorrenze
echo "Il numero di file aventi 5 o piu' righe e': $(wc -l < /tmp/occurrences)"

# Rimuovo il file temporaneo
rm /tmp/occurrences
echo "File temporaneo rimosso."