// --- DHT ---
#include <DHT.h>
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int umidade;
int temperatura;

// ############## W I F I ############## 
 #include <ESP8266WiFi.h>
const char* ssid = "Galaxy";
const char* password = "hnws9108";
WiFiClient nodeMCU;

//############### M Q T T ###############
#include <PubSubClient.h>
const char* mqtt_broker = "teste.mosquitto.org";
const char* topic="monitoramento/teste";
const char* mqtt_username="";
const char* mqtt_password="";
const int mqtt_port = 1883;

PubSubClient client (nodeMCU);

bool mqttStatus = 0;

//############### D I S P L A Y ###############
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool connectMQTT();
void callback(char* topic, byte* payload, unsigned int length);

void setup() {
  Serial.begin(115200);
  dht.begin();
    configurarDisplay(); 
    conectaWifi();
}

void loop() {
  realizarMedicoes();
  mostrarNoDisplay();
  
}

//######### F u n c õ e s  A u x i l i a r e s ####################################



//######### C O N F I G U R A R  C O N E X A O  W I F I #########

void conectaWifi() {
  delay(10);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("Conectando...");
  display.display();

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }
  delay(1000);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("Conectado!");
  display.display();

  mqttStatus = connectMQTT();
  }

//######### C O N F I G U R A R  D I S P L A Y #########

void configurarDisplay(){
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void realizarMedicoes() {
  delay(2000);
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature();
  Serial.println(umidade);
  Serial.println(temperatura);
   if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return;
  }
}
void mostrarNoDisplay(){
  exibeDados("Temperatura",(temperatura),"C*");
  exibeDados("Umidade",(umidade),"%");
  
}
//######### E X I B E  D A D O S #########

void exibeDados(const char* texto1, int medicao, const char* texto2){
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.print(texto1);

  display.setTextSize(5);
  display.setCursor(20,20);
  display.print(medicao);

  display.setTextSize(2);
  display.setCursor(90,20);
  display.print(texto2);

  display.display();
  delay(2000);
}

// ########## VALIDA CONEXAO COM O BROKER #########

bool connectMQTT(){

  byte tentativa = 0;
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  do{
    String client_id = "monitoramento-";
    client_id += String(WiFi.macAddress());

    if(client.connect(client_id.c_str(),mqtt_username, mqtt_password)){
      delay(2000);
      display.setTextSize(2);
      display.setCursor(0,0);
      display.print("Exito na conexao");
      display.display();
    }else{
      delay(2000);
      display.setTextSize(2);
      display.setCursor(0,0);
      display.print("Falha ao conectar");
      display.display();
    }
    tentativa++;
    
  }while(!client.connected() && tentativa<5);

  if(tentativa > 5){
    client.publish(topic, "{42}");
    client.subscribe(topic);
    return 1;
  }else{
    return 0;
  }
}

