/*
  Exemplo simples para recepção usando rc-switch
  
  Biblioteca disponível em: https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

// Instancia do objeto RCSwitch
RCSwitch mySwitch = RCSwitch();

// Definição dos pinos e variáveis
const int sensor_fechado = 8; // GPIO5 = D1
const int sensor_aberto = 5;  // GPIO5 = D5
const int ABRINDO = 0;
const int PARADO = 1;
const int FECHANDO = 2;
int tempo_delay = 500;  // Tempo de atraso padrão para evitar múltiplas leituras

const int pino_ir = 0;  // Pino para o receptor IR
const int rele_aberto = 22;  // Relé para abrir o portão
const int rele_fechado = 26;  // Relé para fechar o portão

int estado_atual = PARADO;
int proximo_estado = ABRINDO;
int ultima_acao = PARADO;  // Define a última ação para alternar corretamente

// Array com os códigos válidos
long codigos_validos[] = {8632593, 8632594, 159420885, 159420917, 159420901, 144477925};

void setup() {
  // Configuração inicial
  Serial.begin(9600);
  mySwitch.enableReceive(pino_ir);  // Receptor no pino 0
  pinMode(sensor_fechado, INPUT_PULLUP);  // Define o pino do sensor de portão fechado como entrada
  pinMode(sensor_aberto, INPUT_PULLUP);   // Define o pino do sensor de portão aberto como entrada

  pinMode(rele_aberto, OUTPUT);
  digitalWrite(rele_aberto, HIGH);  // Inicia o relé desligado

  pinMode(rele_fechado, OUTPUT);
  digitalWrite(rele_fechado, HIGH);  // Inicia o relé desligado

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
      // Alterna entre os estados de abrindo, parando, fechando, parando
      if (proximo_estado == ABRINDO) {
        Serial.println("Portão Abrindo...");
        digitalWrite(rele_aberto, LOW);   // Ativa o relé de abrir
        digitalWrite(rele_fechado, HIGH); // Desativa o relé de fechar
        estado_atual = ABRINDO;
        proximo_estado = PARADO;
        delay(tempo_delay);  // Atraso para evitar múltiplas leituras
      } 
      else if (proximo_estado == FECHANDO) {
        Serial.println("Portão Fechando...");
        digitalWrite(rele_aberto, HIGH);  // Desativa o relé de abrir
        digitalWrite(rele_fechado, LOW);  // Ativa o relé de fechar
        estado_atual = FECHANDO;
        proximo_estado = PARADO;
        delay(tempo_delay);  // Atraso para evitar múltiplas leituras
      } 
      else if (proximo_estado == PARADO) {
        Serial.println("Portão Parado.");
        digitalWrite(rele_aberto, HIGH);  // Desativa ambos os relés
        digitalWrite(rele_fechado, HIGH);

        // Alterna o próximo estado entre ABRINDO e FECHANDO
        if (estado_atual == ABRINDO) {
          proximo_estado = FECHANDO;  // Se o portão estava abrindo, agora irá fechar
        } else if (estado_atual == FECHANDO) {
          proximo_estado = ABRINDO;  // Se o portão estava fechando, agora irá abrir
        }
        estado_atual = PARADO;  // Atualiza o estado para parado
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

  // Verifica os sensores de portão
  verificar_sensores();
}

// Função para verificar os sensores de portão aberto e fechado
void verificar_sensores() {
  // Verifica se o portão está totalmente fechado
  if (digitalRead(sensor_fechado) == LOW) {
    Serial.println("Portão fechado por completo.");
    digitalWrite(rele_aberto, HIGH);  // Desativa o relé de abrir
    digitalWrite(rele_fechado, HIGH); // Desativa o relé de fechar
    proximo_estado = ABRINDO;         // Próxima ação será abrir
    tempo_delay = 1000;               // Atraso maior para evitar múltiplas leituras
  }
  
  // Verifica se o portão está totalmente aberto
  else if (digitalRead(sensor_aberto) == LOW) {
    Serial.println("Portão aberto por completo.");
    digitalWrite(rele_aberto, HIGH);  // Desativa o relé de abrir
    digitalWrite(rele_fechado, HIGH); // Desativa o relé de fechar
    proximo_estado = FECHANDO;        // Próxima ação será fechar
    tempo_delay = 1000;               // Atraso maior para evitar múltiplas leituras
  } else {
    tempo_delay = 500;  // Restabelece o tempo de delay padrão
  }
}
