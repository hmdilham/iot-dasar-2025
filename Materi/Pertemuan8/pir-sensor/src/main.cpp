#include <Arduino.h>

// --- Definisi Pin ---
const int pirPin = 13;  // Pin Input dari sensor PIR
const int ledPin = 26;  // Pin Output ke LED Eksternal

// --- Variabel untuk Logika ---
int pirState = LOW;     // Menyimpan status sensor saat ini
int val = 0;            // Variabel sementara untuk menampung pembacaan pin

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  delay(1000); // Jeda agar serial stabil
  
  Serial.println("Sistem Deteksi Gerak Dimulai...");
  
  // Konfigurasi Pin
  pinMode(pirPin, INPUT);  // PIR adalah Input
  pinMode(ledPin, OUTPUT); // LED adalah Output
  
  // Pastikan LED mati di awal
  digitalWrite(ledPin, LOW);
}

void loop() {
  // 1. Baca nilai dari sensor PIR (HIGH atau LOW)
  val = digitalRead(pirPin);

  // 2. Logika Deteksi
  if (val == HIGH) {
    // --- Jika Gerakan Terdeteksi ---
    digitalWrite(ledPin, HIGH);  // Nyalakan LED
    
    // Cek apakah status sebelumnya LOW (baru saja berubah jadi ada gerakan)
    // Ini mencegah pengiriman pesan "Gerakan!" berulang-ulang setiap milidetik
    if (pirState == LOW) {
      Serial.println("AWAS! Gerakan Terdeteksi!");
      pirState = HIGH; // Update status simpanan
    }
  } 
  else {
    // --- Jika TIDAK Ada Gerakan ---
    digitalWrite(ledPin, LOW);   // Matikan LED
    
    // Cek apakah status sebelumnya HIGH (baru saja berhenti bergerak)
    if (pirState == HIGH) {
      Serial.println("Aman. Gerakan berhenti.");
      pirState = LOW; // Update status simpanan
    }
  }
}