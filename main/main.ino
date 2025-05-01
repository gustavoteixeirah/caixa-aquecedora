#include <Adafruit_AHTX0.h>
#include <Wire.h>

#define TCAADDR 0x70  // Endereço do PCA9548A
#define RELAY_PIN 7   // Pino do relé
#define TEMP_LOWER 28.0  // Liga aquecedor
#define TEMP_UPPER 29.0  // Desliga aquecedor

Adafruit_AHTX0 aht;
bool relayState = false;

void tcaselect(uint8_t i) {
  if (i > 7) return;
  
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Relé desligado
  
  Serial.println("AHT10 com controle de temperatura!");
  
  for (uint8_t t = 0; t < 4; t++) {
    tcaselect(t);
    if (aht.begin()) {
      Serial.print("Sensor AHT10 encontrado no canal ");
      Serial.println(t);
    } else {
      Serial.print("Erro no canal ");
      Serial.println(t);
    }
  }
}

void loop() {
  float tempSum = 0;
  int validReadings = 0;
  
  // Lê todos os sensores e calcula média
  for (uint8_t i = 0; i < 4; i++) {
    tcaselect(i);
    
    sensors_event_t humidity, temp;
    if (aht.getEvent(&humidity, &temp)) {
      tempSum += temp.temperature;
      validReadings++;
      
      Serial.print("Canal ");
      Serial.print(i);
      Serial.print(" - Temp: ");
      Serial.print(temp.temperature);
      Serial.print("°C, Umidade: ");
      Serial.print(humidity.relative_humidity);
      Serial.println("% rH");
    }
  }
  
  if (validReadings > 0) {
    float avgTemp = tempSum / validReadings;
    Serial.print("Temperatura média: ");
    Serial.print(avgTemp);
    Serial.println("°C");
    
    // Controle do relé
    if (avgTemp <= TEMP_LOWER && !relayState) {
      digitalWrite(RELAY_PIN, HIGH);  // Liga relé
      relayState = true;
      Serial.println("Aquecedor LIGADO");
    } 
    else if (avgTemp >= TEMP_UPPER && relayState) {
      digitalWrite(RELAY_PIN, LOW);   // Desliga relé
      relayState = false;
      Serial.println("Aquecedor DESLIGADO");
    }
    
    Serial.print("Estado do relé: ");
    Serial.println(relayState ? "LIGADO" : "DESLIGADO");
  }
  
  Serial.println("-----------------");
  delay(1000);
}