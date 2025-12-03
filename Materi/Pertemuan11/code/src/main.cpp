// --- 1. Konfigurasi Blynk ---
// Salin 3 baris ini dari Blynk.Console (Device Info)
// PENTING: Define SEBELUM include BlynkSimpleEsp32.h
#define BLYNK_TEMPLATE_ID "edit"
#define BLYNK_TEMPLATE_NAME "edit"
#define BLYNK_AUTH_TOKEN "edit"

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// --- 2. Konfigurasi WiFi ---
char ssid[] = "edit"; // GANTI INI
char pass[] = "edit"; // GANTI INI

// --- 3. Konfigurasi Hardware ---
#define LED_PIN 2       // Pin LED (V0)
#define DHTPIN 4        // Pin Sensor DHT (V1)
#define DHTTYPE DHT22   // Tipe Sensor (Ganti DHT22 jika perlu)

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;       // Objek Timer pengganti delay()

// --- FUNGSI 1: KONTROL (HP -> ESP32) ---
// Fungsi ini dipanggil OTOMATIS oleh library Blynk
// ketika tombol di HP (V0) ditekan.
BLYNK_WRITE(V0) {
  int pinValue = param.asInt(); // Baca nilai dari tombol (0 atau 1)
  
  if (pinValue == 1) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Lampu Dinyalakan via Internet");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Lampu Dimatikan via Internet");
  }
}

// --- FUNGSI 2: MONITORING (ESP32 -> HP) ---
// Fungsi ini akan kita panggil setiap detik menggunakan Timer
void kirimSensor() {
  float t = dht.readTemperature(); // Baca suhu
  
  if (isnan(t)) {
    Serial.println("Gagal membaca DHT sensor!");
    return;
  }

  // Kirim data suhu ke Virtual Pin V1
  Blynk.virtualWrite(V1, t);
  
  Serial.print("Suhu dikirim: ");
  Serial.print(t);
  Serial.println(" C");
}

void setup() {
  // Inisialisasi Serial & Pin
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  // Inisialisasi Sensor
  dht.begin();

  // Koneksi ke Blynk & WiFi
  // Fungsi ini memblokir (blocking) sampai terkoneksi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup Timer
  // Jalankan fungsi 'kirimSensor' setiap 1000ms (1 detik)
  timer.setInterval(1000L, kirimSensor);
  
  Serial.println("Sistem Online.");
}

void loop() {
  // WAJIB ADA: Menjaga koneksi ke server
  Blynk.run();
  
  // WAJIB ADA: Menjalankan timer
  timer.run();
}