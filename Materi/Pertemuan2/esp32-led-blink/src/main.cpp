#include <Arduino.h>

// Di ESP32, LED internal biasanya terhubung ke GPIO 2.
// Kita buat variabel untuk menyimpan nomor pin ini agar mudah diubah.
const int ledPin = 2; 

// Fungsi setup() - dijalankan sekali saat boot
void setup() {
  // Mengatur pin LED (GPIO 2) sebagai OUTPUT.
  // Artinya, kita akan mengirim sinyal LISTRIK KELUAR dari pin ini.
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); 
}

// Fungsi loop() - dijalankan terus-menerus
void loop() {
  // Menyalakan LED dengan mengirim sinyal HIGH (tegangan 3.3V)
  digitalWrite(ledPin, HIGH);
  Serial.println("LED ON");
  
  // Memberi jeda selama 1000 milidetik (1 detik)
  // LED akan tetap menyala selama periode ini.
  delay(1000); 

  // Mematikan LED dengan mengirim sinyal LOW (tegangan 0V / Ground)
  digitalWrite(ledPin, LOW);
  Serial.println("LED OFF");

  // Memberi jeda lagi selama 1 detik
  // LED akan tetap mati selama periode ini, sebelum loop() diulang dari awal.
  delay(1000);
}