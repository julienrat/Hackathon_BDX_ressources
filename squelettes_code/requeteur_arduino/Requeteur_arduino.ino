/**
   BasicHTTPSClient.ino

   Created on: 14.10.2018
*/
// Programme pour effectuer une requête HTTPS via Wi-Fi depuis un microcontrôleur Arduino.

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
// Inclusion des bibliothèques nécessaires pour la gestion du Wi-Fi, des requêtes HTTP et de la connexion sécurisée.

// Stockage du certificat SSL racine (CA) pour vérifier la validité du serveur. Ici, il s'agit du certificat utilisé par Gandi pour le serveur "https://jigsaw.w3.org".
const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n" \
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n" \
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n" \
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n" \
"MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n" \
"BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n" \
"aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n" \
"dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n" \
"AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n" \
"3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n" \
"tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n" \
"Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n" \
"VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n" \
"79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n" \
"c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n" \
"Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n" \
"c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n" \
"UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n" \
"Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n" \
"BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n" \
"A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n" \
"Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n" \
"VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n" \
"ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n" \
"8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n" \
"iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n" \
"Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n" \
"XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n" \
"qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n" \
"VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n" \
"L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n" \
"jjxDah2nGN59PRbxYvnKkKj9\n" \
"-----END CERTIFICATE-----\n";

//URL récupérée via XTRADATA https://data.bordeaux-metropole.fr/geojson/help/#/default
const String HTTPS_URL = "https://data.bordeaux-metropole.fr/geojson/features/pc_captv_p?filter=%7B%22gid%22%3A%222451%22%7D&attributes=%5B%22gid%22%2C%22comptage_5m%22%2C%22mdate%22%2C%22libelle%22%5D&maxfeatures=3&orderby=&key=K7CB2JBXGI";

// Fonction pour synchroniser l'horloge du microcontrôleur avec un serveur NTP afin de s'assurer que l'heure est correcte pour valider les certificats SSL.
void setClock() {
  configTime(0, 0, "pool.ntp.org");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2) {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }
  // Attente que l'heure soit récupérée correctement depuis le serveur NTP.

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
  // Affiche l'heure actuelle une fois qu'elle est correctement configurée.
}

WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  // Initialisation de la communication série pour le débogage.

  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Les Usines", "usinesnouvelles");
  // Initialisation du mode Wi-Fi en tant que station et connexion à un point d'accès ("Les Usines" avec le mot de passe "usinesnouvelles").

  Serial.print("Waiting for WiFi to connect...");
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  Serial.println(" connected");
  // Boucle jusqu'à ce que la connexion Wi-Fi soit établie.

  setClock();
  // Synchronisation de l'heure via NTP après connexion au réseau.
}

void loop() {
  WiFiClientSecure *client = new WiFiClientSecure;
  // Création d'un client sécurisé pour gérer la connexion HTTPS.

  if(client) {
    client->setCACert(rootCACertificate);
    // Assignation du certificat racine au client sécurisé pour valider les connexions HTTPS.

    {
      HTTPClient https;
      // Initialisation d'un client HTTP sécurisé.

      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, HTTPS_URL)) {
        // Tentative de connexion à l'URL spécifiée (une API de Bordeaux Métropole).
        
        Serial.print("[HTTPS] GET...\n");
        int httpCode = https.GET();
        // Envoi d'une requête GET au serveur.

        if (httpCode > 0) {
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          // Si la réponse est positive, on affiche le code HTTP.

          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            // Si la requête est réussie (code 200 ou redirection permanente), on récupère et affiche la réponse du serveur.

            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          // Si la requête échoue, on affiche l'erreur correspondante.
        }

        https.end();
        // Fermeture de la connexion HTTPS.
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
        // Si la connexion échoue, un message d'erreur est affiché.
      }
    }

    delete client;
    // Libération de la mémoire allouée pour le client sécurisé.
  } else {
    Serial.println("Unable to create client");
    // Si la création du client échoue, un message d'erreur est affiché.
  }

  Serial.println("Waiting 10s before the next round...");
  delay(10000);
  // Attente de 10 secondes avant la prochaine itération de la boucle.
}
