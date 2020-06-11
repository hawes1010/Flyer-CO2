long interval = 75000;
String date = "";
void setup() {
  // put your setup code here, to run once:
Serial1.begin(9600);
Serial.begin(9600);
Serial1.setTimeout(2000);
 
CO2_config();
delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print(Serial.read());
if (Serial.available()>0){
char comm = Serial.read();
Serial.println(comm);
switch (comm) {
  case 'C': //Calibrate the sensor
    Calibrate();
    break;
  case 'c': //CO2 Current data
    CO2_read();
    break;
  case 's': // Single Span
   ask_value(1);
    break;
   case 't': // Two Span
   ask_value(2);
    break;
   case 'z': // Zero Span
   zero();
    break;
  default:
     Serial.println("Invalid command, try again");
    break;
}
}
delay(100);

}

void ask_value(int span){

if (span == 1){
Serial.println("Please input the value for the span");
while (!Serial.available());
long span1 = Serial.parseInt();
single_span(span1);
}
if (span == 2){
Serial.println("Please input the 1st value for the span");
 while (!Serial.available());
long span1 = Serial.parseInt();
Serial.println("Please input the 2nd value for the span");
while (!Serial.available());
long span2 = Serial.parseInt();
double_span(span1,span2);
}
 
}


void CO2_read(){
// Send request to LICOR
Serial1.print("<LI820><DATA>?</DATA></LI820>");
String co2 = Serial1.readString();
String co2_2 = Serial1.readString();
co2 = co2+co2_2;
String val = co2.substring(18, 29); // you 
Serial.println(val);
}


bool check_ack(String s){
if (s == "<li820><ack>true</ack></li820>")
return true;
else 
return false; 
}


void CO2_config(){
  Serial1.print("<LI820>\n<CFG>\n<OUTRATE>0</OUTRATE>\n</CFG>\n");
  Serial1.print("<RS232>\n");
  Serial1.print("<STRIP>FALSE</STRIP>\n");
  Serial1.print("<ECHO>FALSE</ECHO>\n");
  Serial1.print("<CELL TEMP>FALSE</CELL TEMP>\n");
  Serial1.print("<CO2>TRUE</CO2>\n<CO2ABS>FALSE</CO2ABS>\n<CELLPRES>FALSE</CELLPRES>\n<IVOLT>FALSE</IVOLT>\n");
  Serial1.print("</RS232>\n</LI820>\n");
  Serial.println(Serial1.readString());
}
void Calibrate(){
Serial1.print("<LI820>\n<CAL>\n<DATE>{iso date}</DATE>\n<CO2ZERO>{bool}</CO2ZERO>\n<CO2SPAN>{int}</CO2SPAN>\n");
Serial1.print("<CO2SPAN_A>{int}</CO2SPAN_A>\n<CO2SPAN_B>{int}</CO2SPAN_B>\n</CAL>\n</LI820>\n");
String response = Serial1.readString();
if (!check_ack(response)){
Serial.println("ERROR with Calibration");
return;
}
Calibrate_time();
String cal_1 = Serial1.readString();
String cal_2 = Serial1.readString();
String Cal = cal_1+cal_2;
// wait about a minute for calibration to finish
}



void Calibrate_time(){
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();

  while(currentMillis - previousMillis < interval) {
   currentMillis = millis();
    if (Serial1.available()){
     return;
    }
  }
  Serial.println( "75 seconds have passed for calibration");
}

void zero_time(){
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();

  while(currentMillis - previousMillis < 10000 ) {
   currentMillis = millis();
    if (Serial1.available()){
     return;
    }
  }
  Serial.println( "10 seconds have passed for zero/span");
}

void zero(){
  String date = "2020-06-11";
  char buf[180];
sprintf(buf, "<LI820>\n<CAL>\n<DATE>%s</DATE>\n<CO2ZERO>TRUE</CO2ZERO>\n</CAL>\n</LI820>", date);
Serial1.println(buf);
//Serial1.print("<LI820>\n<CAL>\n<DATE>YYYY-MM-DD</DATE>\n<CO2ZERO>TRUE</CO2ZERO>\n</CAL>\n</LI820>");
zero_time();
String zero1 = Serial1.readString();
Serial.println(zero1);
}

void single_span(int span){
String date = "2020-06-11";
  char buf[180];
sprintf(buf, "<LI820>\n<CAL>\n<DATE>%s</DATE>\n<CO2SPAN>%i</CO2SPAN>\n</CAL>\n</LI820>", date,span);
Serial1.print(buf);
zero_time();
String spans = Serial1.readString();
Serial.println(spans);
}

void double_span(int span1, int span2){
String date = "2020-06-11";
  char buf[180];
sprintf(buf, "<LI820>\n<CAL>\n<DATE>%s</DATE>\n<CO2SPAN_A>%i</CO2SPAN_A>\n</CAL>\n</LI820>", date,span1);
Serial1.print(buf);
zero_time();
String confirm = Serial1.readString();
bool span_success = check_ack(confirm);
if (span_success){
char buf2[180];
sprintf(buf2, "<LI820>\n<CAL>\n<DATE>%s</DATE>\n<CO2SPAN_B>%i</CO2SPAN_B>\n</CAL>\n</LI820>", date,span2);
Serial1.print(buf2);

}
}
