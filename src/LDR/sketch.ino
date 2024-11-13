#define LDR_PIN 34  // Pino ADC conectado ao LDR

// Limite de luz para ajuste de irrigação
int limiteLuz = 2000;  
bool simularLuzBaixa = true;  // Variável para alternar simulação de luz

void setup() {
    Serial.begin(9600);  // Inicia a comunicação serial
    Serial.println("Sistema pronto. Monitorando a intensidade de luz...");
}

void loop() {
    // Simulação do valor do LDR alternando entre luz baixa e adequada
    int ldrValue = simularLuzBaixa ? 1500 : 2500;  // Alterna entre 1500 (baixo) e 2500 (adequado)
    simularLuzBaixa = !simularLuzBaixa;  // Alterna o valor de luz na próxima execução

    Serial.print("Nível de Luz: ");
    Serial.println(ldrValue);  // Exibe o valor simulado no Monitor Serial

    // Verifica o nível de luz para ajuste de irrigação
    if (ldrValue < limiteLuz) {
        Serial.println("Nível de luz baixo - Necessário aumentar irrigação.");
    } else {
        Serial.println("Nível de luz adequado - Irrigação em modo normal.");
    }

    delay(2000);  // Aguarda 2 segundos antes da próxima leitura
}
