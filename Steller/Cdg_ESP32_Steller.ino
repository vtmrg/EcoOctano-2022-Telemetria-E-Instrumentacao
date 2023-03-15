// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. 



#include <Arduino.h>
#include <WiFi.h>
#include "Esp32MQTTClient.h"
#include "AzureIotHub.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"


#define RXD 16     //rx2
#define TXD 17     //referente ao tx2
#define INTERVAL 800
#define DEVICE_ID "myDeviceId"
#define MESSAGE_MAX_LEN 256

//CONFIGURAÇÕES WI-FI

const char* ssid     = "eco2022";
const char* password = "12345678"; 

//const char* ssid     = "TP-LINK_F15428";
//const char* password = "ELT_ECO_2022"; 


// para a conta do Dante --> "HostName=EcoDante.azure-devices.net;DeviceId=myDeviceId;SharedAccessKey=SLcv3U66cQ4oCe2Q92wagxHXFdjxMtdND6BGYRqMigY="

//para a conta da Bianca -->"HostName=Telemetria2022.azure-devices.net;DeviceId=myDeviceId;SharedAccessKey=I1Cs+Bbob8DPs/NX6OUaKkiPwSVGRtfBibmOQlF7JTw="

static const char* connectionString = "HostName=Telemetria2022.azure-devices.net;DeviceId=myDeviceId;SharedAccessKey=I1Cs+Bbob8DPs/NX6OUaKkiPwSVGRtfBibmOQlF7JTw=";


const char *messageData = "{\"deviceId\":\"%s\", \"messageId\":%d, \"velocidade\":%f, \"potencia\":%f, \"distancia\":%f}";


static bool hasWifi = false;
static bool messageSending = true;
static uint64_t send_interval_ms;


float velocidade_f, potencia_f, distancia_f;
String str,virgula= ",", mensagem, velocidade_str, potencia_str, distancia_str;
int i=0,messageCount = 1, k=0;
char a = ',',string1[20], messagePayload[MESSAGE_MAX_LEN];
const char *velo, *pot, *dist;




void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD, TXD);   //incialzia o serial de comunicação com o arduino
  Serial2.setTimeout(100); 
  Serial.println("inicializado");  //mensagem de que foi inicializado 
  Serial.println("ESP32 Device");
  
  // Initialize the WiFi module
  Serial.println(" > WiFi");
  hasWifi = false;
  InitWifi();
  if (!hasWifi)
  {
    return;
  }

  Serial.println(" > IoT Hub");
  Esp32MQTTClient_SetOption(OPTION_MINI_SOLUTION_NAME, "GetStarted");
  Esp32MQTTClient_Init((const uint8_t*)connectionString, true);

  Esp32MQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);
  Esp32MQTTClient_SetMessageCallback(MessageCallback);
  Esp32MQTTClient_SetDeviceTwinCallback(DeviceTwinCallback);
  Esp32MQTTClient_SetDeviceMethodCallback(DeviceMethodCallback);
  
  sdcheck();
  
  send_interval_ms = millis();
}

void loop()
{
  
  if(Serial2.available())
  {
      Serial2.readBytes(string1,19); 
      Serial.print("string1:");
      Serial.println(string1); 
      
      velocidade_str=slice_str_( string1 ,0, 5) ;
      velo=velocidade_str.c_str();

      potencia_str=slice_str_( string1 ,6, 11) ;
      pot=potencia_str.c_str();

      distancia_str=slice_str_( string1 ,12 , 19) ;
      dist=distancia_str.c_str();


  
      velocidade_f= atof(velo);
      potencia_f= atof(pot);
      distancia_f= atof(dist);
   }

  
  if (hasWifi)
  {
    if (messageSending && 
        (int)(millis() - send_interval_ms) >= INTERVAL)
    {
        Serial.print("----- velocidade_str -----:");
        Serial.println(velocidade_str);
        Serial.println(" ");
        Serial.print("----- distancia_str -----:");
        Serial.println(distancia_str);
        Serial.println(" ");
        Serial.print("----- potencia_str -----:");
        Serial.println(potencia_str);
        Serial.println(" ");
        Serial.print("----- velocidade_f ----- :");
        Serial.println(velocidade_f);
        Serial.println(" ");
        Serial.print("----- potencia_f -----:");
        Serial.println(potencia_f);
        Serial.println(" ");
        Serial.print("----- distancia_f -----:");
        Serial.println(distancia_f);
        Serial.println(" ");
      
        
       
              str=messageCount;
              appendFile(SD, "/Steller.txt", str);
              str=".: ";
              appendFile(SD, "/Steller.txt", str);

        
      
              str=velocidade_f;
              appendFile(SD, "/Steller.txt", str);
              appendFile(SD, "/Steller.txt", virgula);
        
              str=potencia_f;
              appendFile(SD, "/Steller.txt", str);
              appendFile(SD, "/Steller.txt", virgula);


              str=distancia_f;
              appendFile(SD, "/Steller.txt", str);
              appendFile(SD, "/Steller.txt", virgula);

              str=".: ";
              appendFile(SD, "/Steller.txt", str);
              

     



        

      
      snprintf(messagePayload,MESSAGE_MAX_LEN, messageData, DEVICE_ID, messageCount++,velocidade_f,potencia_f, distancia_f);
      Serial.println(messagePayload);
      EVENT_INSTANCE* message = Esp32MQTTClient_Event_Generate(messagePayload, MESSAGE);
      Esp32MQTTClient_Event_AddProp(message, "tensaoAlert", "true");
      Esp32MQTTClient_SendEventInstance(message);
      send_interval_ms = millis();
    }
    else
    {
      Esp32MQTTClient_Check();
    }

  }
  delay(100);
  }
  









//FUNCOES



String slice_str_( String dado,  int n1, int n2 ) {
  
   String sub_string = (dado.substring(n1,n2));  

   return sub_string;
   
  }



static void InitWifi()
{
  Serial.println("Connecting...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  hasWifi = true;
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result)
{
  if (result == IOTHUB_CLIENT_CONFIRMATION_OK)
  {
    Serial.println("Send Confirmation Callback finished.");
  }
}

static void MessageCallback(const char* payLoad, int size)
{
  Serial.println("Message callback:");
  Serial.println(payLoad);
}

static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, int size)
{
  char *tens = (char *)malloc(size + 1);
  if (tens == NULL)
  {
    return;
  }
  memcpy(tens, payLoad, size);
  tens[size] = '\0';
  // Display Twin message.
  Serial.println(tens);
  free(tens);
}

static int  DeviceMethodCallback(const char *methodName, const unsigned char *payload, int size, unsigned char **response, int *response_size)
{
  LogInfo("Try to invoke method %s", methodName);
  const char *responseMessage = "\"Successfully invoke device method\"";
  int result = 200;

  if (strcmp(methodName, "start") == 0)
  {
    LogInfo("Start sending tensao and humidity data");
    messageSending = true;
  }
  else if (strcmp(methodName, "stop") == 0)
  {
    LogInfo("Stop sending tensao and humidity data");
    messageSending = false;
  }
  else
  {
    LogInfo("No method %s found", methodName);
    responseMessage = "\"No method found\"";
    result = 404;
  }

  *response_size = strlen(responseMessage) + 1;
  *response = (unsigned char *)strdup(responseMessage);

  return result;
}



void appendFile(fs::FS &fs, const char * path, String message){
    Serial.printf("Adicionando ao arquivo: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Falha em abrir o arquivo para adicionar o dado.");
        return;
    }
   if(file.print(message)){
        Serial.println("Dado adicionado.");
    } else {
        Serial.println("Dado não foi adicionado.");
    }
    file.close();
}

void sdcheck () {
     if(!SD.begin()){
        Serial.println("Inicialização do cartão falhou.");
        return;
    }
    
   uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("Nenhum cartão inserido.");
        return;
    }
  
  
  }
