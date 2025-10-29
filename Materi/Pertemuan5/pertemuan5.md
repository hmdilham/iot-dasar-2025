#### **Sesi 5: Protokol Komunikasi - UART**
* **Topik:** Komunikasi Serial Asinkron (*Universal Asynchronous Receiver-Transmitter*).
* **Sub-Topik:**
    * Konsep dasar UART: pin TX (Transmit) dan RX (Receive), *baud rate*.
    * Penggunaan `Serial.begin()`, `Serial.print()`, `Serial.read()`.
* **Praktik:** Mengirim data dari ESP32 ke komputer (Serial Monitor) dan sebaliknya. Contoh: ESP32 mengirim "Halo!" dan komputer mengirim perintah untuk menyalakan/mematikan LED.


#### **1. Teori UART (Universal Asynchronous Receiver-Transmitter)**
**Materi:**
- **Apa itu UART?**
  - Protokol komunikasi serial asinkron yang menggunakan dua pin: **TX (Transmit)** dan **RX (Receive)**.
  - Digunakan untuk komunikasi antara dua perangkat (contoh: ESP32 dengan komputer, atau dua ESP32).
  - Tidak memerlukan sinyal clock (asinkron), sehingga perlu kesepakatan **baud rate** (kecepatan komunikasi).

- **Komponen Frame UART:**
  1. **Start Bit**: Menandakan awal transmisi data (logika LOW).
  2. **Data Bits**: Data yang dikirim (5-9 bit, umumnya 8 bit).
  3. **Parity Bit**: Untuk pengecekan error (opsional).
  4. **Stop Bit**: Menandakan akhir transmisi (logika HIGH).

- **Baud Rate:**
  - Kecepatan komunikasi (contoh: 9600, 115200 bps).
  - Kedua perangkat harus menggunakan baud rate yang sama.

#### **2. Praktikum 1: Komunikasi UART dengan Serial Monitor**

```cpp
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
```

---
**Referensi Bacaan:**
- [UART Communication Guide](https://www.circuitbasics.com/basics-uart-communication/)
- [ESP32 UART Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html)

