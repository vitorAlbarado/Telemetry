//----DHT-----
#include <DHT.h>
#define DHTPIN 0
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float umidade;
float temperatura;

//----DISPLAY----
#include <Adafruit_SSD1306.h>
//#define SCREEN_HEIGHT 64
#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);


//----SETUP VOID----
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
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.setTextColor(WHITE);
  display.clearDisplay();
}

void realizarMedicoes() {
  delay(2000);
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
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
  display.setCursor(0,0);
  display.print(texto2);

  display.display();
  delay(2000);
}