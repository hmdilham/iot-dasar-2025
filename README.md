# Materi IOT Dasar


### **Silabus Mata Kuliah: Dasar-Dasar IoT dengan ESP32**

**Deskripsi Mata Kuliah:** Mata kuliah ini memberikan pengenalan komprehensif mengenai pengembangan perangkat Internet of Things (IoT) menggunakan board mikrokontroler ESP32. Mahasiswa akan mempelajari konsep dasar, arsitektur, dan perbandingan ESP32 dengan platform lain. Fokus utama adalah pada praktik langsung, mulai dari instalasi lingkungan pengembangan (Arduino IDE dan PlatformIO), pemahaman protokol komunikasi (UART, I2C, SPI), interaksi dengan berbagai sensor dan aktuator, hingga integrasi dengan platform IoT (Blynk) untuk membangun proyek sederhana.

**Capaian Pembelajaran:**
* Mampu menyiapkan dan mengkonfigurasi lingkungan pengembangan untuk ESP32.
* Memahami perbedaan fundamental dan keunggulan ESP32 dibandingkan Arduino Uno.
* Menguasai implementasi protokol komunikasi serial (UART), I2C, dan SPI.
* Mampu membaca data dari berbagai jenis sensor (digital dan analog).
* Mampu mengendalikan berbagai jenis aktuator.
* Mampu mengintegrasikan ESP32 dengan platform IoT untuk monitoring dan kontrol jarak jauh.

---

### **Rancangan Perkuliahan (12 Pertemuan)**

#### **Sesi 1: Pengenalan IoT dan ESP32**
* **Topik:** Konsep dasar Internet of Things, pengenalan mikrokontroler, dan arsitektur ESP32.
* **Sub-Topik:**
    * Apa itu IoT dan contoh aplikasinya di dunia nyata?
    * Pengenalan *System on a Chip* (SoC) ESP32: CPU, WiFi, Bluetooth, GPIO.
    * Mengenal berbagai jenis board pengembangan ESP32 (ESP32-WROOM-32, DevKitC, dll).
* **Praktik:** Diskusi dan studi kasus proyek-proyek IoT yang inspiratif.

---

#### **Sesi 2: Instalasi & First Project**
* **Topik:** Menyiapkan Lingkungan Pengembangan (IDE).
* **Sub-Topik:**
    * **Opsi 1: Arduino IDE:** Instalasi Arduino IDE, setup *board manager* ESP32, dan instalasi driver USB to UART (CH340/CP210x).
    * **Opsi 2: PlatformIO:** Instalasi Visual Studio Code dan ekstensi PlatformIO. Membuat proyek baru untuk ESP32.
    * Struktur dasar program (`setup()` dan `loop()`).
* **Praktik:** Menulis dan mengunggah program "Hello World" ke hardware: membuat LED internal berkedip. 

---

#### **Sesi 3: GPIO dan Input Digital**
* **Topik:** *General Purpose Input/Output* (GPIO) dan Interaksi Dasar.
* **Sub-Topik:**
    * Konsep pin digital: INPUT, OUTPUT, INPUT\_PULLUP.
    * Membaca sinyal digital dari *push button*.
    * Konsep *debouncing* pada saklar.
* **Praktik:** Mengontrol LED eksternal menggunakan *push button*. Satu tombol untuk menyalakan, satu tombol untuk mematikan.

---

#### **Sesi 4: Arduino Uno vs ESP32 & Input Analog**
* **Topik:** Perbandingan Platform dan Membaca Sinyal Analog.
* **Sub-Topik:**
    * **Perbandingan Kunci:**
        * Prosesor (8-bit vs 32-bit dual-core).
        * Kecepatan *clock*.
        * Konektivitas (tanpa *shield* vs WiFi & Bluetooth *built-in*).
        * Memori (RAM & Flash).
        * Tegangan kerja (5V vs 3.3V) dan implikasinya.
    * Pengenalan *Analog to Digital Converter* (ADC).
* **Praktik:** Membaca nilai dari potensiometer dan menampilkannya di Serial Monitor. Mengontrol kecerahan LED (PWM) berdasarkan putaran potensiometer.

---

#### **Sesi 5: Protokol Komunikasi - UART**
* **Topik:** Komunikasi Serial Asinkron (*Universal Asynchronous Receiver-Transmitter*).
* **Sub-Topik:**
    * Konsep dasar UART: pin TX (Transmit) dan RX (Receive), *baud rate*.
    * Penggunaan `Serial.begin()`, `Serial.print()`, `Serial.read()`.
* **Praktik:** Mengirim data dari ESP32 ke komputer (Serial Monitor) dan sebaliknya. Contoh: ESP32 mengirim "Halo!" dan komputer mengirim perintah untuk menyalakan/mematikan LED.

---

#### **Sesi 6: Protokol Komunikasi - I2C**
* **Topik:** Komunikasi Antar Chip (*Inter-Integrated Circuit*).
* **Sub-Topik:**
    * Konsep I2C: pin SDA (Data) dan SCL (Clock), alamat perangkat (*device address*).
    * Keuntungan I2C: hemat pin, banyak perangkat dalam satu bus.
    * Menggunakan *library* `Wire.h`.
* **Praktik:** Membaca data suhu dan kelembapan dari sensor DHT11/DHT22 atau menampilkan teks pada layar OLED 128x64 yang keduanya menggunakan protokol I2C. 

---

#### **Sesi 7: Protokol Komunikasi - SPI**
* **Topik:** Komunikasi Serial Sinkron (*Serial Peripheral Interface*).
* **Sub-Topik:**
    * Konsep SPI: pin MISO, MOSI, SCLK, dan SS (*Slave Select*).
    * Keunggulan SPI: kecepatan tinggi, komunikasi *full-duplex*.
* **Praktik:** Membaca dan menulis data ke modul SD Card menggunakan protokol SPI.

---

#### **Sesi 8: Menggunakan Sensor**
* **Topik:** Mengumpulkan Data dari Lingkungan.
* **Sub-Topik:**
    * **Sensor Digital:** Sensor gerak PIR, sensor sentuh kapasitif.
    * **Sensor Analog:** Sensor cahaya LDR, sensor kelembapan tanah.
    * **Sensor Jarak:** Sensor ultrasonik HC-SR04.
* **Praktik:** Membuat proyek "Detektor Gerakan" menggunakan sensor PIR yang menyalakan LED dan mengirim notifikasi ke Serial Monitor.

---

#### **Sesi 9: Menggunakan Aktuator**
* **Topik:** Memberikan Aksi ke Dunia Fisik.
* **Sub-Topik:**
    * **Aktuator Sederhana:** Buzzer (menghasilkan suara).
    * **Motor:** Mengontrol posisi motor servo.
    * **Switching Tegangan Tinggi:** Menggunakan modul *relay* untuk mengontrol lampu 220V.
* **Praktik:** Membuat "Sistem Parkir Otomatis Sederhana". Sensor ultrasonik mendeteksi jarak mobil, dan motor servo (sebagai palang parkir) akan terbuka jika jarak cukup dekat.

---

#### **Sesi 10: Koneksi WiFi**
* **Topik:** Menghubungkan ESP32 ke Internet.
* **Sub-Topik:**
    * Mode WiFi: *Station* (STA) dan *Access Point* (AP).
    * Memindai jaringan WiFi yang tersedia.
    * Menghubungkan ESP32 ke jaringan WiFi lokal.
* **Praktik:** Menghubungkan ESP32 ke WiFi dan mendapatkan alamat IP. Menampilkan status koneksi pada Serial Monitor.

---

#### **Sesi 11: Interfacing dengan Blynk**
* **Topik:** Platform IoT untuk Monitoring dan Kontrol.
* **Sub-Topik:**
    * Apa itu Blynk? Pengenalan dashboard, widget, dan *virtual pin*.
    * Instalasi *library* Blynk.
    * Setup proyek baru di aplikasi Blynk.
* **Praktik:**
    1.  **Monitoring:** Mengirim data dari sensor (misal: suhu dari DHT11) ke *gauge widget* di aplikasi Blynk.
    2.  **Kontrol:** Menggunakan *button widget* di aplikasi Blynk untuk menyalakan dan mematikan LED yang terhubung ke ESP32 dari mana saja. 

---

#### **Sesi 12: Proyek Mini & Evaluasi**
* **Topik:** Integrasi Konsep dan Proyek Akhir.
* **Sub-Topik:**
    * Mahasiswa diminta merancang dan membangun proyek IoT sederhana yang mengintegrasikan minimal: 1 sensor, 1 aktuator, dan konektivitas Blynk.
    * Contoh Proyek:
        * Sistem penyiram tanaman otomatis berbasis kelembapan tanah.
        * Sistem monitoring suhu ruangan dengan notifikasi di aplikasi.
        * Lampu pintar yang bisa dikontrol via internet.
* **Kegiatan:** Presentasi proyek dan demo alat oleh mahasiswa.