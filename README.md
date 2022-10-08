# ProM Espresso Display
Ersatz für das kaputte Display


### Kommunikation Display / Hauptplatine
Die Kommunikation erfolgt über die serielle Schnittstelle mit 19200Baut.
Das Signal ist invertiert.

### Funktion 
im Speicher des µC auf der Hauptpaltine ist keine Zeit gespeichert. Diese wird vom Display angefordert. Sendet das Display eine Zeit, kann diese "abgemahlen" werden. Ist die Zeit auf Null wird sie erneut vom Display angefordert.

Läuft die Mühle, sendet die Hauptplatine die aktuelle Zeit an das Display.



### Paket von der Hauptplatine
Vom µC der Hauptpaltine werden 9 byte in einem Paket gesendet.
Nach dem vom Display eine Antwort gesendet wurde, wird das nächste Paket gesendet.

Bsp:
'240','6','6','164','0','71','113','0','168'

#### Bedeutung der Zeichen (bytes)

1. Bedeutung: unklar / konstant: ja / Wert: '240'

2. Bedeutung: unklar / konstant: ja / Wert: '6'

3. Bedeutung: unklar / konstant: ja / Wert: '6'

4. Bedeutung: Status / konstant: nein / bekannte Werte:
   	'164' Siebträgertaster nicht gedrückt

   ​	'167' Siebträgertaster gedrückt => Mühle läuft

   ​	'165' Timer abgelaufen / Siebträgertaster immernoch gedrückt

   ​	'160' neue Zeit anfordern

5. Bedeutung: unklar / konstant: ja / Wert: '0'

6. Bedeutung: unklar / konstant: nein / bekannte Werte: '0', '71', '73'

7.  Bedeutung: Bestandteil der Zeit / konstant: nein / bekannte Werte '0' - '254' 

8. Bedeutung: Bestandteil der Zeit / konstant: nein / bekannte Werte '0' - '254' 

9. Bedeutung: unklar / konstant: nein / bekannte Werte: 


#### Berechnung der Zeit
Die maximal einstellbare Zeit der ProM beträgt 18s.

Nimmt man den Binärwert von **byte8** und **byte7** und konvertiert die 16bit in den Dezimalwert 

**Bsp:**

byte8: '3'	bin: 00000011

byte7: '231' bin: 11100111

byte8 &byte7: 0000001111100111 => dec: 999

Multipliziert man den Wert mit **0,018** erhält man **17,982** => eingestellt waren 18s


### Paket vom Timer

Als Antwort sendet das Display ein Paket aus vier bytes:
'240','1','6','9'

#### Bedeutung der Zeichen (bytes)
1. Bedeutung: unklar / konstant: ja / Wert: '240'
2. Bedeutung: unklar / konstant: ja / Wert: '1'
3. Bedeutung: unklar / konstant: ja / Wert: '6'
4. Bedeutung: unklar / konstant: ja / Wert: '9'

Fordert die Hauptplatine eine Zeit vom Display an, so wird diese in einem Paket aus 9 bytes gesendet. 
**'240','6','12','1','0','0','231','3','19'**

#### Bedeutung der Zeichen (bytes)
1. Bedeutung: unklar / konstant: ja / Wert: '240'
2. Bedeutung: unklar / konstant: ja / Wert: '6'
3. Bedeutung: unklar / konstant: ja / Wert: '12'
4. Bedeutung: unklar / konstant: nein / Wert: '1'
5. Bedeutung: unklar / konstant: ja / Wert: '0'
6. Bedeutung: unklar / konstant: ja / Wert: '0'
7. Bedeutung: Bestandteil der Zeit / konstant: nein / bekannte Werte '0' - '254' 
8. Bedeutung: Bestandteil der Zeit / konstant: nein / bekannte Werte '0' - '254' 
9. Bedeutung: Prüfsumme / konstant:nein / Wert: byte9 = 254 - (byte4 + byte5 + byte6 + byte7 + byte8)


## Beispielcode
der Code verwendet Softserial, hiermit lässt sich das Signal invertieren.

Beim Start des Codes wird eine Zeit an die Hauptplatine gesendet, diese Zeit kann dann "abgemahlen" werden.

Über den Serial-Monitor wird die Zeit ausgegeben.

### Ziele
Als Display wird ein 1,8 zoll tft mit ST7735 Chip angestrebt.
3 Buttons 'Minus' | 'Timer/ Setup' | 'Plus'
<!-- Is Timer/Setup for changing between single and double, or planned to have more functionality? -->
