#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD 20x4 di alamat I2C 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Struktur data HARUS SAMA PERSIS dengan Wemos D1 Mini
typedef struct struct_message {
  float ppm;
  float rs;
  float vout;
} struct_message;

struct_message myData;

// Variabel penanda jika ada data baru masuk
volatile bool newData = false;

// Callback saat data diterima dari Wemos
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // Salin memori yang diterima ke variabel struktur
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Tandai bahwa ada data baru yang siap ditampilkan
  newData = true; 
}

void setup() {
  Serial.begin(115200);

  // Mulai LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Tampilan Awal
  lcd.setCursor(0, 0);
  lcd.print("=== MONITOR NH3 ===");
  lcd.setCursor(0, 1);
  lcd.print("Menunggu Wemos...");

  // Set perangkat sebagai Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inisialisasi ESP-NOW");
    lcd.setCursor(0, 2);
    lcd.print("Error ESP-NOW!");
    return;
  }

  // Daftarkan fungsi penerima
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  // Hanya perbarui layar jika ada data baru masuk
  if (newData) {
    // Matikan flag agar tidak update layar terus menerus (boros CPU)
    newData = false;

    // Cetak ke Serial untuk debugging
    Serial.print("Terima Data | PPM: ");
    Serial.println(myData.ppm);

    // Tampilkan PPM (Baris 2)
    lcd.setCursor(0, 1);
    lcd.print("NH3 PPM : ");
    lcd.print(myData.ppm, 2);
    lcd.print("    "); // Spasi ekstra untuk menghapus sisa karakter lama

    // Tampilkan Resistansi Sensor (Baris 3)
    lcd.setCursor(0, 2);
    lcd.print("Resistor: ");
    lcd.print(myData.rs, 2);
    lcd.print(" kOhm ");

    // Tampilkan Tegangan Output (Baris 4)
    lcd.setCursor(0, 3);
    lcd.print("Volt Out: ");
    lcd.print(myData.vout, 2);
    lcd.print(" V   ");
  }
}