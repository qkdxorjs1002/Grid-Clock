#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SSID "ssid"
#define PASSWORD "password"
#define TARGET_URI "http://novang.me/time.php"

void setup()
{
	Serial.begin(115200);
	WiFi.begin(SSID, PASSWORD);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Connecting.....");
	}
}

void loop()
{
	if (WiFi.status() == WL_CONNECTED)
	{
		HTTPClient httpClient;
		httpClient.begin(TARGET_URI);
		int httpCode = httpClient.GET();

		if (httpCode == 200)
		{
			Serial.println(httpClient.getString());
		}
		httpClient.end();
	}
	delay(30000);
}