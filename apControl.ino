


#include "Quadrature.h" //Keith's Quadrature library that I modified to work with Arduino 1.0+ http://www.neufeld.newton.ks.us/electronics/?page_id=249
#include <TM1638.h> // required because the way arduino deals with libraries
#include <TM1640.h> //http://code.google.com/p/tm1638-library/


// define a module on data pin 3, clock pin 2 and another on data 5, and clock 4, display true, intensity 3 out of 7 to lower ma's on
// the Arduino voltage regulator
TM1640 module1(3, 2, true, 2);
TM1640 module2(5, 4, true, 2);

//define rotary encoders
Quadrature NavRadioSmallEnc(54, 55);
Quadrature NavRadioBigEnc(56, 57);
Quadrature Crs2Enc(58, 59);
Quadrature AltEnc(60, 61);
Quadrature HdgEnc(62, 63);
Quadrature Crs1Enc(64, 65);
Quadrature SpdEnc(66, 67);
Quadrature VsEnc(68, 69);


int j1;
int i;

int pinState;
int pinNo; 
int doutpin;

const char RATE = 2; // You may need to change this depending on the encoders you have. 



long AltPosition = -999, HdgPosition = -999, CrsPosition = -999, Crs2Position = -999, VsPosition = -999, SpdPosition = -999, NavRadioBigPosition = -999, NavRadioSmallPosition = -999;

String oldpinStateSTR, pinStateSTR, stringnewstate, stringoldstate, aphdgset, aphdgsetold, apcrs, apcrsold, output, outputold, apalt, apaltold, apairspeed, apairspeedold, apmachset, apmachsetold;
String nav1, nav1old, nav2, nav2old, output2, output2old, apvs, apvsold, apActive, apActiveOld, machIas, machIasOld, altLock, altLockOld, headingLock, headingLockOld, atArmed, atArmedOld;
String apSpdAct, apSpdActOld, apAprAct, apAprActOld, apNavAct, apNavActOld;

void setup() 
{



  Serial.begin(115200);  
  for (int doutPin = 15; doutPin <= 53; doutPin++)  
  {  
    pinMode(doutPin, INPUT);
    digitalWrite(doutPin, HIGH);  
  }  
  stringoldstate = "11111111111111111111111111111111111111";  // 53 - 15 = this number of 1's

  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);
  Serial.flush();

}



void loop() {
  // read the oldest byte in the serial buffer:
  if (Serial.available() > 0) {
    j1 = Serial.read();
    //Serial.println(j1);
    if (j1 == 'r'){     // Found the reading "autopilot heading set"
      delay (11); // It seems to need a delay here
      aphdgset = "";
      aphdgset +=(char)Serial.read();
      aphdgset += (char)Serial.read();
      aphdgset += (char)Serial.read();

      if (aphdgset != aphdgsetold){  // checks to see if its different to the "old" reading

        aphdgsetold = aphdgset; // Writes the current reading to the "old" string.
      }        
    }

    if (j1 == 'a'){     // Found the reading "crs"
      delay (11);
      apcrs ="";
      apcrs += (char)Serial.read();
      apcrs += (char)Serial.read();
      apcrs += (char)Serial.read();


      if (apcrs != apcrsold){

        apcrsold = apcrs;
      }        
    } 
    if(j1 == 'b'){ // Found the first charecter of AP ACTIVE
      delay(11);
      apActive = "";
      apActive += (char)Serial.read();
      if(apActive != apActiveOld){
        if(apActive == "1")
          digitalWrite(7, HIGH);
        if(apActive == "0")
          digitalWrite(7, LOW);
      }
      apActiveOld = apActive;  
    }
    if (j1 == 'u'){     // Found the reading "AP airspeed "
      delay (11);
      apairspeed ="";
      apairspeed += (char)Serial.read();
      apairspeed += (char)Serial.read();
      apairspeed += (char)Serial.read();


      if (apairspeed != apairspeedold){

        apairspeedold = apairspeed;
      }        
    } 
    if (j1 == 'q'){     // Found the reading "AP Vertical Speed set"
      delay (11);
      apvs ="";
      apvs += (char)Serial.read();
      apvs += (char)Serial.read();
      apvs += (char)Serial.read();
      apvs += (char)Serial.read();
      apvs += (char)Serial.read();

      if (apvs != apvsold){

        apvsold = apvs;
      }        
    } 

    if (j1 == 'p'){ //  Found the reading "AP ALT"
      delay (11);
      apalt = "";
      apalt += (char)Serial.read();
      apalt += (char)Serial.read();
      apalt += (char)Serial.read();
      //apalt += (char)Serial.read(); // if you want the trailering zero's, uncomment these lines
      //apalt += (char)Serial.read();

      if (apalt != apaltold){

        apaltold = apalt;
      }
    }
    if (j1 == 'h'){ //  Found the reading "Nav1 Radio"
      delay (11);
      nav1 = "";
      nav1 += (char)Serial.read();
      nav1 += (char)Serial.read();
      nav1 += (char)Serial.read();
      nav1 += (char)Serial.read(); 
      nav1 += (char)Serial.read();
      nav1 += (char)Serial.read();

      nav1.replace(".","");
      if (nav1 != nav1old){

        nav1old = nav1;
      }
    }
    if (j1 == 'i'){ //  Found the reading "Nav2 Radio"
      delay (11);
      nav2 = "";
      nav2 += (char)Serial.read();
      nav2 += (char)Serial.read();
      nav2 += (char)Serial.read();
      nav2 += (char)Serial.read(); 
      nav2 += (char)Serial.read();
      nav2 += (char)Serial.read();

      nav2.replace(".","");
      if (nav2 != nav2old){

        nav2old = nav2;
      }
    }
    if (j1 == 's'){ //  Found the reading "altitude lock active"
      delay (11);
      altLock = "";
      altLock += (char)Serial.read();

      if (altLock != altLockOld){
        if(altLock == "1")
          digitalWrite(8, HIGH);
        else
          digitalWrite(8, LOW);

        altLockOld = altLock;
      }
    }
    if (j1 == 't'){ //  Found the reading "heading lock active"
      delay (11);
      headingLock = "";
      headingLock += (char)Serial.read();

      if (headingLock != headingLockOld){
        if(headingLock == "1")
          digitalWrite(9, HIGH);
        else
          digitalWrite(9, LOW);

        headingLockOld = headingLock;
      }
    }
    if (j1 == 'v'){ //  Found the reading "Approach Hold Active"
      delay (11);
      apAprAct = "";
      apAprAct += (char)Serial.read();

      if (apAprAct != apAprActOld){
        if(apAprAct == "1")
          digitalWrite(12, HIGH);
        else
          digitalWrite(12, LOW);

        apAprActOld = apAprAct;
      }
    }
    if (j1 == 'x'){ //  Found the reading "Approach Hold Active"
      delay (11);
      apNavAct = "";
      apNavAct += (char)Serial.read();

      if (apNavAct != apNavActOld){
        if(apNavAct == "1")
          digitalWrite(15, HIGH);
        else
          digitalWrite(15, LOW);

        apNavActOld = apNavAct;
      }
    }
    if (j1 == '>'){ //  Found the first character of a dual input
      //delay (11);
      j1 = Serial.read();
      //Serial.println(j1);
      if(j1 == 'A'){ // Found the first charecter of AP MACH SET 65 is for A
        delay (11);
        apmachset = "";
        apmachset += (char)Serial.read();
        apmachset += (char)Serial.read();
        apmachset += (char)Serial.read();
        apmachset += (char)Serial.read();

        apmachset.replace(".","");
        if (apmachset != apmachsetold){

          apmachsetold = apmachset;
        }

      }
      if(j1 == 'B'){ // Found the first charecter of AP MACH ACTIVE
        delay(11);
        machIas = "";
        machIas += (char)Serial.read();
        if(machIas != machIasOld){
          if(machIas == "1"){
            digitalWrite(14, HIGH);
            i = 1;
          }
          else{
            i = 0;
            digitalWrite(14, LOW);
          }

          machIasOld = machIas;
        }

      }
      if(j1 == 'D'){ // Found the first charecter of AP Speed Active
        delay(11);
        apSpdAct = "";
        apSpdAct += (char)Serial.read();
        if(apSpdAct != apSpdActOld){
          if(apSpdAct == "1"){
            digitalWrite(11, HIGH);
            //Serial.println("AT ARMED"); 
          }
          else{
            digitalWrite(11, LOW);
            //Serial.println("AT DISARMED");
          }
          apSpdActOld = apSpdAct;
        }

      }

      if(j1 == 'E'){ // Found the first charecter of AT Armed 69 is for E
        delay(11);
        atArmed = "";
        atArmed += (char)Serial.read();
        if(atArmed != atArmedOld){
          if(atArmed == "1"){
            digitalWrite(10, HIGH);
            //Serial.println("AT ARMED"); 
          }
          else{
            digitalWrite(10, LOW);
            //Serial.println("AT DISARMED");
          }
          atArmedOld = atArmed;
        }

      }

    }




    //Display LED Section
    if(i == 1){

      output = apcrsold;
      output += " ";
      output += apmachsetold;
      output += " ";
      output += aphdgsetold;
      output += " ";
      output += apaltold;

      output2 = apvsold;
      output2 += " ";
      output2 += apcrsold;
      output2 += " ";
      output2 += nav1old; 



      if (output != outputold || output2 != output2old){
        module1.setDisplayToString(output,0b0000100000000000);
        module2.setDisplayToString(output2, 0b0000000000001000); 
        outputold = output;
        output2old = output2;
      }

    }
    else{
      output = apcrsold;
      output += " ";
      output += apairspeedold;
      output += " ";
      output += aphdgsetold;
      output += " ";
      output += apaltold;

      output2 = apvsold;
      output2 += " ";
      output2 += apcrsold;
      output2 += " ";
      output2 += nav1old;


      if (output != outputold || output2 != output2old){
        module1.setDisplayToString(output);
        module2.setDisplayToString(output2, 0b0000000000001000);
        outputold = output;
        output2old = output2;
      }


    }


  }

  // Now the bit for the "keys" input

  stringnewstate = "";
  for (int pinNo = 15; pinNo <= 53; pinNo++){ // changed to avoid problems with rotary enc

    pinState = (digitalRead(pinNo)); 
    pinStateSTR = "";
    pinStateSTR += pinState;
    oldpinStateSTR = "";
    oldpinStateSTR += stringoldstate.charAt(pinNo-15);


    if (pinStateSTR != oldpinStateSTR)
    {
      Serial.print ("D"); 
      //if (pinNo < 10) Serial.print ("0");
      Serial.print (pinNo);
      Serial.print (pinStateSTR);
      delay(11);
    }
    stringnewstate += pinState;
  }

  stringoldstate = stringnewstate;
  //delay(11);

  // Altitude Encoder Output
  long newAltPosition = AltEnc.position()/RATE;
  if (newAltPosition != AltPosition) {
    if(AltPosition > newAltPosition){

      Serial.print("D610");


    }
    if(AltPosition < newAltPosition){
      Serial.print("D600");
    }
    AltPosition = newAltPosition;
  }

  //Heading Encoder Output
  long newHdgPosition = HdgEnc.position()/RATE;

  if (newHdgPosition != HdgPosition) {
    if(HdgPosition > newHdgPosition){

      Serial.print("D630");

    }
    if(HdgPosition < newHdgPosition){


      Serial.print("D620");
    }
    HdgPosition = newHdgPosition;
  }

  //Course1 Encoder Output

  long newCrsPosition = Crs1Enc.position()/RATE;

  if (newCrsPosition != CrsPosition) {
    if(CrsPosition > newCrsPosition){
      Serial.print("D650");
    }
    if(CrsPosition < newCrsPosition){
      Serial.print("D640");
    }   
    CrsPosition = newCrsPosition;
  }
  //Speed Encoder Output
  long newSpdPosition = SpdEnc.position()/RATE;
  if (newSpdPosition != SpdPosition) {
    if(SpdPosition > newSpdPosition){
      if(i ==0)
        Serial.print("D670");
      else
        Serial.print("D910"); //mach set
    }
    if(SpdPosition < newSpdPosition){
      if(i == 0)
        Serial.print("D660");
      else
        Serial.print("D900");
    }

    SpdPosition = newSpdPosition;
  }
  //Vertical Speed Encoder Output
  long newVsPosition = VsEnc.position()/RATE;
  if (newVsPosition != VsPosition) {

    if(VsPosition > newVsPosition){
      Serial.print("D690");
    }
    if(VsPosition < newVsPosition){
      Serial.print("D680");
    }

    VsPosition = newVsPosition;
  }
  //Crs2 Encoder Output
  long newCrs2Position = Crs2Enc.position()/RATE;
  if (newCrs2Position != Crs2Position) {

    if(Crs2Position > newCrs2Position){
      Serial.print("D590");
    }
    if(Crs2Position < newCrs2Position){
      Serial.print("D580");
    }

    Crs2Position = newCrs2Position;
  }
  //NavRadioBig Encoder Output
  long newNavRadioBigPosition = NavRadioBigEnc.position()/RATE;
  if (newNavRadioBigPosition != NavRadioBigPosition) {

    if(NavRadioBigPosition > newNavRadioBigPosition){
      Serial.print("D570");
    }
    if(NavRadioBigPosition < newNavRadioBigPosition){
      Serial.print("D560");
    }

    NavRadioBigPosition = newNavRadioBigPosition;
  }
  //NavRadioSmall Encoder Output
  long newNavRadioSmallPosition = NavRadioSmallEnc.position()/RATE;
  if (newNavRadioSmallPosition != NavRadioSmallPosition) {

    if(NavRadioSmallPosition > newNavRadioSmallPosition){
      Serial.print("D550");
    }
    if(NavRadioSmallPosition < newNavRadioSmallPosition){
      Serial.print("D540");
    }

    NavRadioSmallPosition = newNavRadioSmallPosition;
    delay(11);
  }

}
























































