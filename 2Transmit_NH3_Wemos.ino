// #include <ESP8266WiFi.h>
// #include <espnow.h>
// #include <DHT.h> // Tambahkan library DHT

// // === Konfigurasi DHT22 ===
// #define DHTPIN 4      // Pin data DHT22 terhubung ke D4 (GPIO 2)
// #define DHTTYPE DHT22  // Jenis sensor DHT 22 (AM2302)
// DHT dht(DHTPIN, DHTTYPE);

// // uint8_t broadcastAddress[] = {0xAC, 0x27, 0x6E, 0x5E, 0xAF, 0xC8}; //AC:27:6E:5E:AF:C8 
// uint8_t broadcastAddress[] = {0xAC, 0x27, 0x6E, 0x5E, 0x93, 0x08}; // AC:27:6E:5E:93:08

// // Struktur paket data yang akan dikirim (Ditambah temp & hum)
// typedef struct struct_message {
//   float ppm2;
//   float rs;
//   float vout;
//   float temp2; // Variabel suhu
//   float hum2;  // Variabel kelembapan
// } struct_message;

// struct_message myData;

// // === Parameter Kalibrasi & Hardware MQ-135 ===
// const int mqPin = A0; 
// const float RL = 1.0; 
// const float Vcc = 3.3; 
// const float R0 = 0.07;
// // const float a = 6.43;
// // const float b = -1.98;
//   float a = 4.42; 
//   float b = -2.25;
// // Callback saat data selesai dikirim
// void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
//   Serial.print("Status Kirim ESP-NOW: ");
//   if (sendStatus == 0){
//     Serial.println("Sukses");
//   }
//   else{
//     Serial.println("Gagal");
//   }
// }

// void setup() {
//   Serial.begin(115200);
  
//   // Memulai sensor DHT
//   dht.begin();
  
//   // Set perangkat sebagai Wi-Fi Station (syarat ESP-NOW)
//   WiFi.mode(WIFI_STA);

//   // Inisialisasi ESP-NOW
//   if (esp_now_init() != 0) {
//     Serial.println("Error inisialisasi ESP-NOW");
//     return;
//   }

//   // Set peran Wemos sebagai Controller (Pengirim)
//   esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
//   esp_now_register_send_cb(OnDataSent);
  
//   // Daftarkan ESP32 sebagai peer penerima
//   esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

//   Serial.println("Memulai pemanasan sensor MQ-135 dan DHT22...");
//   delay(3000); 
// }

// void loop() {
//   // === 1. Baca Sensor DHT22 ===
//   float hum2 = dht.readHumidity();
//   float temp2 = dht.readTemperature();

//   // Periksa apakah pembacaan DHT gagal
//   if (isnan(hum2) || isnan(temp2)) {
//     Serial.println("Gagal membaca dari sensor DHT!");
//     hum2 = 0.0;
//     temp2 = 0.0;
//   }

//   // === 2. Baca Sensor MQ-135 ===
//   int adcValue = analogRead(mqPin);
//   float Vout = (adcValue / 1023.0) * Vcc;

//   float Rs = 0;
//   if (Vout > 0.0) {
//     Rs = RL * (Vcc - Vout) / Vout;
//   }     

//   float ratio = Rs / R0;
//   float ppm2 = a * pow(ratio, b);

//   // === 3. Cetak ke Serial Monitor untuk debugging ===
//   Serial.print("Suhu: "); Serial.print(temp2); Serial.print(" C\t");
//   Serial.print("Kelembapan: "); Serial.print(hum2); Serial.print(" %\t");
//   Serial.print("PPM NH3: "); Serial.println(ppm2, 2);
//   Serial.print("Rs:  "); Serial.print (Rs,2);
//   Serial.print("Adc:  "); Serial.print (adcValue);
//   Serial.print ("Vout: "); Serial.println (Vout);

//   // === 4. Masukkan data ke dalam struktur untuk dikirim ===
//   myData.ppm2 = ppm2;
//   myData.rs = Rs;
//   myData.vout = Vout;
//   myData.temp2 = temp2;
//   myData.hum2 = hum2;

//   // === 5. Kirim pesan melalui ESP-NOW ===
//   esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

//   delay(2000); // Kirim data setiap 2 detik
// }


#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DHT.h>

// === Konfigurasi DHT22 ===
#define DHTPIN 4      // Pin data DHT22 terhubung ke D2 (GPIO 4)
#define DHTTYPE DHT22  // Jenis sensor DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// === MAC ADDRESS ESP32 PENERIMA ===
// Pastikan ini adalah MAC Address ESP32 Anda!
uint8_t broadcastAddress[] = {0xAC, 0x27, 0x6E, 0x5E, 0x93, 0x08}; 

// === Struktur Paket Data (HARUS SAMA DENGAN ESP32) ===
// Hanya berisi 5 variabel
typedef struct struct_message {
  float ppm1;
  float rs;
  float vout;
  float temp1;
  float hum1;
} struct_message;

struct_message myData;

// === Parameter Kalibrasi & Hardware MQ-135 ===
const int mqPin = A0; 
const float RL = 0.2; 
const float R0 = 0.13;
const float Vcc = 3.3; 
float a = 4.42; 
float b = -2.25;

// Callback saat data selesai dikirim
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Status Kirim: ");
  if (sendStatus == 0){
    Serial.println("Sukses");
  }
  else{
    Serial.println("Gagal");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Memulai sensor DHT
  dht.begin();
  
  // Set perangkat sebagai Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // === TAMPILKAN MAC ADDRESS WEMOS ===
  // Buka Serial Monitor saat Wemos baru dinyalakan.
  // Salin MAC Address ini dan masukkan ke kode ESP32 Anda!
  Serial.println();
  Serial.println("-----------------------------------");
  Serial.print("MAC Address Wemos ini: ");
  Serial.println(WiFi.macAddress());
  Serial.println("-----------------------------------");

  // Inisialisasi ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error inisialisasi ESP-NOW");
    return;
  }

  // Set peran Wemos sebagai Controller (Pengirim)
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Daftarkan ESP32 sebagai peer penerima
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println("Memulai pemanasan sensor MQ-135 dan DHT22...");
  delay(3000); 
}

void loop() {
  // === 1. Baca Sensor DHT22 ===
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  // Periksa apakah pembacaan DHT gagal
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    hum = 0.0;
    temp = 0.0;
  }

  // === 2. Baca Sensor MQ-135 ===
  int adcValue = analogRead(mqPin);
  float Vout = (adcValue / 1023.0) * Vcc;

  float Rs = 0;
  if (Vout > 0.0) {
    Rs = RL * (Vcc - Vout) / Vout;
  }     

  float ratio = Rs / R0;
  float ppm = a * pow(ratio, b);

  // === 3. Cetak ke Serial Monitor untuk debugging ===
  Serial.print("Suhu: "); Serial.print(temp); Serial.print(" C\t");
  Serial.print("Kelembapan: "); Serial.print(hum); Serial.print(" %\t");
  Serial.print("PPM NH3: "); Serial.print(ppm, 2);
  Serial.print("\tRs: "); Serial.print(Rs, 2);
  Serial.print("\tVout: "); Serial.println(Vout);

  // === 4. Masukkan data ke dalam struktur untuk dikirim ===
  myData.ppm1 = ppm;
  myData.rs = Rs;
  myData.vout = Vout;
  myData.temp1 = temp;
  myData.hum1 = hum;

  // === 5. Kirim pesan melalui ESP-NOW ===
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000); // Kirim data setiap 2 detik
}