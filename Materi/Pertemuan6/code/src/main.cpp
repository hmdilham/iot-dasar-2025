#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 4      // Pin DATA DHT (GPIO4/D4)
#define DHTTYPE DHT22 // Tipe sensor (DHT11 atau DHT22)

// Inisialisasi DHT
DHT dht(DHTPIN, DHTTYPE);

// Inisialisasi LCD I2C (address 0x27, 16 kolom, 2 baris)
// Jika tidak bekerja, coba address 0x3F
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi DHT
  dht.begin();
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Tampilkan pesan awal
  lcd.setCursor(0, 0);
  lcd.print("DHT22 Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temperature = dht.readTemperature(); // Baca suhu (°C)
  float humidity = dht.readHumidity();       // Baca kelembaban (%)

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    
    // Tampilkan error di LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check Sensor");
    return;
  }

  // Tampilkan data di Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C | Kelembaban: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Tampilkan data di LCD
  // Baris 1: Suhu
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1); // 1 desimal
  lcd.print(" C");
  lcd.print("  "); // Clear sisa karakter
  
  // Baris 2: Kelembaban
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(humidity, 1); // 1 desimal
  lcd.print(" %");
  lcd.print("  "); // Clear sisa karakter

  delay(2000); // Tunggu 2 detik
}