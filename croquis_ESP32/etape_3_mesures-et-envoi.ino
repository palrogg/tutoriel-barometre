#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

#include "M5Atom.h"
// La bibliotheque du senseur utilise le meme
// nom de constante que WifiClientSecure,
// on la renomme pour eviter une erreur
#define err_t SHT35_err_t
#include "Seeed_SHT35.h"
#undef err_t

// Si on branche le cable a 4 broches d'un senseur Grove sur l'embase d'une
// carte Atom Lite, on utilise les pins 26 et 32 pour les lignes SDA et SCL.
// Autrement, on peut utiliser les ports par defaut, en general A4 et A5.
#define SDAPIN 26
#define SCLPIN 32

SHT35 sensor(SCLPIN);
WiFiClientSecure client;

const char* ssid = "nom-de-votre-reseau-wifi";
const char* password = "mot-de-passe";
String host = "votre-serveur";

void setup() {
  // On lance l'API de l'Atom Lite en activant la sortie vers le moniteur, mais
  // en desactivant le protocole I2C et le display LED.
  M5.begin(true, false, false);
  Wire.begin(SDAPIN, SCLPIN);
  Serial.begin(115200);
  delay(10);

  // On initialise le senseur
  Serial.println("\nInitialisation du senseur...");
  if (sensor.init()) {
    Serial.println("ERREUR lors de l'initialisation du senseur.");
    return;
  }
  // On se connecte au WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("=");
    // nouvel essai chaque seconde
    delay(1000);
  }

  Serial.print("\nConnexion faite avec ");
  Serial.println(ssid);

  // On recupere les mesures du senseur
  float temperature;
  float humidity;

  // On lit les donnees du senseur
  if (NO_ERROR == sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH,
                                                    &temperature, &humidity)) {
    Serial.print("Temperature = ");
    Serial.println(temperature);
    Serial.print("Humidite = ");
    Serial.println(humidity);
    Serial.println();
  } else {
    Serial.println("ERREUR: impossible de lire les mesures du senseur");
    return;
  }

  // On envoie les donnees au serveur
  client.setInsecure();  // pas de verification du certificat
  
  // Si vous utilisez une batterie externe, que vous souhaitez suivre votre voltage
  // et envoyer une alerte quand il tombe trop bas, vous pouvez utiliser
  // le parametre battery_level. Voir la version ESP8266 pour un exemple.
  String path = "/put.php?temperature=" + String(temperature) +
                "&humidity=" + String(humidity) + "&battery_level=9999";
  String url = "https://" + String(host) + path;
  HTTPClient http;

  http.begin(client, url.c_str());
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("Reponse HTTP: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Erreur: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void loop() {}
