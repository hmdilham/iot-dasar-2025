#include<Arduino.h>
#include <WiFi.h> // Untuk ESP32
// #include <ESP8266WiFi.h> // Untuk ESP8266

const char* ssid = "Blindspot";       // Nama jaringan Wi-Fi
const char* password = "ilham.12345"; // Password Wi-Fi

void setup() {
  Serial.begin(115200);

  // Mulai koneksi Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nTerhubung ke Wi-Fi!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Kosongkan loop, koneksi Wi-Fi sudah terjaga
}