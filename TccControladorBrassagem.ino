

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <RtcDS3231.h>

struct EtapaQuente{
  float tempMin = 0;
  float tempMax = 0;
  byte duracao = 0;
};

struct Lupulos{
  byte duracao = 0;
};

struct Receita {

  EtapaQuente mostura[10];
  EtapaQuente fervura[1];
  
  Lupulos lupulo[10];
};

struct Receita receita[10];

// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 22

// Porta do pino de sinal do RTCDS3231
#define SQW_PIN 19 // UNO 2. No Mega, mude para 19
#define INT_NUMBER_INTERRUPTION 4 // UNO 0. No Mega, mude para 4
#define BUZZER 7
//Porta do pino do rele resistencia
#define RELE_RESISTENCIA 6

// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorTemperature;

//RealTime
RtcDS3231<TwoWire> Rtc(Wire);
RtcDateTime compiled;
RtcDateTime now;

volatile bool intFlag = false; // Variável que indica se hounve interrupção
volatile bool intFlagZero = false; // Variável que indica se hounve interrupção
float variacaoMinima;

boolean alarmeAtivo= false;
boolean statusResistencia = true;
boolean _mostura = false;
boolean _fervura = false;


int _step = 1;

void interrompeu() // Rotina chamada quando houver uma interrupção
{
  intFlag = true;
}

#include "receita.h"
#include "metodosSensorTemp.h"
#include "metodosRelogioAlarm.h"
#include "metodosBuzzer.h"

void setup() { 

  Serial.begin(9600);
  addVariacaoMinima(0.5);
  

  iniciaSensorTemp();
  iniciarRtcDs3231();

  // Configura interrupção
  attachInterrupt(INT_NUMBER_INTERRUPTION, interrompeu, FALLING);

  // Pullup no pino de interrupção
  pinMode(SQW_PIN, INPUT_PULLUP);
  pinMode(BUZZER,OUTPUT);
  pinMode(RELE_RESISTENCIA,OUTPUT);
  digitalWrite(RELE_RESISTENCIA, HIGH);
}

void loop() {
  
 
  addEtapaMostura(receita[0].mostura,1,30,2,variacaoMinima );
  addEtapaMostura(receita[0].mostura,2,32,2,variacaoMinima);
  addEtapaFervura(receita[0].fervura,96,60);
  addLupulo(receita[0].lupulo,1,10);
  addLupulo(receita[0].lupulo,2,20);
  //Serial.print("euheuehuehu->>");
  //Serial.println(receita[2].mostura[2].tempMax);
  mostrarTemperatura();
  mostrarHora();
  Menu();

    



}

void controlResistence(float tempSensor, float tempMin, float tempMax, byte duracao ){
  if(tempSensor < tempMax && statusResistencia){
    digitalWrite(RELE_RESISTENCIA, LOW); //Liga rele
  }
  else{
    statusResistencia = false;
      if(tempSensor < tempMin && !statusResistencia){
        digitalWrite(RELE_RESISTENCIA, LOW); //Liga rele
        statusResistencia = true;
      }
      else{
          digitalWrite(RELE_RESISTENCIA, HIGH); //Desliga rele
        
        if(!alarmeAtivo){
         setBuzzerTimer(duracao); 
        }
        
      }
  }
}

void Menu(){

  Serial.println("|****************************|");
  Serial.println("|**|Ardnoid Configuration |**|");
  Serial.println("|**|        Menu          |**|");
  Serial.println("|****************************|");
  Serial.println("");
  Serial.println("Selecione uma opção abaixo:");
  Serial.println("1 Iniciar Mostura");
  
  while(!Serial.available()){}
  int i = Serial.parseInt();

  switch(i){

  case 1:
    _mostura = true;
    brassagem();
  case 2:
    break;
  case 3:
    break;
  default:
  break;
    } 
 }

void brassagem(){
  if(_mostura){
    while(_step <= 10){
          etapaMostura(receita[0].mostura,sizeof(receita[0].mostura)/sizeof(EtapaQuente)); 
      }
      
    Serial.println("Brassagem Finalizada");
  }

  if(_fervura){
    
  }
}

void etapaMostura(EtapaQuente etapa[],int tam){

  switch(_step){
    case 1:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
    while(!verificaAlarm()){
          mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 2:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;

    case 3:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 4:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 5:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 6:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 7:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 8:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 9:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 10:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
       while(!verificaAlarm()){
           mash(etapa,_step);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
  }
  
}

void mash(EtapaQuente etapa[],int i){
        mostrarTemperatura();
        Serial.print("Alvo->");
        Serial.println(etapa[i-1].tempMax);
        Serial.print("Gordura->");
        Serial.println(etapa[i-1].tempMin);
        
        controlResistence(getTemperature(),etapa[i-1].tempMin,etapa[i-1].tempMax, etapa[i-1].duracao);   
}
