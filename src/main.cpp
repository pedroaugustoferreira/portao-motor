/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-03-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
int contador = 0;
int pin_50_relay_abrindo = 50;
int pin_51_relay_fechando = 51;
int pin_52_relay_controle = 52;
int pin_53_sensor_fim_da_abertura = 53;
int pin_54_sensor_fim_do_fechamento = 48;

int postao_status = 0; // 0 parado, 1 abrindo e 2 fechando
int portao_fechado = 0;
int botao_acionado = 0;
int sensor_fim_da_abertura = 0;
int sensor_fim_do_fechamento = 0;
int click = 0;
int ativo = LOW;
int desativado = HIGH;

/**
 * @brief
 *
 */

void setup()
{
  pinMode(pin_50_relay_abrindo, OUTPUT);
  pinMode(pin_51_relay_fechando, OUTPUT);
  pinMode(pin_52_relay_controle, INPUT_PULLUP);
  pinMode(pin_53_sensor_fim_da_abertura, INPUT_PULLUP);
  pinMode(pin_54_sensor_fim_do_fechamento, INPUT_PULLUP);

  digitalWrite(pin_50_relay_abrindo, desativado);
  digitalWrite(pin_51_relay_fechando, desativado);
  digitalWrite(pin_52_relay_controle, desativado);
  digitalWrite(pin_53_sensor_fim_da_abertura, desativado);
  digitalWrite(pin_54_sensor_fim_do_fechamento, desativado);

  Serial.begin(9600);
  Serial.println("Iniciando");
}

void debug()
{

  Serial.print("pin_50_relay_abrindo: ");
  Serial.println(digitalRead(pin_50_relay_abrindo));

  Serial.print("pin_51_relay_fechando: ");
  Serial.println(digitalRead(pin_51_relay_fechando));

  Serial.print("pin_52_relay_controle: ");
  Serial.println(digitalRead(pin_52_relay_controle));

  Serial.print("pin_53_sensor_fim_da_abertura: ");
  Serial.println(digitalRead(pin_53_sensor_fim_da_abertura));

  Serial.print("pin_54_sensor_fim_do_fechamento: ");
  Serial.println(digitalRead(pin_54_sensor_fim_do_fechamento));  

  Serial.print("postao_status: ");
  Serial.println(postao_status);

  Serial.print("sensor_fim_da_abertura: ");
  Serial.println(sensor_fim_da_abertura);

  Serial.println(" ");
}

/**
 * @brief
 *
 */

void loop()
{

 // debug();

  // delay(1000);

  botao_acionado = digitalRead(pin_52_relay_controle);
  sensor_fim_da_abertura   = digitalRead(pin_53_sensor_fim_da_abertura);
  sensor_fim_do_fechamento = digitalRead(pin_54_sensor_fim_do_fechamento);

  if (sensor_fim_da_abertura == ativo)
  {
    digitalWrite(pin_50_relay_abrindo, desativado);
    postao_status = 3 - 1;
  }

  if (sensor_fim_do_fechamento == ativo)
  {
    digitalWrite(pin_51_relay_fechando, desativado);
    postao_status = 0;
  }



  // Serial.print("pin_50_relay_abreindo: ");
  // Serial.println(digitalRead(pin_50_relay_abrindo));

  if (botao_acionado == ativo)
  {

    postao_status++;

    if (postao_status == 1)
    {
      Serial.println("Portao abrindo");
      digitalWrite(pin_50_relay_abrindo, ativo);
    }

    if (postao_status == 2)
    {
      Serial.println("Portao Parado");
      digitalWrite(pin_50_relay_abrindo, desativado);
    }

    if (postao_status == 3)
    {
      Serial.println("Portao Fechando");
      digitalWrite(pin_51_relay_fechando, ativo);
    }

    if (postao_status == 4)
    {
      Serial.println("Portao Parado");
      digitalWrite(pin_51_relay_fechando, desativado);
      postao_status = 0;
    }

    delay(1000);
  }

  //   delay(1000);

  //  }

  //  //   if(fim_de_curso == 0){
  //   //    postao_status = 0;
  //   //    click = 2;
  //    // }

  //   contador++;
  //   Serial.println(contador);
}
