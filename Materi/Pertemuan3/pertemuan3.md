### **Detail Pertemuan 3**

**Nama Mata Kuliah:** Dasar-Dasar IoT dengan ESP32
**Topik Utama:** GPIO dan Input Digital

-----

#### **A. Tujuan Pembelajaran**

Setelah mengikuti sesi ini, mahasiswa diharapkan mampu:

1.  Menjelaskan secara konseptual apa itu GPIO dan sinyal digital (HIGH/LOW).
2.  Memahami perbedaan dan penggunaan mode pin `OUTPUT`, `INPUT`, dan `INPUT_PULLUP`.
3.  Menjelaskan masalah *floating pin* dan bagaimana `INPUT_PULLUP` menyelesaikannya.
4.  Merangkai sirkuit sederhana menggunakan *breadboard*, LED, *push button*, dan resistor.
5.  Membaca kondisi *push button* dalam kode dan menggunakannya untuk mengontrol LED.
6.  Memahami konsep dasar *debouncing* dan mengimplementasikannya dengan `delay()`.

-----

#### **B. Alat dan Bahan**

  * **Software:** Visual Studio Code dengan ekstensi PlatformIO yang sudah terinstal.
  * **Hardware (per mahasiswa/kelompok):**
      * 1x Board ESP32 DevKitC
      * 1x Kabel Data Micro-USB
      * 1x *Breadboard* (papan proyek)
      * 1x LED 5mm (warna apa saja)
      * 1x Resistor 220 Ohm (atau 330 Ohm)
      * 2x *Push Button* (tipe 4-pin)
      * Kabel Jumper (secukupnya)

-----

#### **C. Rancangan Kegiatan (Alokasi Waktu 100 Menit)**

  * **10 Menit: Pembukaan dan Review**

      * Review Sesi 2: "Kita sudah berhasil 'menghidupkan' ESP32 dengan membuat LED internal berkedip. Hari ini, kita akan membuatnya berinteraksi dengan dunia luar."
      * Menyampaikan tujuan sesi ini: membaca input dari tombol untuk mengontrol output ke LED.

  * **35 Menit: Penjelasan Materi (Konsep Inti)**

      * Apa itu GPIO? Analogi "Jalur Komunikasi" (5 menit).
      * Sinyal Digital: HIGH vs LOW (5 menit).
      * Mode Pin: `OUTPUT`, `INPUT`, dan masalah *Floating Pin* (10 menit).
      * Solusi Ajaib: `INPUT_PULLUP` dan logika terbalik (10 menit).
      * Masalah Tombol: Konsep *Debouncing* (5 menit).

  * **45 Menit: Praktikum Terbimbing**

      * Penjelasan skema rangkaian (5 menit).
      * Mahasiswa merakit sirkuit di *breadboard* (15 menit).
      * Membuat proyek baru di PlatformIO dan menulis kode (*live coding*) (20 menit).
      * Upload dan *troubleshooting* (5 menit).

  * **10 Menit: Penutup & Diskusi**

      * Rangkuman: Input -\> Proses -\> Output.
      * Tanya jawab mengenai masalah yang dihadapi.
      * Pengantar Sesi 4: Membaca sinyal analog dari potensiometer.

-----

#### **D. Detail Materi Ajar (Penjelasan Mendalam)**

Anda bisa menyajikan ini dalam bentuk slide presentasi yang menarik.

**1. Apa itu GPIO? (General Purpose Input/Output)** 📍

Bayangkan ESP32 adalah sebuah otak di dalam kotak tertutup. Bagaimana caranya otak ini bisa tahu apa yang terjadi di luar dan bagaimana ia bisa melakukan sesuatu di luar? Jawabannya adalah melalui **GPIO**.

  * **Analogi:** Anggaplah GPIO sebagai **terminal serbaguna** di sebuah stasiun. Satu terminal bisa diatur menjadi **pintu keberangkatan** (mengirim sinyal **OUTPUT**) atau menjadi **pintu kedatangan** (menerima sinyal **INPUT**).
  * Setiap pin GPIO pada ESP32 adalah satu terminal yang bisa kita program fungsinya. Kita bisa menggunakannya untuk menyalakan LED, membaca sensor, memutar motor, dan banyak lagi.

**2. Konsep Pin Digital: Dunia Hitam dan Putih** ⚫⚪

Pin digital hanya mengenal dua kondisi, tidak ada abu-abu.

  * **HIGH (1):** Ada tegangan listrik (pada ESP32, ini berarti **3.3 Volt**). Dianggap sebagai kondisi **ON**.
  * **LOW (0):** Tidak ada tegangan listrik (**0 Volt** atau terhubung ke **Ground/GND**). Dianggap sebagai kondisi **OFF**.

**3. Tiga Mode Pin Digital (`pinMode`)** ⚙️

**a. `OUTPUT`**

  * **Fungsi:** Mengatur pin agar **mengirimkan** sinyal keluar.
  * **Analogi:** Pin ini menjadi sebuah "Mulut" 👄. ESP32 "berbicara" ke komponen lain.
  * **Perintah:** `digitalWrite(pin, HIGH)` untuk menyalakan (memberi 3.3V), `digitalWrite(pin, LOW)` untuk mematikan.
  * **Contoh:** Menyalakan dan mematikan LED.

**b. `INPUT`**

  * **Fungsi:** Mengatur pin agar **membaca** sinyal dari luar.
  * **Analogi:** Pin ini menjadi sebuah "Telinga" 👂. ESP32 "mendengarkan" kondisi dari komponen lain.
  * **Perintah:** `digitalRead(pin)` akan mengembalikan nilai `HIGH` atau `LOW`.
  * **Masalah Serius - *Floating Pin* (Pin Mengambang):**
      * Jika sebuah pin `INPUT` tidak terhubung ke manapun (misal, saat tombol belum ditekan), kondisinya menjadi tidak jelas. Ia seperti antena yang menangkap noise listrik di udara.
      * Akibatnya, hasil pembacaan `digitalRead()` bisa acak, kadang HIGH, kadang LOW, tanpa bisa diprediksi. Ini akan membuat program kita kacau.

**c. `INPUT_PULLUP` (Solusi Cerdas)** ✨

  * **Fungsi:** Ini adalah mode `INPUT` spesial yang **mengaktifkan resistor internal** di dalam chip ESP32.
  * **Cara Kerja:** Resistor internal ini menghubungkan pin ke tegangan 3.3V secara "lemah".
      * **Saat tombol tidak ditekan:** Pin akan ditarik (*pulled up*) ke HIGH oleh resistor internal. `digitalRead()` akan mengembalikan **HIGH**.
      * **Saat tombol ditekan:** Tombol akan menghubungkan pin langsung ke Ground (GND), "mengalahkan" tarikan lemah dari resistor. Pin akan terbaca sebagai **LOW**.
  * **Keuntungan:** Kita tidak perlu menambahkan resistor eksternal pada rangkaian tombol kita, sehingga lebih simpel\!
  * 🚨 **Poin Kunci:** Logikanya menjadi **terbalik**. **Tidak Ditekan = HIGH**, **Ditekan = LOW**.

**4. Konsep Debouncing pada Saklar** 🏀

  * **Masalah:** Secara fisik, saat kita menekan tombol, lempengan logam di dalamnya tidak langsung menempel sempurna. Mereka akan **memantul** (*bounce*) beberapa kali dalam hitungan milidetik sebelum stabil.
  * **Efek:** ESP32 yang sangat cepat akan membaca pantulan ini sebagai puluhan kali penekanan tombol dalam sekejap.
  * **Solusi Sederhana (Software Debouncing):** Setelah mendeteksi penekanan tombol pertama, kita beri jeda `delay()` singkat (misal, 50 milidetik). Ini memberi waktu bagi tombol untuk berhenti memantul. Selama jeda itu, kita abaikan semua pembacaan lain dari tombol.

-----

#### **E. Langkah-Langkah Praktikum (PlatformIO)**

**Langkah 1: Merangkai Sirkuit**

Susun komponen di atas *breadboard* sesuai skema berikut.

  * **LED:** Kaki panjang (Anoda) terhubung ke **Resistor 220Ω**. Ujung resistor lainnya terhubung ke **GPIO 23**. Kaki pendek (Katoda) terhubung ke **GND**.
  * **Tombol ON:** Satu kaki terhubung ke **GPIO 18**. Kaki di seberangnya (secara diagonal) terhubung ke **GND**.
  * **Tombol OFF:** Satu kaki terhubung ke **GPIO 19**. Kaki di seberangnya (secara diagonal) terhubung ke **GND**.

**Langkah 2: Menyiapkan Proyek di PlatformIO**

1.  Buka VS Code.
2.  Klik ikon PlatformIO (kepala alien) \> **New Project**.
3.  **Name:** `Sesi3_TombolLED`
4.  **Board:** `Espressif ESP32 Dev Module`
5.  **Framework:** `Arduino`
6.  Klik **Finish**. Tunggu hingga proyek dibuat.
7.  Buka file `src/main.cpp`.

**Langkah 3: Menulis Kode Program**

Salin dan tempel kode berikut ke dalam `src/main.cpp`. Ajak mahasiswa untuk mengetiknya baris per baris sambil Anda menjelaskan fungsinya.

```cpp
#include <Arduino.h>

// Mendefinisikan pin yang kita gunakan
const int ledPin = 23;      // Pin tempat LED terhubung
const int tombolOnPin = 18; // Pin untuk tombol ON
const int tombolOffPin = 19;// Pin untuk tombol OFF

void setup() {
  // Mengatur mode untuk setiap pin
  pinMode(ledPin, OUTPUT);         // Pin LED sebagai output
  pinMode(tombolOnPin, INPUT_PULLUP); // Pin tombol ON sebagai input dengan pull-up internal
  pinMode(tombolOffPin, INPUT_PULLUP);// Pin tombol OFF sebagai input dengan pull-up internal
}

void loop() {
  // Membaca kondisi kedua tombol
  // Ingat, karena INPUT_PULLUP, DITEKAN = LOW, TIDAK DITEKAN = HIGH
  bool kondisiTombolOn = digitalRead(tombolOnPin);
  bool kondisiTombolOff = digitalRead(tombolOffPin);

  // Logika Program
  // Jika tombol ON ditekan (kondisinya LOW)
  if (kondisiTombolOn == LOW) {
    digitalWrite(ledPin, HIGH); // Nyalakan LED
    delay(50); // Debounce delay sederhana
  }

  // Jika tombol OFF ditekan (kondisinya LOW)
  if (kondisiTombolOff == LOW) {
    digitalWrite(ledPin, LOW); // Matikan LED
    delay(50); // Debounce delay sederhana
  }
}
```

**Langkah 4: Upload dan Uji Coba**

1.  Hubungkan ESP32 ke komputer.
2.  Klik ikon **Upload** (panah →) di status bar bawah VS Code.
3.  Tunggu hingga proses selesai. Jika ada masalah koneksi, tahan tombol **BOOT** saat proses upload.
4.  Setelah berhasil, tekan **tombol ON** (terhubung ke GPIO 18), maka LED akan menyala. Tekan **tombol OFF** (terhubung ke GPIO 19), maka LED akan mati.

-----

#### **F. Penutup dan Diskusi**

  * **Rangkuman:** "Hari ini kita telah membuat sistem interaktif pertama kita. Kita berhasil memberi 'telinga' pada ESP32 menggunakan `INPUT_PULLUP` untuk mendengar tombol, dan tetap menggunakan 'mulut'-nya (`OUTPUT`) untuk memerintah LED. Ini adalah fondasi dari semua proyek IoT."
  * **Diskusi:** Apa yang terjadi jika kita tidak menggunakan `delay(50)`? (LED mungkin berkedip tidak stabil saat tombol ditekan). Apa keuntungan menggunakan `INPUT_PULLUP`? (Rangkaian lebih sederhana, tidak butuh resistor eksternal).