/* 
********************************
  Receptor de RF-433 paraRaspy
  uso de Attiny85 como puente.
  Daniel Lopez Campos

  V 0.8
  mayo-2015
  
********************************

YA NO ------ Basado en http://www.raspberrypi-es.com/category/attiny85/

V 0.1
- Version inicial.Utilizo el attiny como puente entre el emisor de RF433 y Raspberry
No he encontrado forma de que Raspy trabaje como Slave, asi que solicito datos desde
Raspy a Attiny de forma periodica, para ver si han entrado muestras nuevas

V 0.2
- Simplifico el uso de los dos leds, dejando solo uno que indica que ha recibido la 
peticion.

V 0.3
- version funcional

V 0.4 
- borro contenido de "sensor" una vez enviado, para evitar los envios repetidos y 
ver que pasa cuando se pierde una señal.

V 0.6
- Cambio el puerto de envio de datos I2C por el UART, en modo debug (solo puede transmitir)
De esta forma attiny no espera a recibir una peticion para enviar su lectura. ESto va
a simplificar la implementacion de mas sensores, puesto que no tengo que discriminar 
en el attiny, sencillamente remito a Raspy todo lo que llegue. 
Como no dispongo de feedback para que Raspy informe al sensor de la llegada del mensaje
he pensado en realizar el envio 3 veces, para asegurar su llegada.

V 0.7
- Introduzco libreria SoftwareSerial para poder trabajar en TR/TX
- Por el momento funciona como receptor. Voy a implementar protocolo:

   ESTACION      SENSOR                    ESTACION
   solicitud --> respuesta con lectura --> confirmacion de recepcion SI --> ok
                                                       NO --> nueva solicitud
V 0.8
- Version funcional con envio y recepcion RT/XT -->radio RFM433

Pines: 

Attiny85
--------
                _____
    (reset) 5 -|  o  |- Vcc
        RX  3 -|     |- 2  emisor FM
       led  4 -|     |- 1  TX
          GND -|_____|- 0  receptor FM      
*/

// Serie 
  #include <SoftwareSerial.h>
  const int rx=3;
  const int tx=1;
  SoftwareSerial mySerial(rx,tx);

// FM433
 #include <VirtualWire.h>

// LED
 #define led 4

// variables
  char lectura[] = "---------|---------|";


void setup(){
  // Serie
    pinMode(rx,INPUT);
    pinMode(tx,OUTPUT);
    mySerial.begin(9600);

  //FM 433 transmisor
    vw_set_ptt_inverted(true);
    vw_set_tx_pin(2);
    vw_setup(4000);// speed of data transfer Kbps

  //FM 433 receptor
    vw_set_rx_pin(0);
    vw_rx_start();       // Start the receiver PLL running
    delay (3000);   

  // LED
    pinMode(led, OUTPUT);   
}


void loop(){
    if (mySerial.available()>0) {
       
            for (int i=0 ; i<21 ; i++) {
                char dato = mySerial.read();    
                lectura[i] = dato;
            }         
              // Envio de los datos por FM433  
            vw_send((uint8_t *)lectura, strlen(lectura));
            vw_wait_tx(); // Wait until the whole message is gone 
            Blink(1);
    }
    
    else {
        uint8_t buf[VW_MAX_MESSAGE_LEN];
        uint8_t buflen = VW_MAX_MESSAGE_LEN;  
        if (vw_get_message(buf, &buflen)) {
            for (int i = 0 ; i < 21; i++) {
                mySerial.write(buf[i]);
            }
            mySerial.println("");
            Blink(1);
        }
    }
}    


void Blink(byte times){ // poor man's display
  for (byte i=0; i< times; i++){
    digitalWrite(led,HIGH);
    delay (100);
    digitalWrite(led,LOW);
    delay (50);
  }
}





