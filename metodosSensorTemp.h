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
}
