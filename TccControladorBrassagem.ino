#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <RtcDS3231.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Arduino.h>  // for type definitions

struct EtapaQuente{
  float tempMin = 0;
  float tempMax = 0;
  int duracao = 0;
};

struct Lupulos{
  int duracao = 255;
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
#define SQW_PIN 19 //PINO DE INTERRUPÇÃO RELOGIO 19
#define VALVULA_FUIDO 2 //PINO DE INTERRUPÇÃO VAZAO FLUIDO 2
#define INT_NUMBER_INTERRUPTION 4 // INTERRUPÇÃO DO RELOGIO
#define INT_NUMBER_INTERRUPTION_VAZAO 0 // INTERRUPÇÃO DO SENSOR DE VAZAO DE FLUIDO
#define BUZZER 9
//Porta do pino do rele resistencia
#define RELE_RESISTENCIA 29

//Porta do pino do rele valvula
#define RELE_VALVULA 28

//Botoes
#define BTN_CONFIRMA 13
#define BTN_SOBE 12
#define BTN_DESCE 11
#define BTN_CANCELA 10

// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorTemperature;

//RealTime
RtcDS3231<TwoWire> Rtc(Wire);
RtcDateTime compiled;
RtcDateTime now;
RtcDateTime _inicioFervura;

//lcd
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

//Variaveis Millis
unsigned long _millisAnterior = 0;
unsigned long _intervaloBuzzer = 1000;
unsigned int _tocarBuzzer = 1;
unsigned long _intervaloValvula = 1000;
unsigned int _verificaInterrValvula = 1;

volatile bool intFlag = false; // Variável que indica se hounve interrupção
volatile bool intFlagZero = false; // Variável que indica se hounve interrupção
float variacaoMinima;
boolean alarmeAtivo= false;
boolean alarmeAtivoHop= false;
boolean statusResistencia = true;
boolean targetTemperatura = false;
boolean _lupuloFlameOut = false;
boolean _mostura = false;
boolean _fervura = false;
int _lupuloVez = 0;
int _step = 0;

//Variaveis do controle de água
float _vazao = 0 ; //Variável para armazenar o valor em L/min
int _contaPulso; //Variável para a quantidade de pulsos
float _litros = 0; //Variável para Quantidade de agua
float _miliLitros = 0; //Variavel para Conversão

//Funções de Interrupção----------------------------------------------

void incpulso ()
{ 
  _contaPulso++; //Incrementa a variável de contagem dos pulsos
}

void interrompeu() // Rotina chamada quando houver uma interrupção
{
  intFlag = true;
}

//--------------------------------------------------------------------

#include "receita.h"
#include "metodosSensorTemp.h"
#include "metodosRelogioAlarm.h"
#include "metodosBuzzer.h"
#include "metodosEEPROM.h"
#include "metodosLcd.h"


void setup() { 

  Serial.begin(9600);

  lcd.begin(16, 2);
  
  addVariacaoMinima(0.5);
  

  iniciaSensorTemp();
  iniciarRtcDs3231();

  // Configura interrupção
  attachInterrupt(INT_NUMBER_INTERRUPTION, interrompeu, FALLING);
  attachInterrupt(INT_NUMBER_INTERRUPTION_VAZAO, incpulso, RISING); //Configura o pino 2(Interrupção 0) para trabalhar como interrupção


  // Pullup no pino de interrupção
  pinMode(SQW_PIN, INPUT_PULLUP);
  pinMode(VALVULA_FUIDO, INPUT_PULLUP);
 
  pinMode(BUZZER,OUTPUT);
  pinMode(RELE_RESISTENCIA,OUTPUT);
  pinMode(RELE_VALVULA,OUTPUT);
  digitalWrite(RELE_RESISTENCIA, HIGH);
  digitalWrite(RELE_VALVULA, HIGH);

  pinMode(BTN_CONFIRMA,INPUT_PULLUP);
  pinMode(BTN_CANCELA,INPUT_PULLUP);
  pinMode(BTN_SOBE,INPUT_PULLUP);
  pinMode(BTN_DESCE,INPUT_PULLUP);

  lcd.createChar(0, menuCursorRight);
  lcd.createChar(1, menuCursorLeft);
  lcd.createChar(2, relogioIcon);
  lcd.createChar(3, termometroIcon);
  lcd.createChar(4, grausCelsiosIcon);
  lcd.createChar(5, lupuloIcon);
  lcd.createChar(6, targetIcon);
  

}

void loop() {
  
 
  //addEtapaMostura(receita[0].mostura,1,30,10,variacaoMinima );
  //addEtapaMostura(receita[0].mostura,2,32,10,variacaoMinima);
  //addEtapaFervura(receita[0].fervura,33,120);
  //addLupulo(receita[0].lupulo,1,120);
  //addLupulo(receita[0].lupulo,2,60);
  //addLupulo(receita[0].lupulo,3,0);
  //mostrarTemperatura();
  //mostrarHora();
  //Menu();
  mexerMenu();
    
}

void Menu(){

  
//  Serial.println("|****************************|");
//  Serial.println("|**|Ardnoid Configuration |**|");
//  Serial.println("|**|        Menu          |**|");
//  Serial.println("|****************************|");
//  Serial.println("");
//  Serial.println("Selecione uma opção abaixo:");
//  Serial.println("1 Iniciar Mostura");
//  
//  while(!Serial.available()){}
//  int i = Serial.parseInt();
//
//  while(Serial.available() > 0) {
//    char t = Serial.read();
//  }
//  switch(i){
//
//  case 1:
//    _mostura = true;
//    adicionarAgua();
//    brassagem();
//  case 2:
//    break;
//  case 3:
//    break;
//  default:
//  break;
//    } 
 }

void brassagem(){
  if(_mostura){
    while(_step <= 10){
          etapaMostura(receita[0].mostura,sizeof(receita[0].mostura)/sizeof(EtapaQuente)); 
      }
    
    while(!digitalRead(BTN_CONFIRMA)){
        Serial.println("FIM DE MOSTURA");
        Serial.println("RETIRE OS MALTES");
    
        //tone(BUZZER,1500);
        //delay(1000);
        //noTone(BUZZER);
        //delay(1000);
        tocarBuzzer();
      }

      desligaBuzzer();
      _mostura = false;
      _fervura = true;
      _step = 0;
  }

  if(_fervura){

    while(_step <= 0){
       etapaFervura(receita[0].fervura,receita[0].lupulo,sizeof(receita[0].lupulo)/sizeof(Lupulos)); 
    }
  }

  digitalWrite(RELE_RESISTENCIA, HIGH); //Desliga rele
  if(_lupuloFlameOut){
    adicionarLupuloFervura(_lupuloVez);
    _lupuloFlameOut = false;
  }
  Serial.println("|****************************|");
  Serial.println("|**|BRASSAGEM FINALIZADA |**|");
  Serial.println("|****************************|");
  Serial.println("");
  while(!digitalRead(BTN_CONFIRMA)){
  //digitalWrite(BUZZER, HIGH);digitalWrite(BUZZER, LOW);
 
  tocarBuzzer();
  }
  desligaBuzzer();

  intFlag = false; // Variável que indica se hounve interrupção
  intFlagZero = false; // Variável que indica se hounve interrupção
  variacaoMinima;
  alarmeAtivo= false;
  alarmeAtivoHop= false;
  statusResistencia = true;
  targetTemperatura = false;
  _lupuloFlameOut = false;
  _mostura = false;
  _fervura = false;
  _lupuloVez = 0;
  _step = 0;
  
}

void etapaFervura(EtapaQuente etapa[],Lupulos lupulo[], int tam){
    
    switch(_step){
    
     case 0:

       while(!verificaAlarm()){
          mostrarTemperatura();
          Serial.print("Alvo->");
          Serial.println(etapa[_step].tempMax);
          Serial.print("Gordura->");
          Serial.println(etapa[_step].tempMin);
          controlResistenceFervura(getTemperature(),etapa[_step].tempMin,etapa[_step].tempMax, etapa[_step].duracao);

          if(alarmeAtivo){
            if(!alarmeAtivoHop){
              if(lupulo[_lupuloVez].duracao != 255){
                setBuzzerTimerHop(etapa[_step].duracao, lupulo[_lupuloVez].duracao,_inicioFervura); 
              }  
            }            
          }
       }
       _step = 1;
      
    break;
    
    }

   
}

void etapaMostura(EtapaQuente etapa[],int tam){

  switch(_step){

    case 0:

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Pre-Aquecimento");
      Serial.print("Etapa Pré-Aquecimento");

       while(!targetTemperatura){
          lcd.setCursor(0,1);
          mostrarTemperatura();
          lcd.setCursor(9,1);
          mostrarTempAlvo(etapa[_step].tempMax);
          Serial.print("Alvo->");
          Serial.println(etapa[_step].tempMax);
          Serial.print("Gordura->");
          Serial.println(etapa[_step].tempMin);
          controlResistenceTempInicial(getTemperature(),etapa[_step].tempMin,etapa[_step].tempMax, etapa[_step].duracao);
       }
       targetTemperatura = false;
       _step++;
      
    break;
    
    case 1:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");

      //controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
       }
      _step++;
      alarmeAtivo= false;
    break;
    
    case 2:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
      //controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;

    case 3:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
      //controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;
    
    case 4:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
      //controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;
    
    case 5:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
      //controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;
    
    case 6:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
      //controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;
    
    case 7:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");

      //controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;
    
    case 8:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");

     // controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;
    
    case 9:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");
      
     // controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;
    
    case 10:
      Serial.print("Etapa ");Serial.print(_step); Serial.println("->");

     // controlResistence(getTemperature(),etapa[_step-1].tempMin,etapa[_step-1].tempMax, etapa[_step-1].duracao);
      while(!verificaAlarm()){
          mash(etapa,_step);
      }
      _step++;
      alarmeAtivo= false;
    break;
    
  }
  
}



void controlResistence(float tempSensor, float tempMin, float tempMax, int duracao ){
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

void controlResistenceTempInicial(float tempSensor, float tempMin, float tempMax, int duracao ){
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
        
          adicionarMaltesMostura();

          targetTemperatura =  true;
          setBuzzerTimer(duracao);
      }
  }
}

void controlResistenceFervura(float tempSensor, float tempMin, float tempMax, int duracao ){
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
          digitalWrite(RELE_RESISTENCIA, LOW); //Liga rele
        
        if(!alarmeAtivo){
         setBuzzerTimer(duracao); 
        }
        
      }
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

void adicionarMaltesMostura(){

     while(!digitalRead(BTN_CONFIRMA)){
      
      Serial.println("Adicione os Maltes");

      tocarBuzzer();
      //tone(BUZZER,1500);
      //delay(1000);
      //noTone(BUZZER);
      //delay(1000);
     }
     desligaBuzzer();
}

void adicionarLupuloFervura(int posicao){

    while(!digitalRead(BTN_CONFIRMA)){
        Serial.print("ADICIONE LUPULO MINUTO->");
        Serial.println(posicao);

       tocarBuzzer();
        //tone(BUZZER,1500);
        //delay(1000);
        //noTone(BUZZER);
        //delay(1000);
      }
      desligaBuzzer();
      _lupuloVez++;
}


void adicionarAgua(){

  float volumeAgua = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ADD VOL AGUA");
  delay(2000);
  
  Serial.println("ADICIONE QTD DE AGUA->");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ADD VOL AGUA");
  while(!digitalRead(BTN_CONFIRMA)){
        
      if(digitalRead(BTN_SOBE)){
        volumeAgua = volumeAgua + 0.1;
      }
      if(digitalRead(BTN_DESCE)){
        volumeAgua = volumeAgua - 0.1;
        if(volumeAgua<0){
          volumeAgua=0;
        }
      }
     
     lcd.setCursor(0,1);
     lcd.print("VOLUME->");   
     lcd.print(volumeAgua);
     Serial.print("VOLUME->");   
     Serial.println(volumeAgua);
     delay(100);
   }
   
   if(volumeAgua==0){
    return;
   }
   
   digitalWrite(RELE_VALVULA, LOW);
     
   while(_litros <= volumeAgua && !digitalRead(BTN_CANCELA)){
    _contaPulso = 0;   //Zera a variável para contar os giros por segundos

    unsigned long _millisAtual = millis();
  
    if (_millisAtual - _millisAnterior > _intervaloValvula){
       if (_verificaInterrValvula == 1){
           sei();
           _verificaInterrValvula = 0;
        }
       else{
           cli();
           _verificaInterrValvula = 1;
        }
       _millisAnterior = _millisAtual;
    } 
      
//    sei();      //Habilita interrupção
//    delay (1000); //Aguarda 1 segundo
//    cli();      //Desabilita interrupção
  
     _vazao = _contaPulso / 7.5; //Converte para L/min
     _miliLitros = _vazao / 60;
    _litros = _litros + _miliLitros;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enchendo:");
    lcd.setCursor(2,1);
    lcd.print(_litros);
    lcd.print("L");
    Serial.print(_litros);
    Serial.println("L ");
    Serial.println(digitalRead(BTN_CANCELA));
   }
   
   digitalWrite(RELE_VALVULA, HIGH);
}
