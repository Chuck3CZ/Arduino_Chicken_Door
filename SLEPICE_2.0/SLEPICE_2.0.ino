
///////////////////////////--     NASTAVENÍ PROMĚNNÝCH     --//////////////////////////////////
    
    #include <virtuabotixRTC.h> //KNIHOVNAA MODULU REÁLNÉHO ČASU RTC  DS1302 
    
    // Založení objektu reálného času
    virtuabotixRTC myRTC(6, 7, 8);  // Zapojení výstupu RTC (CLK,DAT,RST)
    
    int setHours  =  5, setMinutes = 30, setSeconds = 00; // nastavení hodin, minut and vteřin pro zapnutí relé = jede nahoru
    
    int setHours1 = 22, setMinutes1 = 0, setSeconds1 = 0; // nastavení hodin, minut and vteřin pro zapnutí relé = jede nahoru
    
    
    long RelayTime1 = 3100; //délka zapnutí 1. relé v ms. datatyp int udrží maximálně hodnotu 32767.
    
    long RelayTime2= 1750; //délka zapnutí 2. relé v ms. datatyp int udrží maximálně hodnotu 32767.
    
    const int RelayA = 9;   // definování výstupního kontaktu pro 1. relé 
    const int RelayB = 10;  // definování výstupního kontaktu pro 2. relé 
    
    unsigned long interval = 2000;  // použito pro čtení každých 2000 ms ( 2s )
    unsigned long time_now = 0;


///////////////////////////--    ZAPNUTÍ     --//////////////////////////////////

      
    void setup() {
        pinMode(RelayA, OUTPUT);   // definuje kontakt pro 1.relé jako výstupní
        pinMode(RelayB, OUTPUT);   // definuje kontakt pro 2.relé jako výstupní
        
            //kvůli blokačnímu vztahu mezi 1. a 2. relé je potřeba při startu programu jedno relé zapnout a druhé vypnout/ ponechat vypnuté 
            digitalWrite(RelayA, LOW);  //vypnutý stav / relé připojeno na NC / COM
            digitalWrite(RelayB, HIGH); //zapnutý stav / relé připojeno na NO / COM
        
        Serial.begin(9600); //zapne sériovou linku s přenosovou rychlostí 9600
        Serial.println("***********************************************************");
        Serial.println("Automatické otevírání slepic s relé a obvodem reálného času");
        Serial.println("***********************************************************");
      
        pinMode(LED_BUILTIN, OUTPUT); // nastaví digitální pin vestavěné diody na výstup ( pro účely testování )
        
        // Nastavte aktuální datum, a čas v následujícím formátu:                                                //      vteřiny         //
        //                                                                                                       //       minuty         //
        //                                                                                                       //       hodiny         //
        // vteřiny,   minuty,   hodiny,   den v týdnu (číslo),  den v měsíci,   měsíc,  rok                      //  den v týdnu (číslo) //
        //                                                                                                       //     den v měsíci     //
        //                                                                                                       //       měsíc          //
        //                                                                                                       //        rok           //
        //                                                                                                        
           
        myRTC.setDS1302Time(00, 37, 14, 4, 01, 7, 2021); // Okomentovat, nahrát pro zresetování RTC a znovu zakomentovat. poté nahrát se zakomentováním aby se při startu nemazala čahová hodnota
        
        myRTC.updateTime(); // funkce aktualizuje čas - vyčtení z RTC a dočasné uložení do paměti
        
        // Start printing elements as individuals                                                                 
        Serial.print("Program běži s nastavením RTC Data a času: ");                                                                  
        Serial.print(myRTC.dayofmonth);                                                                         
        Serial.print("/");                                                                                      
        Serial.print(myRTC.month);                                                                              
        Serial.print("/");                                                                                      
        Serial.print(myRTC.year);
        Serial.print(" ");             
        Serial.print("-");             
        Serial.print(" ");                                                                                      
        Serial.print(myRTC.hours);                          
        Serial.print(":");                                                                                      
        Serial.print(myRTC.minutes);                                                                            
        Serial.print(":");                                                                                      
        Serial.println(myRTC.seconds); 
    }


///////////////////////////--NASTAVENÍ SMYČKY--//////////////////////////////////


  void loop() {
      myRTC.updateTime(); // aktualizuje proměnou času                                                                                 
    
      // začátek výpisu proměnné času                                                                   
     if (millis() - time_now >= interval)  
      {
        //time_now += interval;
        time_now = millis();
        Serial.print("RTC Time: ");                                                                                                                                                    
        Serial.print(myRTC.hours);                                                                              
        Serial.print(":");                                                                                      
        Serial.print(myRTC.minutes);                                                                            
        Serial.print(":");                                                                                      
        Serial.println(myRTC.seconds);                                                                          
      }                                                       
      
      if (myRTC.hours == setHours && myRTC.minutes == setMinutes && myRTC.seconds == setSeconds)
      {
        Serial.print("Motor zapnut v: ");
        Serial.print(myRTC.hours);  // zobrazí hodinovou hodnotu z rtc modulu
        Serial.print(":");                                                                                      
        Serial.print(myRTC.minutes);  // zobrazí minutovou hodnotu z rtc modulu
        Serial.print(":");                                                                                           
        Serial.println(myRTC.seconds);  // zobrazí vteřinovou hodnotu z rtc modulu
                                                                                       
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
    if (myRTC.hours == setHours1 && myRTC.minutes == setMinutes1 && myRTC.seconds == setSeconds1)
      {
        Serial.print(" activated at: ");
        Serial.print(myRTC.hours);  // zobrazí hodinovou hodnotu z rtc modulu                                                                          
        Serial.print(":");                                                                                      
        Serial.print(myRTC.minutes);  // zobrazí minutovou hodnotu z rtc modulu                                                                             
        Serial.print(":");                                                                                           
        Serial.println(myRTC.seconds);  // zobrazí vteřinovou hodnotu z rtc modulu
                                                                                       
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
