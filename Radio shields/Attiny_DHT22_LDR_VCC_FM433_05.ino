/*------------------------------------------
  Proyecto Hackaton  2015
  
  Lectura de sensores y envio por radio via emisor FM433 MHz
  para montar en Attyni85
 
  Daniel Lopez Campos
  Oct- 2015
  
  V 0.5
//------------------------------------------

Basado en https://github.com/RobTillaart/Arduino/tree/master/libraries
  Basado en  skech de Mahannad Rawashdeh

El equipo lleva incorporado los siguientes sensores:

- DHT22 /DHT11 para medicion de Temperatura y Hr
- LDR para medicion de intensidad de iluminacion
- lectura de tension de la bateria de litio

V 0.1
 - Primera version funcional
 
 - Introduzco codigo para identificar a varios sensores
 - Codifico otros sensores con numeros consecutivos: DHT01, DHT02, DHT03 ...
 - Reenvio el mismo dato 3 veces para controlar la perdida de señal. numero como 13 23 33

V 0.2
 - Máximo tamaño de envio de mensage: 26 bytes (incluido null) 
 - Incorporo lectura de LDR y Vcc. 
 - Realizo lectura de LDR activando la corriente 2 sg antes de la medida y desconectandola 
 después, de manera que no caliento el sensor.

 V0.5 
 - Versión funcional(Ha dado problemas con algún DHT22)
 
 Pines: 

Attiny85
--------
                      _____
          (reset) 5 -|  o  |- Vcc
señal Vcc bateria 3 -|     |- 2  emisor FM
             LDR  4 -|     |- 1  alimentacion LDR  
                GND -|_____|- 0  DHT22 

*/

 

#include <stdlib.h>
 
// variables
 float T;
 float Hr;
 int LDR;
 int Vcc;
 
 //                ID n  T    Hr    LDR Vcc
 char sensor[26]= "S0203 ---------- --------";
 char pos[4] = "123";
 char Temp[6];
 char Humedad[6];
 char LDR_c[5];
 char Vcc_c[5];
 char espacio[2] = " ";
 
 #define LDR_Vcc 1
 
 const uint32_t intervalo = 600000;       //intervalo de toma de muestra 15 s en attiny es el doble.
 
// DHT 22
 #include "dht.h"
 dht DHT;
 #define DHT22_PIN 0         // Pin en el que se encuentra conectado el sensor DHT22

// FM433 MHz
 #include <VirtualWire.h>


void setup() {
  //FM 433
    vw_set_ptt_inverted(true);
    vw_set_tx_pin(2);
    vw_setup(4000);// speed of data transfer Kbps

    pinMode(LDR_Vcc, OUTPUT);
}


void loop(){
  // lectura de DHT  
    int chk = DHT.read22(DHT22_PIN);
    
    if (chk == DHTLIB_OK) {     
        Hr = DHT.humidity;    // Lee humedad relativa 
        T = DHT.temperature;  // Lee temperatura en Celsius

      // conversion de un flotante en un array char
        dtostrf(Hr, 2, 2, Humedad);
        dtostrf(T, 2, 2, Temp);   
    }
    
  // Lectura LDR
    digitalWrite(LDR_Vcc, HIGH);
    delay(120000);
    LDR = analogRead(A2);
    digitalWrite(LDR_Vcc, LOW);
    // conversion de int en array char 
      dtostrf(LDR, 0, 0, LDR_c);


  // Lectura Vcc
    Vcc = analogRead(A3);
    // conversion de int en array char 
      dtostrf(Vcc, 0, 0, Vcc_c);
      
        
    for (int i=0; i<3; i++) { 
        sensor[3] = pos[i];
        muestra();     
      
      // Envio de los datos por FM433  
        vw_send((uint8_t *)sensor, strlen(sensor));
        vw_wait_tx(); // Wait until the whole message is gone  
        delay(5000);
    }
    
    delay(intervalo);  // intervalo
}


void muestra() {
  
      // incorporo Temperatura 
         for(int i=0; i<5; i++) {
             int h=i+6;
             sensor[h] = Temp[i];
         }

      // incorporo Humedad 
         for(int i=0; i<5; i++) {
             int h=i+11;
             sensor[h] = Humedad[i];
         }

       // incorporo LDR
          if (LDR <10) LDR_c[1] = LDR_c[2] = LDR_c[3] = espacio[0]; 
          if ((LDR >10) && (LDR <100))  LDR_c[2]  = LDR_c[3] = espacio[0]; 
          if ((LDR >100) && (LDR <1000))  LDR_c[3] = espacio[0]; 

          for(int i=0; i<4; i++) {
              int h=i+17;
              sensor[h] = LDR_c[i];
         }
         
        // incorporo Vcc
          if (Vcc <10) Vcc_c[1] = Vcc_c[2] = Vcc_c[3] = espacio[0]; 
          if ((Vcc >10) && (Vcc <100))  Vcc_c[2]  = Vcc_c[3] = espacio[0]; 
          if ((Vcc >100) && (Vcc <1000))  Vcc_c[3] = espacio[0]; 

          for(int i=0; i<4; i++) {
              int h=i+21;
              sensor[h] = Vcc_c[i];
         }
}


