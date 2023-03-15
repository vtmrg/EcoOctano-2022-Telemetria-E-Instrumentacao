// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. 
#include <Arduino.h>
#include <WiFi.h>
#include "Esp32MQTTClient.h"
#include "AzureIotHub.h"
//#include "openssl/ssl.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"


#define RXD 16     //rx2
#define TXD 17     //referente ao tx2
#define INTERVAL 800
#define DEVICE_ID "myDeviceId"
#define MESSAGE_MAX_LEN 256

// Please input the SSID and password of WiFi
/*const char* ssid     = "wifiVivivi";
const char* password = "gatodebotinas"; */


const char* ssid     = "TP-LINK_F15428";
const char* password = "ELT_ECO_2022"; 

//const char* ssid     = "Galaxy A51DC8B";
//const char* password = "vowv1285"; 

String str,virgula= ",", mensagem;
int i=0;
char a = ',',string1[200], messagePayload[MESSAGE_MAX_LEN];


/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessKey=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
static const char* connectionString = "HostName=Telemetria2022.azure-devices.net;DeviceId=myDeviceId;SharedAccessKey=I1Cs+Bbob8DPs/NX6OUaKkiPwSVGRtfBibmOQlF7JTw=";


//const char *messageData = "{\"deviceId\":\"%s\", \"messageId\":%d, \"temperatura\":%f, \"velocidade\":%d, \"corrente\":%f, \"tensao\":%f}";

const char *messageData = "{\"deviceId\":\"%s\", \"messageId\":%d, \"velocidade\":%d, \"potencia\":%f}";

int messageCount = 1;
static bool hasWifi = false;
static bool messageSending = true;
static uint64_t send_interval_ms;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utilities


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino sketch


void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD, TXD);   //incialzia o serial de comunicação com o arduino
  Serial2.setTimeout(100); 
  //Porta= Serial(115200, SERIAL_8N1, RXD, TXD)
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
   Serial2.readBytes(string1,2);
   Serial.print("string1:");
   Serial.println(string1);
   }

  
  if (hasWifi)
  {
    if (messageSending && 
        (int)(millis() - send_interval_ms) >= INTERVAL)
    {
      int velocidade_int= atoi(string1);
      //String potencia=string2;
      float  potencia_float=0;
      str=velocidade_int;
      appendFile(SD, "/vel_potencia.txt", str);
      appendFile(SD, "/vel_potencia.txt", virgula);
      str=potencia_float;
      appendFile(SD, "/vel_potencia.txt", str);
      appendFile(SD, "/vel_potencia.txt", virgula);
      Serial.print("vet:");
      Serial.println(velocidade_int);
      snprintf(messagePayload,MESSAGE_MAX_LEN, messageData, DEVICE_ID, messageCount++,velocidade_int,potencia_float);
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
  delay(10);
  }
  









//FUNCOES





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
