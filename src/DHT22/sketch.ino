#include <DHT.h>      // Biblioteca para o sensor DHT22

// Definições para o DHT22
#define DHTPIN 15      // Pino do DHT22
#define DHTTYPE DHT22  // Tipo de sensor
DHT dht(DHTPIN, DHTTYPE);

// Definições para os LEDs
#define LED_IRRIGACAO 16     // LED que indica irrigação ativada - Laranja
#define LED_NIVEL_BAIXO 17   // LED para indicar nível baixo de água - Azul

// Definição para o Relé
#define RELE_PIN 4           // Pino de controle do relé (para ligar/desligar a bomba de água)

// Limites de umidade e temperatura para simulação
const float LIMITE_UMIDADE = 50.0;
const float LIMITE_TEMPERATURA = 30.0;

// Variável para simulação de alternância de umidade e temperatura
bool simularUmidadeAlta = false;
bool simularTemperaturaAlta = false;

void setup() {
  Serial.begin(115200);  // Inicializa o Monitor Serial
  dht.begin();           // Inicializa o sensor DHT22

  // Configura os pinos dos LEDs e do relé como saída
  pinMode(LED_IRRIGACAO, OUTPUT);
  pinMode(LED_NIVEL_BAIXO, OUTPUT);
  pinMode(RELE_PIN, OUTPUT);

  // Garante que os LEDs e o relé comecem apagados/desligados
  digitalWrite(LED_IRRIGACAO, LOW);
  digitalWrite(LED_NIVEL_BAIXO, LOW);
  digitalWrite(RELE_PIN, LOW);
}

void loop() {
  // Lê a temperatura e umidade do DHT22
  float temperatura = simularTemperaturaAlta ? 35.0 : 24.0; // Simulação alternada para demonstração
  float umidade = simularUmidadeAlta ? 60.0 : 40.0;         // Alterna entre umidade alta e baixa

  // Exibe os valores no Monitor Serial
  Serial.printf("Temperatura: %.2f °C, Umidade: %.2f %%\n", temperatura, umidade);

  // Lógica para ativar o relé e o LED de Irrigação com base na umidade
  if (umidade < LIMITE_UMIDADE) {
    Serial.println("Irrigação necessária - Relé e LED de Irrigação ativados");
    digitalWrite(RELE_PIN, HIGH);         // Liga o relé para ativar a bomba
    digitalWrite(LED_IRRIGACAO, HIGH);    // Liga o LED de irrigação
  } else {
    Serial.println("Irrigação desnecessária - Relé e LED de Irrigação desativados");
    digitalWrite(RELE_PIN, LOW);          // Desliga o relé para desligar a bomba
    digitalWrite(LED_IRRIGACAO, LOW);     // Desliga o LED de irrigação
  }

  // Controle do LED de nível baixo de água
  if (umidade < LIMITE_UMIDADE) {
    digitalWrite(LED_NIVEL_BAIXO, HIGH);  // Acende o LED azul em caso de umidade baixa
  } else {
    digitalWrite(LED_NIVEL_BAIXO, LOW);   // Apaga o LED azul caso contrário
  }

  // Alerta de temperatura alta
  if (temperatura > LIMITE_TEMPERATURA) {
    Serial.println("Alerta: Temperatura alta!");
  } else {
    Serial.println("Temperatura dentro do normal.");
  }

  // Alterna os valores de simulação
  simularUmidadeAlta = !simularUmidadeAlta;
  simularTemperaturaAlta = !simularTemperaturaAlta;

  delay(2000);  // Aguarda 2 segundos antes da próxima leitura
}
