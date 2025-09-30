### **Pertemuan 2**

**Nama Mata Kuliah:** Dasar-Dasar IoT dengan ESP32
**Topik Utama:** Instalasi Lingkungan Pengembangan & Proyek Pertama "Blink"
**Referensi Tambahan :** Ebook Arduino ID hal.17

-----

#### **A. Tujuan Pembelajaran**

Setelah mengikuti sesi ini, mahasiswa diharapkan mampu:

1.  Menginstal dan mengkonfigurasi salah satu dari dua lingkungan pengembangan: Arduino IDE atau PlatformIO (di VS Code).
2.  Memahami fungsi driver USB-to-UART (CH340/CP210x) dan cara menginstalnya.
3.  Memahami struktur dasar dari sebuah program Arduino (`setup()` dan `loop()`).
4.  Menulis, mengompilasi (*compile*), dan mengunggah (*upload*) program sederhana ke board ESP32.
5.  Membuat LED internal pada board ESP32 berkedip (*Blink*), sebagai "Hello World" di dunia *hardware*.

-----

**OPSI A: Arduino IDE (Direkomendasikan untuk Pemula)**

1.  **Download Arduino IDE:** Kunjungi situs resmi `arduino.cc` dan unduh versi terbaru (rekomendasikan versi 2.x).
2.  **Instalasi:** Jalankan installer yang sudah diunduh.
3.  **Menambahkan Dukungan ESP32:**
        * Buka Arduino IDE.
        * Masuk ke menu `File > Preferences`.
        * Di bagian **"Additional Boards Manager URLs"**, masukkan URL berikut:
        ```
        https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
        ```
        * Klik OK.
4.  **Instalasi Paket ESP32:**
        * Masuk ke menu `Tools > Board > Boards Manager...`
        * Di kotak pencarian, ketik `esp32`.
        * Akan muncul "esp32 by Espressif Systems". Klik **Install**. Proses ini mungkin butuh beberapa menit.
5.  **Konfigurasi Proyek:**
        * Setelah instalasi selesai, tutup Boards Manager.
        * Masuk ke `Tools > Board > esp32` dan pilih board yang paling umum: **"ESP32 Dev Module"**.
        * Di `Tools > Port`, pilih **nomor COM** yang muncul di Device Manager tadi.
        * **Setup Selesai\!** Arduino IDE siap digunakan.

-----

**OPSI B: PlatformIO (Lebih Profesional)**

1.  **Download VS Code:** Kunjungi situs resmi `code.visualstudio.com` dan instal.
2.  **Instal Ekstensi PlatformIO:**
        * Buka VS Code.
        * Klik ikon **Extensions** di sidebar kiri (terlihat seperti 4 kotak).
        * Cari `PlatformIO IDE` dan klik **Install**. Tunggu hingga instalasi selesai (bisa memakan waktu cukup lama). VS Code mungkin akan meminta untuk di-*reload*.
3.  **Membuat Proyek Baru:**
        * Klik ikon PlatformIO (kepala alien) di sidebar kiri, lalu klik **"New Project"**.
        * **Project Name:** Beri nama, misal `ESP32-Blink`.
        * **Board:** Ketik dan pilih **"Espressif ESP32 Dev Module"**.
        * **Framework:** Pastikan terpilih **"Arduino"**.
        * Klik **Finish**. PlatformIO akan menyiapkan struktur folder proyek.
4.  **Buka File Kode:** Di dalam folder proyek, buka `src > main.cpp`. Di sinilah kita akan menulis kode.
5.  **Konfigurasi & Upload:** PlatformIO biasanya mendeteksi port COM secara otomatis. Proses upload dilakukan dengan menekan ikon panah (→) di status bar bawah.

**Bagian 2: Proyek Pertama - Membuat ESP32 "Berkedip"**

  * **Konsep Kunci:**

      * `void setup() { ... }`: Bagian ini hanya dijalankan **satu kali** saat ESP32 pertama kali dinyalakan atau di-reset. Fungsinya untuk melakukan inisialisasi awal, seperti menentukan fungsi sebuah pin.
      * `void loop() { ... }`: Bagian ini dijalankan **berulang-ulang tanpa henti** setelah `setup()` selesai. Di sinilah logika utama program kita berada.

  * **Live Coding :**

<!-- end list -->

```cpp
#include <Arduino.h>

// Di ESP32, LED internal biasanya terhubung ke GPIO 2.
// Kita buat variabel untuk menyimpan nomor pin ini agar mudah diubah.
const int ledPin = 2; 

// Fungsi setup() - dijalankan sekali saat boot
void setup() {
  // Mengatur pin LED (GPIO 2) sebagai OUTPUT.
  // Artinya, kita akan mengirim sinyal LISTRIK KELUAR dari pin ini.
  pinMode(ledPin, OUTPUT); 
}

// Fungsi loop() - dijalankan terus-menerus
void loop() {
  // Menyalakan LED dengan mengirim sinyal HIGH (tegangan 3.3V)
  digitalWrite(ledPin, HIGH); 
  
  // Memberi jeda selama 1000 milidetik (1 detik)
  // LED akan tetap menyala selama periode ini.
  delay(1000); 

  // Mematikan LED dengan mengirim sinyal LOW (tegangan 0V / Ground)
  digitalWrite(ledPin, LOW); 
  
  // Memberi jeda lagi selama 1 detik
  // LED akan tetap mati selama periode ini, sebelum loop() diulang dari awal.
  delay(1000);
}
```

  * **Langkah-langkah Upload:**
    1.  Minta mahasiswa mengetik ulang kode di atas ke dalam IDE mereka.
    2.  **Verify/Compile:**
          * **Arduino IDE:** Klik ikon centang (✓).
          * **PlatformIO:** Klik ikon centang (✓) di status bar bawah.
          * Pastikan tidak ada error di konsol output.
    3.  **Upload:**
          * **Arduino IDE:** Klik ikon panah (→).
          * **PlatformIO:** Klik ikon panah (→).
          * Perhatikan konsol. Jika muncul tulisan `Connecting........_____....._____`, beberapa board ESP32 mengharuskan kita **menekan dan menahan tombol BOOT** di board hingga proses upload dimulai.
    4.  **Hasil:** Setelah "Done uploading" muncul, LED biru kecil di board ESP32 akan mulai berkedip dengan interval satu detik. Beri tepuk tangan\!

**Bagian 3: Troubleshooting (Pasti Terjadi)**

Siapkan slide atau catatan untuk masalah paling umum:

  * **Error: "COM port not found" atau "Port not selected"**:
      * **Solusi:** Pastikan board sudah terhubung, driver sudah terinstal, dan Port yang benar sudah dipilih di menu `Tools > Port`. Coba cabut-pasang kabel USB.
  * **Error: "Failed to connect to ESP32: Timed out"**:
      * **Solusi 1:** Coba tekan dan tahan tombol **BOOT** saat tulisan `Connecting...` muncul di konsol.
      * **Solusi 2:** Kabel USB yang digunakan mungkin jelek atau hanya untuk *charge*. Ganti dengan kabel data yang berkualitas.
  * **Upload Sukses, tapi LED Tidak Berkedip**:
      * **Solusi:** Beberapa varian board ESP32 mungkin menggunakan nomor pin yang berbeda untuk LED *onboard*. Minta mahasiswa mencoba mengganti `const int ledPin = 2;` menjadi nomor pin lain, misalnya `13` atau `22`.