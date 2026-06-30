#include "arduino_secrets.h"
#include "thingProperties.h"

#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

#define LED_PIN D5

#define RELAY D6

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 2000;

void setup()
{
  Serial.begin(115200);
  delay(1500);

  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);

  digitalWrite(LED_PIN, LOW);

  dht.begin();

  initProperties();
  thresholdTemperature = 30.0;

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  Serial.println("System Started");
}

void loop()
{
  ArduinoCloud.update();

  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if(isnan(humidity) || isnan(temperature))
    {
      Serial.println("Failed to read DHT11");
      return;
    }

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity : ");
    Serial.print(humidity);
    Serial.println(" %");

 if (temperature >= thresholdTemperature)
{
    Alert = true;
    fan = true;

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(RELAY, LOW);

    Serial.println("Fan ON");
}
else if (temperature <= thresholdTemperature - 1)
{
    Alert = false;
    fan = false;

    digitalWrite(LED_PIN, LOW);
    digitalWrite(RELAY, HIGH);

    Serial.println("Fan OFF");
}
  }
}
void onThresholdTemperatureChange() {
  Serial.print("Threshold changed to: ");
  Serial.println(thresholdTemperature);
}