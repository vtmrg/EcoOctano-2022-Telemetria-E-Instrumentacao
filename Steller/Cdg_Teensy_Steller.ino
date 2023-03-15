


#include <SPI.h>  
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define TEMPOATT 80      
#define TEMPOATT2 1000      
#define TEMPOATT3 200     
int j;
const int OLED_RESET=-1, pinoInterrupt = 3, pinoBotao=10;
const float circunferencia=1.445;
const byte SCREEN_WIDTH = 128, SCREEN_HEIGHT = 64;         
int i_vel=0;
float t0=0, t1=0 ,tempo_p0 =0,comprimento=0, periodo=0, velocidadeAnt=0,velocidade_display=0,velocidade =0,distancia=0,distanciaAnt=0, 
                             velocidade_serial=0,potencia_serial=0, distancia_serial=0;

String mensagem,str_velocidade_serial, str_distancia_serial, str_potencia_serial;
 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
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
    serial();
    Serial.println(" ");
    Serial.print("----- zerou -----:");
    Serial.println(" ");
    Serial.print("----- VEL REAL -----:");
    Serial.println(velocidade_display);
    t0 = millis();
    
    }

  draw(velocidade_display);  

 
}




  
void calculo_vel()      
{

 
      
   
    i_vel++; 
    
    if(i_vel>1)         
    {
        periodo=((millis()-tempo_p0)); 
        tempo_p0=millis();
        i_vel=0;
        if(periodo!=0.00){
         
          if((periodo>(100.0)) and (periodo<(1000.0)) ){
             comprimento = 3600.0*circunferencia;
             distancia=distancia+(circunferencia);
             
             velocidadeAnt = velocidade;
             velocidade = (comprimento/periodo);
             
        Serial.println(" ");
        Serial.print("----- DISPLAY ----- :");
        Serial.println(velocidade_display);
        Serial.println(" ");
        Serial.print("----- VEL REAL -----:");
        Serial.println(velocidade);
       
                }
       
                
        }
        
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
    else{
      distancia_serial=distancia;
      str_distancia_serial=String(distancia_serial);
      }

    
  if(velocidade_display<10.00) { //para manter o padrão de dois dígitos, verificar se isso funciona na hora do esp32 receber
    velocidade_serial=velocidade_display;
    str_velocidade_serial="0"+String(velocidade_serial);
    }
   else {
    velocidade_serial=velocidade_display;
    str_velocidade_serial=String(velocidade_serial);
    
    }
  
    float potencia_serial=0.00;
    String str_potencia_serial="0"+String(potencia_serial);
    String virgula= ",";
    String string_teste=String(str_velocidade_serial+virgula+str_potencia_serial+virgula+str_distancia_serial);
    mensagem=String(string_teste) ;
    Serial1.print(mensagem); //comunicação com o esp
    Serial1.flush(); 
  
  }


 
