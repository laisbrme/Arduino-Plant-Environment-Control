/*
CENTRO UNIVERSITÁRIO DE VALENÇA - UNIFAA
CURSO DE ANÁLISE E DESENVOLVIMENTO DE SISTEMAS

LAÍS BRUM MENEZES

Projeto: Arduino-Plant-Environment-Control
Disciplina: Internet of Things - Cuidando de Nossas Vidas

 Sistema de controle de temperatura e umidade de uma planta
 Componentes:
  - Arduino UNO (ou similar)
  - Sensor de temperatura e umidade: DHT22
  - Display LCD 16x2 com módulo I2C
  - LEDs para indicação de controle de dispositivos
     Podem ser substituídos por relés para acionamento de equipamentos
*/

// Importação de bibliotecas:
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Definições:
#define DHT_PIN 2      // Pino conectado ao DHT
#define DHT_TYPE DHT22 // Tipo do sensor
#define LED_TEMP 4     // Led para simulação de controle de temperatura
#define LED_HUM 5      // Led para simulação de controle de umidade

// Configurações:
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);

// Variáveis de limites:
/*
  CONSIDERAÇÕES DO PROCESSO DE EVAPOTRANSPIRAÇÃO
  - A temperatura mínima foi estipulada com base no cultivo da banana
  (pertencente ao grupo de culturas: frutas e forrageiras) para o
    planejamento de plantio em função de geadas, onde na região Sudeste a
    ocorrência de geada concentra-se nos meses de junho a agosto.

  - As umidades relativas, mínima e máxima, foram definidas com base na curva
    característica de retenção de água no solo, onde os valores recomendados
    para o fator de segurança de disponibilidade de água no solo para a
    manutenção encontra-se na tabela a seguir, sendo fator o mínimo o
    Ponto de Murcha Permanente e o máximo o Ponto de Saturação.

    |  GRUPOS DE CULTURAS    |  FATOR f    |
    |------------------------|-------------|
    |  Olerícolas            |  0,2 a 0,4  |
    |  Frutas e Forrageiras  |  0,3 a 0,5  |
    |  Grãos e Algodão       |  0,4 a 0,6  |

*/
float tempMin = 10.0; // Temperatura mínima (em graus Celsius)
float humMin = 30.0;  // Umidade relativa mínima (em %)
float humMax = 50.0;  // Umidade relativa máxima

void setup()
{

  // Inicialização dos LEDs
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_HUM, OUTPUT);
  digitalWrite(LED_TEMP, LOW); // Led desligado
  digitalWrite(LED_HUM, LOW);  // Led desligado

  // Inicialização do sensor e LCD
  dht.begin();
  lcd.init();
  lcd.backlight();

  // Mensagem inicial no display
  lcd.setCursor(0, 0);
  lcd.print("Controle Planta");
  delay(3000);
  lcd.clear();
}

void loop()
{

  // Leitura do sensor
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  // Exibição no LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(umidade);
  lcd.print("%");

  // Controle de ocorrência de geada com relação à temperatura
  if (temperatura <= tempMin)
  {
    digitalWrite(LED_TEMP, HIGH); // Aciona Alerta

    // Exibição do Alerta no LCD
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Alerta de");

    lcd.setCursor(5, 1);
    lcd.print("Geada");
  }

  // Controle de irrigação com relação à umidade:
  if (umidade <= humMin)
  {
    digitalWrite(LED_HUM, HIGH); // Aciona Irrigação

    // Exibição do Alerta no LCD
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Irrigacao");

    lcd.setCursor(3, 1);
    lcd.print("Acionada");
  }
  else if (umidade >= humMax)
  {
    digitalWrite(LED_HUM, LOW); // Desliga Irrigação
  }

  delay(2000); // Espera 2 segundos antes da próxima leitura
}
