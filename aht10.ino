// █████████████████████████████████████████████████████
//
// ESP32/ESP8266 BlackBook by PowerTGS (2024)
//
// author : Tomasz Majcher 
// e-mail : powerTGS440@gmail.com
// phone  : +48 668 082121
//
// █████████████████████████████████████████████████████
//
// DHT11 demo file
//
// source file      : sourcce/sensor/ath10/ath10.ino
// device tested    : esp32/esp2866
// code platform    : ArduinoIDE
// 
// █████████████████████████████████████████████████████
// W Y K O R Z Y S T A N I E    P A M I Ę C I   R A M   &   F L A S H
// █████████████████████████████████████████████████████
//
// Variables and constants in RAM (global, static), used 28604 / 80192 bytes (35%)
// Code in flash (default, ICACHE_FLASH_ATTR), used 246264 / 1048576 bytes (23%)

// █████████████████████████████████████████████████████
// M A K R O
// █████████████████████████████████████████████████████

#define SERIAL_MONITOR        true                          // SERIAL włączony
#define SERIAL_SPEED          115200                        // prędkość SERIAL
#define SETUP_DELAY           5000                          // opoznienie SETUP
#define LOOP_DELAY            10000                         // opóźnienie pętli LOOP 

#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#define F(string_literal) (FPSTR(PSTR(string_literal)))

// █████████████████████████████████████████████████████
// B I B L I O T E K I   A H T 1 0
// █████████████████████████████████████████████████████   
                       
#include <AHT10.h>
#include <Wire.h>

// █████████████████████████████████████████████████████
// O B I E K T   K L A S Y   D H T
// █████████████████████████████████████████████████████

AHT10 myAHT10(AHT10_ADDRESS_0X38);

// █████████████████████████████████████████████████████
// Z M I E N N E   G L O B A L N E
// █████████████████████████████████████████████████████

struct AHT_SENSOR                     // struktura dla czujnika AHT10
{
    float   temperatura_C = 0;        // zmienna dla przechowywania temp w st. C    
    float   wilgotnosc = 0;           // zmienna dla przechowywania wilgotnosci
}   aht_sensor;

bool F_Read_AHT_OK = false;         // flaga błedów dla ATH10 (odczyt danych)
bool F_Sensor_AHT  = false;         // flaga błedów dla ATH10 (czy istnieje)

// █████████████████████████████████████████████████████
// D E K L A R A C J E   F U N K C J I
// █████████████████████████████████████████████████████

void Read_AHT_Sensor (float &temp_C, float &humi);

#if SERIAL_MONITOR
    void Show_AHT_Sensor (float &temp_C, float &humi);
#endif

// █████████████████████████████████████████████████████
// S E T U P
// █████████████████████████████████████████████████████

void setup() 
{
    #if SERIAL_MONITOR
        Serial.begin(SERIAL_SPEED); 
        delay(SETUP_DELAY);       
    #endif

    if(myAHT10.begin() != true)
    {
        #if SERIAL_MONITOR
            Serial.printf_P ( PSTR ("\n\n[AHT10] Sensor Test. Urządzenia nie znaleziono" ));                    
            delay(SETUP_DELAY);
         #endif
         F_Sensor_AHT = false;
    }
    else
    {
        #if SERIAL_MONITOR
            Serial.printf_P ( PSTR ("\n\n[AHT10] Sensor Test. Poprawnie zainicjowano." ));                    
            delay(SETUP_DELAY);        // dla tego kiepskiego czujnika trzeba dać opóźnienie        
        #endif
        F_Sensor_AHT = true;
    }
}

// █████████████████████████████████████████████████████
// L O O P
// █████████████████████████████████████████████████████

void loop() 
{    
    if(F_Sensor_AHT) Read_AHT_Sensor (aht_sensor.temperatura_C, aht_sensor.wilgotnosc);      

    #if SERIAL_MONITOR                
        if(F_Sensor_AHT) Show_AHT_Sensor (aht_sensor.temperatura_C, aht_sensor.wilgotnosc);          
    #endif

    delay(LOOP_DELAY);
}

// █████████████████████████████████████████████████████
// R E A D   A H T   S E N S O R
// █████████████████████████████████████████████████████
// funkcja odczytuje wartości z czujnika DHT11/DHT22
// na wejsciu otrzymuje adresy zmiennych w strukturze dht_sensor
// █████████████████████████████████████████████████████

void Read_AHT_Sensor (float &temp_C, float &humi)
{    
    float t = myAHT10.readTemperature(AHT10_FORCE_READ_DATA);            // zmienna lokalna, tymczasowa    
    float h = myAHT10.readHumidity(AHT10_USE_READ_DATA);                 // zmianne lokalna, tymczasowa

     F_Read_AHT_OK = false;                         // resetujemy FLAGĘ, ustawiamy FALSE

    if (isnan(t) || isnan(h))                         // jeśli odczyt z czujnika nie prawidłowy
    {
        #if SERIAL_MONITOR
            Serial.printf_P( PSTR ("\n\n[AHT_Sensor] Wykryto błąd odczytu z czujnika.") );
            Serial.printf_P( PSTR ("\n[AHT_Sensor] Ustawiam Flagę: F_Sensor_AHT_OK na FALSE.") );
        #endif               
    }
    else
    {
        #if SERIAL_MONITOR
            Serial.printf_P( PSTR ("\n\n[AHT_Sensor] Dane z czujnika wyglądają na prawidłowe.") );
            Serial.printf_P( PSTR ("\n[AHT_Sensor] Ustawiam Flagę: F_Sensor_AHT_OK na TRUE.") );
        #endif
        
        F_Read_AHT_OK = true;     // dane wyglądają Okey, ustawiamy FLAGĘ na TRUE          
                      
        temp_C = t;               // kopiujemy do struktury wartości ze zmiennych tymczasowych        
        humi   = h;               // kopiujemy do struktury wartości ze zmiennych tymczasowych
    }    
}

// █████████████████████████████████████████████████████
// S H O W   D H T   S E N S O R
// █████████████████████████████████████████████████████
// funkcja odczytuje wartości z czujnika ATH20
// na wejsciu otrzymuje adresy struktury ath_sensor 
// █████████████████████████████████████████████████████

#if SERIAL_MONITOR
void Show_AHT_Sensor (float &temp_C, float &humi)
{
    Serial.printf_P( PSTR ("\n[AHT_Sensor] Temperatura = %.02f ºC"), temp_C);    
    Serial.printf_P( PSTR ("\n[AHT Sensor] Wilgotnosc  = %.02f %RH"), humi); 
}
#endif

// █████████████████████████████████████████████████████
// END OF FILE : src/sensor/aht10/aht10.ino
// █████████████████████████████████████████████████████
