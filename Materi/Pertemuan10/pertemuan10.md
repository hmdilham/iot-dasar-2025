## **Sesi 10: Koneksi WiFi dengan WiFiManager**

### **1. Konsep Dasar: Masalah Hardcoding vs. Solusi Dinamis**

Dalam metode konvensional, kita sering menulis kredensial WiFi seperti ini di awal kode:

```cpp
const char* ssid = "WifiKu";
const char* password = "rahasia123";
```

**Masalahnya:** Jika Anda membawa alat tersebut ke tempat lain (misalnya ke kantor atau rumah teman), alat tersebut tidak akan bisa terkoneksi karena SSID berbeda. Anda harus mengedit kode, mengompilasi ulang, dan mengunggahnya lagi. Ini tidak efisien untuk produk jadi.

**Solusinya (WiFiManager):**
WiFiManager memungkinkan ESP32 bertindak sebagai **Access Point (AP)** ketika tidak menemukan jaringan WiFi yang dikenal.

1.  Saat dinyalakan, jika tidak ada koneksi internet, ESP32 membuat jaringan WiFi sendiri (misalnya: "ESP32-Setup").
2.  Pengguna menghubungkan HP/Laptop ke jaringan tersebut.
3.  Halaman konfigurasi (Captive Portal) akan otomatis muncul.
4.  Pengguna memilih WiFi rumah/kantor dan memasukkan password melalui antarmuka web.
5.  ESP32 menyimpan data tersebut, restart, dan otomatis terhubung ke internet.

-----

### **2. Persiapan: Instalasi Library**

Sebelum menulis kode, peserta harus menginstal library yang dibutuhkan.

1.  Buka Arduino IDE.
2.  Masuk ke menu **Sketch** -\> **Include Library** -\> **Manage Libraries**.
3.  Ketik di kolom pencarian: **WiFiManager**.
4.  Cari library yang dibuat oleh **tzapu**.
5.  Klik **Install** (Pilih versi terbaru).

-----

### **3. Praktik: Kode Program WiFiManager**

Salin kode berikut ke Arduino IDE. Kode ini dirancang untuk menangani koneksi WiFi secara otomatis tanpa perlu menulis SSID di dalam skrip.

```cpp
#include <WiFi.h>
#include <WiFiManager.h> // Library Wi-Fi Manager

// Pin Configuration
#define TRIGGER_PIN 0      // GPIO0 untuk trigger config mode (BOOT button)
#define LED_PIN 2          // Built-in LED untuk indikator status

// Debouncing Configuration
#define DEBOUNCE_DELAY 50  // ms
#define BUTTON_HOLD_TIME 3000 // Tekan 3 detik untuk masuk config mode

// Variables untuk button handling
unsigned long lastDebounceTime = 0;
unsigned long buttonPressTime = 0;
bool buttonPressed = false;
bool configPortalActive = false;

// Variables untuk WiFi monitoring
unsigned long lastWiFiCheck = 0;
const unsigned long WIFI_CHECK_INTERVAL = 30000; // Check setiap 30 detik

// Forward declarations
void setupWiFiManager();
void handleButton();
void checkWiFiConnection();
void blinkLED(int times, int delayMs);
void configModeCallback(WiFiManager *myWiFiManager);
void saveConfigCallback();

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n====================================");
  Serial.println("ESP32 WiFi Manager - Advanced Setup");
  Serial.println("====================================");
  
  // Setup pins
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Blink LED untuk indikasi startup
  blinkLED(3, 200);
  
  // Setup WiFi Manager
  setupWiFiManager();
}

void loop() {
  // Handle button untuk trigger config mode
  handleButton();
  
  // Check WiFi connection periodically
  if (!configPortalActive) {
    checkWiFiConnection();
  }
  
  // LED indicator - blink jika terhubung
  if (WiFi.status() == WL_CONNECTED && !configPortalActive) {
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 2000) {
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
      lastBlink = millis();
    }
  }
}

void setupWiFiManager() {
  WiFiManager wm;
  
  // Set callback untuk config mode
  wm.setAPCallback(configModeCallback);
  wm.setSaveConfigCallback(saveConfigCallback);
  
  // Uncomment untuk reset settings (testing)
  // Serial.println("[DEBUG] Resetting WiFi settings...");
  // wm.resetSettings();
  
  // Set timeout untuk AP mode (detik)
  wm.setConfigPortalTimeout(180); // 3 menit
  
  // Set minimum signal quality (0-100)
  wm.setMinimumSignalQuality(20);
  
  // Set WiFi scan dengan info lebih detail
  wm.setScanDispPerc(true);
  
  // Custom HTML/CSS untuk portal (opsional)
  // wm.setCustomHeadElement("<style>body{background-color:#f0f0f0;}</style>");
  
  // Custom parameter (contoh untuk API key atau server address)
  // WiFiManagerParameter custom_api_key("api_key", "API Key", "", 40);
  // WiFiManagerParameter custom_server("server", "Server Address", "api.example.com", 40);
  // wm.addParameter(&custom_api_key);
  // wm.addParameter(&custom_server);
  
  Serial.println("\n[INFO] Memulai WiFi Manager...");
  Serial.println("[INFO] Mencoba koneksi otomatis...");
  
  // Jalankan Wi-Fi Manager dengan autoConnect
  bool res = wm.autoConnect("ESP32-Config", "password123"); // AP dengan password
  
  if (!res) {
    Serial.println("\n[ERROR] Gagal terhubung ke WiFi!");
    Serial.println("[ERROR] Timeout atau tidak ada konfigurasi.");
    Serial.println("[INFO] Restarting ESP32 dalam 3 detik...");
    blinkLED(10, 100);
    delay(3000);
    ESP.restart();
  } else {
    Serial.println("\n[SUCCESS] Terhubung ke Wi-Fi!");
    Serial.println("====================================");
    Serial.print("SSID        : ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address  : ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway     : ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS         : ");
    Serial.println(WiFi.dnsIP());
    Serial.print("MAC Address : ");
    Serial.println(WiFi.macAddress());
    Serial.print("Signal (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.println("====================================");
    Serial.println("\n[INFO] Tekan dan tahan tombol BOOT (GPIO0)");
    Serial.println("       selama 3 detik untuk masuk config mode\n");
    
    // LED ON solid saat berhasil terhubung
    digitalWrite(LED_PIN, HIGH);
    delay(2000);
    digitalWrite(LED_PIN, LOW);
  }
  
  // Ambil custom parameters jika ada
  // Serial.print("API Key: ");
  // Serial.println(custom_api_key.getValue());
}

void handleButton() {
  int reading = digitalRead(TRIGGER_PIN);
  
  // Debouncing
  if (reading == LOW && !buttonPressed) {
    if (buttonPressTime == 0) {
      buttonPressTime = millis();
    }
    
    // Check berapa lama tombol ditekan
    if (millis() - buttonPressTime > BUTTON_HOLD_TIME) {
      buttonPressed = true;
      configPortalActive = true;
      
      Serial.println("\n[INFO] Tombol ditekan > 3 detik!");
      Serial.println("[INFO] Masuk mode konfigurasi...");
      Serial.println("====================================");
      
      // Blink LED cepat sebelum masuk config mode
      blinkLED(5, 100);
      
      WiFiManager wm;
      wm.setConfigPortalTimeout(300); // 5 menit untuk reconfig
      
      // Start config portal (blocking)
      if (!wm.startConfigPortal("ESP32-Config", "password123")) {
        Serial.println("[INFO] Config portal timeout atau dibatalkan");
      } else {
        Serial.println("[SUCCESS] Konfigurasi baru disimpan!");
        Serial.println("[INFO] Restarting ESP32...");
        delay(1000);
        ESP.restart();
      }
      
      configPortalActive = false;
      buttonPressed = false;
      buttonPressTime = 0;
    }
  } else if (reading == HIGH) {
    // Tombol dilepas
    buttonPressTime = 0;
    buttonPressed = false;
  }
}

void checkWiFiConnection() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastWiFiCheck >= WIFI_CHECK_INTERVAL) {
    lastWiFiCheck = currentMillis;
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\n[WARNING] WiFi terputus!");
      Serial.println("[INFO] Mencoba reconnect...");
      
      // Blink LED cepat saat reconnecting
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
      }
      
      WiFi.reconnect();
      
      // Tunggu max 10 detik untuk reconnect
      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
      }
      
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[SUCCESS] Reconnected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
      } else {
        Serial.println("\n[ERROR] Reconnect gagal!");
        Serial.println("[INFO] Akan restart dalam 5 detik...");
        delay(5000);
        ESP.restart();
      }
    } else {
      // Connection OK - print status
      Serial.print("[STATUS] WiFi OK | IP: ");
      Serial.print(WiFi.localIP());
      Serial.print(" | RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
    }
  }
}

void blinkLED(int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(LED_PIN, LOW);
    delay(delayMs);
  }
}

void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("\n[INFO] Masuk Config Mode (AP Mode)");
  Serial.println("====================================");
  Serial.print("SSID AP     : ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.print("IP Address  : ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Password    : password123");
  Serial.println("====================================");
  Serial.println("[INFO] Buka browser dan akses IP di atas");
  Serial.println("[INFO] untuk konfigurasi WiFi\n");
  
  // LED blink perlahan di config mode
  digitalWrite(LED_PIN, HIGH);
}

void saveConfigCallback() {
  Serial.println("\n[INFO] Konfigurasi akan disimpan...");
}
```

##### File platformio.ini
```cpp
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps = 
    tzapu/WiFiManager@^2.0.17

```


-----

### **4. Langkah Pengujian (Skenario Pengguna)**

Untuk memahami cara kerjanya, ikuti langkah simulasi ini:

1.  **Upload Kode:** Unggah kode di atas ke ESP32.
2.  **Cek Serial Monitor:** Buka Serial Monitor (Baud rate 115200). Anda akan melihat pesan bahwa ESP32 memulai Access Point karena belum ada WiFi yang tersimpan (atau jika Anda mengaktifkan `wm.resetSettings()`).
3.  **Koneksi via HP/Laptop:**
      * Buka pengaturan WiFi di HP atau Laptop Anda.
      * Cari jaringan bernama **ESP32-Config** dan **password123**.
      * Hubungkan ke jaringan tersebut.
4.  **Konfigurasi:**
      * Biasanya browser akan otomatis terbuka (Captive Portal). Jika tidak, buka browser dan ketik **192.168.4.1**.
      * Pilih menu **Configure WiFi**.
      * Pilih SSID (WiFi rumah/lokal Anda) dan masukkan passwordnya.
      * Klik **Save**.
5.  **Verifikasi:**
      * Lihat kembali Serial Monitor.
      * ESP32 akan restart, kemudian otomatis terhubung ke WiFi yang baru saja Anda masukkan.
      * Pesan "Terhubung ke WiFi\!" dan Alamat IP akan muncul.

### **5. Penjelasan Teknis Singkat**

  * **`WiFiManager wm;`**: Membuat instans objek untuk mengatur semua logika WiFi.
  * **`wm.resetSettings();`**: Perintah ini penting saat *development*. Fungsinya menghapus SSID/Password yang pernah tersimpan sebelumnya di memori flash ESP32, sehingga kita bisa menguji mode Access Point berulang kali. **Penting:** Matikan (comment) baris ini saat alat sudah siap digunakan (deployed).
  * **`wm.autoConnect("NamaAP")`**: Ini adalah fungsi "pintar". Ia melakukan pengecekan apakah ada kredensial tersimpan. Jika ada, ia konek. Jika tidak, ia jadi router (AP).
  * **`WiFi.localIP()`**: Fungsi bawaan ESP32 untuk meminta informasi IP Address yang diberikan oleh router lokal.

-----

Apakah Anda ingin saya melanjutkan ke **Sesi 11** dengan topik pemanfaatan koneksi ini (misalnya: Mengambil waktu internet/NTP atau Mengirim data sederhana)?