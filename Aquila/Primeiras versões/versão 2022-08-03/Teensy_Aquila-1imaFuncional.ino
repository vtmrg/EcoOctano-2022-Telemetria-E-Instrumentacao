#include <SPI.h>  //raio=8cm
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define TEMPOATT 200      
#define TEMPOATT2 80.0      
#define TEMPOATT3 200     

const int OLED_RESET=-1, pinoInterrupt = 3, pinoBotao=10;
//#define OLED_RESET 4
const byte SCREEN_WIDTH = 128, SCREEN_HEIGHT = 64;         
int i_vel=0, dado1,dado2,k;
float t0=0, t1=0 ,tempo_p0 =0,comprimento=0, periodo=0, velocidadeAnt=0,circunferencia=1.445,velocidade_display=0,velocidade =0, tempo=0, tempoatualizacao=0,distancia=0,distanciaAnt=0, velocidade_serial=0,potencia_serial=0, distancia_serial=0;

String mensagem,str_velocidade_serial, str_distancia_serial, str_potencia_serial;
 
const unsigned char epd_bitmap_logoazul [] PROGMEM = {
 // 'logoazul, 64x64px
  0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x07, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x0e, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x0c, 0x00, 0x00, 
  0x00, 0x00, 0xc0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 
  0x00, 0x06, 0x00, 0x1f, 0xf0, 0x00, 0x60, 0x00, 0x00, 0x08, 0x01, 0xff, 0xff, 0x80, 0x18, 0x00, 
  0x00, 0x30, 0x0f, 0xfb, 0xff, 0xe0, 0x0c, 0x00, 0x00, 0x60, 0x3f, 0xf8, 0xff, 0xf8, 0x06, 0x00, 
  0x00, 0xc0, 0x3f, 0xf8, 0x3f, 0xfe, 0x03, 0x00, 0x00, 0x80, 0x1f, 0xf8, 0x1f, 0xff, 0x01, 0x80, 
  0x01, 0x02, 0x0f, 0xf8, 0x0f, 0xff, 0xc0, 0x80, 0x02, 0x07, 0x87, 0xf8, 0x07, 0xff, 0xe0, 0x40, 
  0x06, 0x0f, 0xc3, 0xf9, 0x03, 0xcf, 0xf0, 0x60, 0x04, 0x1f, 0xe1, 0xf8, 0x83, 0x8f, 0xf8, 0x20, 
  0x08, 0x3f, 0xf0, 0xf8, 0xc3, 0x0f, 0xfc, 0x10, 0x08, 0x3f, 0xf8, 0x38, 0x62, 0x4f, 0xfc, 0x18, 
  0x10, 0x7f, 0xfe, 0x1c, 0x72, 0x4f, 0xfe, 0x08, 0x10, 0x7f, 0xff, 0x0e, 0x32, 0xcf, 0xff, 0x08, 
  0x20, 0xff, 0xff, 0x87, 0x3a, 0xcf, 0xff, 0x04, 0x20, 0xff, 0xff, 0xc1, 0x9e, 0x9f, 0xff, 0x84, 
  0x21, 0xff, 0xff, 0xe0, 0xff, 0xbf, 0xff, 0x86, 0x41, 0xf8, 0x01, 0xf8, 0x7f, 0xff, 0xff, 0x82, 
  0x43, 0xf8, 0x01, 0xfc, 0x1f, 0xff, 0xff, 0x82, 0x43, 0xf8, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x02, 
  0x43, 0xf8, 0x7f, 0xff, 0x00, 0x00, 0x00, 0x03, 0x83, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xc1, 
  0x83, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x87, 0xf8, 0x7f, 0xfc, 0x0e, 0x01, 0xff, 0xe1, 
  0x87, 0xf8, 0x01, 0xf8, 0x0e, 0x00, 0x7f, 0xe1, 0x87, 0xf8, 0x01, 0xe0, 0x0e, 0x00, 0x3f, 0xe1, 
  0x87, 0xf8, 0x01, 0xe0, 0xfe, 0x3e, 0x1f, 0xe1, 0x87, 0xf8, 0x01, 0xc1, 0xfe, 0x3f, 0x1f, 0xe1, 
  0x83, 0xf8, 0x7f, 0xc3, 0xfe, 0x3f, 0x8f, 0xe1, 0x83, 0xf8, 0x7f, 0xc3, 0xfe, 0x3f, 0x8f, 0xe1, 
  0x83, 0xf8, 0x7f, 0xc3, 0xfe, 0x3f, 0x8f, 0xc1, 0x43, 0xf8, 0x7f, 0xc1, 0xff, 0x3f, 0x8f, 0xc3, 
  0x43, 0xf8, 0x00, 0xe0, 0xff, 0x1f, 0x1f, 0xc2, 0x43, 0xf8, 0x00, 0xe0, 0x0f, 0x80, 0x1f, 0xc2, 
  0x41, 0xf8, 0x00, 0xf0, 0x0f, 0x80, 0x3f, 0x82, 0x61, 0xf8, 0x00, 0xfc, 0x0f, 0xe0, 0xff, 0x86, 
  0x20, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x84, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 
  0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 
  0x08, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x18, 0x08, 0x01, 0xf8, 0x0c, 0x00, 0x00, 0x00, 0x10, 
  0x04, 0x03, 0x1c, 0x0c, 0x00, 0x00, 0x00, 0x20, 0x06, 0x03, 0x0d, 0xfe, 0xff, 0x73, 0xc0, 0x60, 
  0x02, 0x07, 0x0d, 0x8f, 0xdf, 0xbe, 0x60, 0x40, 0x01, 0x03, 0x0f, 0x0d, 0x8f, 0x9e, 0x60, 0x80, 
  0x01, 0x83, 0x9d, 0x8d, 0x8f, 0x1e, 0x61, 0x80, 0x00, 0xc1, 0xf9, 0xec, 0xff, 0x1b, 0xe3, 0x00, 
  0x00, 0x60, 0x60, 0xc0, 0x60, 0x01, 0x86, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 
  0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 
  0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x70, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x70, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xe0, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  //circunferencia=circunferencia/9.0;
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  Serial.println("Inicializando");
  //drawlogo();
  Serial.begin(9600);
  Serial1.begin(115200); // comunicação com o esp
  pinMode(pinoInterrupt, INPUT);                                      
  attachInterrupt(digitalPinToInterrupt(pinoInterrupt), calculo_vel, FALLING);    
}



void loop(){
  
  if (velocidade_display!=0.00 && ( (millis()-t0)> TEMPOATT2)) { 
    velocidade_display=0.00;
    velocidade=0.00;
    Serial.println(" ");
    Serial.print("----- VEL REAL -----:");
    Serial.println(velocidade_display);
    t1 = millis();
    }

 updt_display();
    
 
}




  
void calculo_vel()      
{

 
      
   
    i_vel++; 
    
    if(i_vel>1)         
    {
        periodo=((millis()-tempo_p0)); //dois imas
        tempo_p0=millis();
        i_vel=0;
        if(periodo!=0.00){
         
          if((periodo>(100.0)) and (periodo<(1000.0)) ){
             comprimento = 3600.0*circunferencia;
             distancia=distancia+(circunferencia);
             
             velocidadeAnt = velocidade;
             velocidade = (comprimento/periodo);

          /*   if (abs(velocidadeAnt - velocidade) >= 10.0){
              velocidade = velocidadeAnt;
              velocidade_display=velocidadeAnt;
             Serial.println(" ");
        Serial.print("----- PERIODO -----:");
        Serial.println(periodo);*/
     
       
             
        Serial.println(" ");
        Serial.print("----- DISPLAY ----- :");
        Serial.println(velocidade_display);
        Serial.println(" ");
        Serial.print("----- VEL REAL -----:");
        Serial.println(velocidade_display);
       
                }
       
                
        }
        
    }
    
     if (velocidade_display!=0.00 && ( (millis()-tempo)> TEMPOATT2)) { 
    velocidade_display=0.00;
   
    tempo=millis();
    }
     
     if((millis()-t0)>TEMPOATT){
        velocidade_display=velocidade;
        serial();
         
    Serial.println(" ");
    Serial.print("----- mensgaem -----:");
    Serial.println(mensagem); 
        t0=millis();
        
        }
                             
}


void updt_display()      
{
  draw(velocidade_display);                         
}



void draw(float dado_velocidade) {
 int exibicao;
exibicao=int(dado_velocidade);
display.clearDisplay();  
display.setTextSize(8.7);
display.setTextColor(WHITE);
display.setCursor(22,7);
display.println(exibicao);
display.display();
}


void drawlogo() {
display.clearDisplay();
display.drawBitmap(32, 0, epd_bitmap_logoazul , 64, 64, 1); // (0,0) posicao no display, (128,64) tam. da img., (conix_64x64) image name.
display.display();
delay(1500);
}

void serial(){
   if(distancia<10.00) { //para manter o padrão de dois dígitos, verificar se isso funciona na hora do esp32 receber
    
    distancia_serial=distancia;
    str_distancia_serial="000"+String(distancia_serial);
    }
   else if(distancia<100.00) { //para manter o padrão de dois dígitos, verificar se isso funciona na hora do esp32 receber
    distancia_serial=distancia;
    str_distancia_serial="00"+String(distancia_serial);
    }
    else if(distancia<1000.00) { //para manter o padrão de dois dígitos, verificar se isso funciona na hora do esp32 receber
    distancia_serial=distancia;
    str_distancia_serial="0"+String(distancia_serial);
    }

    
  if(velocidade_display<10.00) { //para manter o padrão de dois dígitos, verificar se isso funciona na hora do esp32 receber
    velocidade_serial=velocidade_display;
    str_velocidade_serial="0"+String(velocidade_serial);
    }
  
    float potencia_serial=0.00;
    String str_potencia_serial="0"+String(potencia_serial);
    String virgula= ",";
    String string_teste=String(str_velocidade_serial+virgula+str_potencia_serial+virgula+str_distancia_serial);
    mensagem=String(string_teste) ;
    Serial1.print(mensagem); //comunicação com o esp
    Serial1.flush(); 
  
  }
