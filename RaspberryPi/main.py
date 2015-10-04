from time import sleep
import serial
import sys
import ibmiotf.device
from ibmiotf.codecs import jsonIotfCodec

deviceOptions = {"org": '9d8hsm', "type": 'Raspberry_gateway', "id": 'b827eba83e87', "auth-method": 'token', "auth-token": 'Q!8w0TMSw?9Si7JJMz'}
deviceCli = ibmiotf.device.Client(deviceOptions)
deviceCli.connect()
arduino = serial.Serial("/dev/ttyAMA0", timeout=1)
sleep(2)
lastTail = " "
while(1==1):
        idSensor = "S00"
        tempe = "00.00"
        hume = "00.00"
        ldr = "0000"
        vcc = "0000"
        #Obtener
        try:
                cadena = (arduino.readline())
                cadena2 = (cadena.decode('ascii'))
                if len(cadena2) != 0:
                        idSensor = cadena2[:3]
                        tail = cadena2[5:]
                        if(tail != lastTail):
                                lastTail = tail
                                tempe = cadena2[6:11]
                                hume = cadena2[11:16]
                                ldr = cadena2[17:21]
                                vcc = cadena2[21:24]
                                #Parsear
                                #Mandar a bluemix
                                myData = { 'idSensor' : idSensor, 'temperature' : tempe, 'Humidity' : hume, 'LDR': ldr, 'Battery' : vcc}
                                deviceCli.setMessageEncoderModule('json', jsonIotfCodec)
                                deviceCli.publishEvent(event="newData", msgFormat="json", data=myData)
        except KeyboardInterrupt:
                arduino.close()
