# EserciziSistemiOperativi

Esercizi sugli script shell, programmazione di sistema in C e utilizzo di make

## Visual Studio Code extentions

Su Visual Studio Code è disponibile un buon spell checker: `spellchecker`. Questa estensione segnalerà (giustamente) la sostituzione `` `comando` `` come deprecated e consiglierà di sostituirla con la notazione `$(comando)`, che però è supportata nativamente soltanto dalla bash.
\
Siccome lo spell checker traccia una barra sul testo, esso risulta un po' invasivo. È possibile disattivare questo fastidioso warning andando nella pagina dell'estensione, premere sull'ingranaggio a fianco del tasto "Disinstalla", aprire "Impostazioni dell'estensione" e aggiungere alla lista "Exclude" il codice warning `SC2006`.

Un'altra comoda estensione, preferibile se si lavora con la bash, è `Prettier`, che tramite la combinazione di tasti `CTRL` `Shift` `I` permette di formattare sia lo script shell sia molti altri moduli in molti altri linguaggi. 
\
Tuttavia, è possibile utilizzare anche l'estensione `shell-format`. Essa consente di formattare soltanto i file di script sh, bash e zsh con la combinazione di tasti `Shitft` `ALT` `F`.
