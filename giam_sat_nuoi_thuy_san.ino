#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "OpenSpace_6.3"
#define WIFI_PASSWORD "1041050127"

#define FIREBASE_HOST "ho-thuy-san-database-default-rtdb.asia-southeast1.firebasedatabase.app"

#define API_KEY "AIzaSyAPKItADUgY3W072l8pFUnJ2YMM-r0bZ3U"

#define USER_EMAIL "test_mode@gmail.com"
#define USER_PASSWORD "xxxxxxxxx"

#define do_duc_nuoc_sensor_pin   0
#define nhiet_do_nuoc_sensor_pin 1
#define do_man_nuoc_sensor_pin   2
#define relay_may_bom   		 D1

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int do_duc_nuoc_value, nhiet_do_nuoc_value, do_man_nuoc_value;
int percent = 0;
String may_bom = "0";

void setup(){
	Serial.begin(115200);   // Initiate a serial communication

	Serial.println("__Giam Sat Ho Nuoi Thuy San__");

	pinMode(relay_may_bom, OUTPUT);

	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("Connecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(300);
	}
	Serial.println();
	Serial.print("Connected with IP: ");
	Serial.println(WiFi.localIP());
	Serial.println();

	config.host = FIREBASE_HOST;
	config.api_key = API_KEY;

	Firebase.reconnectWiFi(true);

	auth.user.email = USER_EMAIL;
	auth.user.password = USER_PASSWORD;

	Firebase.reconnectWiFi(true);

	Firebase.begin(&config, &auth);
}
void loop() {
  // đọc cảm 3 cảm biến
	do_duc_nuoc_value = analogRead(do_duc_nuoc_sensor_pin);
	percent = map(do_duc_nuoc_value, 200, 900, 100, 0);

	nhiet_do_nuoc_value = analogRead(nhiet_do_nuoc_sensor_pin);
	do_man_nuoc_value = analogRead(do_man_nuoc_sensor_pin);
  
  // gửi dữ liệu lên firebase
	Firebase.RTDB.setString(&fbdo, "gia tri do duc nuoc", String(percent));
	Firebase.RTDB.setString(&fbdo, "gia tri nhiet do nuoc", String(nhiet_do_nuoc_value));
	Firebase.RTDB.setString(&fbdo, "gia tri do man nuoc", String(do_man_nuoc_value));

  // nhận lệnh điều khiển máy bơm từ firebase
  may_bom = Firebase.RTDB.getString(&fbdo, "May Bom");
  digitalWrite(relay_may_bom, may_bom.toInt());

	// digitalWrite(LED, !digitalRead(LED));
	delay(200);
}
