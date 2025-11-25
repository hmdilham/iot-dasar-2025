### **Materi Sesi 7: Protokol Komunikasi - SPI (Serial Peripheral Interface)**

#### **A. Teori: Memahami SPI**

**1. Apa itu SPI?**
SPI (*Serial Peripheral Interface*) adalah protokol komunikasi data sinkron jarak dekat yang dikembangkan oleh Motorola. Berbeda dengan UART (asinkron, tanpa jam) atau I2C (2 kabel, lambat tapi hemat pin), SPI dirancang untuk **kecepatan tinggi**.

**2. Arsitektur Master-Slave**
Dalam SPI, selalu ada satu perangkat **Master** (biasanya ESP32 kita) yang mengendalikan komunikasi, dan satu atau lebih perangkat **Slave** (sensor, layar, atau SD Card).

**3. Empat Jalur Utama (The Big Four):**
Tidak seperti I2C yang hanya butuh 2 kabel, SPI butuh 4 kabel data utama:

  * **SCLK (Serial Clock):** Detak jantung komunikasi. Dihasilkan oleh Master. Setiap detak menandakan satu bit data dikirim/diterima.
  * **MOSI (Master Out Slave In):** Jalur data dari ESP32 (Master) KE perangkat (Slave).
  * **MISO (Master In Slave Out):** Jalur data dari perangkat (Slave) KEMBALI ke ESP32 (Master).
  * **CS / SS (Chip Select / Slave Select):** Pin untuk "membangunkan" slave.
      * Jika Master ingin bicara dengan Slave A, ia akan membuat pin CS Slave A menjadi **LOW**.
      * Jika pin CS bernilai **HIGH**, Slave akan mengabaikan semua sinyal (pura-pura tuli).
      * Inilah cara SPI menangani banyak slave: kita butuh satu pin CS unik untuk setiap perangkat tambahan.

**4. Keunggulan SPI:**

  * **Full-Duplex:** Bisa mengirim dan menerima data secara *bersamaan* (seperti telepon, bukan walkie-talkie).
  * **Kecepatan Tinggi:** Bisa mencapai kecepatan transfer data jauh di atas I2C (sangat cocok untuk SD Card atau layar TFT yang butuh transfer data besar).
  * **Sederhana:** Tidak butuh alamat unik seperti I2C.

-----

#### **B. Praktikum: Membaca & Menulis Kartu SD (MicroSD)**

Pada praktik ini, kita akan menjadikan ESP32 sebagai *Data Logger* sederhana. Kita akan menulis teks ke dalam file `.txt` di kartu SD, lalu membacanya kembali.

**Alat yang Dibutuhkan:**

1.  ESP32 DevKitC.
2.  Modul MicroSD Card Adapter (yang mendukung antarmuka SPI).
3.  Kartu MicroSD (Format **FAT32** sebelum digunakan, maks 16GB/32GB disarankan).
4.  Kabel Jumper.

**1. Skema Rangkaian (Wiring Diagram)**

ESP32 memiliki pin SPI *default* (biasa disebut VSPI). Gunakan koneksi berikut:

| Pin pada Modul SD Card | Pin pada ESP32 (Default VSPI) | Keterangan |
| :--- | :--- | :--- |
| **CS** | **GPIO 5** | Chip Select |
| **SCK / CLK** | **GPIO 18** | Serial Clock |
| **MOSI** | **GPIO 23** | Master Out Slave In |
| **MISO** | **GPIO 19** | Master In Slave Out |
| **VCC** | **5V** (VIN) | Modul biasanya butuh 5V |
| **GND** | **GND** | Ground |

**2. Persiapan PlatformIO (`platformio.ini`)**

Meskipun *library* SD dan SPI adalah bawaan kerangka kerja Arduino, praktik terbaik di PlatformIO adalah memastikan dependensi terlihat rapi.

Cek file `platformio.ini`, pastikan isinya minimal seperti ini:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
```

**3. Kode Program (`src/main.cpp`)**

Salin kode berikut. Kode ini mencakup penanganan error (jika kartu SD tidak terdeteksi) dan fungsi dasar *file handling*.

```cpp
#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Definisi Pin CS (Chip Select)
// Kita bisa mengubah ini ke pin digital mana saja, asalkan kabelnya sesuai.
const int CS_PIN = 5;

// --- Fungsi Pembantu untuk Menulis File ---
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Menulis file ke: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Gagal membuka file untuk menulis");
    return;
  }
  if(file.print(message)){
    Serial.println("File berhasil ditulis");
  } else {
    Serial.println("Gagal menulis file");
  }
  file.close();
}

// --- Fungsi Pembantu untuk Membaca File ---
void readFile(fs::FS &fs, const char * path){
  Serial.printf("Membaca file dari: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Gagal membuka file untuk membaca");
    return;
  }

  Serial.println("--- Isi File Mulai ---");
  while(file.available()){
    // Membaca karakter per karakter dan menampilkannya
    Serial.write(file.read());
  }
  Serial.println("\n--- Isi File Selesai ---");
  file.close();
}

// --- Fungsi Pembantu untuk Menambah Data (Append) ---
void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Menambah data ke file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Gagal membuka file untuk append");
    return;
  }
  if(file.print(message)){
    Serial.println("Pesan berhasil ditambahkan");
  } else {
    Serial.println("Gagal menambahkan pesan");
  }
  file.close();
}

void setup() {
  // Mulai Serial Monitor
  Serial.begin(115200);
  // Beri waktu sebentar agar Serial Monitor siap
  delay(1000); 

  Serial.println("\n\n--- Mulai Tes SD Card ---");

  // Inisialisasi SD Card
  // Jika gagal (misal: kabel kendor, kartu tidak ada, format salah)
  if(!SD.begin(CS_PIN)){
    Serial.println("Gagal mendeteksi SD Card!");
    Serial.println("Cek kabel, pastikan format FAT32, dan coba reset.");
    return; // Hentikan program di sini
  }

  Serial.println("SD Card terdeteksi!");

  // 1. Cek tipe kartu
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE){
    Serial.println("Tidak ada kartu SD terpasang.");
    return;
  }

  // 2. Menulis File baru (akan menimpa jika file sudah ada)
  writeFile(SD, "/log_iot.txt", "Halo! Ini adalah baris pertama.\r\n");

  // 3. Menambah baris baru (Append)
  appendFile(SD, "/log_iot.txt", "Ini baris kedua: Data Sensor masuk.\r\n");
  
  // 4. Membaca kembali isi file
  readFile(SD, "/log_iot.txt");
  
  Serial.println("Tes Selesai. Cabut SD Card dan cek di komputer jika mau.");
}

void loop() {
  // Kosong, karena kita hanya menjalankan tes satu kali di setup.
}
```

-----

#### **C. Langkah-Langkah Walkthrough untuk Mahasiswa**

1.  **Format Kartu SD:** Ingatkan mahasiswa untuk memformat kartu MicroSD mereka ke **FAT32** di laptop sebelum memasangnya ke modul. ESP32 sering gagal membaca format exFAT atau NTFS.
2.  **Rakit Rangkaian:** Minta mahasiswa merakit kabel saat ESP32 dalam kondisi mati (tidak tercolok USB) untuk menghindari korsleting.
3.  **Upload & Monitor:**
      * Setelah upload sukses, segera buka Serial Monitor.
      * Jika muncul "Gagal mendeteksi SD Card\!", minta mereka menekan tombol **EN/Reset** pada ESP32.
      * Jika masih gagal, cek kabel **MISO** dan **MOSI** (sering tertukar) dan pastikan modul SD Card mendapatkan daya 5V yang cukup.

-----

#### **D. Referensi Tambahan (Link)**

Berikut adalah sumber bacaan teknis yang kredibel untuk materi ini:

1.  **Espressif Official API Guide (SDMMC & SPI):** Dokumentasi resmi dari pembuat chip.
      * *Link:* [docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi\_master.html](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi_master.html)
2.  **Random Nerd Tutorials - ESP32 MicroSD Card:** Tutorial visual yang sangat bagus untuk pemula.
      * *Link:* [randomnerdtutorials.com/esp32-microsd-card-arduino/](https://randomnerdtutorials.com/esp32-microsd-card-arduino/)
3.  **SparkFun - Serial Peripheral Interface (SPI):** Penjelasan teori SPI yang sangat mendalam dengan diagram interaktif.
      * *Link:* [sparkfun.com/tutorials/serial-peripheral-interface-spi](https://www.google.com/search?q=https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi)