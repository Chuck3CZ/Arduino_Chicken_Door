///////////////////////////--     NASTAVENÍ PROMĚNNÝCH     --//////////////////////////////////

    #include "Wire.h"

     // Založení objektu reálného času   
    int setHours = 6, setMinutes = 00, setSeconds = 00; // nastavení hodin, minut and vteřin pro zapnutí relé = jede nahoru
    int setHours1 = 21, setMinutes1 = 07, setSeconds1 = 00; // nastavení hodin, minut and vteřin pro zapnutí relé = jede nahoru
    
    
    long RelayTime1 = 3100; //délka zapnutí 1. relé v ms. datatyp int udrží maximálně hodnotu 32767.
    long RelayTime2= 1750; //délka zapnutí 2. relé v ms. datatyp int udrží maximálně hodnotu 32767.
    
    const int RelayA = 9;   // definování výstupního kontaktu pro 1. relé 
    const int RelayB = 10;  // definování výstupního kontaktu pro 2. relé 
    
    unsigned long interval = 2000;  // použito pro čtení každých 2000 ms ( 2s )
    unsigned long time_now = 0;

#define DS3231_I2C_ADDRESS 0x68
#define DS3231_TEMPERATURE_ADDR 0x11


// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
    return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
    return( (val/16*10) + (val%16) );
}

///////////////////////////--    ZAPNUTÍ     --//////////////////////////////////

      
    void setup() {
       Wire.begin();
        pinMode(RelayA, OUTPUT);   // definuje kontakt pro 1.relé jako výstupní
        pinMode(RelayB, OUTPUT);   // definuje kontakt pro 2.relé jako výstupní

        // Nastavte aktuální datum, a čas v následujícím formátu:                                                //      vteřiny         //
        //                                                                                                       //       minuty         //
        //                                                                                                       //       hodiny         //
        // vteřiny,   minuty,   hodiny,   den v týdnu (číslo),  den v měsíci,   měsíc,  rok                      //  den v týdnu (číslo) //
        //                                                                                                       //     den v měsíci     //
        //                                                                                                       //       měsíc          //
        //                                                                                                       //        rok           //
        //                                                                                                        
        // DS3231 seconds, minutes, hours, day, date, month, year
        //den v týdnu je počítán od neděle, neděle 1., pondělí 2., ...
        // setDS3231time(00,35,21,2,14,03,21);
        
      //kvůli blokačnímu vztahu mezi 1. a 2. relé je potřeba při startu programu jedno relé zapnout a druhé vypnout/ ponechat vypnuté 
      digitalWrite(RelayA, LOW);  //vypnutý stav / relé připojeno na NC / COM
      digitalWrite(RelayB, HIGH); //zapnutý stav / relé připojeno na NO / COM
        
        Serial.begin(9600); //zapne sériovou linku s přenosovou rychlostí 9600
        Serial.println("***********************************************************");
        Serial.println("Automatické otevírání slepic s relé a obvodem reálného času");
        Serial.println("***********************************************************");
      
        pinMode(LED_BUILTIN, OUTPUT); // nastaví digitální pin vestavěné diody na výstup ( pro účely testování )
    }


void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year){
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}


void displayTime(){
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    // send it to the serial monitor
     // začátek výpisu proměnné času                                                                   
     if (millis() - time_now >= interval)  
      {
        //time_now += interval;
        time_now = millis();
        Serial.print(hour);
    // convert the byte variable to a decimal number when displayed
    Serial.print(":");
    if (minute<10){
        Serial.print("0");
    }
    Serial.print(minute);
    Serial.print(":");
    if (second<10){
        Serial.print("0");
    }
    Serial.print(second);
    Serial.print(" ");
    Serial.print(dayOfMonth);
    Serial.print(".");
    Serial.print(month);
    Serial.print(".");
    Serial.print(year);
    Serial.print(" ");
    switch(dayOfWeek){
        case 1:
            Serial.println("Neděle");
            break;
        case 2:
            Serial.println("Pondělí");
            break;
        case 3:
            Serial.println("Úterý");
            break;
        case 4:
            Serial.println("Středa ");
            break;
        case 5:
            Serial.println("Čtvrtek");
            break;
        case 6:
            Serial.println("Pátek");
            break;
        case 7:
            Serial.println("Sobota");
            break;
    }                                                                        
      }                                                       
      
      if (hour == setHours && minute == setMinutes && second == setSeconds)
      {
        Serial.print("Motor ccw zapnut v: ");
        Serial.print(hour, DEC);  // zobrazí hodinovou hodnotu z rtc modulu
        Serial.print(":");                                                                                      
        Serial.print(minute, DEC);  // zobrazí minutovou hodnotu z rtc modulu
        Serial.print(":");                                                                                           
        Serial.println(second, DEC);  // zobrazí vteřinovou hodnotu z rtc modulu
                                                                                       
        digitalWrite(RelayA, HIGH);  //turns on relay
        digitalWrite(LED_BUILTIN, LOW);   //turns on the on-board LED (used for testing)
            
        Serial.println("*** relay turned ON ***");
        Serial.print(" operation time: ");
        Serial.print(RelayTime1 / 1000); // calculate time of operation in second      
        Serial.println(" seconds");
        delay(RelayTime1); // relay operation time
        digitalWrite(RelayA, LOW); // turn off the relay
        digitalWrite(LED_BUILTIN, HIGH);  // turn off the on-board LED (used for testing only)
        Serial.println("***  relay turned OFF ***");                                                                                                                    
      }
    if (hour == setHours1 && minute == setMinutes1 && second == setSeconds1)
      {
        Serial.print("Motor cw zapnut v: ");
        Serial.print(hour, DEC);  // zobrazí hodinovou hodnotu z rtc modulu
        Serial.print(":");                                                                                      
        Serial.print(minute, DEC);  // zobrazí minutovou hodnotu z rtc modulu
        Serial.print(":");                                                                                           
        Serial.println(second, DEC);  // zobrazí vteřinovou hodnotu z rtc modulu
                                                                                       
        digitalWrite(RelayB, LOW);  //turns on the  relay
        digitalWrite(LED_BUILTIN, LOW);   //turns on the on-board LED (used for testing)
            
        Serial.println("*** relay turned ON ***");
        Serial.print(" operation time: ");
        Serial.print(RelayTime2 / 1000); // calculate time of operation in second      
        Serial.println(" seconds");
        delay(RelayTime2); //  relay operation time
        digitalWrite(RelayB, HIGH); // turn off the  relay
        digitalWrite(LED_BUILTIN, HIGH);  // turn off the on-board LED (used for testing only)
        Serial.println("*** relay turned OFF ***");                                                                                                                    
      }
}
 

///////////////////////////--NASTAVENÍ SMYČKY--//////////////////////////////////




  void loop() {
      displayTime(); // aktualizuje proměnou času                                                                               
  }
