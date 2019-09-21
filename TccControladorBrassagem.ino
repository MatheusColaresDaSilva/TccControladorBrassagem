#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <RtcDS3231.h>

struct Receita {
  float etapMostura1[3]; //{TempMin,TempMax,Time}
  float etapMostura2[3]; //{TempMin,TempMax,Time}
  float etapMostura3[3]; //{TempMin,TempMax,Time}
  float etapMostura4[3]; //{TempMin,TempMax,Time}
  float etapMostura5[3]; //{TempMin,TempMax,Time}
  float etapMostura6[3]; //{TempMin,TempMax,Time}
  float etapMostura7[3]; //{TempMin,TempMax,Time}
  float etapMostura8[3]; //{TempMin,TempMax,Time}
  float etapMostura9[3]; //{TempMin,TempMax,Time}
  float etapMostura10[3]; //{TempMin,TempMax,Time}

  byte etapaFervura[2]; //{Temp,Time}

  byte lupulo1[1]; //{Time}
  byte lupulo2[1]; //{Time}
  byte lupulo3[1]; //{Time}
  byte lupulo4[1]; //{Time}
  byte lupulo5[1]; //{Time}
  byte lupulo6[1]; //{Time}
  byte lupulo7[1]; //{Time}
  byte lupulo8[1]; //{Time}
  byte lupulo9[1]; //{Time}
  byte lupulo10[1]; //{Time}
  
};

// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 22

// Porta do pino de sinal do RTCDS3231
#define SQW_PIN 19 // UNO 2. No Mega, mude para 19
#define INT_NUMBER_INTERRUPTION 4 // UNO 0. No Mega, mude para 4
#define BUZZER 7

// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorTemperature;

//RealTime
RtcDS3231<TwoWire> Rtc(Wire);
RtcDateTime compiled;
RtcDateTime now;

volatile bool intFlag = false; // Variável que indica se hounve interrupção
boolean a=true;
boolean b=false;
float variacaoMinima;

void setup() { 

  Serial.begin(9600);
  

  iniciaSensorTemp();
  iniciarRtcDs3231();

  // Configura interrupção
  attachInterrupt(INT_NUMBER_INTERRUPTION, interrompeu, FALLING);

  // Pullup no pino de interrupção
  pinMode(SQW_PIN, INPUT_PULLUP);
  pinMode(BUZZER,OUTPUT);
}

void loop() {

  Serial.print("Temperatura->");
  Serial.println(getTemperature());
  
  addVariacaoMinima(0.5);
  
  struct Receita receita[10];
 
  addEtapaMostura(receita[0].etapMostura1,68,60);
  addEtapaMostura(receita[0].etapMostura2,75,10);
  addEtapaFervura(receita[0].etapaFervura,96,60);
  addLupulo(receita[0].lupulo1,10);
  addLupulo(receita[0].lupulo2,20);
  
  addEtapaMostura(receita[1].etapMostura1,65,60);
  addEtapaMostura(receita[1].etapMostura2,75,10);
  addEtapaFervura(receita[1].etapaFervura,96,60);
  addLupulo(receita[1].lupulo1,10);
  addLupulo(receita[1].lupulo2,20);

  if(a){
    a=false;
    b=true;
    setBuzzerTimer(receita[0].lupulo1);  
  }
  

  RtcDateTime now = Rtc.GetDateTime();
  /*Serial.print(formatDate(now,"d/m/y") + " ");
  Serial.println(formatTime(now,"h:m:s"));
  Serial.println();*/

  if (intFlag)
  {
    intFlag = false; // Reseta flag
    DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags(); //Variável para testar qual alarme disparou
    if (flag & DS3231AlarmFlag_Alarm1)
    {
      Serial.println(">>> Alarm 1 disparou <<<");

      if(b){
        b=false;
         setBuzzerTimer(receita[0].lupulo2);
      }
      digitalWrite(BUZZER, HIGH);
      delay(1000);
      digitalWrite(BUZZER, LOW);
       
    }
  }
  
  int length = sizeof(receita[0].etapMostura1)/sizeof(float);
  etapaMostura(receita[0].etapMostura1,sizeof(receita[0].etapMostura1)/sizeof(float));
}

void addEtapaMostura(float etapa[], float tempMax, float tempo){
	
  etapa[0] = tempMax - variacaoMinima;
  etapa[1] = tempMax;
  etapa[2] = tempo;
  
}

void addEtapaFervura(byte etapa[], byte temp, byte tempo){
  
  etapa[0] = temp;
  etapa[1] = tempo;
  
}

void addLupulo(byte hop[], byte tempo){
  
  hop[0] = tempo;
     
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

void setBuzzerTimer(byte valor[]){

 RtcDateTime now = Rtc.GetDateTime();
 RtcDateTime alarmTime = now + valor[0];

  DS3231AlarmOne alarm(
    alarmTime.Day(),
    alarmTime.Hour(),
    alarmTime.Minute(),
    alarmTime.Second(),
    DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(alarm);

  // Efetiva os alarmes
  Rtc.LatchAlarmsTriggeredFlags();

  /*Serial.print("Hora do agora ");
  Serial.print(formatDate(now,"d/m/y") + " ");
  Serial.print(formatTime(now,"h:m:s"));
  Serial.print(" + ");
  Serial.print(valor[0]);
  Serial.print("seg");
  Serial.println();
  Serial.print("Hora do alarme ");
  Serial.print("--> ");
  Serial.print(formatDate(alarmTime,"d/m/y") + " ");
  Serial.print(formatTime(alarmTime,"h:m:s"));
  Serial.println();*/

}

void etapaMostura(float etapa[], byte sizeOf){

  for(int i = 0; i < sizeOf ;i++){

    Serial.print("nois: ->");
    Serial.println(etapa[i]);
  }
}
