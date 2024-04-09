#!/usr/bin/bash

# Verifica del numero di parametri inserito
if [[ $# -lt 3 ]]; then
    echo "Numero di parametri errato."
    exit 1
fi

# Assegno un contatore per tenere traccia dei passi che faccio nel for
N=1

# Ciclo che verifica che i primi N parametri siano nomi assoluti di directory
for i; do
    if [[ $N -eq $# ]]; then
        # Se sono entrato qui significa che ho terminato di leggere gli N parametri
        # Ora verifico che l'ultimo parametro sia un numero intero
        case $i in
        *[!0-9]*)
            echo "Il parametro $i non e' un numero"
            exit 5
            ;;
        *)
            if [[ $i -le 0 ]]; then
                echo "Il numero $i non e' positivo."
            else
                echo "Il numero $i e' positivo."
            fi
            X=$i
            ;;
        esac
        break
    fi

    # Lista delle cartelle traversabili
    dirFound=

    # Verifico che i parametri forniti siano path assoluti
    case $i in
    /*)
        echo "Il parametro $i e' un path assoluto."
        # Verifico che i parametri forniti siano directory esistenti
        if [[ -d $i && -x $i ]]; then
            # Aggiorno la lista di cartelle trovate
            dirFound="$dirFound $i"
            echo "Il parametro e' una directory"
        else
            echo "Il parametro non e' una directory"
            exit 4
        fi
        ;;
    *)
        echo "Il parametro $i non e' un path assoluto."
        exit 2
        ;;
    esac

    # Incremento il contatore
    N=$((N + 1))
done

# Aggiorno il PATH
PATH=$(pwd):$PATH
export PATH

N=1
for g; do
    if [[ $N -eq $# ]]; then
        # Se sono entrato qui significa che ho terminato di leggere gli N parametri
        break
    fi

    # Incremento il contatore
    N=$((N + 1))
done
