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
