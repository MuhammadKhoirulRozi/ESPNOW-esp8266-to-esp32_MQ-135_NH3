#include <ESP8266WiFi.h>
#include <espnow.h>

// === GANTI DENGAN MAC ADDRESS ESP32 ANDA ===
// Contoh format: {0x24, 0x6F, 0x28, 0xAE, 0x9D, 0x14}
uint8_t broadcastAddress[] = {0xAC, 0x27, 0x6E, 0x5E, 0x93, 0x08}; //AC:27:6E:5E:93:08

// Struktur paket data yang akan dikirim
typedef struct struct_message {
  float ppm;
  float rs;
  float vout;
} struct_message;

struct_message myData;

// === Parameter Kalibrasi & Hardware ===
const int mqPin = A0; 
const float RL = 1.0; 
const float R0 = 3.6;
const float Vcc = 3.3; 
const float a = 6.43;
const float b = -1.98;

// Callback saat data selesai dikirim
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Status Kirim ESP-NOW: ");
  if (sendStatus == 0){
    Serial.println("Sukses");
  }
  else{
    Serial.println("Gagal");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Set perangkat sebagai Wi-Fi Station (syarat ESP-NOW)
  WiFi.mode(WIFI_STA);

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

  Serial.println("Memulai pemanasan sensor MQ-135...");
  delay(3000); 
}

void loop() {
  // 1. Baca nilai ADC dari pin
  int adcValue = analogRead(mqPin);

  // 2. Hitung Tegangan Output (Vout) - Menggunakan 1023.0 untuk Wemos D1 Mini
  float Vout = (adcValue / 1023.0) * Vcc;

  // 3. Hitung Resistansi Sensor (Rs)
  float Rs = 0;
  if (Vout > 0.0) {
    Rs = RL * (Vcc - Vout) / Vout;
  }     

  // 4. Hitung Rasio Rs/R0 & PPM
  float ratio = Rs / R0;
  float ppm = a * pow(ratio, b);

  // Cetak ke Serial Monitor (Opsional untuk debugging)
  Serial.print("ADC: "); Serial.print(adcValue);
  Serial.print("\t Vout: "); Serial.print(Vout, 2);
  Serial.print(" V\t Rs: "); Serial.print(Rs, 2);
  Serial.print(" kOhm\t PPM: "); Serial.println(ppm, 2);

  // 5. Masukkan data ke dalam struktur untuk dikirim
  myData.ppm = ppm;
  myData.rs = Rs;
  myData.vout = Vout;

  // Kirim pesan melalui ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(2000); // Kirim data setiap 2 detik
}