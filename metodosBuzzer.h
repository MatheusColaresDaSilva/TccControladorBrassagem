void setBuzzerTimer(byte valor){

 RtcDateTime now = getHora();
 _inicioFervura = getHora();
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

  if(now == alarmTime){
    intFlagZero = true;
  }
  else{
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
  
}

void setBuzzerTimerHop(byte tempoFervura, byte tempoLupulo, RtcDateTime inicioFervura){

 byte valor = tempoFervura - tempoLupulo;

 if(valor == 0){
  adicionarLupuloFervura(_lupuloVez);
  return;
 }
 
 RtcDateTime alarmTime = inicioFervura + valor;
  
  DS3231AlarmTwo alarm2(
    alarmTime.Day(),
    alarmTime.Hour(),
    alarmTime.Minute(),
    DS3231AlarmTwoControl_HoursMinutesDayOfMonthMatch );
  Rtc.SetAlarmTwo(alarm2);

  // Efetiva os alarmes
  Rtc.LatchAlarmsTriggeredFlags();
  alarmeAtivoHop = true;

//  Serial.print("Hora do alarme Lupulo");
//  Serial.print("--> ");
//  Serial.print(formatDate(alarmTime,"d/m/y") + " ");
//  Serial.print(formatTime(alarmTime,"h:m:s"));
//  Serial.println(); 
 
  
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

    if (flag & DS3231AlarmFlag_Alarm2)
    {
        adicionarLupuloFervura(_lupuloVez);
        alarmeAtivoHop = false;
    }

    
    if(intFlagZero){
      intFlagZero = false;
      return true;
    }
  }
  return false;
}
