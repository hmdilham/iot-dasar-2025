### **Materi Sesi 11: Interfacing dengan Blynk IoT**

**Tujuan Pembelajaran:**

1.  Mahasiswa memahami konsep arsitektur Blynk (Cloud, App, Device).
2.  Mahasiswa memahami konsep "Virtual Pin" sebagai jembatan data.
3.  Mahasiswa mampu melakukan konfigurasi *Template* pada Blynk Console.
4.  Mahasiswa mampu membuat sistem monitoring dan kontrol dua arah (Full Duplex) via internet.

-----

#### **A. Teori: Apa itu Blynk?**

**1. Ekosistem Blynk**
Blynk bukan sekadar aplikasi HP, melainkan ekosistem lengkap yang terdiri dari 3 bagian:

  * **Blynk App (Mobile):** Antarmuka (UI) di smartphone untuk memantau dan mengontrol.
  * **Blynk Server (Cloud):** "Otak" yang menghubungkan smartphone dengan hardware (ESP32).
  * **Blynk Library (Device):** Kode program yang ditanam di ESP32 untuk berbicara dengan server.

**2. Konsep Kunci: Virtual Pins (V0, V1, V2...)** 📍
Ini adalah konsep tersulit bagi pemula, namun paling krusial.

  * **Pin Fisik (GPIO):** Nyata, ada kakinya (misal GPIO 2, GPIO 4).
  * **Pin Virtual (V):** Saluran data di dalam *software* (Cloud).
      * Bayangkan Virtual Pin sebagai **"Keranjang Ajaib"** di awan.
      * Untuk **Monitoring:** ESP32 melempar data suhu ke "Keranjang V1". Aplikasi HP melihat isi "Keranjang V1" dan menampilkannya di layar.
      * Untuk **Kontrol:** Aplikasi HP menaruh perintah "NYALA" di "Keranjang V0". ESP32 mengecek "Keranjang V0" dan menyalakan lampu sesuai isinya.

**3. Aturan Emas Blynk: JANGAN PAKAI `delay()`** ⚠️
Di dalam fungsi `loop()`, dilarang keras menggunakan `delay()` yang panjang (misal `delay(2000)`).

  * **Alasan:** `delay()` membuat prosesor "tidur". Saat tidur, koneksi ke server Blynk akan putus (*disconnect*).
  * **Solusi:** Gunakan **BlynkTimer** (timer berbasis software) untuk mengirim data secara berkala.

-----

#### **B. Persiapan: Setup Blynk Cloud & Mobile**

Karena Blynk versi baru menggunakan sistem *Template*, langkah ini harus dilakukan di **Website (PC)** terlebih dahulu, baru di HP.

**Langkah 1: Konfigurasi di Blynk.Console (Web)**

1.  Buka [blynk.cloud](https://blynk.cloud) dan Login/Register.
2.  Masuk ke **Developer Zone** (ikon kunci Inggris) \> **My Templates** \> **New Template**.
      * Name: `Praktikum IoT ESP32`
      * Hardware: `ESP32`
      * Connection Type: `WiFi`
3.  Masuk ke tab **Datastreams** \> **New Datastream** \> **Virtual Pin**:
      * **Untuk Tombol (LED):** Name: `Kontrol LED`, Pin: `V0`, Data Type: `Integer` (0-1).
      * **Untuk Suhu (DHT):** Name: `Suhu Ruangan`, Pin: `V1`, Data Type: `Double`, Units: `Celsius`, Min/Max: 0-50.
4.  Simpan (*Save*).

**Langkah 2: Menambahkan Device**

1.  Klik ikon **Search (Kaca Pembesar)** \> **New Device** \> **From Template**.
2.  Pilih template yang baru dibuat.
3.  **PENTING:** Akan muncul **"Template ID"**, **"Device Name"**, dan **"Auth Token"**. Salin kode ini, kita butuh untuk di PlatformIO.

**Langkah 3: Setup di Aplikasi HP**

1.  Buka aplikasi Blynk IoT di HP.
2.  Device yang dibuat di web otomatis muncul.
3.  Buka device tersebut (ikon kunci Inggris/Setup Dashboard).
4.  Tambahkan **Button Widget**:
      * Pilih Datastream: `Kontrol LED (V0)`.
      * Mode: `Switch`.
5.  Tambahkan **Gauge Widget**:
      * Pilih Datastream: `Suhu Ruangan (V1)`.

-----

#### **C. Praktikum: Monitoring Suhu & Kontrol LED**

Kita akan menggabungkan materi **Sesi 6 (DHT11)** dan **Sesi 2 (LED)**.

**1. Skema Rangkaian (Wiring Diagram)**

  * **DHT11 Sensor:**
      * VCC -\> 3V3 ESP32
      * GND -\> GND ESP32
      * DATA -\> **GPIO 4**
  * **LED:**
      * Anoda (+) -\> **GPIO 2** (Atau gunakan Built-in LED jika ada di pin 2)
      * Katoda (-) -\> GND (via Resistor 220Ω)

**2. Persiapan PlatformIO (`platformio.ini`)**

Tambahkan library Blynk dan DHT sensor.

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps =
    blynkkk/Blynk @ ^1.3.2
    adafruit/DHT sensor library @ ^1.4.4
    adafruit/Adafruit Unified Sensor @ ^1.1.9
```

**3. Kode Program (`src/main.cpp`)**

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// --- 1. Konfigurasi Blynk ---
// Salin 3 baris ini dari Blynk.Console (Device Info)
#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"      // GANTI INI
#define BLYNK_TEMPLATE_NAME "Praktikum IoT" // GANTI INI
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxxx"   // GANTI INI

// --- 2. Konfigurasi WiFi ---
char ssid[] = "NamaWiFiAnda"; // GANTI INI
char pass[] = "PasswordWiFi"; // GANTI INI

// --- 3. Konfigurasi Hardware ---
#define LED_PIN 2       // Pin LED (V0)
#define DHTPIN 4        // Pin Sensor DHT (V1)
#define DHTTYPE DHT11   // Tipe Sensor (Ganti DHT22 jika perlu)

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
```

-----

#### **D. Langkah Walkthrough Praktikum**

1.  **Modifikasi Kode:** Minta mahasiswa mengganti `BLYNK_TEMPLATE_ID`, `Token`, `SSID`, dan `Password` dengan milik mereka sendiri.
2.  **Upload:** Tekan tombol upload di PlatformIO.
3.  **Cek Serial Monitor:** Pastikan muncul logo Blynk besar dan tulisan "Ready".
4.  **Uji Kontrol (V0):** Tekan tombol di aplikasi HP. Perhatikan LED di board ESP32 menyala/mati hampir seketika.
5.  **Uji Monitoring (V1):** Perhatikan Widget Gauge di HP. Coba tiup sensor DHT atau pegang dengan jari hangat, jarum gauge harus bergerak naik.

-----

#### **E. Referensi**

1.  **Dokumentasi Resmi Blynk:** Panduan paling akurat untuk fitur-fitur baru.
      * *Link:* [docs.blynk.io](https://docs.blynk.io/en/)
2.  **Random Nerd Tutorials - Getting Started with Blynk 2.0:** Tutorial langkah demi langkah yang sangat visual.
      * *Link:* [randomnerdtutorials.com/esp32-blynk-2-0-intro/](https://www.google.com/search?q=https://randomnerdtutorials.com/esp32-blynk-2-0-intro/)
3.  **Daftar Virtual Pin & Data Stream:** Penjelasan teknis tentang tipe data di Blynk.
      * *Link:* [docs.blynk.io/en/getting-started/template-quick-setup\#datastreams](https://www.google.com/search?q=https://docs.blynk.io/en/getting-started/template-quick-setup%23datastreams)