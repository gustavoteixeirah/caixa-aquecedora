#include <Adafruit_AHTX0.h>
#include <Wire.h>

#define TCAADDR 0x70  // Endereço padrão do PCA9548A

Adafruit_AHTX0 aht;

void tcaselect(uint8_t i) {
  if (i > 7) return;
  
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Serial.println("Adafruit AHT10/AHT20 demo com PCA9548A!");
  
  // Verifica cada canal do multiplexador
  for (uint8_t t = 0; t < 4; t++) {
    tcaselect(t);
    if (aht.begin()) {
      Serial.print("Sensor AHT10 encontrado no canal ");
      Serial.println(t);
    } else {
      Serial.print("Não foi possível encontrar sensor no canal ");
      Serial.println(t);
    }
  }
}

void loop() {
  for (uint8_t i = 0; i < 4; i++) {
    tcaselect(i);  // Seleciona o canal do multiplexador
    
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    
    Serial.print("Canal ");
    Serial.print(i);
    Serial.print(" - Temperatura: ");
    Serial.print(temp.temperature);
    Serial.print(" °C, Umidade: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% rH");
  }
  Serial.println("-----------------");
  delay(1000);
}