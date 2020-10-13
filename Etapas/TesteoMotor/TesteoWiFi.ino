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
  // Declaramos el Buffer del mensaje
  String frase = "";
  char lectura = wifi.read();
  boolean sumar = true;
  
  // Si ha llegado algun caracter (o sea, cuando la lectura es distinta a -1), comienza a acumularse
  if(lectura != -1)
  {
    // Se acumula hasta que se dejen de recibir caracteres (-1), o se reciba un salto de linea (10)
    while(lectura != -1 && lectura != 10)
    {
      if(sumar == true) frase+=lectura;
      if(frase == "+IPD,")
      {
         frase="";
         sumar = false;
      }
      lectura = wifi.read();
      if(lectura == ':' and sumar == false) sumar=true;
    }
    Serial.println(frase);
  }

  // Aqui van los metodos de cada instruccion
  
  // Acelerar el motor
  if (frase == ":acelerar")
  {
    digitalWrite(13, HIGH);
  }
  
  // Frenar el motor
  if (frase == ":frenar")
  {
    digitalWrite(13, LOW);
  }
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
