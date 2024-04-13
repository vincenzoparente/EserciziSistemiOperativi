#!/bin/sh

# Mi sposto nella directory passata come primo parametro
cd "$1"

# Mi assicuro che la directory non sia vuota
if test "`ls -A`" = ""; then
    echo "La directory $1 e' vuota. Esplorazione della gerarchia terminata."
    # Esco con valore 0 perché cio' non e' un errore
    exit 0
fi

# Controllo che la directory contenga almeno un file con terminazione .$2
for F in *; do
    
done