## Validação do LDR: Intensidade de Luz
O código seguinte testa a resposta do sistema ao simular níveis de luz alta e baixa para ajustar a irrigação.
```
#define LDR_PIN 34
int limiteLuz = 2000;
bool simularLuzBaixa = true;

void setup() {
    Serial.begin(115200);
    pinMode(LDR_PIN, INPUT);
}

void loop() {
    int ldrValue = simularLuzBaixa ? 1500 : 2500; // Simula níveis de luz

    Serial.printf("Testando LDR: Nível de Luz: %d\n", ldrValue);

    if (ldrValue < limiteLuz) {
        Serial.println("Esperado: Nível de luz baixo - Necessário aumentar irrigação.");
    } else {
        Serial.println("Nível de luz adequado - Irrigação em modo normal.");
    }

    simularLuzBaixa = !simularLuzBaixa;
    delay(2000);
}



