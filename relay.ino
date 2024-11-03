#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "";
const char* password = "";

ESP8266WebServer server(80);

// Defina o pino do relay e o LED
const int relayPin = D1; // Pino do relay
const int ledPin = LED_BUILTIN; // LED embutido da placa

// Configurações de IP fixo
IPAddress local_IP(192, 168, 15, 149);
IPAddress gateway(192, 168, 15, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 15, 1);

// Estrutura para armazenar informações das redes WiFi
struct WiFiNetwork {
  String ssid;
  int rssi;
};

// Função de comparação para ordenar pelo sinal mais forte (RSSI)
int compareWiFiNetworks(const void* a, const void* b) {
  return ((WiFiNetwork*)b)->rssi - ((WiFiNetwork*)a)->rssi;
}

void setup() {
  Serial.begin(9600); // Ajusta a porta serial para 9600 bps

  // Configura os pinos
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay desligado inicialmente
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // LED desligado inicialmente

  // Escanear redes WiFi disponíveis
  Serial.println("Escaneando redes WiFi...");
  int n = WiFi.scanNetworks();

  if (n == 0) {
    Serial.println("Nenhuma rede encontrada.");
  } else {
    // Armazenar redes e ordenar pelo sinal
    WiFiNetwork networks[n];
    for (int i = 0; i < n; ++i) {
      networks[i].ssid = WiFi.SSID(i);
      networks[i].rssi = WiFi.RSSI(i);
    }
    
    // Ordena o array de redes por RSSI (sinal mais forte primeiro)
    qsort(networks, n, sizeof(WiFiNetwork), compareWiFiNetworks);

    // Exibe redes ordenadas
    Serial.println("Redes disponíveis:");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(networks[i].ssid);
      Serial.print(" (Sinal: ");
      Serial.print(networks[i].rssi);
      Serial.println(" dBm)");
    }
  }
  Serial.println();

  // Configurar IP fixo
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("Falha ao configurar IP fixo");
  }

  // Conectar ao WiFi
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  
  // Aguardar conexão ao WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi. IP: ");
  Serial.println(WiFi.localIP());

  // Manter o LED aceso para indicar que estamos conectados
  digitalWrite(ledPin, LOW);

  // Configura o handler para a URL /portao
  server.on("/portao", handleRelay);

  // Inicia o servidor
  server.begin();
  Serial.println("Servidor web iniciado.");
}

void loop() {
  // Mantém o servidor rodando
  server.handleClient();
}

void handleRelay() {
  Serial.println("Ativando relay por 0.5 segundo...");
  digitalWrite(relayPin, LOW); // Ativa o relay
  delay(500); // Espera 1 segundo
  digitalWrite(relayPin, HIGH); // Desativa o relay

  // Responde à requisição
  server.send(200, "text/plain", "Relay acionado por 0.5 segundo");
}
