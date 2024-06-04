#include <Wire.h> // подключение библиотеки, используется для связи микроконтроллера с устройствами и модулями через интерфейс I2C 
#include "DHT.h" // подключение библиотеки, предназначена для работы с датчиками температуры и влажности DHT 22
#include <iarduino_RTC.h> // подключение библиотеки, позволяет читать и записывать время RTC модулей на базе чипов DS1302
#include <LiquidCrystal_I2C.h> // подключение библиотеки, предназначена для работы по 8-битному (4-битному) параллельному интерфейс, для дисплея, подключенного по шине I2C

LiquidCrystal_I2C _lcd1(0x20, 16, 2); // адрес дисплея, количество символов в строке, количество строк
int analogYL38 = PC3;//A3 - контакт, к которому подключен YL-38
int analog = PC2;//А2 - контакт к которому подключён датчик уровня воды 
#define DHTPIN PD4
#define DHTTYPE DHT11
int t;//переменная для хранения значений температуры
int h;//переменная для хранения значений относительной влажности воздуха
int val2;//переменная для хранения значений обьёма воды в резервуаре
int z;//переменная для хранения значений влажности почвы в виде процентного ссоотношения
int val;//переменная для хранения значений влажности почвы
int i;// переменная для циклов
int t_normal=20;
int h_normal=65;
int but1=0;
int but2=0;
int but3=0;
int but4=0;
int soilOn=0;
int timer=0;
DHT dht1(DHTPIN, DHTTYPE); // переопределение названия датчика

void setup()
{
  pinMode(12, OUTPUT); // настраиваем контакт №12 на выход красного светодиода
  pinMode(analogYL38, INPUT);//настраиваем контакт А3 на выход датчика влажности почвы
  pinMode(analog, INPUT);//настраиваем контакт А2 на выход датчика уровня воды
  Wire.begin(); // инициализирует библиотеку Wire и подключается к шине I2C
  pinMode(PD5,INPUT);
   pinMode(8,INPUT);
    pinMode(9,INPUT);
     pinMode(10,INPUT);
  dht1.begin();// инициализация ЖКИ
  delay(10); // задержка 10 мс
  _lcd1.init(); // инициализация ЖКИ
   _lcd1.clear(); // очистка дисплея
  _lcd1.setCursor(0, 0); // установка курсора на первую строку первый ряд для вывода информации
  _lcd1.backlight(); //включение подсветки ЖКИ
}

void loop()
{    but1=digitalRead(PD5);
      if(but1==LOW){
       _lcd1.setCursor(0, 0); // установка курсора на первую строку первый ряд для вывода информации
   _lcd1.print("Settings      "); // вывод "Opasniy uroven"
   but2=digitalRead(10);
   if(but2!=0)
  {
    _lcd1.setCursor(0, 1); // установка курсора на вторую строку первый ряд для вывода информации
      _lcd1.print(" normal temp "); // вывод "Opasniy uroven"
      _lcd1.print(t_normal);
      but3=digitalRead(8);
      if(but3==0)
      {
        t_normal=t_normal+1;
         }
         but4=digitalRead(9); 
       if(but4==0)
      {
        t_normal=t_normal-1;
        }
 }
    if(but2==0)
  {
    _lcd1.setCursor(0, 1); // установка курсора на вторую строку первый ряд для вывода информации
      _lcd1.print("normal vlazh "); // вывод "Opasniy uroven"
      _lcd1.print(h_normal);
      but3=digitalRead(8);
      if(but3==0)
      {
        h_normal=h_normal+1;
        } 
      but4=digitalRead(9);       
         if(but4==0)
      {
        h_normal=h_normal-1;
        }
        but1=digitalRead(PD5); 
 }
   delay(100);   }
  if(but1==HIGH){
 h = dht1.readHumidity();//чтение данных с датчика DHT11
 t = dht1.readTemperature();// чтение данных с датчика DHT11
 
  _lcd1.clear(); // очистка дисплея
  _lcd1.setCursor(0, 0); // установка курсора на первую строку первый ряд для вывода информации
  _lcd1.print("temp:");//вывод "temp" 
  _lcd1.print(t);// вывод  значения температуры с датчика DHT11 
  _lcd1.setCursor(0, 1); // установка курсора на вторую строку первый ряд для вывода информации
  _lcd1.print("vlazhn v:");
  _lcd1.print(h); // вывод "vlazhn v" и значения влажности с датчика DHT11 
  if(t>=t_normal-5){
  digitalWrite(PD2, LOW);
  }// установка низкого уровня напряжения на выводе 2 - подогрев
  if (t <t_normal-5) { 
    digitalWrite(PD2, HIGH); // установка высокого уровня напряжения на выводе 2 - подогрев 
      }
    if(t<=t_normal+5){
      digitalWrite(PD3, LOW);
      }// установка низкого уровня напряжения на выводе 3 - охлаждение
 if(t>t_normal+5)
    {
      digitalWrite(PD3, HIGH);// установка высокого уровня напряжения на выводе 3 - охлаждение
    } 
  delay(1000); // задержка 1 с
 
   val2 = analogRead(analog);//чтение данных с поплавкового датчика уровня воды
   z = map(val2,0,1023,0,100);
  _lcd1.clear(); // очистка дисплея
  _lcd1.setCursor(0, 0); // установка курсора на первую строку первый ряд для вывода информации
  _lcd1.print("urov v:" );// вывод "urov v:"
  _lcd1.print(z);// вывод значения уровня воды в резервуаре
  
  delay(1000);// задержка 1 с
  if (val2 < 905) // если значение, считанное с датчика YL-38, меньше 30%
  { val = analogRead(analogYL38);// Считываем значение с контакта A3
_lcd1.clear(); // очистка дисплея
  _lcd1.setCursor(0, 0); // установка курсора на первую строку первый ряд для вывода информации
   _lcd1.print("Soil M:"); // вывод "Opasniy uroven"
    z = 100 - val / 10.24; // вычисление значения влажности
    _lcd1.print(z); // вывод "Opasniy uroven"
     if(z>=(h_normal-5)) {
      _lcd1.setCursor(0, 1); // установка курсора на вторую строку первый ряд для вывода информации
      _lcd1.print("norm ,pol OFF"); // вывод "Opasniy uroven"
      digitalWrite(PD1, LOW);//установка низкого уровня напряжения на выводе 1 - помпа
      delay(1000);
      soilOn=0;
    }
    
    if (z < (h_normal-5)) // если значение, считанное с датчика YL-38, меньше 30%
    {if(soilOn==0){soilOn=1;timer=0;}
    if(soilOn==1){timer=timer+1;}
      _lcd1.setCursor(0, 1); // установка курсора на вторую строку первый ряд для вывода информации
      _lcd1.print("low soilM level"); // вывод "Opasniy uroven"
      if(soilOn==1&&timer<5) 
      {digitalWrite(PD1, HIGH);//установка высокого уровня напряжения на выводе 1 - помпа
      }
      else{digitalWrite(PD1, LOW);//установка низкого уровня напряжения на выводе 1 - помпа
      }
      if(timer==105){
        soilOn=0;
        }
       delay(1000);
    }
    if(z>(h_normal+7))
    {
       _lcd1.setCursor(0, 1); // установка курсора на вторую строку первый ряд для вывода информации
      _lcd1.print("Excess SoilM lvl"); // вывод "Excess SoilM lvl"
       digitalWrite(PD1, LOW);//установка низкого уровня напряжения на выводе 1 - помпа
      for ( i = 0; i < 5; i++ ) // цикл для проигрывания мелодии
      {
        digitalWrite(12, HIGH); // установка высокого уровня напряжения на выводе 12 - светодиод
        delay( 1500 ); // задержка 1500 мс
        digitalWrite(12, LOW); // установка низкого уровня напряжения на выводе 12 - светодиод
        delay( 1500 ); // задержка 1500 мс
      }
      }
  }
 else {
      _lcd1.setCursor(0, 1); // установка курсора на вторую строку первый ряд для вывода информации
      _lcd1.print("low water lvl"); // вывод "low water lvl"
       digitalWrite(PD1, LOW);//установка низкого уровня напряжения на выводе 1 - помпа
      for ( i = 0; i < 5; i++ ) // цикл для проигрывания мелодии
      {
        digitalWrite(12, HIGH); // установка высокого уровня напряжения на выводе 12 - светодиод
        delay( 500 ); // задержка 500 мс
        digitalWrite(12, LOW); // установка низкого уровня напряжения на выводе 12 - светодиод
        delay( 500 ); // задержка 500 мс
      }
    }but1=digitalRead(PD5); }


}
