#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// --- PREENCHA AQUI AS SUAS CREDENCIAIS ---
const char* ssid = "NOME_DO_SEU_WIFI"; 
const char* password = "SENHA_DO_SEU_WIFI";
const char* mqtt_server = "broker.emqx.io"; // Servidor público de alta estabilidade
const int mqtt_port = 1883;

// --- CONFIGURAÇÃO DE TÓPICOS MQTT ---
const char* topico_ocupacao = "senac/flowsense/ocupacao";
const char* client_id = "FlowSense-ESP32-v002"; // Mudei o ID para evitar conflitos na nuvem

// --- DEFINIÇÃO DOS PINOS (ESP32-C3 SuperMini) ---
const int PIN_TRIG = 3;  
const int PIN_ECHO = 2;  
const int PIN_BOTAO_SAIDA = 4; // Fio solto agora simula a SAÍDA
const int PIN_LED_VERDE = 5;   
const int PIN_LED_VERMELHO = 6;
const int PIN_BUZZER = 7;      
const int PIN_SERVO = 9;       

Servo portaServo;
WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis de Controle de Fluxo
int ocupacaoAtual = 0; 
const int CAPACIDADE_MAXIMA = 5;

// Variáveis de Temporização (Cooldown)
bool botaoPressionado = false;
unsigned long ultimaEntrada = 0;
const unsigned long intervaloMovimento = 3000; // 3 segundos entre cada leitura

void abrirPorta() {
  portaServo.write(90); 
  delay(3000);          
  portaServo.write(0);  
}

void conectarWiFi() {
  delay(10);
  Serial.print("\nConectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado.");
  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void reconectarMQTT() {
  while (!client.connected()) {
    Serial.print("Aguardando conexão MQTT...");
    if (client.connect(client_id)) {
      Serial.println("conectado.");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos.");
      delay(5000);
    }
  }
}

void publicarOcupacao() {
  char msg_payload[10];
  snprintf(msg_payload, 10, "%d", ocupacaoAtual);
  Serial.print("Enviando para nuvem: ");
  Serial.println(msg_payload);
  client.publish(topico_ocupacao, msg_payload);
}

void setup() {
  Serial.begin(9600);

  // Configuração Específica do ESP32Servo
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  portaServo.setPeriodHertz(50);
  portaServo.attach(PIN_SERVO, 500, 2400); 
  portaServo.write(0); 

  // Configuração dos Pinos
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BOTAO_SAIDA, INPUT_PULLUP);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_VERMELHO, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Inicia com o LED Verde aceso (Sala vazia = Liberado)
  digitalWrite(PIN_LED_VERDE, HIGH);
  digitalWrite(PIN_LED_VERMELHO, LOW);

  // Iniciar Conexões
  conectarWiFi();
  client.setServer(mqtt_server, mqtt_port);
  reconectarMQTT();
  
  Serial.println("FlowSense iniciado!");
  publicarOcupacao(); 
}

void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }
  client.loop(); 

  // ---------------------------------------------------------
  // 1. ENTRADA (Sensor Ultrassônico - Físico)
  // ---------------------------------------------------------
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duracao = pulseIn(PIN_ECHO, HIGH, 30000);
  long distancia = duracao / 58;

  // Se detectou alguém perto E já passou o tempo de espera (cooldown)
  if (distancia > 0 && distancia < 20 && millis() - ultimaEntrada > intervaloMovimento) {
    ultimaEntrada = millis(); // Reseta o cronômetro
    
    // Verifica se ainda tem espaço na sala
    if (ocupacaoAtual < CAPACIDADE_MAXIMA) {
      ocupacaoAtual++; 
      Serial.print("Entrada detectada! Ocupacao atual: ");
      Serial.println(ocupacaoAtual);
      publicarOcupacao(); 

      // Pisca o LED Verde para confirmar a entrada
      digitalWrite(PIN_LED_VERDE, LOW);
      delay(100);
      digitalWrite(PIN_LED_VERDE, HIGH);
      
      tone(PIN_BUZZER, 2000, 200); 
      abrirPorta();
    } else {
      // Se a sala estiver cheia, bloqueia a entrada
      Serial.println("Entrada Bloqueada: Capacidade Maxima atingida!");
      digitalWrite(PIN_LED_VERDE, LOW);
      for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_LED_VERMELHO, HIGH);
        tone(PIN_BUZZER, 400, 300); 
        delay(400);
        digitalWrite(PIN_LED_VERMELHO, LOW);
        delay(200);
      }
    }
  }

  // ---------------------------------------------------------
  // 2. SAÍDA (Simulação com Fio no Pino 4)
  // ---------------------------------------------------------
  int leituraBotao = digitalRead(PIN_BOTAO_SAIDA);

  if (leituraBotao == LOW && !botaoPressionado) {
    botaoPressionado = true; 

    // Só permite diminuir se tiver gente na sala
    if (ocupacaoAtual > 0) {
      ocupacaoAtual--; 
      Serial.print("Saida simulada! Ocupacao atual: ");
      Serial.println(ocupacaoAtual);
      publicarOcupacao(); 

      tone(PIN_BUZZER, 1500, 150);
      abrirPorta();
    }
  }

  if (leituraBotao == HIGH) {
    botaoPressionado = false;
  }

  // ---------------------------------------------------------
  // 3. ATUALIZAÇÃO DOS LEDs GERAIS
  // ---------------------------------------------------------
  if (ocupacaoAtual >= CAPACIDADE_MAXIMA) {
    digitalWrite(PIN_LED_VERDE, LOW);
    digitalWrite(PIN_LED_VERMELHO, HIGH);
  } else {
    digitalWrite(PIN_LED_VERDE, HIGH);
    digitalWrite(PIN_LED_VERMELHO, LOW);
  }

  delay(100); 
}
