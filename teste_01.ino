#include <RTClib.h>
#include <dht.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "RTClib.h"

#define endereco 0x27 
#define colunas 16
#define linhas 2

#define sensor_temp A0
#define grupo1 9
#define grupo2 8
#define grupo3 7
#define btn_mais 12
#define btn_menos 13
#define btn_ok 10
#define btn_voltar 11
#define alarme 6
#define neb 5

LiquidCrystal_I2C lcd(endereco, colunas, linhas);

dht DHT;
RTC_DS1307 rtc;

char daysOfTheWeek[7][12]= {"Domingo","Segunda","Terca","Quarta","Quinta","Sexta","Sabado"};

float tempC=0;
float umid=0;
float pres=0;
float temp_max=35;
float temp_min=10;
float temp_grupo1=10;
float temp_grupo2=10;
float temp_grupo=10;
float temp_desejada=10;
float umid_max=80;
float umid_min=30;
float umid_acionamento=45;
float pres_max=70;
float pres_min=30;
float pres_acionamento=30;
float vent_on=0;
float vent_off=0;
float vent_qtd=0;

bool v_btn_mais = 1;
bool va_btn_mais = 1;
bool v_btn_menos = 1;
bool va_btn_menos = 1;
bool v_btn_ok = 1;
bool va_btn_ok = 1;
bool v_btn_voltar=1;
bool va_btn_voltar=1;

int tela = 1;
int x,y;

int dia=1;
int mes=2;
int ano=2020;

int hora=14;
int minuto=10;
int segundo=20;

void setup() {
  rtc.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("   INICIANDO... "));
  delay (2000);
  
  pinMode(sensor_temp,INPUT);
  pinMode(grupo1,OUTPUT);
  pinMode(grupo2,OUTPUT);
  pinMode(grupo3,OUTPUT);
  pinMode(btn_mais,INPUT_PULLUP);
  pinMode(btn_menos,INPUT_PULLUP);
  pinMode(btn_ok,INPUT_PULLUP);
  pinMode(alarme,OUTPUT);
  pinMode(neb,OUTPUT);
  pinMode(btn_voltar,INPUT_PULLUP);
}

float ler_converter_celsius(){
  float leitura_sensor = DHT.read11(sensor_temp);
  float temp = DHT.temperature;
  return temp;
}

float ler_umidade(){
  float leitura_sensor = DHT.read11(sensor_temp);
  float umidade = DHT.humidity;
  return umidade;
}

float ler_pres(){
  float leitura_sensor = 10;
  float converte = 5;
  return converte;
}

// ****************** ACIONAMENTOS ******************  

void acionamento_temp(){  
  if (tempC >= temp_max){
    digitalWrite(grupo1,1);
    digitalWrite(grupo2,1);
    digitalWrite(grupo3,1);
  }else if (tempC <= temp_min){
    digitalWrite(grupo1,0);
    digitalWrite(grupo2,0);
    digitalWrite(grupo3,0);
  }else if (tempC == temp_desejada){
    digitalWrite(grupo1,0);
    digitalWrite(grupo2,0);
    digitalWrite(grupo3,0);
  }else {
    digitalWrite(grupo1,0);
    digitalWrite(grupo2,0);
    digitalWrite(grupo3,0);
  }
}

void acionamento_umid(){
  if (umid <= umid_min){
    digitalWrite(neb,1);
  }else if (umid >= umid_acionamento && tempC >= temp_desejada){
    digitalWrite(neb,0);
  }else if(umid <= umid_acionamento && tempC >= temp_desejada){
    digitalWrite(neb,1);
  }else {digitalWrite(neb,0);}
}
void acionamento_pres(){
  
}

void acionamento_vent_min(){
  DateTime atual = rtc.now();
  DateTime timer (atual + TimeSpan(0,0,0,5));
    if(atual.second() == timer.second()) digitalWrite(neb,1);
}

void acionamento_alarme(){
  if(tempC >= temp_max || tempC <= temp_min){
  digitalWrite(alarme,HIGH);
  }else if(umid >= umid_max || umid <= umid_min){
  digitalWrite(alarme,HIGH);
  }//else if(pres >= pres_max || pres <= pres_min){
  //digitalWrite(alarme,HIGH);
  //}
  else {
    digitalWrite(alarme,LOW);
  }
}

void executar(){
  tempC = ler_converter_celsius();
  umid = ler_umidade();
  pres = ler_pres();
  lcd.setCursor(0, 1);
  lcd.print(tempC,1);
  lcd.setCursor(4, 1);
  lcd.print("C  ");
  lcd.setCursor(7,1);
  lcd.print(umid,0);
  lcd.setCursor(9, 1);
  lcd.print("%  ");
  lcd.setCursor(12, 1);
  lcd.print(pres,0);
  lcd.setCursor(13,1);
  lcd.print("  P    ");
  acionamento_temp();
  acionamento_umid();
  acionamento_pres();
  acionamento_vent_min();
}
  
// CONFIGURAÇÕES DO RELÓGIO

void config_dia(){

  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print(F("Dia: "));
  lcd.setCursor(5, 1);
  lcd.print(dia);
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    dia++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    dia--;
  }
  va_btn_menos = v_btn_menos;

  if (dia == 33) {
    lcd.clear();
    dia = 1;
  }
  else if(dia == 0) dia = 32;
}

void config_mes(){

  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print(F("Mes: "));
  lcd.setCursor(5, 1);
  lcd.print(mes);
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    mes++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    mes--;
  }
  va_btn_menos = v_btn_menos;

  if (mes == 13) {
    lcd.clear();
    mes = 1;
  }
  else if(mes == 0) mes = 12;
}

void config_ano(){
  
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print(F("Ano: "));
  lcd.setCursor(5, 1);
  lcd.print(ano);
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    ano++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    ano--;
  }
  va_btn_menos = v_btn_menos;
}

void config_hora(){
  
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print(F("Hora: "));
  lcd.setCursor(5, 1);
  lcd.print(hora);
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    hora++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    hora--;
  }
  va_btn_menos = v_btn_menos;

  if (hora == 25) {
    lcd.clear();
    hora = 1;
  }
  else if(hora == 0) hora = 24;
}

void config_minuto(){
  
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print(F("Minuto: "));
  lcd.setCursor(8, 1);
  lcd.print(minuto);
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    minuto++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    minuto--;
  }
  va_btn_menos = v_btn_menos;

  if (minuto == 60) {
    lcd.clear();
    minuto = 0;
  }
  else if(minuto == -1) minuto = 59;
}

void config_segundo(){
  
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print(F("Segundo: "));
  lcd.setCursor(9, 1);
  lcd.print(segundo);
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    segundo++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    segundo--;
  }
  va_btn_menos = v_btn_menos;

  if (segundo == 60) {
    lcd.clear();
    segundo = 0;
  }
  else if(segundo == -1) segundo = 59;
}


// CONFIGURAÇÃO E EXIBIÇÃO DE DATA E HORA TELA 999

void data_hora(){
  while (y <= 1){
  rtc.adjust(DateTime(ano,mes,dia,hora,minuto,segundo));
  y++;
  }
  DateTime now = rtc.now();
  lcd.setCursor(4, 0);
  lcd.print(now.day(),DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);

  lcd.setCursor(4, 1);
  lcd.print(now.hour(),DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  }

// CONFIGURAÇÕES DE TEMPERATURA 

void config_temp_max(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(temp_max);
  lcd.setCursor(10,1);
  lcd.print(" C    ");
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    temp_max++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    temp_max--;
  }
  va_btn_menos = v_btn_menos;
}

void config_temp_min(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(temp_min);
  lcd.setCursor(10,1);
  lcd.print(" C    ");
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    temp_min++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    temp_min--;
  }
  va_btn_menos = v_btn_menos;
}

void config_temp_desejada(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(temp_desejada);
  lcd.setCursor(10,1);
  lcd.print(" C    ");
  
  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    temp_desejada++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    temp_desejada--;
  }
  va_btn_menos = v_btn_menos;
}


// CONFIGURAÇÕES DE UMIDADE

void config_umid_max(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(umid_max);
  lcd.setCursor(10,1);
  lcd.print("      ");
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    umid_max++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    umid_max--;
  }
  va_btn_menos = v_btn_menos;
}

void config_umid_min(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(umid_min);
  lcd.setCursor(10,1);
  lcd.print("      ");
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    umid_min++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    umid_min--;
  }
  va_btn_menos = v_btn_menos;
}

void config_umid_acionamento(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando  "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(umid_acionamento);
  lcd.setCursor(10,1);
  lcd.print("      ");
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    umid_acionamento++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    umid_acionamento--;
  }
  va_btn_menos = v_btn_menos;
}


//CONFIGURAÇÕES DE PRESSÃO

void config_pres_max(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(pres_max);
  lcd.setCursor(10,1);
  lcd.print("      ");
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    pres_max++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    pres_max--;
  }
  va_btn_menos = v_btn_menos;
}

void config_pres_min(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(pres_min);
  lcd.setCursor(10,1);
  lcd.print("      ");
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    pres_min++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    pres_min--;
  }
  va_btn_menos = v_btn_menos;
}

void config_pres_acionamento(){
  lcd.setCursor(0,0);
  lcd.print(F("  Configurando  "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(pres_acionamento);
  lcd.setCursor(10,1);
  lcd.print("      ");
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    pres_acionamento++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    pres_acionamento--;
  }
  va_btn_menos = v_btn_menos;
}

// CONFIGURAÇÕES DE VENTILAÇÃO MÍNIMA

void config_vent_min_on(){

  lcd.setCursor(0,0);
  lcd.print(F("  Configurando  "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(7,1);
  lcd.print(vent_on,0);
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    vent_on++;
    vent_on++;
    vent_on++;
    vent_on++;
    vent_on++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    vent_on--;
    vent_on--;
    vent_on--;
    vent_on--;
    vent_on--;
  }
  va_btn_menos = v_btn_menos;
  
}

void config_vent_min_off(){

  lcd.setCursor(0,0);
  lcd.print(F("  Configurando  "));
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.setCursor(7,1);
  lcd.print(vent_off,0);
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    vent_off++;
    vent_off++;
    vent_off++;
    vent_off++;
    vent_off++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    vent_off--;
    vent_off--;
    vent_off--;
    vent_off--;
    vent_off--;
    
  }
  va_btn_menos = v_btn_menos;
  
}

void config_vent_min_qtd(){

  lcd.setCursor(0,0);
  lcd.print(F("  Configurando  "));
  lcd.setCursor(0,1);
  lcd.print("        ");
  lcd.setCursor(8,1);
  lcd.print(vent_qtd,0);
  lcd.setCursor(9,1);
  lcd.print("                    ");
  
    v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
    vent_qtd++;
  }
  va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
    vent_qtd--;
  }
  va_btn_menos = v_btn_menos;
  
}


// ********************* TELAS *********************

void controle_tela(){
  
// TELA PRINCIPAL COM VALORES LIDOS PELOS SENSORES
  
  if(tela == 1){
    lcd.setCursor(0,0);
    lcd.print(F(" TEM   UMI  PRES  "));
    executar();
    x=0;
  }

 // TELA RELOGIO
 
  else if(tela == 999){

    while(x <= 1){
      lcd.clear();
      x++;
    }
    data_hora();
    delay(5000);
    tela=1;
    }
  
// MENU DE SELEÇÃO COM 10 ESCOLHAS (TEMP, UMID, PRES, VENT M., ILUM, V1, V2, V3, N1, AQ1)
  
  else if(tela == 2){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Temperatura    "));
  }else if(tela == 3){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Umidade        "));
  }else if(tela == 4){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Pressao        "));
  }else if(tela == 5){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Ventilacao Min"));
  }else if(tela == 6){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Iluminacao     "));
  }else if(tela == 7){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Ventilacao 1   "));
   }else if(tela == 8){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Ventilacao 2   "));
   }else if(tela == 9){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Ventilacao 3   "));
   }else if(tela == 10){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Nebulizacao 1  "));
   }else if(tela == 11){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Aquecimento 1  "));
  }else if(tela == 12){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Data e Hora  "));
  }
  
// MENU DE EDIÇÃO *TEMPERATURA* COM 3 OPÇÕES (TEM MAX, MIN, DESEJADA)
  
  else if(tela == 20){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Temp. Desejada"));
  }else if(tela == 21){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU      "));
    lcd.setCursor(0,1);
    lcd.print(F("> Temp Maxima   "));
  }else if(tela == 22){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU      "));
    lcd.setCursor(0,1);
    lcd.print(F("> Temp Minima   "));
  }else if(tela == 200){
    config_temp_desejada();
  }else if(tela == 210){
    config_temp_max();
  }else if(tela == 220){
    config_temp_min();
  }
// MENU DE EDIÇÃO *UMIDADE* COM 3 OPÇÕES (MAX, MIN E UMIDADE DE ACIONAMENTO)
  
  else if(tela == 30){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Umidade Neb.ON"));
  }else if(tela == 31){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Umidade Max.  "));
  }else if(tela == 32){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU      "));
    lcd.setCursor(0,1);
    lcd.print(F("> Umidade Min.  "));
  }else if(tela == 300){
    config_umid_acionamento();
  }else if(tela == 310){
    config_umid_max();
  }else if(tela == 320){
    config_umid_min();
  }
  
// MENU DE EDIÇÃO *PRESSÃO* C/ 3 OPÇÕES (PRES MAX, MIN E PRESSÃO DE ACIONAMENTO)
  else if(tela == 40){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Cortina ON     "));
  }else if(tela == 41){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Pressao Max.   "));
  }else if(tela == 42){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU      "));
    lcd.setCursor(0,1);
    lcd.print(F("> Pressao Min.  "));
  }else if(tela == 400){
    config_pres_acionamento();
  }else if(tela == 410){
    config_pres_max();
  }else if(tela == 420){
    config_pres_min();
  }
  
// MENU DE EDIÇÃO *VENTILAÇÃO MÍNIMA* 3 OPÇÕES (TEMPO ON, TEMPO OFF, QTD VENTILAÇÃO)
  else if(tela == 50){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON             "));
  }else if(tela == 51){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF            "));
  }else if(tela == 52){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> QTD VENTILACAO "));
  }else if(tela == 500)config_vent_min_on();
   else if(tela == 510)config_vent_min_off();
   else if(tela == 520)config_vent_min_qtd();
  
// MENU DE EDIÇÃO *ILUMINAÇÃO* 10 OPÇÕES (HORÁRIO 1, 2, 3, 4...)
  else if(tela == 60){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 1      "));
  }else if(tela == 61){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 2      "));
  }else if(tela == 62){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 3      "));
  }else if(tela == 63){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 4      "));
  }else if(tela == 64){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 5      "));
  }else if(tela == 65){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 6      "));
  }else if(tela == 66){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 7      "));
  }else if(tela == 67){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 8      "));
  }else if(tela == 68){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 9      "));
  }else if(tela == 69){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> HORARIO 10     "));
  }
  
  else if(tela == 600){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 601){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 610){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 611){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 620){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 621){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 630){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 631){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 640){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 641){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 650){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 651){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 660){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 661){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 670){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 671){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 680){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 681){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }else if(tela == 690){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> ON (HORA)      "));
  }else if(tela == 691){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> OFF(HORA)      "));
  }
  
  else if(tela == 6000);
  else if(tela == 6010);
  else if(tela == 6100);
  else if(tela == 6110);
  else if(tela == 6200);
  else if(tela == 6210);
  else if(tela == 6300);
  else if(tela == 6310);
  else if(tela == 6400);
  else if(tela == 6410);
  else if(tela == 6500);
  else if(tela == 6510);
  else if(tela == 6600);
  else if(tela == 6610);
  else if(tela == 6700);
  else if(tela == 6710);
  else if(tela == 6800);
  else if(tela == 6810);
  else if(tela == 6900);
  else if(tela == 6910);
    
// MENU DE EDIÇÃO *VENTILAÇÃO 1* 
  else if(tela == 70){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Ventilacao 1   "));
  }
  else if(tela == 701);
    
// MENU DE EDIÇÃO *VENTILAÇÃO 2* 
  else if(tela == 80){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Ventilacao 2   "));
  }
  else if(tela == 801);
    
// MENU DE EDIÇÃO *VENTILAÇÃO 3* 
  else if(tela == 90){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Ventilacao 3   "));
  }
  else if(tela == 901);
    
// MENU DE EDIÇÃO *NEBULIZAÇÃO 1*
  else if(tela == 100){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Temperatura    "));
  }else if(tela == 101){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Umidade        "));
  }
  else if(tela == 1000);
  else if(tela == 1010);
    
// MENU DE EDIÇÃO *AQUECIMENTO 1*
  else if(tela == 110);

// MENU DE EDIÇÃO DATA E HORA

else if(tela == 120){
    lcd.setCursor(0,0);
    lcd.print("      MENU       ");
    lcd.setCursor(0,1);
    lcd.print(F("> Data           "));
  }else if(tela == 121){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Horario        "));
  }
  else if(tela == 1200){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Dia            "));
    }
  else if(tela == 1201){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Mes            "));
    }
  else if(tela == 1202){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Ano            "));
  }else if(tela == 1210){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Hora           "));
  }else if(tela == 1211){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Minuto         "));
  }else if(tela == 1212){
    lcd.setCursor(0,0);
    lcd.print(F("      MENU       "));
    lcd.setCursor(0,1);
    lcd.print(F("> Segundo        "));
    }
  else if(tela == 12000){
    config_dia();
  }
  else if(tela == 12001){
    config_mes();
    }
  else if(tela == 12002){
    config_ano();
    }
  else if(tela == 12100){
    config_hora();
  }
  else if(tela == 12101){
    config_minuto();
  }
  else if(tela == 12102){
    config_segundo();
  }

  v_btn_mais = digitalRead(btn_mais);
  if(!v_btn_mais && va_btn_mais){
         if(tela==1) tela = 999;
         else if(tela==999) tela = 1;
    // MENU
    else if(tela==2) tela = 3;
    else if(tela==3) tela = 4;
    else if(tela==4) tela = 5;
    else if(tela==5) tela = 6;
    else if(tela==6) tela = 7;
    else if(tela==7) tela = 8;
    else if(tela==8) tela = 9;
    else if(tela==9) tela = 10;
    else if(tela==10)tela = 11;
    else if(tela==11)tela = 12;
    else if(tela==12)tela = 2;
    // TEMPERATURA
    else if(tela==20)tela = 21;
    else if(tela==21)tela = 22;
    else if(tela==22)tela = 20;
    // UMIDADE
    else if(tela==30)tela = 31;
    else if(tela==31)tela = 32;
    else if(tela==32)tela = 30;
    // PRESSÃO
    else if(tela==40)tela = 41;
    else if(tela==41)tela = 42;
    else if(tela==42)tela = 40;
    // VENTILAÇÃO MÍNIMA
    else if(tela==50)tela = 51;
    else if(tela==51)tela = 52;
    else if(tela==52)tela = 50;
    // ILUMINAÇÃO
    else if(tela==60)tela = 61;
    else if(tela==61)tela = 62;
    else if(tela==62)tela = 63;
    else if(tela==63)tela = 64;
    else if(tela==64)tela = 65;
    else if(tela==65)tela = 66;
    else if(tela==66)tela = 67;
    else if(tela==67)tela = 68;
    else if(tela==68)tela = 69;
    else if(tela==69)tela = 60;
    
    else if(tela==600)tela = 601;
    else if(tela==601)tela = 600;
      
    else if(tela==610)tela = 611;
    else if(tela==611)tela = 610;
      
    else if(tela==620)tela = 621;
    else if(tela==621)tela = 620;
      
    else if(tela==630)tela = 631;
    else if(tela==631)tela = 630;
      
    else if(tela==640)tela = 641;
    else if(tela==641)tela = 640;
      
    else if(tela==650)tela = 651;
    else if(tela==651)tela = 650;
    
    else if(tela==660)tela = 661;
    else if(tela==661)tela = 660;
      
    else if(tela==670)tela = 671;
    else if(tela==671)tela = 670;
      
    else if(tela==680)tela = 681;
    else if(tela==681)tela = 680;
    
    else if(tela==690)tela = 691;
    else if(tela==691)tela = 690;
      
    // NEBULIZAÇÃO
    else if(tela==100)tela = 101;
    else if(tela==101)tela = 100;

    // DATA E HORA
    else if(tela==120)tela = 121;
    else if(tela==121)tela = 120;
    
    else if(tela==1200)tela = 1201;
    else if(tela==1201)tela = 1202;
    else if(tela==1202)tela = 1200;

    else if(tela==1210)tela = 1211;
    else if(tela==1211)tela = 1212;
    else if(tela==1212)tela = 1210;
  }
    va_btn_mais = v_btn_mais;
  
  v_btn_menos = digitalRead(btn_menos);
  if(!v_btn_menos && va_btn_menos){
         if(tela==1) tela = 1;
    // MENU
    else if(tela==2) tela = 12;
    else if(tela==3) tela = 2;
    else if(tela==4) tela = 3;
    else if(tela==5) tela = 4;
    else if(tela==6) tela = 5;
    else if(tela==7) tela = 6;
    else if(tela==8) tela = 7;
    else if(tela==9) tela = 8;
    else if(tela==10)tela = 9;
    else if(tela==11)tela = 10;
    else if(tela==12)tela = 11;
    // TEMPERATURA
    else if(tela==20)tela = 22;
    else if(tela==21)tela = 20;
    else if(tela==22)tela = 21;
    // UMIDADE
    else if(tela==30)tela = 32;
    else if(tela==31)tela = 30;
    else if(tela==32)tela = 31;
    // PRESSÃO
    else if(tela==40)tela = 42;
    else if(tela==41)tela = 40;
    else if(tela==42)tela = 41;
    // VENTILAÇÃO MÍNIMA
    else if(tela==50)tela = 52;
    else if(tela==51)tela = 50;
    else if(tela==52)tela = 51;
    // ILUMINAÇÃO
    else if(tela==60)tela = 69;
    else if(tela==61)tela = 60;
    else if(tela==62)tela = 61;
    else if(tela==63)tela = 62;
    else if(tela==64)tela = 63;
    else if(tela==65)tela = 64;
    else if(tela==66)tela = 65;
    else if(tela==67)tela = 66;
    else if(tela==68)tela = 67;
    else if(tela==69)tela = 68;
    
    else if(tela==600)tela = 601;
    else if(tela==601)tela = 600;
      
    else if(tela==610)tela = 611;
    else if(tela==611)tela = 610;
      
    else if(tela==620)tela = 621;
    else if(tela==621)tela = 620;
      
    else if(tela==630)tela = 631;
    else if(tela==631)tela = 630;
      
    else if(tela==640)tela = 641;
    else if(tela==641)tela = 640;
      
    else if(tela==650)tela = 651;
    else if(tela==651)tela = 650;
    
    else if(tela==660)tela = 661;
    else if(tela==661)tela = 660;
      
    else if(tela==670)tela = 671;
    else if(tela==671)tela = 670;
      
    else if(tela==680)tela = 681;
    else if(tela==681)tela = 680;
    
    else if(tela==690)tela = 691;
    else if(tela==691)tela = 690;
      
    // NEBULIZAÇÃO
    else if(tela==100)tela = 101;
    else if(tela==101)tela = 100;

    // DATA E HORA
    else if(tela==121)tela = 120;
    else if(tela==120)tela = 121;
    
    else if(tela==1200)tela = 1202;
    else if(tela==1202)tela = 1201;
    else if(tela==1201)tela = 1200;

    else if(tela==1212)tela = 1211;
    else if(tela==1211)tela = 1210;
    else if(tela==1210)tela = 1212;
  }
  va_btn_menos = v_btn_menos;
  
  v_btn_ok = digitalRead(btn_ok);
  if(!v_btn_ok && va_btn_ok){
         if(tela==1) tela = 2;
    // MENU
    else if(tela==2) tela = 20;
    else if(tela==3) tela = 30;
    else if(tela==4) tela = 40;
    else if(tela==5) tela = 50;
    else if(tela==6) tela = 60;
    else if(tela==7) tela = 70;
    else if(tela==8) tela = 80;
    else if(tela==9) tela = 90;
    else if(tela==10)tela = 100;
    else if(tela==11)tela = 110;
    else if(tela==12)tela = 120;
    // TEMPERATURA
    else if(tela==20) tela = 200;
    else if(tela==21) tela = 210;
    else if(tela==22) tela = 220;
    // UMIDADE
    else if(tela==30) tela = 300;
    else if(tela==31) tela = 310;
    else if(tela==32) tela = 320;
    // PRESSÃO
    else if(tela==40) tela = 400;
    else if(tela==41) tela = 410;
    else if(tela==42) tela = 420;
    // VENTILAÇÃO MÍNIMA
    else if(tela==50) tela = 500;
    else if(tela==51) tela = 510;
    else if(tela==52) tela = 520;
    // ILUMINAÇÃO
    else if(tela==60) tela = 600;
    else if(tela==61) tela = 610;
    else if(tela==62) tela = 620;
    else if(tela==63) tela = 630;
    else if(tela==64) tela = 640;
    else if(tela==65) tela = 650;
    else if(tela==66) tela = 660;
    else if(tela==67) tela = 670;
    else if(tela==68) tela = 680;
    else if(tela==69) tela = 690;
    
    else if(tela==600)tela = 6000;
    else if(tela==601)tela = 6010;
    else if(tela==610)tela = 6100;
    else if(tela==611)tela = 6110;
    else if(tela==620)tela = 6200;
    else if(tela==621)tela = 6210;
    else if(tela==630)tela = 6300;
    else if(tela==631)tela = 6310;
    else if(tela==640)tela = 6400;
    else if(tela==641)tela = 6410;
    else if(tela==650)tela = 6500;
    else if(tela==651)tela = 6510;
    else if(tela==660)tela = 6600;
    else if(tela==661)tela = 6610;
    else if(tela==670)tela = 6700;
    else if(tela==671)tela = 6710;
    else if(tela==680)tela = 6800;
    else if(tela==681)tela = 6810;
    else if(tela==690)tela = 6900;
    else if(tela==691)tela = 6910;
      
    // VENTILAÇÃO 1
    else if(tela==70) tela = 701;
    // VENTILAÇÃO 2
    else if(tela==80) tela = 801;
    // VENTILAÇÃO 3
    else if(tela==90) tela = 901;
    // NEBULIZAÇÃO
    else if(tela==100) tela = 1000;
    else if(tela==101) tela = 1010;
    // AQUECIMENTO
    else if(tela==110) tela = 1100;

    // DATA E HORA
    else if(tela==120) tela = 1200;
    else if(tela==121) tela = 1210;
    
    else if(tela==1200) tela = 12000;
    else if(tela==1201) tela = 12001;
    else if(tela==1202) tela = 12002;

    else if(tela==1210) tela = 12100;
    else if(tela==1211) tela = 12101;
    else if(tela==1212) tela = 12102;
 
  }
  va_btn_ok = v_btn_ok;


v_btn_voltar = digitalRead(btn_voltar);
  if(!v_btn_voltar && va_btn_voltar){
         if(tela==1) tela = 1;
    // MENU
    else if(tela==2) tela = 1;
    else if(tela==3) tela = 1;
    else if(tela==4) tela = 1;
    else if(tela==5) tela = 1;
    else if(tela==6) tela = 1;
    else if(tela==7) tela = 1;
    else if(tela==8) tela = 1;
    else if(tela==9) tela = 1;
    else if(tela==10) tela = 1;
    else if(tela==11) tela = 1;
    else if(tela==12) tela = 1;
    // TEMPERATURA
    else if(tela==20) tela = 2;
    else if(tela==21) tela = 2;
    else if(tela==22) tela = 2;
    else if(tela==200) tela = 20;
    else if(tela==210) tela = 21;
    else if(tela==220) tela = 22;
    // UMIDADE
    else if(tela==30) tela = 3;
    else if(tela==31) tela = 3;
    else if(tela==32) tela = 3;
    else if(tela==300) tela = 30;
    else if(tela==310) tela = 31;
    else if(tela==320) tela = 32;
    // PRESSAO
    else if(tela==40) tela = 4;
    else if(tela==41) tela = 4;
    else if(tela==42) tela = 4;
    else if(tela==400) tela = 40;
    else if(tela==410) tela = 41;
    else if(tela==420) tela = 42;
    // VENTILAÇÃO MÍNIMA
    else if(tela==50) tela = 5;
    else if(tela==51) tela = 5;
    else if(tela==52) tela = 5;
    else if(tela==500) tela = 50;
    else if(tela==510) tela = 51;
    else if(tela==520) tela = 52;
    // ILUMINAÇÃO
    else if(tela==60) tela = 6;
    else if(tela==61) tela = 6;
    else if(tela==62) tela = 6;
    else if(tela==63) tela = 6;
    else if(tela==64) tela = 6;
    else if(tela==65) tela = 6;
    else if(tela==66) tela = 6;
    else if(tela==67) tela = 6;
    else if(tela==68) tela = 6;
    else if(tela==69) tela = 6;
    
    else if(tela==600)tela = 60;
    else if(tela==601)tela = 60;
    else if(tela==610)tela = 61;
    else if(tela==611)tela = 61;
    else if(tela==620)tela = 62;
    else if(tela==621)tela = 62;
    else if(tela==630)tela = 63;
    else if(tela==631)tela = 63;
    else if(tela==640)tela = 64;
    else if(tela==641)tela = 64;
    else if(tela==650)tela = 65;
    else if(tela==651)tela = 65;
    else if(tela==660)tela = 66;
    else if(tela==661)tela = 66;
    else if(tela==670)tela = 67;
    else if(tela==671)tela = 67;
    else if(tela==680)tela = 68;
    else if(tela==681)tela = 68;
    else if(tela==690)tela = 69;
    else if(tela==691)tela = 69;
    
    else if(tela==6000)tela = 600;
    else if(tela==6010)tela = 601;
    else if(tela==6100)tela = 610;
    else if(tela==6110)tela = 611;
    else if(tela==6200)tela = 620;
    else if(tela==6210)tela = 621;
    else if(tela==6300)tela = 630;
    else if(tela==6310)tela = 631;
    else if(tela==6400)tela = 640;
    else if(tela==6410)tela = 641;
    else if(tela==6500)tela = 650;
    else if(tela==6510)tela = 651;
    else if(tela==6600)tela = 660;
    else if(tela==6610)tela = 661;
    else if(tela==6700)tela = 670;
    else if(tela==6710)tela = 671;
    else if(tela==6800)tela = 680;
    else if(tela==6810)tela = 681;
    else if(tela==6900)tela = 690;
    else if(tela==6910)tela = 691;
    // VENTILAÇÃO 1
    else if(tela==70) tela = 7;
    else if(tela==701) tela = 70;
    // VENTILAÇÃO 2
    else if(tela==80) tela = 8;
    else if(tela==801) tela = 80;
    // VENTILAÇÃO 3
    else if(tela==90) tela = 9;
    else if(tela==901) tela = 90;
    // NEBULIZAÇÃO
    else if(tela==100)  tela = 10;
    else if(tela==1000) tela = 100;
    else if(tela==1010) tela = 101;
    // AQUECIMENTO
    else if(tela==110) tela = 11;
    else if(tela==1100) tela = 110;
    // DATA E HORA
    else if(tela==12000) tela = 1200;
    else if(tela==12001) tela = 1201;
    else if(tela==12002) tela = 1202;

    else if(tela==12100) tela = 1210;
    else if(tela==12101) tela = 1211;
    else if(tela==12102) tela = 1212;
    
    else if(tela==1200) tela = 120;
    else if(tela==1201) tela = 120;
    else if(tela==1202) tela = 120;

    else if(tela==1210) tela = 121;
    else if(tela==1211) tela = 121;
    else if(tela==1212) tela = 121;
    
    else if(tela==120){
      tela = 12;
      y=0;
    }
    else if(tela==121){
      tela = 12;
      y=0;
    }
  }
  va_btn_voltar = v_btn_voltar;
  
}
  
void loop() {
  controle_tela();
  acionamento_alarme();
}
 
