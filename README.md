# xIAL
Náhradný projekt IAL - 7. Minimálna kostra grafu

## Otváranie súborov
### Formát súboru
Kódovanie súboru je UTF-8.

Formát súboru je veľmi jednoduchý. Ako prvý reťazec musí súbor obsahovať reťazec ".xIALAdjMatrix". Nasledovať musí EOL v akomkoľvek štýle (LF/CRLF...).

Na ďalšom riadku nasleduje počet uzlov zadaný číselne ( číslicami 0-9), ukončený EOL.

Na novom riadku budú uložené názvy uzlov v takom poradí, akom sú zapísané v matici vodorovne zľava doprava a zvislo zvrchu nadol. Názvy uzlov sú reťazce obsahujúce ľubovoľné znaky. Reťazce sú oddelené čiarkou. V prípade, ýe názov uzlu obsahuje čiarku, musí byť escapovaná znakom '\' => (takto\,).
Na ďalšom riadku začína maticou uložený graf - prvok matice je váha hrany (max. 2^64) spájajúca vrchol reprezentovaný indexom v matici, ktorý je namapovaný na názvy uzlov. Ak uzly nie sú spojené, bude prítomné malé písmeno 'x'. Všetky váhy hrán alebo písmená x sú oddelené čiarkami, nie sú tolerované žiadne biele znaky.

 

### Implementačné limity
Veľkosť grafu - max. 2^8 uzlov a 2^16 hrán. Maximálna váha hrany je (2^64/2)-1, povolené sú aj záporné váhy.
Pri načítaní súboru je program ošetrený proti pretečeniu, ktoré hrozí pri väčších grafoch.