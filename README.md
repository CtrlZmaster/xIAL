# xIAL
Náradný projekt IAL - 7. Minimálna kostra grafu

## Otváranie súborov
### Formát súboru
Kódovanie súboru je UTF-8.

Formát súboru je veľmi jednoduchý. Ako prvý reťazec musí súbor obsahovať reťazec ".xIALAdjMatrix". Nasledovať musí EOL v akomkoľvek štýle (LF/CRLF...).

Na ďalšom riadku nasleduje počet uzlov zadaný číselne ( číslicami 0-9), ukončený EOL.

Na novom riadku začína maticou uložený graf - ak nazeráme na maticu ako tabuľku, v záhlaví bude obsahovať názvy uzlov obalené v úvodzovkách. Názvy uzlov sú reťazce obsahujúce ľubovoľné znaky s výnimkou úvodzoviek, ktoré možno použiť len v tvare " \" ". Reťazce sú oddelené čiarkou.

Prvý riadok matice obsahuje len názvy uzlov.
Druhý až posledný riadok obsahuje práve jeden vyššie špecifikovaný názov a ďalej číselné hodnoty maximálne 2^64, ktoré reprezentujú váhu hrany. Ak uzly nie sú spojené, bude prítomné malé písmeno 'x'. Všetky váhy hrán alebo písmená x sú oddelené čiarkami, nie sú tolerované žiadne biele znaky.

### Implementačné limity
Veľkosť grafu - max. 2^8 uzlov a 2^16 hrán.
Pri načítaní súboru je program ošetrený proti pretečeniu, ktoré hrozí pri väčších grafoch.