#include <SoftwareSerial.h>
// Autores: Ignacio Pieve Roiger & Leila Aylen Spini

SoftwareSerial wifi(6, 7);

void setup() {
  wifi.begin(115200);
  wifi.setTimeout(10000);
  Serial.begin(9600);

  reiniciar();
  delay(3000);
  conectarWifi();
  delay(3000);
  conectarServidor();

  Serial.println("Las conexiones se efectuaron con exito");


  // Se le comunica al servidor que se trata de un cliente Arduino
  wifi.println("AT+CIPSEND=7");
  delay(500);
  wifi.println("Arduino");
  delay(500);
}


void loop() {
  // Loop code
}


void reiniciar(){
  wifi.println("AT+RST");
  if(wifi.find("OK"))
  {
    Serial.println("Reiniciado");
  }
  else
  {
    Serial.println("Error reiniciando, intentando nuevamente");
    reiniciar();
  }
}


void conectarWifi(){
  // SSID y Clave del WiFi
  wifi.println("AT+CWJAP=\"Fibertel WiFi997 2.4GHz\",\"panqueques\"");
  if(wifi.find("OK"))
  {
    Serial.println("Conectado a WiFi");
  }
  else
  {
    Serial.println("Error conectandose al WiFi, intentando nuevamente");
    conectarWifi();
  }
}


void conectarServidor(){
  wifi.println("AT+CIPSTART=\"TCP\",\"192.168.0.122\",25000");
  if(wifi.find("OK"))
  {
    Serial.println("Conectado al servidor");
  }
  else
  {
    Serial.println("Error conectandose al servidor, intentando nuevamente");
    conectarServidor();
  }
}
