#include <ArduinoJson.h>
#include <SD.h>
#include <TMRpcm.h>
#include <SPI.h>
#define SD_ChipSelectPin 53
TMRpcm tmrpcm;
int pinMotor = 3;

void setup() {
  Serial1.begin(9600);
  Serial1.setTimeout(10000);
  Serial.begin(9600);

  reiniciar();
  delay(3000);
  conectarWifi();
  delay(3000);
  conectarServidor();

  Serial.println("Las conexiones se efectuaron con exito");

  tmrpcm.speakerPin = 5;
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
  }

  // Se le comunica al servidor que se trata de un cliente Arduino
  Serial1.println("AT+CIPSEND=7");
  delay(1000);
  Serial1.println("Arduino");
  Serial1.setTimeout(250);
}

void loop() {
  char* leido;
  String suma;
  String fraseDefinitiva;
  if (Serial1.available())
  {
    String frase = Serial1.readString();
    if (frase.charAt(8) == ':')
    {
      fraseDefinitiva = frase.substring(9);
    }
    else
    {
      fraseDefinitiva = frase.substring(10);
    }
    Serial.println(fraseDefinitiva);
    StaticJsonDocument<300> doc;
    deserializeJson(doc, fraseDefinitiva);
    char* instruccion = doc["instruccion"];
    Serial.println("Instruccion: " + String(instruccion));

    if (doc["instruccion"] == "acelerar")
    {
      Serial.println("acelerando");
      digitalWrite(pinMotor, HIGH);
    }

    if (doc["instruccion"] == "frenar")
    {
      Serial.println("frenando");
      digitalWrite(pinMotor, LOW);
    }

    if (doc["instruccion"] == "clima")
    {
      Serial.println("reproduciendo temperatura");
      Serial.println("temperatura.wav");
      tmrpcm.play("temperatura.wav");
      delay(4000);
      leido = doc["temperatura"];
      String grados(leido);
      suma = grados + ".wav";
      tmrpcm.play(suma.c_str());
      delay(2000);
      Serial.println(suma);
      tmrpcm.play("grados_centigrados.wav");
      delay(3000);
      Serial.println("grados_centigrados.wav");
      tmrpcm.play("la_humedad_es_del.wav");
      delay(3000);
      Serial.println("la_humedad_es_del.wav");
      leido = doc["humedad"];
      String porcentaje(leido);
      suma = porcentaje + ".wav";
      tmrpcm.play(suma.c_str());
      delay(2000);
      Serial.println(suma);
      tmrpcm.play("por_ciento.wav");
      delay(3000);
      Serial.println("por_ciento.wav");
    }

    if (doc["instruccion"] == "dolar")
    {
      tmrpcm.play("precio_dolar.wav");
      Serial.println("precio_dolar.wav");
      leido = doc["precio"];
      String precio(leido);
      suma = precio + ".wav";
      tmrpcm.play(suma.c_str());
      Serial.println(suma);
    }

    if (doc["instruccion"] == "mensajeria")
    {
      if (doc["valor"] == "si")
      {
        tmrpcm.play("hay_notas_nuevas.wav");
        Serial.println("hay_notas_nuevas.wav");
      }
      else
      {
        tmrpcm.play("no_hay_notas_nuevas.wav");
        Serial.println("no_hay_notas_nuevas.wav");
      }
    }

    if (doc["instruccion"] == "alarma")
    {
      tmrpcm.play("alarma.wav");
      Serial.println("alarma.wav");
    }

    if (doc["instruccion"] == "unrecognized")
    {
      tmrpcm.play("noReconocido.wav");
      Serial.println("noReconocido.wav");
    }
  }
}


void reiniciar() {
  Serial1.println("AT+RST");
  if (Serial1.find("OK"))
  {
    Serial.println("Reiniciado");
  }
  else
  {
    Serial.println("Error reiniciando, intentando nuevamente");
    reiniciar();
  }
}


void conectarWifi() {
  // SSID y Clave del WiFi
  Serial1.println("AT+CWJAP=\"Fibertel WiFi997 2.4GHz\",\"panqueques\"");
  if (Serial1.find("OK"))
  {
    Serial.println("Conectado a WiFi");
  }
  else
  {
    Serial.println("Error conectandose al WiFi, intentando nuevamente");
    conectarWifi();
  }
}


void conectarServidor() {
  Serial1.println("AT+CIPSTART=\"TCP\",\"192.168.0.58\",25000");
  if (Serial1.find("OK"))
  {
    Serial.println("Conectado al servidor");
  }
  else
  {
    Serial.println("Error conectandose al servidor, intentando nuevamente");
    conectarServidor();
  }
}
