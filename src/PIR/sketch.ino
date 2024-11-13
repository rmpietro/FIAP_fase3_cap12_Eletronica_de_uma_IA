#include <ESP32Servo.h>  // Biblioteca específica para o ESP32

// Definições dos pinos
#define PIR_PIN 23       // Pino de saída do sensor PIR
#define BUZZER_PIN 22    // Pino do buzzer (conforme exemplo dos professores)
#define SERVO_PIN 21     // Pino de controle do servomotor

// Objeto para controlar o servomotor
Servo meuServo;

// Variáveis para simulação de movimento
unsigned long tempoUltimaSimulacao = 0;
const unsigned long intervaloSimulacao = 5000;  // Simula movimento a cada 5 segundos

void setup() {
  Serial.begin(115200);
  
  // Configura os pinos
  pinMode(PIR_PIN, INPUT);          // PIR como entrada
  pinMode(BUZZER_PIN, OUTPUT);      // Buzzer como saída
  meuServo.attach(SERVO_PIN);       // Inicializa o servomotor
  meuServo.write(0);                // Servo começa na posição inicial (0 graus)
  
  Serial.println("Sistema pronto. Aguardando movimento...");
}

void loop() {
  // Simulação de movimento a cada 5 segundos
  if (millis() - tempoUltimaSimulacao >= intervaloSimulacao) {
    tempoUltimaSimulacao = millis(); // Atualiza o tempo da última simulação
    Serial.println("Movimento detectado!");  // Exibe mensagem de movimento

    // Emite sons variados no buzzer
    tone(BUZZER_PIN, 1000);  // Emite um som de 1000 Hz
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);

    tone(BUZZER_PIN, 1500);  // Emite um som de 1500 Hz
    delay(500);
    noTone(BUZZER_PIN);
    delay(500);

    tone(BUZZER_PIN, 2000);  // Emite um som de 2000 Hz
    delay(500);
    noTone(BUZZER_PIN);
    
    // Movimenta o servomotor para 90 graus
    meuServo.write(90);
    delay(1000);  // Aguarda 1 segundo com o servo a 90 graus

    // Retorna o servomotor à posição inicial
    meuServo.write(0);

    delay(1000);  // Aguarda antes da próxima simulação
  } else {
    // Garante que o buzzer está desligado se não há movimento
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(500);  // Intervalo entre verificações do PIR
}
