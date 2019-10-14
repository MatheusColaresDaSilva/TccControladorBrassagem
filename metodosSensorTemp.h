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

void mostrarTemperatura(){
  
  Serial.print("Temperatura->");
  Serial.println(getTemperature());
  lcd.write(byte(3));
  lcd.print(" ");
  lcd.print(getTemperature(),1);
  lcd.write(byte(4));
}

void mostrarTempAlvo(float temp){
  
  Serial.print("Alvo->");
  Serial.println(temp);
  lcd.write(byte(6));
  lcd.print(" ");
  lcd.print(temp,1);
  lcd.write(byte(4));
}
