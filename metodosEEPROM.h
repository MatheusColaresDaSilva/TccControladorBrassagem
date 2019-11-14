int retornaPosicaoNome(int index) {
  switch (index) {
    case 720:
      return 120;
      break;
    case 850:
      return 131;
      break;
    case 980:
      return 142;
      break;
    case 1110:
      return 153;
      break;
    case 1240:
      return 164;
      break;
    case 1370:
      return 175;
      break;
    case 1500:
      return 186;
      break;
    case 1630:
      return 197;
      break;
    case 1760:
      return 208;
      break;
    case 1890:
      return 219;
      break;
  }

  return -1;
}

int retornaPosicaoReceita(int index) {
  switch (index) {
    case 100:
      return 720;
      break;
    case 102:
      return 850;
      break;
    case 104:
      return 980;
      break;
    case 106:
      return 1110;
      break;
    case 108:
      return 1240;
      break;
    case 110:
      return 1370;
      break;
    case 112:
      return 1500;
      break;
    case 114:
      return 1630;
      break;
    case 116:
      return 1760;
      break;
    case 118:
      return 1890;
      break;
  }

  return -1;
}

void writeString(char add,String data)
{
  int _size = 10;//data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   //Add termination null character for String Data
}
 
 
String read_String(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}


template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;

}

void eepromClear() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  Serial.print("ok");
}

int verificaPosicaoMemoriaVazia() {
  int posicao;
  for (int i = 100 ; i < 120; i = i + 2) {
    Serial.print(i); Serial.print("--->");
    EEPROM_readAnything(i, posicao);
    Serial.print(posicao); Serial.print("--->"); Serial.println(retornaPosicaoReceita(i));
    if (posicao == 0) {

      EEPROM_writeAnything(i, retornaPosicaoReceita(i)) ;
      return retornaPosicaoReceita(i);
    }
  }
  return -1;
}

void gravaReceitaEEPROM(int indexReceita, String nome, Receita receita) {
  Serial.print("Nome//---->"); Serial.println(retornaPosicaoNome(indexReceita));
  writeString(retornaPosicaoNome(indexReceita), nome);
  Serial.println(EEPROM_writeAnything(indexReceita, receita)) ;
}


void deletarReceitaEEPROM(int posicao , int index) {

   EEPROM_writeAnything(posicao,0) ;
   struct Receita receitaDel;
   gravaReceitaEEPROM(index,"                   ", receitaDel);

}

void escolheReceita(){
  delay(150);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Escolha Receita");
  
bool carregarReceitas = true;
int receita_pos = 1;
int pos = 100;
int val;
String nomeReceitaLer;
  while (carregarReceitas) {
    
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Receita ");
       lcd.print(receita_pos);
       nomeReceitaLer = read_String(retornaPosicaoNome(retornaPosicaoReceita(pos)));
        EEPROM_readAnything(retornaPosicaoReceita(pos), receita[0]);
        
       lcd.setCursor(0, 1);
       EEPROM_readAnything(pos, val);
        if(val == 0){
          lcd.print("VAZIO");
        }
        else { 
        lcd.print(nomeReceitaLer);
        }
         
        if (digitalRead(BTN_SOBE)) {
          receita_pos++;
          pos = pos+2;
          if (pos > 118) {
            pos = 100;
            receita_pos = 1;
          }
    
        }
        else if (digitalRead(BTN_DESCE)) {
         receita_pos--;
         pos = pos-2;
          if (pos < 100) {
            pos = 118;
            receita_pos = 10;
          }
        }
       
       else if(digitalRead(BTN_CONFIRMA) && digitalRead(BTN_CANCELA)){
          carregarReceitas = false;
          return NULL;
        }
        
        else if (digitalRead(BTN_CONFIRMA)) {
          if(val == 0){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("NEGADO");
            }
          else { 
                lcd.clear();
               _mostura = true;
                adicionarAgua();
                brassagem();
                carregarReceitas = false;
          }
        
          
        }
    
        if (digitalRead(BTN_CANCELA)) {
         carregarReceitas = false;
        }
        
        delay(150);
    }
   
  
}

void deletarReceita(){
  delay(150);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Escolha Receita");
  
bool carregarReceitas = true;
int receita_pos = 1;
int pos = 100;
int val;
String nomeReceitaLer;
  while (carregarReceitas) {
    
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Receita ");
       lcd.print(receita_pos);
       nomeReceitaLer = read_String(retornaPosicaoNome(retornaPosicaoReceita(pos)));
        EEPROM_readAnything(retornaPosicaoReceita(pos), receita[0]);
        
       lcd.setCursor(0, 1);
       EEPROM_readAnything(pos, val);
        if(val == 0){
          //pos = 100;
          lcd.print("VAZIO");
        }
        else { 
        lcd.print(nomeReceitaLer);
        }
         
        if (digitalRead(BTN_SOBE)) {
          receita_pos++;
          pos = pos+2;
          if (pos > 118) {
            pos = 100;
            receita_pos = 1;
          }
    
        }
        else if (digitalRead(BTN_DESCE)) {
         receita_pos--;
         pos = pos-2;
          if (pos < 100) {
            pos = 118;
            receita_pos = 10;
          }
        }
       
       else if(digitalRead(BTN_CONFIRMA) && digitalRead(BTN_CANCELA)){
          carregarReceitas = false;
          return NULL;
        }
        
        else if (digitalRead(BTN_CONFIRMA)) {
          if(val == 0){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("NEGADO");
            }
          else { 
               lcd.clear();
               _mostura = true;
               deletarReceitaEEPROM(pos,retornaPosicaoReceita(pos));
               carregarReceitas = false;
          }
        
          
        }
    
        if (digitalRead(BTN_CANCELA)) {
         carregarReceitas = false;
         return NULL;
        }
        
        delay(150);
    }
   
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RECEITA DELETADA");
  delay(2000);
}

void menuConfigVarMin(){

  bool varMinConfig = true;
  float varMin;
  Serial.println(EEPROM_readAnything(10, varMin));
  

  while (varMinConfig) {
      delay(150);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Escolha VarMin");
      lcd.setCursor(0, 1);
      lcd.print(varMin);
      
    if(digitalRead(BTN_SOBE)){
           varMin = varMin + 0.5;
             if(varMin > 5){
              varMin = 0;
             }
      
        }
        else if(digitalRead(BTN_DESCE)){
           varMin = varMin - 0.5;
             if(varMin < 0){
              varMin = 5;
             }
        }
                
        else if(digitalRead(BTN_CONFIRMA)){
          Serial.println(EEPROM_writeAnything(10,varMin));
          varMinConfig = false;
        }
        
        if(digitalRead(BTN_CANCELA)){
          varMinConfig = false ;
          
        }
  }
}

void menuConfigTempFerv(){
  
  bool varFervTemp = true;
  float tempFerv;
  Serial.println(EEPROM_readAnything(14, tempFerv));
  

  while (varFervTemp) {
      delay(150);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Escolha Temp Ferv");
      lcd.setCursor(0, 1);
      lcd.print(tempFerv);
      
    if(digitalRead(BTN_SOBE)){
           tempFerv = tempFerv + 0.5;
             if(tempFerv > 110){
              tempFerv = 0;
             }
      
        }
        else if(digitalRead(BTN_DESCE)){
           tempFerv = tempFerv - 0.5;
             if(tempFerv < 0){
              tempFerv = 110;
             }
        }
        
        else if(digitalRead(BTN_CONFIRMA)){
          Serial.println(EEPROM_writeAnything(14,tempFerv));
          varFervTemp = false;
        }
        
        if(digitalRead(BTN_CANCELA)){
          varFervTemp = false ;
          
        }
  }
 
}
