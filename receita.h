void addVariacaoMinima(float valor){
  variacaoMinima = valor;
}

void addEtapaMostura(EtapaQuente etapa[],int posicao, float tempMax, byte tempo, float variacaoMin){
  
  etapa[posicao-1].tempMin = tempMax - variacaoMin;
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
