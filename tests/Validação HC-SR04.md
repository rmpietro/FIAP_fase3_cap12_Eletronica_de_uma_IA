## Validação do HC-SR04: Nível de Água
O código abaixo testa o comportamento do sistema ao simular diferentes níveis de água para verificar os LEDs de alerta e de irrigação.
```
#define TRIGGER_PIN 5
#define ECHO_PIN 18
#define LED_ALERTA_PIN 14
#define LED_IRRIGACAO_PIN 16

void setup() {
    Serial.begin(115200);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_ALERTA_PIN, OUTPUT);
    pinMode(LED_IRRIGACAO_PIN, OUTPUT);
}

void loop() {
    float distancia[] = {5.0, 15.0, 25.0}; // Simula níveis de água: baixo, ideal, alto

    for (int i = 0; i < 3; i++) {
        Serial.printf("Testando HC-SR04: Distância simulada: %.2f cm\n", distancia[i]);

        if (distancia[i] < 10.0) {
            Serial.println("Alerta esperado: Nível de água muito baixo! Necessário iniciar irrigação.");
            digitalWrite(LED_ALERTA_PIN, HIGH);
            digitalWrite(LED_IRRIGACAO_PIN, HIGH);
        } else if (distancia[i] < 20.0) {
            Serial.println("Nível ideal: Irrigação em standby.");
            digitalWrite(LED_ALERTA_PIN, LOW);
            digitalWrite(LED_IRRIGACAO_PIN, HIGH);
        } else {
            Serial.println("Nível alto: Irrigação desnecessária.");
            digitalWrite(LED_ALERTA_PIN, LOW);
            digitalWrite(LED_IRRIGACAO_PIN, LOW);
        }
        delay(2000);
    }
}

