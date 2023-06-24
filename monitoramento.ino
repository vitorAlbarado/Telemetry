// --- DHT ---
#include <DHT.h>
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int umidade;
int temperatura;

//----DISPLAY----
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



void setup() {
  Serial.begin(115200);
  dht.begin();
    configurarDisplay(); 
}

void loop() {
  realizarMedicoes();
  mostrarNoDisplay();
  
}

//---Funcões Auxiliares----

//----CONFIGURA DISPLAY----
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
//----EXIBE DADOS----
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

