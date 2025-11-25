### **Materi Sesi 8: Menggunakan Sensor**

**Tujuan:** Mahasiswa memahami perbedaan prinsip kerja sensor digital, analog, dan berbasis waktu (ultrasonik), serta mampu mengimplementasikannya pada ESP32.

-----

#### **A. Teori: Klasifikasi Sensor**

**1. Sensor Digital (0 atau 1)**
Sensor jenis ini hanya memberikan dua kemungkinan output: HIGH (3.3V) atau LOW (0V). Ini adalah jenis sensor yang paling mudah dibaca oleh mikrokontroler.

  * **PIR (Passive Infrared) Sensor:**
      * **Cara Kerja:** Sensor ini tidak memancarkan cahaya, tetapi "melihat" perubahan radiasi inframerah (panas tubuh) di depannya. Sensor ini memiliki dua slot material sensitif IR. Saat tidak ada gerakan, kedua slot menerima jumlah radiasi yang sama (seimbang). Saat tubuh hangat melintas, satu slot menangkap radiasi lebih dulu dari yang lain, menciptakan perbedaan sinyal yang memicu output HIGH.
      * **Tuning:** Modul HC-SR501 biasanya memiliki dua potensiometer oranye: satu untuk sensitivitas (jarak), satu untuk *time delay* (berapa lama sinyal tetap HIGH setelah gerakan terdeteksi).
  * **Sensor Sentuh Kapasitif (Built-in):**
      * **Cara Kerja:** ESP32 memiliki fitur unik yaitu 10 pin GPIO yang bisa mendeteksi sentuhan jari (capacitive touch). Tubuh manusia mengandung muatan listrik. Saat kita menyentuh pin ini, kapasitansi sirkuit berubah, dan ESP32 bisa mendeteksinya tanpa tombol fisik.

**2. Sensor Analog (Rentang Nilai)**
Dunia nyata tidak hanya hitam-putih (0/1), tapi memiliki gradasi. Sensor analog menghasilkan tegangan variabel (misal 0V hingga 3.3V) yang merepresentasikan besaran fisik.

  * **ESP32 ADC (Analog to Digital Converter):** ESP32 memiliki ADC resolusi 12-bit. Artinya, ia bisa menerjemahkan tegangan 0-3.3V menjadi angka digital **0 hingga 4095**.

  * **LDR (Light Dependent Resistor):**

      * Resistansi LDR berubah tergantung intensitas cahaya (Terang = Resistansi Rendah, Gelap = Resistansi Tinggi).
      * **PENTING:** Mikrokontroler hanya bisa membaca *perubahan tegangan*, bukan perubahan resistansi. Oleh karena itu, LDR harus dirangkai dalam sirkuit **Pembagi Tegangan (Voltage Divider)** bersama resistor tetap (biasanya 10kΩ) agar bisa dibaca pin analog.
      * 
  * **Sensor Kelembapan Tanah (Soil Moisture - Resistive):**

      * Bekerja dengan mengalirkan arus listrik melalui tanah. Tanah basah menghantarkan listrik lebih baik (resistansi rendah) daripada tanah kering.

**3. Sensor Jarak Ultrasonik (HC-SR04)**
Sensor ini unik karena tidak mengirim sinyal analog maupun digital sederhana, melainkan berbasis durasi waktu gelombang suara.

  * **Cara Kerja (Prinsip Kelelawar):**
    1.  **Trigger:** Mengirim gelombang suara ultrasonik (40kHz).
    2.  **Echo:** Menunggu pantulan suara tersebut kembali.
    3.  **Kalkulasi:** Jarak dihitung berdasarkan selisih waktu kirim dan terima.
    <!-- end list -->
      * *Rumus:* $Jarak = \frac{Waktu \times Kecepatan Suara (343 m/s)}{2}$

-----

#### **B. Praktikum: Proyek "Detektor Gerakan"**

Pada praktik ini, kita akan membuat sistem keamanan sederhana. Jika PIR mendeteksi gerakan, ESP32 akan menyalakan LED peringatan dan mengirim laporan ke komputer.

**1. Alat dan Bahan**

  * 1x ESP32 DevKitC.
  * 1x Sensor PIR (HC-SR501).
  * 1x LED (Warna Merah disarankan).
  * 1x Resistor 220Ω (untuk LED).
  * Kabel Jumper (Male-to-Female & Male-to-Male).
  * Breadboard.

**2. Skema Rangkaian (Wiring Diagram)**

| Komponen | Pin Komponen | Pin ESP32 / Power | Keterangan |
| :--- | :--- | :--- | :--- |
| **PIR Sensor** | VCC / + | VIN (5V) | PIR HC-SR501 lebih stabil di 5V |
| | GND / - | GND | Ground |
| | OUT / D | GPIO 13 | Pin Input Digital |
| **LED** | Anoda (+) | GPIO 26 | Pin Output (via Resistor 220Ω) |
| | Katoda (-) | GND | Ground |

*Catatan: Jika Anda membuka kubah putih lensa PIR, Anda akan melihat label pin VCC, OUT, dan GND di dekat kaki-kakinya.*

**3. Kode Program PlatformIO (`src/main.cpp`)**

Salin kode berikut ke proyek PlatformIO Anda. Kode ini menggunakan teknik *State Change Detection* agar serial monitor tidak dibanjiri pesan berulang-ulang saat orang masih berdiri di depan sensor.

```cpp
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
```

-----

#### **C. Langkah-Langkah Walkthrough**

1.  **Setting Hardware:**

      * Pasang rangkaian sesuai diagram.
      * **Penting:** Pada sensor PIR HC-SR501, pastikan *jumper* (biasanya ada pin kuning kecil di sudut) diposisikan pada mode **'H' (Repeat Trigger)**. Ini memungkinkan sensor tetap HIGH selama gerakan terus terjadi.
      * Putar potensiometer "Sensitivity" dan "Time Delay" ke posisi tengah terlebih dahulu menggunakan obeng kecil.

2.  **Upload & Monitor:**

      * Sambungkan ESP32, upload kode via PlatformIO.
      * Buka Serial Monitor.

3.  **Pengujian:**

      * Arahkan tangan Anda di depan sensor. LED harus menyala dan Serial Monitor mencetak "AWAS\! Gerakan Terdeteksi\!".
      * Diamkan tangan atau menjauh. LED harus mati dan muncul pesan "Aman. Gerakan berhenti.".
      * **Troubleshooting:** Sensor PIR butuh waktu "pemanasan" (kalibrasi) sekitar 30-60 detik saat pertama kali diberi daya. Selama waktu ini, sensor mungkin memberikan alarm palsu.

-----

#### **D. Referensi Tambahan**

Untuk memperdalam materi, berikut adalah referensi terpercaya yang bisa Anda berikan kepada mahasiswa:

1.  **Random Nerd Tutorials - ESP32 PIR Motion Sensor:** Panduan sangat lengkap tentang cara kerja PIR dengan ESP32.
      * *Link:* [randomnerdtutorials.com/esp32-pir-motion-sensor-interrupts-timers/](https://randomnerdtutorials.com/esp32-pir-motion-sensor-interrupts-timers/)
2.  **Last Minute Engineers - How HC-SR501 PIR Works:** Penjelasan teknis mendalam tentang cara kerja internal sensor PIR.
      * *Link:* [lastminuteengineers.com/pir-sensor-arduino-tutorial/](https://lastminuteengineers.com/pir-sensor-arduino-tutorial/)
3.  **Espressif - ADC (Analog to Digital Converter):** Dokumentasi resmi mengenai karakteristik input analog pada ESP32.
      * *Link:* [docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)