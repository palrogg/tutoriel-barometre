#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Task.h>

#include "Seeed_BME280.h"

ADC_MODE(ADC_VCC);

// AJOUTS: duree de sommeil par defaut (15 minutes)
// et gestionnaire de taches
#define DEFAULT_SLEEPING_TIME 15 * 60 * 1000000
TaskManager taskManager;

BME280 bme280;
WiFiClientSecure client;
// A changer: votre reseau et mot de passe, l'adresse de votre serveur
const char* ssid = "nom-reseau-wifi";
const char* password = "mot-de-passe-reseau-wifi";
String serverName = "http://ip-ou-adresse-serveur/";

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
    // AJOUT: on transmet le voltage
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

  // AJOUT: on entre en sommeil profond
  delay(2000); // Un petit delai pour se laisser le temps de flasher la carte
  taskManager.EnterSleep(DEFAULT_SLEEPING_TIME);
}
