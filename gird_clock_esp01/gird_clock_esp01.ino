#include <SoftwareSerial.h>

SoftwareSerial ESP01(SCK, MISO);

void setup()
{
	Serial.begin(9600);
	ESP01.begin(9600);
	delay(1000);
	ESP01.println("AT+RST");
}

void loop()
{
	if (ESP01.available())
	{
		Serial.write(ESP01.read());
	}

	if (Serial.available())
	{
		int _serial = Serial.read();
		if (_serial == 49) 
		{
			ESP01.println("AT+CIPSTART=\"TCP\",\"novang.me\",80");
		}
		else if (_serial == 50)
		{
			ESP01.println("AT+CIPSEND=44");
		}
		else if (_serial == 51)
		{
			ESP01.print("GET /time.php HTTP/1.1\r\nHost: novang.me\r\n\r\n");
		}
		ESP01.write(Serial.read());
	}
}


