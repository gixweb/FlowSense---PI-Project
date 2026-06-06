#include <WiFi.h>
#include <PubSubClient.h>

// --- PREENCHA AQUI A SUA REDE ---
const char* ssid = "NOME_DO_SEU_WIFI"; 
const char* password = "SENHA_DO_SEU_WIFI";

const char* mqtt_server = "broker.emqx.io"; 
const int mqtt_port = 1883;
const char* topico_ocupacao = "senac/flowsense/ocupacao";
const char* client_id = "FlowSense-ESP32-Principal"; 

// --- PINOS DOS SENSORES ---
const int PIN_TRIG_SAIDA = 0;  
const int PIN_ECHO_SAIDA = 1;  
const int PIN_ECHO_ENTRADA = 2; 
const int PIN_TRIG_ENTRADA = 3;  

// --- PORTAS DOS LEDS ---
const int PIN_LED_VERMELHO = 4;  // Seu LED Vermelho na direita
const int PIN_LED_AZUL = 21;     // Seu LED Azul no pino 21 (esquerda)

WiFiClient espClient;
PubSubClient client(espClient);

int ocupacaoAtual = 0; 
const int CAPACIDADE_MAXIMA = 5;

unsigned long ultimaEntrada = 0;
unsigned long ultimaSaida = 0;
const unsigned long intervaloMovimento = 1000; 

long medirDistancia(int pinoTrig, int pinoEcho) {
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  long duracao = pulseIn(pinoEcho, HIGH, 30000); 
  if (duracao == 0) return 999; 
  return duracao / 58; 
}

void conectarWiFi() {
  delay(10);
  Serial.print("\nConectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado com sucesso!");
}

void reconectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando à Nuvem...");
    if (client.connect(client_id)) {
      Serial.println("Conectado!");
    } else {
      delay(5000);
    }
  }
}

void publicarOcupacao() {

  char payload[200];

  snprintf(
    payload,
    sizeof(payload),
    "{\"ocupacao\":%d,\"capacidade\":%d}",
    ocupacaoAtual,
    CAPACIDADE_MAXIMA
  );

  client.publish(
    topico_ocupacao,
    payload
  );

  Serial.println(payload);
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_TRIG_ENTRADA, OUTPUT);
  pinMode(PIN_ECHO_ENTRADA, INPUT);
  pinMode(PIN_TRIG_SAIDA, OUTPUT);
  pinMode(PIN_ECHO_SAIDA, INPUT);
  
  pinMode(PIN_LED_AZUL, OUTPUT);
  pinMode(PIN_LED_VERMELHO, OUTPUT);

  // Garante que o sistema liga com tudo apagado
  digitalWrite(PIN_LED_AZUL, LOW);
  digitalWrite(PIN_LED_VERMELHO, LOW);

  conectarWiFi();
  client.setServer(mqtt_server, mqtt_port);
  reconectarMQTT();
  
  Serial.println("Sistema Iniciado!");
  publicarOcupacao(); 
}

void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }
  client.loop(); 

  long distanciaEntrada = medirDistancia(PIN_TRIG_ENTRADA, PIN_ECHO_ENTRADA);
  delay(60); 
  
  long distanciaSaida = medirDistancia(PIN_TRIG_SAIDA, PIN_ECHO_SAIDA);
  delay(60); 

  // ==========================
  // 1. LÓGICA DE ENTRADA
  // ==========================
  if (distanciaEntrada > 0 && distanciaEntrada < 20 && millis() - ultimaEntrada > intervaloMovimento) {
    ultimaEntrada = millis(); 
    
    if (ocupacaoAtual < CAPACIDADE_MAXIMA) {
      ocupacaoAtual++; 
      Serial.print("Entrada! Lotação: "); Serial.println(ocupacaoAtual);
      publicarOcupacao(); 
      
      // Pisca o Azul APENAS no momento da entrada
      digitalWrite(PIN_LED_AZUL, HIGH);
      delay(300); 
      digitalWrite(PIN_LED_AZUL, LOW);
      
    } else {
      Serial.println("Bloqueado: Sala Cheia!");
      // Tenta entrar com sala cheia: Pisca o Vermelho 3x a avisar do erro
      for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_LED_VERMELHO, LOW); delay(150);
        digitalWrite(PIN_LED_VERMELHO, HIGH); delay(150);
      }
    }
  }

  // ==========================
  // 2. LÓGICA DE SAÍDA (ATUALIZADA)
  // ==========================
  if (distanciaSaida > 0 && distanciaSaida < 20 && millis() - ultimaSaida > intervaloMovimento) {
    ultimaSaida = millis(); 
    if (ocupacaoAtual > 0) {
      ocupacaoAtual--; 
      Serial.print("Saída! Lotação: "); Serial.println(ocupacaoAtual);
      publicarOcupacao(); 

      // Pisca o Vermelho indicando a saída
      // (Pausa de 50ms apagado primeiro para garantir o contraste visual se a sala estava lotada)
      digitalWrite(PIN_LED_VERMELHO, LOW); 
      delay(50);
      digitalWrite(PIN_LED_VERMELHO, HIGH);
      delay(300); 
      digitalWrite(PIN_LED_VERMELHO, LOW);
    }
  }

  // ==========================
  // 3. CONTROLE ESTÁTICO DOS LEDS
  // ==========================
  if (ocupacaoAtual >= CAPACIDADE_MAXIMA) {
    // Lotação Máxima: Trava o Vermelho aceso e FORÇA o Azul a ficar apagado
    digitalWrite(PIN_LED_VERMELHO, HIGH); 
    digitalWrite(PIN_LED_AZUL, LOW);  
  } else {
    // Tem Vagas: Mantém os dois apagados à espera de ação
    digitalWrite(PIN_LED_VERMELHO, LOW);  
    digitalWrite(PIN_LED_AZUL, LOW);  
  }
}
