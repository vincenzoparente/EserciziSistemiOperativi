#!/bin/sh

# Mi sposto nella directory passata come parametro
cd $1

for i in *; do
    if test -d $i -a -x $i; then
        echo "Il file $i e' una directory traversabile."
        # Verifico che il nome della directory sia esattamente di 3 caratteri
        if test `echo $i | wc -c` -eq 3; then
            echo "Il file $i ha un nome di 3 caratteri."
            # Verifico che tutti i caratteri dispari siano uguali al secondo parametro
            if test `echo $i | head -1` = "$2" -a `echo $i | head -3` = "$2"; then

            fi
        fi
    fi
done