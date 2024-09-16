// Adaptation du fichier d’exemple “WifiClientInsecure.ino”

#include <WiFiClientSecure.h>

const char* ssid     = "nom-de-votre-reseau-wifi";
const char* password = "mot-de-passe";

const char*  server = "www.howsmyssl.com";  // Serveur de test

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // On tente de se connecter
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // 1 seconde d’attente entre chaque essai
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.println("\nStarting connection to server...");
  client.setInsecure(); // On ne verifie pas avec une signature
  if (!client.connect(server, 443))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");
    
    // Requete HTTP tres manuelle. On utilisera plutot une bibliotheque pour ca
    client.println("GET https://www.howsmyssl.com/a/check HTTP/1.0");
    client.println("Host: www.howsmyssl.com");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    // On affiche la reponse du serveur
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  }
}

void loop() {
}
