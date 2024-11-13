#include <Arduino.h>

#define TRIGGER_PIN 5         // Pino TRIG do HC-SR04
#define ECHO_PIN 18           // Pino ECHO do HC-SR04
#define LED_ALERTA_PIN 14     // Pino do LED de alerta de nível de água
#define LED_IRRIGACAO_PIN 16  // Pino do LED de irrigação

const float limiteBaixo = 10.0;   // Limite para nível de água baixo em cm
const float limiteIdeal = 20.0;   // Limite para nível de água ideal em cm
unsigned long intervaloLeitura = 2000;  // Intervalo entre leituras em milissegundos
unsigned long ultimaLeitura = 0;        // Armazena o tempo da última leitura
int simulacaoNivel = 0;  // Variável para alternar entre os níveis de água

void setup() {
    Serial.begin(9600);          // Inicializa o Monitor Serial com a taxa de 9600
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_ALERTA_PIN, OUTPUT);     // Configura o LED de alerta como saída
    pinMode(LED_IRRIGACAO_PIN, OUTPUT);  // Configura o LED de irrigação como saída
    digitalWrite(LED_ALERTA_PIN, LOW);   // Garante que o LED de alerta comece apagado
    digitalWrite(LED_IRRIGACAO_PIN, LOW); // Garante que o LED de irrigação comece apagado
}

void loop() {
    // Verifica se é hora de fazer uma nova leitura
    if (millis() - ultimaLeitura >= intervaloLeitura) {
        ultimaLeitura = millis();  // Atualiza o tempo da última leitura

        // Alterna entre diferentes níveis de água para simulação
        float distance;
        if (simulacaoNivel == 0) {
            distance = 5.0;   // Simula nível muito baixo
        } else if (simulacaoNivel == 1) {
            distance = 15.0;  // Simula nível ideal
        } else {
            distance = 25.0;  // Simula nível alto
        }

        // Incrementa a variável de simulação para alternar entre os níveis
        simulacaoNivel = (simulacaoNivel + 1) % 3;

        // Exibe a distância simulada no Monitor Serial
        Serial.printf("Distância simulada: %.2f cm\n", distance);

        // Verifica os níveis de água e emite alertas
        if (distance < limiteBaixo) {
            digitalWrite(LED_ALERTA_PIN, HIGH);  // Acende o LED de alerta
            digitalWrite(LED_IRRIGACAO_PIN, HIGH);  // Ativa o LED de irrigação
            Serial.println("Alerta: Nível de água muito baixo! Necessário iniciar irrigação.");
        } else if (distance < limiteIdeal) {
            digitalWrite(LED_ALERTA_PIN, LOW);  // Desativa o LED de alerta
            digitalWrite(LED_IRRIGACAO_PIN, HIGH);  // Ativa o LED de irrigação para standby
            Serial.println("Nível de água ideal: Irrigação em standby.");
        } else {
            digitalWrite(LED_ALERTA_PIN, LOW);    // Desativa o LED de alerta
            digitalWrite(LED_IRRIGACAO_PIN, LOW); // Desativa o LED de irrigação
            Serial.println("Nível de água alto: Irrigação desnecessária.");
        }
    }

    // Código de leitura real usando pulseIn, mas sem afetar a simulação
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    pulseIn(ECHO_PIN, HIGH);  // Mantém o uso do sensor com pulseIn

    // Atraso antes da próxima leitura
    delay(500);
}
