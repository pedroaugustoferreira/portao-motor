/*
  Exemplo simples para recepção usando rc-switch
  
  Biblioteca disponível em: https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

// Instancia do objeto RCSwitch
RCSwitch mySwitch = RCSwitch();

// Definição dos pinos e variáveis
const int sensor_fechado = 8; // GPIO5 = D1
const int sensor_aberto = 5; // GPIO5 = D5
const int ABRINDO = 0;
const int PARADO = 1;
const int FECHANDO = 2;
int tempo_delay = 500;

const int pino_ir = 0; // mega pino 2
const int rele_aberto = 22;
const int rele_fechado = 26;

int proximo_estado = ABRINDO;
int ultima_acao;

// Array com os códigos válidos
long codigos_validos[] = {8632593, 8632594};

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
    // Obtém o código recebido
    long codigo_recebido = mySwitch.getReceivedValue();
    
    // Verifica se o código recebido está no array de códigos válidos
    bool codigo_valido = false;
    for (int i = 0; i < sizeof(codigos_validos) / sizeof(codigos_validos[0]); i++) {
      if (codigo_recebido == codigos_validos[i]) {
        codigo_valido = true;
        break;
      }
    }
    
    // Executa a ação apenas se o código for válido
    if (codigo_valido) {
      // Alterna entre os estados
      if (proximo_estado == ABRINDO) {
        Serial.print("Portão Abrindo - delay: ");
        Serial.println(tempo_delay);
        digitalWrite(rele_aberto, LOW);
        digitalWrite(rele_fechado, HIGH);
        proximo_estado = PARADO;
        delay(tempo_delay);
        // tempo_delay = 500;
      } 
      else if (proximo_estado == FECHANDO) {
        Serial.print("Portão Fechando - delay: ");
        Serial.println(tempo_delay);
        digitalWrite(rele_aberto, HIGH);
        digitalWrite(rele_fechado, LOW);
        proximo_estado = PARADO;
        delay(tempo_delay);
        // tempo_delay = 500;
      } 
      else if (proximo_estado == PARADO) {
        Serial.print("Portão Parado - delay: ");
        Serial.println(tempo_delay);
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

      Serial.print("Código válido recebido: ");
      Serial.println(codigo_recebido);
    } else {
      Serial.print("Código inválido: ");
      Serial.println(codigo_recebido);
    }

    delay(tempo_delay); // Atraso para simular a ação
    mySwitch.resetAvailable(); // Reseta o status de disponibilidade do receptor
  }

  // Verifica se o botão foi pressionado
  if (digitalRead(sensor_fechado) == LOW) {
    Serial.print("Portão fechado por completo - delay: ");
    Serial.println(tempo_delay);
    digitalWrite(rele_aberto, HIGH);
    digitalWrite(rele_fechado, HIGH);
    proximo_estado = ABRINDO;
    // delay(tempo_delay);
    tempo_delay = 3000; // Atraso para evitar múltiplas leituras do botão
  }
  else if (digitalRead(sensor_aberto) == LOW) {
    Serial.print("Portão aberto por completo - delay: ");
    Serial.println(tempo_delay);
    digitalWrite(rele_aberto, HIGH);
    digitalWrite(rele_fechado, HIGH);
    proximo_estado = FECHANDO;
    // delay(tempo_delay);
    tempo_delay = 3000; // Atraso para evitar múltiplas leituras do botão
  }else
  {
    tempo_delay = 500;
  }

}
