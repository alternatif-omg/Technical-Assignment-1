#include <WiFi.h>
#include <HTTPClient.h>

// Mengganti dengan kredensial WiFi Anda
const char* ssid = "Wifi Choi";
const char* password = "tanpapassword";

// Alamat server lokal
const char* serverName = "http://192.168.1.89:8000/api/rain";

// Pin sensor hujan
const int rainSensorPin = 36; // Analog Output (Ao) ke GPIO 36
const int rainSensorPower = 32; // VCC ke GPIO 32
const int rainSensorGround = 13; // Digital Output (Do) ke GPIO 13

void setup() {
  Serial.begin(115200);

  // Menyalakan sensor hujan
  pinMode(rainSensorPower, OUTPUT);
  digitalWrite(rainSensorPower, HIGH);
  pinMode(rainSensorGround, OUTPUT);
  digitalWrite(rainSensorGround, LOW);

  WiFi.begin(ssid, password);
  Serial.println("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Terhubung ke WiFi dengan IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Membaca nilai sensor hujan
  int rainValue = analogRead(rainSensorPin);

  // Menentukan intensitas hujan
  String rainIntensity;
  if (rainValue < 500) {
    rainIntensity = "Tidak Hujan";
  } else if (rainValue < 1000) {
    rainIntensity = "Hujan Rendah";
  } else if (rainValue < 2000) {
    rainIntensity = "Hujan Sedang";
  } else {
    rainIntensity = "Hujan Intensitas Tinggi";
  }

  // Membuat HTTP POST request
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  String requestBody = "{\"rain_value\":" + String(rainValue) + ", \"rain_intensity\":\"" + rainIntensity + "\"}";
  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Respons dari server: " + response);
    Serial.println("Intensitas Hujan: " + rainIntensity);
  } else {
    Serial.println("Gagal mengirim data. Kode respons: " + String(httpResponseCode));
  }

  http.end();
  delay(5000); // Menunggu 5 detik sebelum mengambil data berikutnya
}