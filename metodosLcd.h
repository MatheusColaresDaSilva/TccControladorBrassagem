byte menuCursorRight[8] = {
  B01000, //  *  
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};

byte menuCursorLeft[8] = {
  B000010, //  *
  B00100, //  *
  B01000, // *
  B10000, //*     
  B01000, // *  
  B00100, //  *
  B00010, //   *
  B00000  //
};

byte relogioIcon[8] = {
  B00100, //|_|_|*|_|_|    
  B01110, //|_|*|*|*|_|   
  B01110, //|_|*|*|*|_|    
  B01110, //|_|*|*|*|_|     
  B01110, //|_|*|*|*|_|    
  B11111, //|*|*|*|*|*|   
  B00100, //|_|_|*|_|_|  
  B00000  //|_|_|_|_|_|
};

byte termometroIcon[8] = {
  B00100, //|_|_|*|_|_|    
  B01010, //|_|*|_|*|_|   
  B01010, //|_|*|_|*|_|    
  B01110, //|_|*|*|*|_|     
  B01110, //|_|*|*|*|_|    
  B11111, //|*|*|*|*|*|   
  B11111, //|*|*|*|*|*|  
  B01110  //|_|*|*|*|_|
};

byte grausCelsiosIcon[8] = {
  B00000, //|_|_|_|_|_|    
  B11000, //|*|*|_|_|_|   
  B11000, //|*|*|_|_|_|    
  B00111, //|_|*|*|*|*|     
  B00100, //|_|*|_|_|_|    
  B00100, //|_|*|_|_|_|   
  B00111, //|_|*|*|*|*|  
  B00000  //|_|_|_|_|_|
};

byte lupuloIcon[8] = {
  B00100, //|_|_|*|_|_|    
  B01110, //|_|*|*|*|_|   
  B11111, //|*|*|*|*|*|    
  B01110, //|_|*|*|*|_|     
  B11111, //|*|*|*|*|*|    
  B01110, //|_|*|*|*|_|   
  B11111, //|*|*|*|*|*|  
  B00100  //|_|_|*|_|_|
};

String _menuItems[] = {"Criar Receita", "Iniciar Brassagem"};
String _menuMostura[] = {"Mostura 1", "Mostura 2", "Mostura 3",
                         "Mostura 4", "Mostura 5", "Mostura 6",
                         "Mostura 7", "Mostura 8", "Mostura 9","Mostura 10"};
String _menuFervura[] = {"Tempo","Lupulo 1", "Lupulo 2", "Lupulo 3",
                         "Lupulo 4", "Lupulo 5", "Lupulo 6",
                         "Lupulo 7", "Lupulo 8", "Lupulo 9","Lupulo 10"};                         


String alfabeto[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","X","Y","Z"," "};

int _menuAtual=0;
bool _menuAtualMostura=false;
bool _menuAtualFervura=false;

int letra=0;
int letraGravar=0;
int linha=0;
int coluna=0;
bool escrevendo = false;

long previousMillisLcd = 0;
long intervalLcd = 1000;
unsigned long currentMillisLcd;

String nomeReceita[10];

void addLetra(){
  
  letra++;
    if(letra >= (sizeof(alfabeto)/6)){
    letra =0;
   }
}   

void removeLetra(){
  
  letra--;
    if(letra < 0){
    letra = (sizeof(alfabeto)/6) -1;
   }
}

void apagarLetra(){
  
  removeLetra();
  lcd.setCursor(coluna,linha);
  lcd.print(" ");
  

  coluna--;
  if(coluna < 0){
    coluna = 0;
   }
 
  letra = 0;
  nomeReceita[letraGravar] =  " ";
  letraGravar--;
  if(coluna < 0){
    letraGravar = 0;
   }
  lcd.setCursor(coluna,linha);
  lcd.print(alfabeto[letra]);

   escrevendo = false;
  
}

void addNomeReceita() {  
   if(digitalRead(BTN_SOBE) && letraGravar<10){
     lcd.setCursor(coluna,linha);
     lcd.blink();
     addLetra();
     lcd.print(alfabeto[letra]);
     nomeReceita[letraGravar] = alfabeto[letra]; 
    
     escrevendo = true;
     previousMillisLcd = millis();

  }
  else if(digitalRead(BTN_DESCE)&& letraGravar<10){
     lcd.setCursor(coluna,linha);
     lcd.blink();
     removeLetra();
     lcd.print(alfabeto[letra]);
     nomeReceita[letraGravar] = alfabeto[letra];
    escrevendo = true;
    previousMillisLcd = millis();

  }
  else{
    lcd.blink();
    if(letraGravar<10){
    lcd.blink();
    lcd.setCursor(coluna,linha);
    lcd.print(alfabeto[letra]);
    }
    
  }
  
  if(digitalRead(BTN_CONFIRMA)){
    lcd.clear();
    for (int i =0;i<sizeof(nomeReceita)/6;i++){
      lcd.setCursor(i,0);
      lcd.print(nomeReceita[i]);
    }
    
  }
  
  if(digitalRead(BTN_CANCELA)){
    lcd.blink();
    apagarLetra();
    
  }
  
   currentMillisLcd = millis();
  //Lógica de verificação do tempo
  if ((currentMillisLcd - previousMillisLcd > intervalLcd) && escrevendo) { 
      escrevendo = false;
      letraGravar++;
    
      coluna++;
      letra =0;
    
  }
  delay(150);
 
}


void menuMostura(){
  delay(150);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Configurar Mash");
  delay(2000);
  
  int posicaoMostura = 1;
  bool timeTemp = true;
  float tempo = 0;
  float tempratura = 0;
  
 while(_menuAtualMostura && posicaoMostura <= 10){
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(_menuMostura[posicaoMostura-1]);
    lcd.setCursor(0, 1);
    lcd.write(byte(2));
    lcd.setCursor(6, 1);
    lcd.write(byte(3));

    if(timeTemp){
       lcd.blink();
       lcd.setCursor(8, 1);
       lcd.print( tempratura);
       lcd.write(byte(4));
       lcd.setCursor(2, 1);
       lcd.print((int) tempo);

        if(digitalRead(BTN_SOBE)){
           tempo++;
             if(tempo > 999){
              tempo = 0;
             }
      
        }
        else if(digitalRead(BTN_DESCE)){
           tempo--;
             if(tempo < 0){
              tempo = 999;
             }
        }
        else if(digitalRead(BTN_CONFIRMA)){
          timeTemp = false ;
        }
        
        if(digitalRead(BTN_CANCELA)){
          timeTemp = true ;
          
        }

    }
    else{
       lcd.blink();
       lcd.setCursor(2, 1);
       lcd.print((int) tempo);
       lcd.setCursor(8, 1);
       lcd.print( tempratura);
       lcd.write(byte(4));

        if(digitalRead(BTN_SOBE)){
           tempratura= tempratura+0.5;
             if(tempratura > 120){
              tempratura = 0;
             }
      
        }
        else if(digitalRead(BTN_DESCE)){
           tempratura= tempratura-0.5;
             if(tempratura < 0){
              tempratura = 120;
             }
        }
        else if(digitalRead(BTN_CONFIRMA)){
          addEtapaMostura(receita[0].mostura,posicaoMostura,tempratura,(int) tempo,variacaoMinima );
          posicaoMostura++;
           timeTemp = true ;
           tempo = 0;
           tempratura = 0;
        }
        
        if(digitalRead(BTN_CANCELA)){
          timeTemp = true ;
          
        }
    }
   
    delay(150);
 }

 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Mash Configurado");
 delay(2000);

}

void menuFervura(){
  delay(150);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Config Fervura");
  delay(2000);
  
  int posicaoFervura = 1;
  bool timeTemp = true;
  float tempo = 0;
  float tempratura = 0;
  
 while(_menuAtualFervura && posicaoFervura < 2){
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(_menuFervura[posicaoFervura-1]);
    lcd.setCursor(0, 1);
    lcd.write(byte(2));
    lcd.setCursor(6, 1);
    lcd.write(byte(3));

    if(timeTemp){
       lcd.blink();
       lcd.setCursor(8, 1);
       lcd.print( tempratura);
       lcd.write(byte(4));
       lcd.setCursor(2, 1);
       lcd.print((int) tempo);

        if(digitalRead(BTN_SOBE)){
           tempo++;
             if(tempo > 999){
              tempo = 0;
             }
      
        }
        else if(digitalRead(BTN_DESCE)){
           tempo--;
             if(tempo < 0){
              tempo = 999;
             }
        }
        else if(digitalRead(BTN_CONFIRMA)){
          timeTemp = false ;
        }
        
        if(digitalRead(BTN_CANCELA)){
          timeTemp = true ;
          
        }

    }
    else{
       lcd.blink();
       lcd.setCursor(2, 1);
       lcd.print((int) tempo);
       lcd.setCursor(8, 1);
       lcd.print( tempratura);
       lcd.write(byte(4));

        if(digitalRead(BTN_SOBE)){
           tempratura= tempratura+0.5;
             if(tempratura > 120){
              tempratura = 0;
             }
      
        }
        else if(digitalRead(BTN_DESCE)){
           tempratura= tempratura-0.5;
             if(tempratura < 0){
              tempratura = 120;
             }
        }
        else if(digitalRead(BTN_CONFIRMA)){
          addEtapaFervura(receita[0].fervura,tempratura,(int) tempo);
          posicaoFervura++;
           timeTemp = true ;
           tempo = 0;
           tempratura = 0;
        }
        
        if(digitalRead(BTN_CANCELA)){
          timeTemp = true ;
          
        }
    }
   
    delay(150);
 }

 while(_menuAtualFervura && posicaoFervura < 12){
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(_menuFervura[posicaoFervura-1]);
    lcd.setCursor(0, 1);
    lcd.write(byte(2));
    lcd.setCursor(6, 1);
    lcd.write(byte(5));

       lcd.blink();
       lcd.setCursor(3, 1);
       lcd.print((int) tempo);

        if(digitalRead(BTN_SOBE)){
           tempo++;
             if(tempo > 999){
              tempo = 0;
             }
      
        }
        else if(digitalRead(BTN_DESCE)){
           tempo--;
             if(tempo < 0){
              tempo = 999;
             }
        }
        else if(digitalRead(BTN_CONFIRMA)){
          addLupulo(receita[0].lupulo,posicaoFervura-1,(int) tempo);
          posicaoFervura++;
           timeTemp = true ;
           tempo = 0;
           tempratura = 0;
        }
        
        if(digitalRead(BTN_CANCELA)){
          
          
        }

   
    delay(150);
 }
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Receita Criada");
 delay(2000);
Serial.println("====================================") ;
    Serial.println("Mostura :") ;
    for(int i = 0 ; i< sizeof(receita[0].mostura[0]);i++){
     Serial.print("TempMin->");Serial.print( receita[0].mostura[i].tempMin);Serial.print(" TempMax->");Serial.print( receita[0].mostura[i].tempMax);Serial.print(" Duracao->");Serial.print( receita[0].mostura[i].duracao) ;  
     Serial.println();
    }
    Serial.println("Fervura :") ;
    for(int i = 0 ; i< 1;i++){
     Serial.print("TempMin->");Serial.print( receita[0].fervura[i].tempMin);Serial.print(" TempMax->");Serial.print( receita[0].fervura[i].tempMax);Serial.print(" Duracao->");Serial.print( receita[0].fervura[i].duracao) ;  
     Serial.println();
    }
    Serial.println("Lupulo :") ;
    for(int i = 0 ; i< 10;i++){
     Serial.print("Hora->");Serial.print( receita[0].lupulo[i].duracao) ;  
     Serial.println();
    }
}

void mexerMenu(){

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(_menuItems[_menuAtual]);
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.setCursor(15, 1);
  lcd.write(byte(0));
  
 if(digitalRead(BTN_SOBE)){
     _menuAtual++;
       if(_menuAtual >1){
        _menuAtual = 0;
       }

  }
  else if(digitalRead(BTN_DESCE)){
     _menuAtual--;
       if(_menuAtual < 0){
        _menuAtual = 1;
       }
  }

  if(digitalRead(BTN_CONFIRMA)){
    switch(_menuAtual){
      case 0:
       _menuAtualMostura = true;
        menuMostura();
       _menuAtual++;
       _menuAtualMostura = false;
        _menuAtualFervura = true;
       menuFervura();
      _menuAtual++;
      _menuAtualFervura = false;
      break;
    }
  }
   delay(150);
}
