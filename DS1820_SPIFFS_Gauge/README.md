ESP8266_Arduino DS1820 SPIFFS Gauge Example
===========================================

Questo esempio legge la temperatura da un termometro DS1820.

Utilizza la libreria SPIFFS per salvare le pagine nella flash memory del ESP.

La pagina è aggiornata dinamicamente tramite AJAX e XML.

La temperatura è mostrata nella pagina internet del server ESP su un display (gauge) scritto in Javascript. 

##SPIFFS
Upload the contents of the data folder with MkSPIFFS Tool:

Use "ESP8266 Sketch Data Upload" in "Tools" menu in Arduino IDE

Istruzioni per installare MkSPIFFS Tool:

* https://github.com/esp8266/Arduino/blob/master/doc/filesystem.md#uploading-files-to-file-system

##DS1820 Sensore Temperatura

Usare una resistenza da 1.8Kohm come resistenza di pull up tra VCC 3.3V e il GPIO2 dove è collegato il cavo segnale del sensore. Alimentare il sensore DS1820 con i 3.3V. 

Il codice è derivato da:

* https://github.com/DennisSc/easyIoT-ESPduino/blob/master/sketches/ds18b20.ino

* https://github.com/milesburton/Arduino-Temperature-Control-Library

##Javascript GAUGE
Il codice del Javascript Gauge è derivato da:

* http://www.esp8266.com/viewtopic.php?p=36089

* http://newmaan.com/gauge.min.js

##Javascript AJAX e XML
Il codice Javascript AJAX e XML che dinamicamente aggiorna la pagina internet deriva da questi esempi sviluppati per le Ethernet Shield per Arduino.  


* [Esempio di Luca Dentella](http://www.lucadentella.it/2014/10/18/enc28j60-e-arduino-19/)  
* [Esempio di Starting Electronics](http://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/SD-card-IO/)  

##Screenshots

![Image](https://github.com/roboticboyer/Esempi_ESP8266/blob/master/DS1820_SPIFFS_Gauge/Doc/Arduino_IDE_Browser_Gauge.png)

##Download
[Download the Example](https://github.com/roboticboyer/Esempi_ESP8266/zipball/master/DS1820_SPIFFS_Gauge/ESP_SPIFS_DS1820_Gauge_gio_00)
