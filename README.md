# xIAL
Náhradný projekt IAL - 7. Minimálna kostra grafu

## Preklad programu
Preklad prebieha pomocou utility make. Pre zostavenie finálneho programu stačí v hlavnom adresári projektu spustiť príkaz make bez parametrov, výsledkom je binárny súbor sptree.

## Spustenie programu
./sptree \<filename>

./sptree -h

Program sa preloží do binárneho súboru s názvom sptree (skratka zo spanning tree). Program očakáva len jeden argument a tým je buď cesta k súboru, alebo prepínač -h, ktorý vypíše krátku nápovedu.
## Otváranie súborov
### Formát súboru
Kódovanie súboru je UTF-8 alebo kódovania s ním kompatibilné (ASCII).

Formát súboru je veľmi jednoduchý. Pri jeho návrhu sme vychádzali z formátu CSV, ktorý je veľmi jednoduchý na spracovanie a kontrolu.

Riadok 1: Ako prvý reťazec musí súbor obsahovať reťazec ".xIALAdjMatrix". Nasledovať musí znak ukončenia riadka (EOL znak) v akomkoľvek štýle (LF/CRLF...). Tento reťazec umožňuje kontrolovať, že vstupom je naozaj súbor podporovaného typu.

Riadok 2: Počet uzlov zadaný číselne (číslicami 0-9), bez akýchkoľvek znamienok, ukončený EOL znakom. Maximálny počet uzlov je 255. Slúži na zjednodušenie implementácie syntaktickej kontroly matice susednosti - je ľahšie čítať zo vstupu neznáme dáta a overiť ich správnosť, keď vieme, koľko ich má byť.

Riadok 3: Názvy uzlov v takom poradí, v akom sú zapísané v matici vodorovne zľava doprava a zvislo zvrchu nadol. Názvy uzlov sú reťazce obsahujúce ľubovoľné znaky. Reťazce sú oddelené čiarkou. V prípade, že názov uzlu obsahuje čiarku, musí byť escapovaná znakom '\\' => (takto\\,). To znamená, že aj samotný znak "\\" musí byť escapovaný (takto: \\\\niečoZaLomítkom). Riadok je ukončený EOL znakom.

Ďalšie riadky (max. 255 - max. počet uzlov): Obsahujú maticou uložený graf - prvkami matice sú váhy hrán (min. -(2^64/2) max. 2^64/2-1) spájajúca vrchol reprezentovaný indexom v matici, ktorý je namapovaný na názvy uzlov. Ak uzly nie sú spojené, váha hrany je 0. Všetky váhy hrán sú oddelené čiarkami, nie sú tolerované žiadne biele znaky. Každý (aj posledný) riadok končí EOL znakom.

Súbor musí byť ukončený hneď za posledným EOL znakom.

### Implementačné limity
Veľkosť grafu - max. 2^8-1 uzlov a 2^16-1 hrán. Maximálna váha hrany je 2^64/2-1, povolené sú aj záporné váhy (min. -(2^64/2)).
Pri načítaní súboru je program ošetrený proti pretečeniu, ktoré hrozí pri väčších grafoch a proti syntaktickým a lexikálnym chybám v súbore.