#include <Arduino.h>

// --- Konfigurasi ---
// Tentukan pin untuk LED. 
// Kita akan gunakan LED internal (built-in) yang ada di board.
// Pada kebanyakan board ESP32, ini adalah GPIO 2.
const int ledPin = 2; 

// --- Fungsi Setup ---
// Fungsi ini hanya berjalan SATU KALI saat ESP32 pertama kali dinyalakan.
void setup() {
  // 1. Inisialisasi Komunikasi Serialda
  // Memulai port serial dengan baud rate yang telah ditentukan.
  Serial.begin(115200);

  // 2. Inisialisasi Pin LED
  // Mengatur pin LED sebagai OUTPUT.
  pinMode(ledPin, OUTPUT);

  // 3. Beri jeda singkat
  // Ini untuk memberi waktu pada Serial Monitor untuk terhubung 
  // setelah ESP32 menyala, agar pesan di bawah ini tidak terlewat.
  delay(1000); 

  // 4. Mengirim data dari ESP32 ke Komputer (Serial Monitor)
  // Serial.println() mengirimkan teks dan diakhiri dengan baris baru.
  Serial.println("---------------------------------");
  Serial.println("Halo! ESP32 telah terhubung.");
  Serial.println("Sistem Kontrol LED via UART");
  Serial.println("---------------------------------");
  Serial.println("Ketik '1' lalu Enter untuk menyalakan LED.");
  Serial.println("Ketik '0' lalu Enter untuk mematikan LED.");
  Serial.println("Menunggu perintah...");
}

// --- Fungsi Loop ---
// Fungsi ini berjalan berulang-ulang tanpa henti.
void loop() {
  
  // 1. Cek apakah ada data yang dikirim DARI Komputer
  // Serial.available() akan mengembalikan jumlah byte (karakter) 
  // yang ada di buffer (antrian) serial.
  if (Serial.available() > 0) {
    
    // 2. Baca data yang masuk
    // Serial.read() membaca satu karakter (byte) dari buffer.
    char perintahMasuk = Serial.read();

    // 3. Proses perintah
    if (perintahMasuk == '1') {
      // Jika perintah adalah karakter '1'
      digitalWrite(ledPin, HIGH); // Nyalakan LED
      Serial.println("Perintah diterima: [1] -> LED NYALA");
    } 
    else if (perintahMasuk == '0') {
      // Jika perintah adalah karakter '0'
      digitalWrite(ledPin, LOW); // Matikan LED
      Serial.println("Perintah diterima: [0] -> LED MATI");
    }
    // Karakter lain (seperti Enter, spasi, atau huruf) akan diabaikan
  }

  // Jika tidak ada data serial, loop() akan berulang
  // tanpa melakukan apa-apa, hanya menunggu.
}