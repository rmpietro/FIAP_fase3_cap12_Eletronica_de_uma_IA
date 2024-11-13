## Validação do DHT22: Umidade e Temperatura
O código abaixo testa o comportamento do sistema quando a umidade e a temperatura estão em níveis críticos para ativação da irrigação e do alerta de temperatura.
```
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LED_IRRIGACAO_PIN 16
#define LED_NIVEL_BAIXO 17
#define RELE_PIN 4

void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(LED_IRRIGACAO_PIN, OUTPUT);
    pinMode(LED_NIVEL_BAIXO, OUTPUT);
    pinMode(RELE_PIN, OUTPUT);
}

void loop() {
    float temperatura = 35.0; // Simula temperatura alta
    float umidade = 40.0;     // Simula umidade baixa

    Serial.printf("Testando DHT22: Temperatura: %.2f °C, Umidade: %.2f %%\n", temperatura, umidade);

    if (umidade < 50.0) {
        Serial.println("Esperado: Irrigação ativada.");
        digitalWrite(RELE_PIN, LOW);
        digitalWrite(LED_IRRIGACAO_PIN, HIGH);
        digitalWrite(LED_NIVEL_BAIXO, HIGH);
    } else {
        Serial.println("Esperado: Irrigação desativada.");
        digitalWrite(RELE_PIN, HIGH);
        digitalWrite(LED_IRRIGACAO_PIN, LOW);
        digitalWrite(LED_NIVEL_BAIXO, LOW);
    }

    if (temperatura > 30.0) {
        Serial.println("Alerta esperado: Temperatura alta!");
    }

    delay(2000);
}
```
## A partir da imagem, verificamos o funcionamento pelo Monitor Serial:
![Monitor Serial do DHT-22](../assets/Validaçaodht22.png)
