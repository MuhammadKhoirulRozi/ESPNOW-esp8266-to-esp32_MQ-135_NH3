// #include <esp_now.h>
// #include <WiFi.h>
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>

// // Inisialisasi LCD 20x4 di alamat I2C 0x27
// LiquidCrystal_I2C lcd(0x27, 20, 4);

// // === 1. MAC ADDRESS PENGIRIM (WEMOS) ===
// // GANTI KODE HEX DI BAWAH INI DENGAN MAC ADDRESS ASLI MASING-MASING WEMOS!
// uint8_t macWemos1[] = {0xD8, 0xBF, 0xC0, 0x0E, 0x70, 0x46}; // MAC D8:BF:C0:0E:70:46
// uint8_t macWemos2[] = {0xA4, 0xCF, 0x12, 0xFF, 0x0F, 0xFA}; // MAC A4:CF:12:FF:0F:FA
// uint8_t macWemos3[] = {0x48, 0x3F, 0xDA, 0x8A, 0x5A, 0xCA}; // MAC 48:3F:DA:8A:5A:CA
// uint8_t macWemos4[] = {0x8C, 0xAA, 0xB5, 0x10, 0x26, 0x0E}; // MAC 8C:AA:B5:10:26:0E

// // MAC ADDRESS ESP8266
// // 1. D8:BF:C0:0E:70:46
// // 2. A4:CF:12:FF:0F:FA
// // 3. 48:3F:DA:8A:5A:CA
// // 4. 8C:AA:B5:10:26:0E

// // === 2. STRUKTUR DATA (HARUS SAMA PERSIS DENGAN WEMOS PENGIRIM) ===
// typedef struct struct_message {
//   // float ppm1;
//   // float rs;
//   // float vout;
//   // float temp1;
//   // float hum1;

//     // float ppm;
//   float ppm1;
//   float ppm2;
//   float ppm3;
//   float ppm4;
//   // float rs;
//   // float vout;
//   // float temp;
//   float temp1;
//   float temp2;
//   float temp3;
//   float temp4;
//   // float hum;
//   float hum1;
//   float hum2;
//   float hum3;
//   float hum4;
// } struct_message;

// struct_message incomingData; 

// // === 3. VARIABEL PENAMPUNG DATA DARI 4 NODE ===
// float ppm_node[4]  = {0.0, 0.0, 0.0, 0.0};
// float temp_node[4] = {0.0, 0.0, 0.0, 0.0};
// float hum_node[4]  = {0.0, 0.0, 0.0, 0.0};

// // Variabel penanda jika ada data baru masuk
// volatile bool newData = false;

// // === 4. CALLBACK SAAT DATA DITERIMA ===
// void OnDataRecv(const uint8_t * mac, const uint8_t *incoming, int len) {
//   // Salin memori yang diterima ke variabel incomingData
//   memcpy(&incomingData, incoming, sizeof(incomingData));
  
//   // Cocokkan MAC Address pengirim untuk menentukan kamar data (Index Array)
//   if (memcmp(mac, macWemos1, 6) == 0) {
//     ppm_node[0]  = incomingData.ppm1;
//     temp_node[0] = incomingData.temp1;
//     hum_node[0]  = incomingData.hum1;
//   }
//   else if (memcmp(mac, macWemos2, 6) == 0) {
//     ppm_node[1]  = incomingData.ppm1;
//     temp_node[1] = incomingData.temp1;
//     hum_node[1]  = incomingData.hum1;
//   }
//   else if (memcmp(mac, macWemos3, 6) == 0) {
//     ppm_node[2]  = incomingData.ppm1;
//     temp_node[2] = incomingData.temp1;
//     hum_node[2]  = incomingData.hum1;
//   }
//   else if (memcmp(mac, macWemos4, 6) == 0) {
//     ppm_node[3]  = incomingData.ppm1;
//     temp_node[3] = incomingData.temp1;
//     hum_node[3]  = incomingData.hum1;
//   }

//   // Tandai bahwa ada data baru yang siap ditampilkan
//   newData = true; 
// }

// void setup() {
//   Serial.begin(115200);

//   // Mulai LCD
//   lcd.init();
//   lcd.backlight();
//   lcd.clear();
  
//   // Tampilan Awal
//   lcd.setCursor(0, 0);
//   lcd.print("==MONITOR NH3 & TH==");
//   lcd.setCursor(0, 1);
//   lcd.print("Menunggu Wemos...");

//   // Set perangkat sebagai Wi-Fi Station
//   WiFi.mode(WIFI_STA);

//   // Inisialisasi ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error inisialisasi ESP-NOW");
//     lcd.setCursor(0, 2);
//     lcd.print("Error ESP-NOW!");
//     return;
//   }

//   // Daftarkan fungsi penerima
//   esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
// }

// void amoniath() {
//   // Hitung rata-rata dari ke-4 Node 
//   float amonia = (ppm_node[0] + ppm_node[1] + ppm_node[2] + ppm_node[3]) / 4.0;
//   float temp   = (temp_node[0] + temp_node[1] + temp_node[2] + temp_node[3]) / 4.0;
//   float hum    = (hum_node[0] + hum_node[1] + hum_node[2] + hum_node[3]) / 4.0;

//   // Cetak ke Serial untuk debugging
//   Serial.print("Terima Data | PPM: ");
//   Serial.print(" PPM1:"); Serial.print(ppm_node[0]);
//   Serial.print(" PPM2:"); Serial.print(ppm_node[1]);
//   Serial.print(" PPM3:"); Serial.print(ppm_node[2]);
//   Serial.print(" PPM4:"); Serial.print(ppm_node[3]);
//   Serial.print(" AVG_PPM:"); Serial.println(amonia);
  
//   Serial.print("            | Temp1:"); Serial.print(temp_node[0]);
//   Serial.print(" Temp2:"); Serial.print(temp_node[1]);
//   Serial.print(" Temp3:"); Serial.print(temp_node[2]);
//   Serial.print(" Temp4:"); Serial.print(temp_node[3]);
//   Serial.print(" AVG_TEMP:"); Serial.println(temp);
  
//   Serial.print("            | Hum1:"); Serial.print(hum_node[0]);
//   Serial.print(" Hum2:"); Serial.print(hum_node[1]);
//   Serial.print(" Hum3:"); Serial.print(hum_node[2]);
//   Serial.print(" Hum4:"); Serial.print(hum_node[3]);
//   Serial.print(" AVG_HUM:"); Serial.println(hum);
//   Serial.println("--------------------------------------------------");

//   // Tampilkan PPM (Baris 2)
//   lcd.setCursor(0, 1);
//   lcd.print("NH3 PPM : ");
//   lcd.print(amonia, 2);
//   lcd.print("    "); // Spasi ekstra untuk menghapus sisa karakter lama

//   // Tampilkan Suhu (Baris 3)
//   lcd.setCursor(0, 2);
//   lcd.print("Suhu: ");
//   lcd.print(temp, 2);
//   lcd.print(" C    ");

//   // Tampilkan Kelembapan (Baris 4)
//   lcd.setCursor(0, 3);
//   lcd.print("Hum : ");
//   lcd.print(hum, 2);
//   lcd.print(" %    ");
// }

// void loop() {
//   // Hanya perbarui layar & Serial jika ada data baru masuk
//   if (newData) {
//     newData = false; // Matikan flag agar tidak update terus menerus
//     amoniath();
//   }
// }



#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD 20x4 di alamat I2C 0x27```````````````
LiquidCrystal_I2C lcd(0x27, 20, 4);

uint8_t macWemos1[] = {0xD8, 0xBF, 0xC0, 0x0E, 0x70, 0x46}; // MAC D8:BF:C0:0E:70:46
uint8_t macWemos2[] = {0xA4, 0xCF, 0x12, 0xFF, 0x0F, 0xFA}; // MAC A4:CF:12:FF:0F:FA
uint8_t macWemos3[] = {0x48, 0x3F, 0xDA, 0x8A, 0x5A, 0xCA}; // MAC 48:3F:DA:8A:5A:CA
uint8_t macWemos4[] = {0x8C, 0xAA, 0xB5, 0x10, 0x26, 0x0E}; // MAC 8C:AA:B5:10:26:0E

// // MAC ADDRESS ESP8266
// // 1. D8:BF:C0:0E:70:46
// // 2. A4:CF:12:FF:0F:FA
// // 3. 48:3F:DA:8A:5A:CA
// // 4. 8C:AA:B5:10:26:0E

// === 2. STRUKTUR DATA (HARUS SAMA PERSIS DENGAN WEMOS PENGIRIM) ===
typedef struct struct_message {
  float ppm1;
  float rs;
  float vout;
  float temp1;
  float hum1;
} struct_message;

struct_message incomingData; 

// === 3. VARIABEL PENAMPUNG DATA DARI 4 NODE ===
float ppm_node[4]  = {0.0, 0.0, 0.0, 0.0};
float temp_node[4] = {0.0, 0.0, 0.0, 0.0};
float hum_node[4]  = {0.0, 0.0, 0.0, 0.0};

// Variabel penanda jika ada data baru masuk
volatile bool newData = false;

// === 4. CALLBACK SAAT DATA DITERIMA ===
void OnDataRecv(const uint8_t * mac, const uint8_t *incoming, int len) {
  // Salin memori yang diterima ke variabel incomingData
  memcpy(&incomingData, incoming, sizeof(incomingData));
  
  // Cocokkan MAC Address pengirim untuk menentukan kamar data (Index Array)
  if (memcmp(mac, macWemos1, 6) == 0) {
    ppm_node[0]  = incomingData.ppm1;
    temp_node[0] = incomingData.temp1;
    hum_node[0]  = incomingData.hum1;
  }
  else if (memcmp(mac, macWemos2, 6) == 0) {
    ppm_node[1]  = incomingData.ppm1;
    temp_node[1] = incomingData.temp1;
    hum_node[1]  = incomingData.hum1;
  }
  else if (memcmp(mac, macWemos3, 6) == 0) {
    ppm_node[2]  = incomingData.ppm1;
    temp_node[2] = incomingData.temp1;
    hum_node[2]  = incomingData.hum1;
  }
  else if (memcmp(mac, macWemos4, 6) == 0) {
    ppm_node[3]  = incomingData.ppm1;
    temp_node[3] = incomingData.temp1;
    hum_node[3]  = incomingData.hum1;
  }

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
  lcd.print("==MONITOR NH3 & TH==");
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

void amoniath() {
  // Hitung rata-rata dari ke-4 Node 
  float amonia = (ppm_node[0] + ppm_node[1] + ppm_node[2] + ppm_node[3]) / 4.0;
  float temp   = (temp_node[0] + temp_node[1] + temp_node[2] + temp_node[3]) / 4.0;
  float hum    = (hum_node[0] + hum_node[1] + hum_node[2] + hum_node[3]) / 4.0;

  // Cetak ke Serial untuk debugging
  Serial.print("Terima Data | PPM: ");
  Serial.print(" PPM1:"); Serial.print(ppm_node[0]);
  Serial.print(" PPM2:"); Serial.print(ppm_node[1]);
  Serial.print(" PPM3:"); Serial.print(ppm_node[2]);
  Serial.print(" PPM4:"); Serial.print(ppm_node[3]);
  Serial.print(" AVG_PPM:"); Serial.println(amonia);
  
  Serial.print("            | Temp1:"); Serial.print(temp_node[0]);
  Serial.print(" Temp2:"); Serial.print(temp_node[1]);
  Serial.print(" Temp3:"); Serial.print(temp_node[2]);
  Serial.print(" Temp4:"); Serial.print(temp_node[3]);
  Serial.print(" AVG_TEMP:"); Serial.println(temp);
  
  Serial.print("            | Hum1:"); Serial.print(hum_node[0]);
  Serial.print(" Hum2:"); Serial.print(hum_node[1]);
  Serial.print(" Hum3:"); Serial.print(hum_node[2]);
  Serial.print(" Hum4:"); Serial.print(hum_node[3]);
  Serial.print(" AVG_HUM:"); Serial.println(hum);
  Serial.println("--------------------------------------------------");

  // Tampilkan PPM (Baris 2)
  lcd.setCursor(0, 1);
  lcd.print("NH3 PPM : ");
  lcd.print(amonia, 2);
  lcd.print("    "); // Spasi ekstra untuk menghapus sisa karakter lama

  // Tampilkan Suhu (Baris 3)
  lcd.setCursor(0, 2);
  lcd.print("Suhu: ");
  lcd.print(temp, 2);
  lcd.print(" C    ");

  // Tampilkan Kelembapan (Baris 4)
  lcd.setCursor(0, 3);
  lcd.print("Hum : ");
  lcd.print(hum, 2);
  lcd.print(" %    ");
}

void loop() {
  // Hanya perbarui layar & Serial jika ada data baru masuk
  if (newData) {
    newData = false; // Matikan flag agar tidak update terus menerus
    amoniath();
  }
}