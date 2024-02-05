long Number;

String inputChar;

void setup(){
  Serial.begin(115200);  
}

void loop(){  
  SerialEvent();  
  }

void SerialEvent(){
  while(Serial.available()){
    inputChar = Serial.readString();    
    Number = inputChar.toInt(); 
    Serial.println(Number);  
  }    
}
