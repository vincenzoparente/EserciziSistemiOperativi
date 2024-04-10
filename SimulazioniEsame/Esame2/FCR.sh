#!/bin/bash

# Aggiungo alla variabile d'ambiente PATH il path corrente
PATH=$(pwd):$PATH
# Aggiungo le modifiche effettuate alla variabile d'ambiente esportandola
export PATH

# Scorro tutti i file nella directory
for i in $(ls $1); do
    # Verifico di avere diritto di lettura del file e che esso abbia Y righe o piu'
    if [[ -r "$i" && $(wc -l <"$i") -ge "$2" ]]; then
        head -5 "$i" | tail -1 | cat >"$i".quinta
        echo "$(pwd)/$i" >> /tmp/occurrences
    else
        # Controllo se e' una directory e nel caso se e' traversabile
        if test "$i" -d; then
            if test "$i" -x; then
                FCR.sh "$j" "$2"
            fi
        else
            touch "$i".NOquinta
        fi
    fi
done
