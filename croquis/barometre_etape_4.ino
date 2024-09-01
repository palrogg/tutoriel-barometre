#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "Seeed_BME280.h"

// A changer: votre reseau et mot de passe, l'adresse de votre serveur
const char* ssid = "nom-reseau-wifi";
const char* password = "mot-de-passe-reseau-wifi";
String serverName = "http://ip-ou-adresse-serveur/";

BME280 bme280;
WiFiClientSecure client;

// AJOUT: mode permettant de lire le voltage
ADC_MODE(ADC_VCC);

void setup() {
  delay(100);            // Un petit delai qui peut eviter des erreurs
  Serial.begin(115200);  // On prepare la transmission de messages

  // On initialise le barometre
  if (!bme280.init()) {
    Serial.println("Device error!");
  }

  // Connexion au WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");

  // Boucle: on attend que la connexion fonctionne
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i);
    Serial.print(" ");
    if (i > 20) {
      Serial.println("Connection timed out.");
    }
  }

  Serial.println("\n");
  Serial.println("Connection established! :-)");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  // On check l'adresse IP du barometre
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // AJOUT: on lit le voltage
    int batteryLevel = ESP.getVcc();
    String batteryLevelStr = String(batteryLevel);
    Serial.println("Battery level:" + batteryLevelStr);

    String temperature = String(bme280.getTemperature());
    String humidity = String(bme280.getHumidity());

    // On envoie une requete au serveur
    String path = "/simple-put.php?temperature=" + temperature +
                  "&humidity=" + humidity + "&battery_level=" + batteryLevelStr;
    String url = serverName + path;
    http.begin(client, url.c_str());
    int httpResponseCode = http.GET();

    // On envoie le resultat au moniteur
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  // On attent 5 minutes (5 fois 60 millisecondes)
  // Etape suivante: on utilisera "EnterSleep"
  delay(5 * 60 * 1000);
}
