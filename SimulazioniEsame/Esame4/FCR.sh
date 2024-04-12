#!/bin/sh

# Mi sposto nella directory passata come parametro
cd $1

for i in *; do
    if test -d $i -a -x $i; then
        echo "Il file $i e' una directory traversabile."
        # Verifico che il nome della directory sia esattamente di 4 caratteri (3 caratteri + 1 carattere di newline)
        if test `echo $i | wc -c` -eq 4; then
            echo "Il file $i ha un nome di 3 caratteri."
            # Verifico che tutti i caratteri dispari (primo e terzo) siano uguali al secondo parametro
            if test `echo $i | head -c1` = "$2" -a `echo $i | head -c3 | tail -c1` = "$2"; then
                # La directory soddisfa tutte le condizioni, quindi la aggiungo al file temporaneo passato come terzo parametro
                echo "`pwd`"/"$i" >> "$3"
            else 
                echo "Non tutti i caratteri dispari del file $i sono uguali a $2."
            fi
        else
            echo "Il file $i non ha un nome di 3 caratteri."
        fi

        # A prescindere dal risultato, chiamo lo script ricorsivo perchè l'elemento $i e' una directory ed e' traversabile
        FCR.sh "$i" "$2" "$3"
    fi
done