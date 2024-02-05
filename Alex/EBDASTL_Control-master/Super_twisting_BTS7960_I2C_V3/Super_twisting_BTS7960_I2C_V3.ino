#include <Wire.h>
#include <AS5600.h>

const unsigned int MAX_LENGTH = 6;

AMS_5600 as5600;

//Pin de PWM
const int PinMotorPWM1 = 0;
const int PinMotorPWM2 = 2;

//Contantes Super Twisting
const float C1 = 400;
const float C2 = 0.7;//0.7;
const float b = 2;//200;

//Variables auxiliares
float posx = 0;
float velx = 0;
float tiempo = 0;
float voltaje = 0;
float posant = 0;
float dt = 0.001; //Tiempo de muestreo
int PWM = 0;

//Variables Super Twisting
float error = 0;
float errorant = 0;
float deriv = 0;
float sigma = 0;
float sign = 0;
float W = 0;
float Wp = 0;
float epsilon = 1000;

//Variables As5600
float AngMin = 10;
float AngMax;
float AngMax_s;
float Zero = 0;
float posDes = 0;
String lasResponse;
String noMagnetStr = "Error; magnet no detected";

//Comunicacion serial
//String inputString;
char inputString;
char inputChar[MAX_LENGTH];
float Data = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); //Unirse al bus 12c como master
  Serial.begin(115200); //Inicio comunicacion serial

  //I2C COMMUNICATION

  //Deteccion del encoder magnetico
  if(as5600.detectMagnet() == 0){
    while(1){
      if(as5600.detectMagnet() == 1){
        Serial.print("Magnitud Actual: ");
        Serial.println(as5600.getMagnitude());
        break;
      }
      else{
        Serial.println("No se detecta iman");
      }
      delay(1000);
    }
  }
  /*modo = 0, Salida PWM, modo = 1, Salida analogica 
  (rango completo de 0 a 100% entre GND y VDD*/
  as5600.setOutPut(0);

  //Salida de los pines driver motor
  pinMode(PinMotorPWM1, OUTPUT);
  pinMode(PinMotorPWM2, OUTPUT);

}

//Conversion de datos de angulo crudo a grados
float convertRawAngleToDegrees(word newAngle)
{
  float retVal = newAngle * 0.087; //0.087= 360/4095 <- Conversion de cuentas a grados
  return retVal;
}

void loop() {  
  //Inicialisacion de  tiempo
  tiempo = millis();
  
  //Definir posicion deseada
    //Comunicacion serial
  if(Serial.available()){
    while(Serial.available()>0){
      //inputString = Serial.read(); 
      inputString = Serial.readBytes(inputChar,MAX_LENGTH);       
      if(inputChar[0] ='v'){
         Data = int(inputChar[1]-48)*10000 + int(inputChar[2]-48)*1000 + int(inputChar[3]-48)*100 + int(inputChar[4]-48)*10 + int(inputChar[5]-48);  
         Data = Data/100000;
         //Serial.println(Data);         
        }
        else{          
          Data = 0;
          //Serial.print(" Serial  ");
          //Serial.println(inputChar);  
        }
    }    
    
//Conversion de serial a angulo       
    AngMax_s = Data;
    AngMax = AngMax_s*150;
    if(AngMax>=150){
      AngMax = 150;
    }     
  }
  if(AngMax < AngMin){
    AngMax = AngMin;      
  }
  
  //Asignacion del valor a posicion deseada    
  posDes= as5600.setEndPosition(AngMax);

  //Leer posicion y velocidad angular
  posx = convertRawAngleToDegrees(as5600.getRawAngle());
  velx = (posx - posant);

//CONTROL

  //Super Twisting
    //Signo
  if(sigma>0){
    sign = 1;
  }
  else if(sigma<0){
    sign = -1;
  }
  else{
    sign = 0;
  }

    //Tangente Hiperbolica
  //sign=tanh(epsilon*error);
  
    //Ecuaciones de super twisting
  error = posDes - posx;
  W = b*sign*dt + Wp;
  deriv = (error-errorant)/dt;
  sigma = C1*error + deriv;

  //Asignacion de voltaje para control de motor
  voltaje = C2*sqrt(abs(sigma))*sign + W;

  //Conversion de voltaje-PWM
  if(voltaje>0){
    PWM = map(abs(voltaje),0,12,0,255);
  }
  else if(voltaje<0){
    PWM = map(abs(voltaje),0,12,-255,0);
  }
  else{
    PWM = 0;
  }  

  //Obtencion de datos anteriores
  errorant = error;
  Wp = W;
  posant = posx;
  
  //Limitacion de parametros
  if(voltaje>12){
    voltaje = 12;
  }
  if(voltaje<-12){
    voltaje = -12;
  }
  if(PWM > 255){
    PWM = 255;
  }
  if(PWM <-255){
    PWM = -255;
  }

  //Seleccion de giro de motor
  if(voltaje >= 0){
    analogWrite(PinMotorPWM1,PWM);
    analogWrite(PinMotorPWM2,0);
  }
  if(voltaje < 0){
    analogWrite(PinMotorPWM1,0);
    analogWrite(PinMotorPWM2,PWM);
  }

  //Impresion de datos
  Serial.print("PosX  ");
  Serial.print(posx);
  Serial.print(";");
  Serial.print(" PWM  ");
  Serial.print(PWM);
  Serial.print(";");
  Serial.print("  Error ");
  Serial.print(error);
  Serial.print(";");
  Serial.print(" Serial  ");
  Serial.print(AngMax_s); 
  Serial.print(";");
  Serial.print(" Time  ");
  Serial.println(tiempo/1000); 


  while((millis()-tiempo)<(1000*dt)){    
  }

}
