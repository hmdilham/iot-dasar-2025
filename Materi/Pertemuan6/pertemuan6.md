### **Pertemuan 6: Protokol Komunikasi - I2C**

#### **1. Teori I2C (Inter-Integrated Circuit)**
**Materi:**
- **Apa itu I2C?**
  - Protokol komunikasi serial **sinkron** yang menggunakan dua pin: **SDA (Serial Data)** dan **SCL (Serial Clock)**.
  - Mendukung komunikasi **multi-master** dan **multi-slave**.
  - Kecepatan: Standar (100 kbps), Fast Mode (400 kbps), High-Speed (3.4 Mbps).

- **Komponen Frame I2C:**
  1. **Start Condition**: Sinyal LOW pada SDA saat SCL HIGH.
  2. **Address Frame**: 7-bit atau 10-bit alamat slave.
  3. **Read/Write Bit**: Menentukan operasi (0 = write, 1 = read).
  4. **ACK/NACK**: Konfirmasi dari slave/master.
  5. **Data Frame**: 8-bit data.
  6. **Stop Condition**: Sinyal HIGH pada SDA saat SCL HIGH.

- **Alamat I2C:**
  - Setiap slave memiliki alamat unik (0x08 hingga 0x77 untuk 7-bit).
  - Contoh alamat sensor: BMP280 (0x76 atau 0x77), OLED (0x3C).

**Referensi Bacaan:**
- [I2C Communication Guide](https://www.circuitbasics.com/basics-of-the-i2c-communication-protocol/)
- [ESP32 I2C Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html)
- [ESP32 I2C Example](https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/)
- [I2C Scanner Code](https://playground.arduino.cc/Main/I2cScanner/)

---

#### **2: Membaca Data dari Sensor DHT11/DHT22**
**Tujuan:**  
Membaca data suhu dan kelembaban dari sensor DHT11/DHT22.

**Komponen yang Dibutuhkan:**
- ESP32/ESP8266
- Sensor DHT11/DHT22
- Resistor 10kΩ (untuk DHT11)
- Breadboard dan kabel jumper

**Rangkaian Elektronika:**
1. Hubungkan **VCC** sensor ke **3.3V** ESP.
2. Hubungkan **GND** sensor ke **GND** ESP.
3. Hubungkan **DATA** sensor ke pin GPIO4 (ESP32) atau D4 (ESP8266).
4. Pasang resistor **10kΩ** antara pin DATA dan VCC (hanya untuk DHT11).

**Skema Rangkaian:**
```
ESP32/ESP8266:
- 3.3V -> VCC DHT
- GND  -> GND DHT
- GPIO4/D4 -> DATA DHT
- Resistor 10kΩ antara DATA dan VCC (untuk DHT11)
```

**Kode Program:**
```cpp
#include <DHT.h>

#define DHTPIN 4      // Pin DATA DHT (GPIO4/D4)
#define DHTTYPE DHT22 // Tipe sensor (DHT11 atau DHT22)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature(); // Baca suhu (°C)
  float humidity = dht.readHumidity();       // Baca kelembaban (%)

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C | Kelembaban: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000); // Tunggu 2 detik
}
```

**Instalasi Library di PlatformIO:**
- Tambahkan library berikut di `platformio.ini`:
  ```ini
  lib_deps = 
    adafruit/DHT sensor library@^1.4.4
  ```


-----

### 3: Menampilkan Teks pada LCD 16x2 (Modul I2C)

Ini adalah contoh klasik untuk mempraktikkan I2C. Kita akan menggunakan layar LCD 16x2 yang sudah dilengkapi dengan modul "backpack" I2C (biasanya menggunakan chip PCF8574).

#### Skema Rangkaian Elektronika

Anda hanya membutuhkan 4 kabel untuk menghubungkan modul I2C LCD ke ESP32.

#### Penjelasan Rangkaian

1.  **GND (Ground):** Hubungkan pin `GND` pada modul I2C ke pin `GND` pada ESP32.
2.  **VCC (Daya):** Hubungkan pin `VCC` pada modul I2C ke pin `5V` pada ESP32. (Meskipun ESP32 bekerja di 3.3V, modul LCD ini umumnya bekerja paling baik dengan 5V untuk daya dan *backlight*).
3.  **SDA (Serial Data):** Hubungkan pin `SDA` pada modul I2C ke pin **GPIO 21** pada ESP32 (pin SDA standar).
4.  **SCL (Serial Clock):** Hubungkan pin `SCL` pada modul I2C ke pin **GPIO 22** pada ESP32 (pin SCL standar).

#### Persiapan PlatformIO (`platformio.ini`)

Kita perlu menambahkan *library* untuk mengendalikan LCD I2C.

1.  Buka file `platformio.ini` Anda.
2.  Tambahkan baris berikut di bawah `[env:esp32dev]` untuk menyertakan *library* `LiquidCrystal_I2C`:

<!-- end list -->

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

; Tambahkan library untuk LCD I2C
lib_deps = 
    fmalpartida/LiquidCrystal_I2C@^1.1.4
```

#### Kode Program (`src/main.cpp`)

```cpp
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// --- Konfigurasi LCD ---
// Alamat I2C umum adalah 0x27 atau 0x3F.
// Jika layar tidak menyala, ganti alamatnya.
const int lcdAddress = 0x27; 
const int lcdColumns = 16;
const int lcdRows = 2;

// Inisialisasi objek LCD
// (alamat, kolom, baris)
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

// Variabel untuk penghitung
int counter = 0;

void setup() {
  // Inisialisasi LCD
  lcd.init();
  
  // Nyalakan backlight (lampu latar)
  lcd.backlight();

  //--- Pesan Selamat Datang ---
  
  // Posisikan kursor di kolom 0, baris 0
  lcd.setCursor(0, 0); 
  lcd.print("Halo, Dosen!"); // Tampilkan teks

  // Posisikan kursor di kolom 0, baris 1
  lcd.setCursor(0, 1);
  lcd.print("Sesi I2C - ESP32");
  
  // Jeda 2 detik untuk membaca pesan
  delay(2000);

  // Bersihkan layar untuk loop()
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Status Aktif:");
}

void loop() {
  // Posisikan kursor di baris kedua
  lcd.setCursor(0, 1);
  
  // Tampilkan pesan yang dinamis
  lcd.print("Hitungan: ");
  lcd.print(counter);

  // Beri sedikit jeda agar angka tidak terlalu cepat
  // dan bersihkan sisa angka sebelumnya jika perlu
  lcd.print("   "); 
  
  counter++; // Tambah nilai hitungan

  // Jeda 1 detik sebelum loop berikutnya
  delay(1000);
}
```

---

#### **4. Troubleshooting I2C**
**Masalah Umum dan Solusi:**
1. **Sensor Tidak Terdeteksi**:
   - Periksa alamat I2C dengan kode scanner:
     ```cpp
     #include <Wire.h>
     void setup() {
       Serial.begin(115200);
       Wire.begin();
     }
     void loop() {
       byte error, address;
       for (address = 1; address < 127; address++) {
         Wire.beginTransmission(address);
         error = Wire.endTransmission();
         if (error == 0) {
           Serial.print("Device ditemukan di alamat 0x");
           Serial.println(address, HEX);
         }
       }
       delay(5000);
     }
     ```
   - Pastikan kabel SDA/SCL terhubung dengan benar.
2. **Data Tidak Valid**:
   - Tambahkan resistor pull-up 4.7kΩ pada SDA dan SCL.
   - Periksa sumber daya (pastikan sensor menggunakan 3.3V).
