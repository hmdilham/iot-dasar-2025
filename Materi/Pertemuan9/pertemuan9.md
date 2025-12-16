## Sesi 9: Menggunakan Aktuator

### 1. Pengantar Teori: Menggerakkan Dunia Fisik

Dalam sistem IoT, sensor bertindak sebagai "panca indera", sedangkan aktuator bertindak sebagai "otot". Pada sesi ini, kita akan membahas tiga jenis output:

1. **Suara (Buzzer):** Untuk notifikasi audio.
2. **Gerakan (Servo Motor):** Untuk pergerakan presisi (sudut 0° - 180°).
3. **Switching Daya Besar (Relay):** Untuk mengontrol peralatan listrik rumah tangga (AC 220V) menggunakan sinyal kecil DC dari ESP32.

---

### 2. Penjelasan Komponen

#### A. Buzzer (Active vs Passive)

* **Active Buzzer:** Cukup diberi tegangan (HIGH), langsung berbunyi. Lebih mudah diprogram.
* **Passive Buzzer:** Membutuhkan sinyal PWM (gelombang frekuensi) untuk menghasilkan nada. Bisa digunakan untuk membuat melodi.
* *Pada sesi ini kita menggunakan Active Buzzer atau fungsi `tone()` sederhana.*

#### B. Motor Servo (SG90)

* Berbeda dengan motor DC biasa yang berputar terus menerus, Servo bergerak ke **sudut tertentu** dan menahannya.
* Kabel Servo biasanya ada 3:
  * **Merah:** VCC (5V atau 3.3V).
  * **Cokelat/Hitam:** GND.
  * **Oranye/Kuning:** Signal (Masuk ke pin GPIO PWM).
* **PENTING:** ESP32 membutuhkan library khusus servo, bukan library bawaan Arduino biasa.

#### C. Modul Relay (Saklar Elektronik)

* Relay adalah saklar yang digerakkan oleh elektromagnet.
* **Logika:** Sinyal 3.3V dari ESP32 mengaktifkan magnet, yang menarik saklar besi untuk menyambungkan arus besar (misal lampu 220V).
* **Safety Warning:** *Hati-hati saat mendemokan Relay dengan listrik 220V. Pastikan tidak menyentuh bagian bawah PCB saat terhubung ke listrik PLN.*

---

### 3. Praktik Utama: Sistem Parkir Otomatis Sederhana

**Skenario:**

Kita akan membuat simulasi palang pintu parkir.

1. **Sensor Ultrasonik (HC-SR04)** mendeteksi keberadaan "mobil".
2. Jika mobil mendekat (jarak < 10 cm), **Buzzer** berbunyi "bip" dan **Servo** bergerak 90 derajat (Palang Terbuka).
3. Jika mobil menjauh, **Servo** kembali ke 0 derajat (Palang Tertutup).

#### A. Konfigurasi PlatformIO

Untuk project ini menggunakan PlatformIO, buat file `platformio.ini` dengan konfigurasi berikut:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps = 
	madhephaestus/ESP32Servo@^3.0.5
```

**Penjelasan:**
- `platform = espressif32`: Platform untuk ESP32
- `board = esp32dev`: Board ESP32 DevKit
- `framework = arduino`: Menggunakan framework Arduino
- `monitor_speed = 115200`: Kecepatan serial monitor
- `lib_deps`: Library ESP32Servo untuk kontrol servo motor

**Alternatif (Arduino IDE):**

Jika menggunakan Arduino IDE, install library secara manual:

1. Menu **Sketch** → **Include Library** → **Manage Libraries**
2. Cari dan Install: **ESP32Servo** by Kevin Harrington (versi 3.0.5 atau lebih baru)

#### B. Rangkaian (Wiring)

| Komponen | Pin Komponen | Pin ESP32 |
| --- | --- | --- |
| **Servo** | Kabel Merah | VIN (5V) / 3V3 |
|  | Kabel Cokelat | GND |
|  | Kabel Oranye | GPIO 13 |
| **Ultrasonik** | VCC | VIN (5V) / 3V3 |
|  | GND | GND |
|  | TRIG | GPIO 5 |
|  | ECHO | GPIO 18 |
| **Buzzer** | Kaki Positif (+) | GPIO 4 |
|  | Kaki Negatif (-) | GND |

#### C. Kode Program (Sketch)

```cpp
#include <Arduino.h>
#include <ESP32Servo.h>

// Definisi Pin
#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 13
#define BUZZER_PIN 4

// Konstanta
#define JARAK_DETEKSI 10       // Jarak deteksi dalam cm
#define WAKTU_TERBUKA 3000     // Waktu palang terbuka (3 detik)
#define INTERVAL_BACA 300      // Interval pembacaan sensor (300ms)

// Inisialisasi Objek Servo
Servo myservo;

// Variabel untuk jarak
long duration;
int distance;

// State Management
enum State {
  PALANG_TERTUTUP,
  PALANG_TERBUKA,
  MENUNGGU_TUTUP
};

State currentState = PALANG_TERTUTUP;
unsigned long previousMillis = 0;
unsigned long gateOpenTime = 0;

// Fungsi untuk membaca jarak dari sensor ultrasonik
int bacaJarak() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // Menghitung cm
}

void setup() {
  Serial.begin(115200);

  // Setup Pin Ultrasonik
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Setup Pin Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Setup Servo
  // ESP32Servo membutuhkan alokasi timer
  myservo.setPeriodHertz(50); 
  myservo.attach(SERVO_PIN, 500, 2400); // Pin, min pulse, max pulse
  
  // Posisi awal tertutup
  myservo.write(0); 
  Serial.println("=== Sistem Parkir Otomatis Siap ===");
  Serial.println("Status: Palang Tertutup");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Baca sensor setiap INTERVAL_BACA ms (non-blocking)
  if (currentMillis - previousMillis >= INTERVAL_BACA) {
    previousMillis = currentMillis;
    
    distance = bacaJarak();
    
    Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.print(" cm | Status: ");
    
    // State Machine
    switch (currentState) {
      case PALANG_TERTUTUP:
        Serial.println("Palang Tertutup");
        
        // Jika mobil terdeteksi
        if (distance > 0 && distance < JARAK_DETEKSI) {
          Serial.println(">>> MOBIL TERDETEKSI! <<<");
          
          // 1. Bunyikan buzzer dulu sebagai peringatan
          digitalWrite(BUZZER_PIN, HIGH);
          delay(200); // Delay singkat untuk buzzer (masih acceptable)
          digitalWrite(BUZZER_PIN, LOW);
          
          // 2. Buka palang
          Serial.println(">>> MEMBUKA PALANG <<<");
          myservo.write(90); // Gerakkan servo ke 90 derajat
          
          // Catat waktu palang dibuka
          gateOpenTime = millis();
          currentState = PALANG_TERBUKA;
        }
        break;
        
      case PALANG_TERBUKA:
        Serial.println("Palang Terbuka");
        
        // Tunggu waktu minimum terbuka
        if (millis() - gateOpenTime >= WAKTU_TERBUKA) {
          // Setelah waktu tunggu, pindah ke state menunggu tutup
          currentState = MENUNGGU_TUTUP;
          Serial.println(">>> SIAP MENUTUP PALANG <<<");
        }
        break;
        
      case MENUNGGU_TUTUP:
        Serial.println("Menunggu Area Kosong");
        
        // Jika area sudah kosong (jarak > threshold)
        if (distance == 0 || distance >= JARAK_DETEKSI) {
          Serial.println(">>> AREA KOSONG - MENUTUP PALANG <<<");
          
          // Tutup palang
          myservo.write(0); // Kembali ke 0 derajat
          
          // Bunyi konfirmasi penutupan
          digitalWrite(BUZZER_PIN, HIGH);
          delay(100);
          digitalWrite(BUZZER_PIN, LOW);
          
          currentState = PALANG_TERTUTUP;
          Serial.println("=== Palang Tertutup - Sistem Siap ===");
        }
        // Jika masih ada objek, tetap di state ini
        else {
          Serial.println(">>> Masih ada objek, tunggu... <<<");
        }
        break;
    }
  }
}

```

---

### 4. Demonstrasi Tambahan: Relay (Opsional)

Jika waktu memungkinkan, Anda bisa menambahkan demo Relay secara terpisah atau digabungkan.

**Skenario:**

Menggunakan Relay untuk menyalakan lampu indikator "Garasi Penuh".

**Kode Tambahan (Snippet):**

```cpp
// Tambahkan definisi pin Relay
#define RELAY_PIN 2

void setup() {
  // ... setup lainnya ...
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Biasanya Relay aktif LOW, matikan dulu di awal
}

void loop() {
  // ... logika jarak ...
  
  if (distance < 10) {
     // Nyalakan lampu via Relay saat mobil masuk
     digitalWrite(RELAY_PIN, LOW); // LOW biasanya mengaktifkan Relay (NO connect ke COM)
  } else {
     digitalWrite(RELAY_PIN, HIGH); // Matikan Relay
  }
}

```

**Catatan untuk Trainer:**
Saat menjelaskan Relay, tekankan konsep **NO (Normally Open)**, **NC (Normally Closed)**, dan **COM (Common)**.

* **COM:** Sumber listrik masuk.
* **NO:** Kabel ke lampu (Listrik mengalir HANYA saat Relay aktif).
* **NC:** Listrik mengalir saat Relay MATI (Jarang dipakai untuk lampu, sering untuk sistem keamanan).