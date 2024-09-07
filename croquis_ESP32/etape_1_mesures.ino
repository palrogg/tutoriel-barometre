// Code adapte de la “basic demo” fournie par Grove
// pour le senseur SHT35 (licence MIT)
// https://github.com/Seeed-Studio/Seeed_SHT35/blob/master/examples/basic_demo/basic_demo.ino

#include <Wire.h>

#include "M5Atom.h"
#include "Seeed_SHT35.h"

// Si on branche le cable a 4 broches d'un senseur Grove sur l'embase d'une carte Atom Lite,
// on utilise les pins 26 et 32 pour les lignes SDA et SCL.
// Autrement, on peut utiliser les ports par defaut, en general A4 et A5.
#define SDAPIN 26
#define SCLPIN 32

SHT35 sensor(SCLPIN);

void setup() {
  // On lance l'API de l'Atom Lite en activant la sortie vers le moniteur, mais
  // en desactivant le protocole I2C et le display LED.
  M5.begin(true, false, false);
  Wire.begin(SDAPIN, SCLPIN);
  Serial.begin(115200);
  delay(10);
  Serial.println("Setup de la carte...");
  if (sensor.init()) {
    Serial.println("ERREUR lors de l'initialisation du senseur.");
  }
}

void loop() {
  float temperature;
  float humidity;

  // On lit les donnees du senseur
  if (NO_ERROR==sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &temperature, &humidity)) {
    Serial.print("Temperature = ");
    Serial.println(temperature);
    Serial.print("Humidite = ");
    Serial.println(humidity);
    Serial.println();
  } else {
    Serial.println("ERREUR: impossible de lire les mesures du senseur");
  }
  delay(5000);
}
