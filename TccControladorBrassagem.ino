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
  
  Lupulos lupulo[10]; //{Time}
  
};

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
float variacaoMinima;

boolean alarmeAtivo= false;
boolean statusResistencia = true;


int _step = 1;

void setup() { 

  Serial.begin(9600);
  addVariacaoMinima(1.5);
  

  iniciaSensorTemp();
  iniciarRtcDs3231();

  // Configura interrupção
  attachInterrupt(INT_NUMBER_INTERRUPTION, interrompeu, FALLING);

  // Pullup no pino de interrupção
  pinMode(SQW_PIN, INPUT_PULLUP);
  pinMode(BUZZER,OUTPUT);
  pinMode(RELE_RESISTENCIA,OUTPUT);
}

void loop() {

  Serial.print("Temperatura->");
  Serial.println(getTemperature());
    
  struct Receita receita[10];
 
  addEtapaMostura(receita[0].mostura,1,30,60);
  addEtapaMostura(receita[0].mostura,2,35,10);
  addEtapaFervura(receita[0].fervura,96,60);
  addLupulo(receita[0].lupulo,1,10);
  addLupulo(receita[0].lupulo,2,20);

 //Serial.println("Dados Receita:");
 //Serial.print("Etapa1:");Serial.print(receita[0].mostura[0].tempMin);Serial.print(",");Serial.print(receita[0].mostura[0].tempMax);Serial.print(",");Serial.println(receita[0].mostura[0].duracao);
  
 /*addEtapaMostura(receita[1].mostura,1,65,60);
 addEtapaMostura(receita[1].mostura,2,75,10);
 addEtapaFervura(receita[1].fervura,96,60);
 addLupulo(receita[1].lupulo,1,10);
 addLupulo(receita[1].lupulo,2,20);*/

  RtcDateTime now = Rtc.GetDateTime();
  /*Serial.print(formatDate(now,"d/m/y") + " ");
  Serial.println(formatTime(now,"h:m:s"));
  Serial.println();*/

 verificaAlarm();

 if(_step<3){
 etapaMostura(receita[0].mostura,sizeof(receita[0].mostura)/sizeof(EtapaQuente)); 
 }
 else{
  Serial.println("Brassagem Finalizada");
 }
 
}

void addEtapaMostura(EtapaQuente etapa[],int posicao, float tempMax, byte tempo){
  
  etapa[posicao-1].tempMin = tempMax - variacaoMinima;
  etapa[posicao-1].tempMax = tempMax;
  etapa[posicao-1].duracao = tempo;
  
}

void addEtapaFervura(EtapaQuente etapa[], float tempMax, float tempo){
  
  etapa[0].tempMin = tempMax;
  etapa[0].tempMax = tempMax;
  etapa[0].duracao = tempo;
  
}

void addLupulo(Lupulos hop[],int posicao, byte tempo){
  
  hop[posicao-1].duracao = tempo;
     
}

void addVariacaoMinima(float valor){
  variacaoMinima = valor;
}


void iniciaSensorTemp(){
  sensors.begin();
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensorTemperature, 0)) 
  Serial.println("Sensores nao encontrados !");
}

float getTemperature(){

  sensors.requestTemperatures();
  return sensors.getTempC(sensorTemperature);
}




void interrompeu() // Rotina chamada quando houver uma interrupção
{
  intFlag = true;
}

// Funções para formatação dos dados
String formatDate(const RtcDateTime& dt, String format) 
{
  String d = dt.Day() < 10 ? "0" + String(dt.Day()) : String(dt.Day()) ; 
  String m = dt.Month() < 10 ? "0" + String(dt.Month()) : String(dt.Month()) ;
  String y = String(dt.Year()) ;
  format.replace("d",d);
  format.replace("m",m);
  format.replace("y",y);
  return format;
}

String formatTime(const RtcDateTime& dt, String format)
{
  String h = dt.Hour() < 10 ? "0" + String(dt.Hour()) : String(dt.Hour()) ;
  String m = dt.Minute() < 10 ? "0" + String(dt.Minute()) : String(dt.Minute()) ;
  String s = dt.Second() < 10 ? "0" + String(dt.Second()) : String(dt.Second()) ;
  format.replace("h",h);
  format.replace("m",m);
  format.replace("s",s);
  return format;
}

void iniciarRtcDs3231(){

  // Configuração inicial do RTC
  Rtc.Begin();
  compiled = RtcDateTime(__DATE__, __TIME__);
  if (!Rtc.IsDateTimeValid())
  {
    Rtc.SetDateTime(compiled);
  }
  if (!Rtc.GetIsRunning())
  {
    Rtc.SetIsRunning(true);
  }
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Rtc.SetDateTime(compiled);
  }
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmOne); //Habilta 2 alarmes
}

void setBuzzerTimer(byte valor){

 RtcDateTime now = Rtc.GetDateTime();
 RtcDateTime alarmTime = now + valor;

  DS3231AlarmOne alarm(
    alarmTime.Day(),
    alarmTime.Hour(),
    alarmTime.Minute(),
    alarmTime.Second(),
    DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(alarm);

  // Efetiva os alarmes
  Rtc.LatchAlarmsTriggeredFlags();
  alarmeAtivo = true;

  
  Serial.print("Hora do agora ");
  Serial.print(formatDate(now,"d/m/y") + " ");
  Serial.print(formatTime(now,"h:m:s"));
  Serial.print(" + ");
  Serial.print(valor);
  Serial.print("seg");
  Serial.println();
  Serial.print("Hora do alarme ");
  Serial.print("--> ");
  Serial.print(formatDate(alarmTime,"d/m/y") + " ");
  Serial.print(formatTime(alarmTime,"h:m:s"));
  Serial.println();

}

boolean verificaAlarm(){
  if (intFlag){
    intFlag = false; // Reseta flag
    DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags(); //Variável para testar qual alarme disparou
    if (flag & DS3231AlarmFlag_Alarm1)
    {
      digitalWrite(BUZZER, HIGH);
      delay(1000);
      digitalWrite(BUZZER, LOW);
      return true;
    }
  }
  return false;
}

void etapaMostura(EtapaQuente etapa[],int tam){

  switch(_step){
    case 1:
      Serial.println("Etapa 1->");
      
       while(!verificaAlarm()){
        Serial.print("Temperatura->");
        Serial.println(getTemperature());
        Serial.print("Alvo->");
        Serial.println(etapa[0].tempMax);
        Serial.print("Gordura->");
        Serial.println(etapa[0].tempMin);
        
        controlResistence(getTemperature(),etapa[0].tempMin,etapa[0].tempMax, etapa[0].duracao);
       }
       _step++;
       alarmeAtivo= false;
    break;
    
    case 2:
      Serial.println("Etapa 2->");
      
       while(!verificaAlarm()){
        Serial.print("Temperatura->");
        Serial.println(getTemperature());
        Serial.print("Alvo->");
        Serial.println(etapa[1].tempMax);
        Serial.print("Gordura->");
        Serial.println(etapa[1].tempMin);
        
        controlResistence(getTemperature(),etapa[1].tempMin,etapa[1].tempMax, etapa[1].duracao);
       }
       _step++;
       alarmeAtivo= false;
    break;
  }
 
  
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
