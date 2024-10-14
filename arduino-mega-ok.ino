/*
  Exemplo simples para recepção usando rc-switch
  
  Biblioteca disponível em: https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

// Instancia do objeto RCSwitch
RCSwitch mySwitch = RCSwitch();

// Definição dos pinos e variáveis
const int sensor_fechado = 8; // GPIO5 = D1
const int sensor_aberto = 14; // GPIO5 = D5
const int ABRINDO = 0;
const int PARADO = 1;
const int FECHANDO = 2;

const int pino_ir = 0; // mega pino 2
const int rele_aberto = 22;
const int rele_fechado = 26;

int proximo_estado = ABRINDO;
int ultima_acao;

void setup() {
  // Configuração inicial
  Serial.begin(9600);
  mySwitch.enableReceive(pino_ir); // Receptor no pino 4 GPIO4 = D2
  pinMode(sensor_fechado, INPUT_PULLUP); // Define o pino do botão como entrada com pull-up
  pinMode(sensor_aberto, INPUT_PULLUP);

  pinMode(rele_aberto, OUTPUT);
  digitalWrite(rele_aberto, HIGH);

  pinMode(rele_fechado, OUTPUT);
  digitalWrite(rele_fechado, HIGH);

  Serial.println("Setup concluído");
}

void loop() {
  // Verifica se há dados disponíveis do receptor
  if (mySwitch.available()) {
    // Alterna entre os estados
    if (proximo_estado == ABRINDO) {
      
      Serial.println("Portão Abrindo");
      digitalWrite(rele_aberto, LOW);
      digitalWrite(rele_fechado, HIGH);
      proximo_estado = PARADO;
      delay(2000);
    } 
    else if (proximo_estado == FECHANDO) {
      Serial.println("Portão Fechando");
      digitalWrite(rele_aberto, HIGH);
      digitalWrite(rele_fechado, LOW);
      proximo_estado = PARADO;
    } 
    else if (proximo_estado == PARADO) {
      Serial.println("Portão Parado");
      digitalWrite(rele_aberto, HIGH);
      digitalWrite(rele_fechado, HIGH);

      // Alterna o próximo estado entre ABRINDO e FECHANDO
      if (ultima_acao == ABRINDO) {
        proximo_estado = FECHANDO;
        ultima_acao = FECHANDO;
      } else {
        proximo_estado = ABRINDO;
        ultima_acao = ABRINDO;
      }
    }

    delay(1000); // Atraso para simular a ação
    mySwitch.resetAvailable(); // Reseta o status de disponibilidade do receptor

    // (Comentado) Código para depuração - Recebimento de dados do RCSwitch
    // Serial.print("Recebido: ");
    // Serial.print(mySwitch.getReceivedValue());      
    // Serial.print(" / ");
    // Serial.print(mySwitch.getReceivedBitlength());
    // Serial.print("bit ");
    // Serial.print("Protocolo: ");
    // Serial.println(mySwitch.getReceivedProtocol());
  }

  // Verifica se o botão foi pressionado
  if (digitalRead(sensor_fechado) == LOW) {
    Serial.println("Portão fechado por completo");
    digitalWrite(rele_aberto, HIGH);
    digitalWrite(rele_fechado, HIGH);
    proximo_estado = ABRINDO;
    delay(1000); // Atraso para evitar múltiplas leituras do botão
  }
    if (digitalRead(sensor_aberto) == LOW) {
    Serial.println("Portão aberto por completo");
    digitalWrite(rele_aberto, HIGH);
    digitalWrite(rele_fechado, HIGH);
    proximo_estado = FECHANDO;
    delay(1000); // Atraso para evitar múltiplas leituras do botão
  }

}
