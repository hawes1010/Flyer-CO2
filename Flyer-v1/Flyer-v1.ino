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

String co2 = CO2_read();
Serial.println(co2); // CO2 xml value starts at substring 82 till 93 of this string.
//String val = co2.substring(82, 93);
//Serial.println(val);
delay(10000);// this controls when 
// next create a zero command and a span command
}

String CO2_read(){
// Send request to LICOR
Serial1.print("<LI820><DATA>?</DATA></LI820>");
String co2 = Serial1.readString();
String co2_2 = Serial1.readString();
co2 = co2+co2_2;
String val = co2.substring(18, 29); // you 
return val;
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
