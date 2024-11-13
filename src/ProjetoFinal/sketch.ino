#include <DHT.h>
#include <ESP32Servo.h>  // Biblioteca específica para o ESP32

// Definições para o DHT22
#define DHTPIN 15            // Pino do sensor DHT22
#define DHTTYPE DHT22        // Tipo de sensor (DHT22)
DHT dht(DHTPIN, DHTTYPE);

// Definições para o HC-SR04
#define TRIGGER_PIN 5        // Pino TRIG do sensor ultrassônico
#define ECHO_PIN 18          // Pino ECHO do sensor ultrassônico
#define LED_ALERTA_PIN 14    // Pino do LED de alerta de nível de água
#define LED_IRRIGACAO_PIN 16 // Pino do LED de irrigação (laranja)
#define LED_NIVEL_BAIXO 17   // Pino do LED azul para nível baixo de água

// Definição para o Relé
#define RELE_PIN 4           // Pino de controle do relé

// Definições para o PIR e componentes associados
#define PIR_PIN 23           // Pino de saída do sensor PIR
#define BUZZER_PIN 22        // Pino do buzzer
#define SERVO_PIN 21         // Pino de controle do servomotor

// Definição para o Sensor LDR
#define LDR_PIN 34           // Pino ADC conectado ao LDR
int limiteLuz = 2000;        // Limite de luz para ajuste de irrigação
bool simularLuzBaixa = true; // Variável para alternar simulação de luz

// Objetos e variáveis
Servo meuServo;               // Objeto para controlar o servomotor
bool simularUmidadeAlta = false;  // Simulação de umidade alta e baixa
bool simularTemperaturaAlta = false; // Simulação de temperatura alta e baixa
int simulacaoNivel = 0;       // Alterna entre os níveis de água
unsigned long ultimaLeitura = 0;    // Tempo da última leitura do HC-SR04
unsigned long tempoUltimaSimulacao = 0; // Tempo da última simulação de movimento
const unsigned long intervaloSimulacao = 5000; // Intervalo para simulação de movimento
bool simularMovimento = false; // Variável para simulação de movimento

void setup() {
  Serial.begin(115200);        // Inicializa o Monitor Serial
  dht.begin();                 // Inicializa o sensor DHT22
  
  // Configuração dos pinos dos sensores e LEDs
  pinMode(LED_IRRIGACAO_PIN, OUTPUT);
  pinMode(LED_NIVEL_BAIXO, OUTPUT);
  pinMode(LED_ALERTA_PIN, OUTPUT);
  pinMode(RELE_PIN, OUTPUT);          
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);     
  meuServo.attach(SERVO_PIN);  // Inicializa o servomotor
  meuServo.write(0);           // Servo começa na posição inicial
  
  Serial.println("Sistema pronto. Monitorando sensores...");
}

void loop() {
  // --- Sensor DHT22 ---
  float temperatura = simularTemperaturaAlta ? 35.0 : 24.0;  // Alterna entre temperaturas para simulação
  float umidade = simularUmidadeAlta ? 60.0 : 40.0;          // Alterna entre umidade para simulação

  Serial.printf("Temperatura: %.2f °C, Umidade: %.2f %%\n", temperatura, umidade);

  // Ativa o relé e o LED de irrigação caso a umidade esteja baixa
  if (umidade < 50.0) {
    Serial.println("Irrigação necessária - Relé e LED de Irrigação ativados");
    digitalWrite(RELE_PIN, LOW);             // Liga o relé
    digitalWrite(LED_IRRIGACAO_PIN, HIGH);   // Liga o LED de irrigação
    digitalWrite(LED_NIVEL_BAIXO, HIGH);     // Liga o LED azul para indicar nível baixo
  } else {
    Serial.println("Irrigação desnecessária - Relé e LED de Irrigação desativados");
    digitalWrite(RELE_PIN, HIGH);            // Desliga o relé
    digitalWrite(LED_IRRIGACAO_PIN, LOW);    // Desliga o LED de irrigação
    digitalWrite(LED_NIVEL_BAIXO, LOW);      // Desliga o LED azul
  }

  // Exibe alerta de temperatura alta
  if (temperatura > 30.0) {
    Serial.println("Alerta: Temperatura alta!");
  } else {
    Serial.println("Temperatura dentro do normal.");
  }

  // Alterna os valores de simulação
  simularUmidadeAlta = !simularUmidadeAlta;
  simularTemperaturaAlta = !simularTemperaturaAlta;

  // --- Sensor HC-SR04 ---
  if (millis() - ultimaLeitura >= 2000) {  // Executa a cada 2 segundos
    ultimaLeitura = millis();
    
    // Envia um pulso ultrassônico
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    float distanceReal = (duration * 0.0343) / 2;  // Calcula a distância real
    
    // Simulação de níveis de água
    float distance;
    if (simulacaoNivel == 0) distance = 5.0;
    else if (simulacaoNivel == 1) distance = 15.0;
    else distance = 25.0;
    simulacaoNivel = (simulacaoNivel + 1) % 3;

    Serial.printf("Distância real: %.2f cm | Distância simulada: %.2f cm\n", distanceReal, distance);

    // Verifica os níveis de água e aciona o LED de alerta e de irrigação
    if (distance < 10.0) {
      digitalWrite(LED_ALERTA_PIN, HIGH);  // Liga o LED de alerta
      digitalWrite(LED_IRRIGACAO_PIN, HIGH);  // Liga o LED de irrigação
      Serial.println("Alerta: Nível de água muito baixo! Necessário iniciar irrigação.");
    } else if (distance < 20.0) {
      digitalWrite(LED_ALERTA_PIN, LOW);   // Desliga o LED de alerta
      digitalWrite(LED_IRRIGACAO_PIN, HIGH);  // Liga o LED de irrigação em standby
      Serial.println("Nível de água ideal: Irrigação em standby.");
    } else {
      digitalWrite(LED_ALERTA_PIN, LOW);    // Desliga o LED de alerta
      digitalWrite(LED_IRRIGACAO_PIN, LOW); // Desliga o LED de irrigação
      Serial.println("Nível de água alto: Irrigação desnecessária.");
    }
  }

  // --- Simulação de Movimento ---
  if (millis() - tempoUltimaSimulacao >= intervaloSimulacao) { // Alterna a cada 5 segundos
    tempoUltimaSimulacao = millis();
    simularMovimento = !simularMovimento;  // Alterna simulação de movimento
  }

  // Ativa o buzzer e o servomotor em caso de movimento detectado
  if (simularMovimento) {
    Serial.println("Movimento detectado!");
    tone(BUZZER_PIN, 1000); delay(500); noTone(BUZZER_PIN); delay(500);
    tone(BUZZER_PIN, 1500); delay(500); noTone(BUZZER_PIN); delay(500);
    tone(BUZZER_PIN, 2000); delay(500); noTone(BUZZER_PIN);
    
    meuServo.write(90);    // Movimenta o servo para 90 graus
    delay(1000);
    meuServo.write(0);     // Retorna o servo para a posição inicial
  }

  // --- Sensor LDR ---
  int ldrValue = simularLuzBaixa ? 1500 : 2500;  // Alterna entre luz baixa e adequada
  simularLuzBaixa = !simularLuzBaixa;            // Alterna o valor de luz na próxima execução

  Serial.print("Nível de Luz: ");
  Serial.println(ldrValue);

  // Verifica o nível de luz e ajusta a necessidade de irrigação
  if (ldrValue < limiteLuz) {
    Serial.println("Nível de luz baixo - Necessário aumentar irrigação.");
  } else {
    Serial.println("Nível de luz adequado - Irrigação em modo normal.");
  }

  delay(2000);  // Aguarda 2 segundos antes da próxima leitura
}
