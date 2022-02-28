# VUT FIT 2021/22 IPP Projekt 1: HTTP Server

Autor: Jakub Šediba (xsedib00)

Vytvorený program slúši ako HTTP server, ktorý vykonáva požiadavky na užívateľom zvolenom porte. Tento program dokáže získať informácie o hostname servera, názvu procesora a aktuálneho zaťaženia procesora. Program je napísaný v jazyku C.

## Preklad programu

Na preklad programu je vytvorený Makefile.
Na preloženie programu vykonajte nasledujúce kroky:
* Presuňte sa do zložky, ktorá obsahuje súbory server.c, server.h a Makefile
* Použite príkaz `make`

## Používanie programu

### Spustenie programu
Na spustenie programu použite nasledujúci syntax:
```
./hinfosvc [číslo_portu]
```
Číslo portu musí byť v rozsahu 0 - 65535 a nesmie sa prekrývať s číslami portov používanými inými aplikáciami.

### Ukončenie programu
Na ukončenie programu môžeme použiť klávesovú kombináciu `ctrl+C`, alebo ho môžeme ukončiť zaslaním signálu 2 procesu, v ktorom program beží:
```
kill -2 [číslo_procesu]
```

## Podporované vlastnosti
* Získanie hostname: `https://localhost:[číslo_portu]/hostname`
* Získanie názvu procesora: `https://localhost:[číslo_portu]/cpu-name`
* Získanie zaťaženia procesora: `https://localhost:[číslo_portu]/load`


## Príklady používania programu

### Spustenie serveru
Na spustenie serveru na porte 12345 použijeme nasledujúci príkaz:
```
./hinfosvc 12345
```

### Získanie informácií pomocou webového prehliadača
Do webového prehliadača zadáme túto adresu:
```
http://localhost:12345/hostname
```
Prehliadač na stránke zobrazí v textovej forme hostname servera:
```
merlin.fit.vutbr.cz
```
### Získanie informácií pomocou curl
Do terminálu zadáme 
```
curl http://localhost:12345/cpu-name
```
Na termínal nám bude vypísaný typ proceru, napríklad:
```
Intel(R) Xeon(R) Silver 4214R CPU @ 2.40GHz
```