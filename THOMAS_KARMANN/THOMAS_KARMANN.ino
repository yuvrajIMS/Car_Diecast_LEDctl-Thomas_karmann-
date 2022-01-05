/*This project was made using Arduino 33 IOT board, which was embedded in a 1:18 car-diecast model(VW Karmann Ghia).
The objective was to control LEDs (as car lamps) and horn of car using a bluetooth terminal. This wireless communication 
was possible via Arduino's Serial monitor or PC's BLE terminal or Smartphone's BLE terminal. We used LightBlue smartphone app as BLE termial.
This device's bluetooth name is "Arduino". Power to this setup was provided by a mini-power bank. 
input as "1" - Car front and backlamps ON
input as "2" - Honking with frequency of 335hz
input as "3" - Right indicator (3 Blinks)
input as "4" - Left indicator (3 Blinks)
input as "5" - both side addi lamps ON
input as "0" - ALL OFF 
*/

#include <ArduinoBLE.h>
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int Headlamps = 2; // pin-2 to use LED as headlamps.
const int Backlamps = 3; // pin-3 to use  LED as backlamps.
const int Horn = 4; // pin-4 to use buzzer as horn.
const int Rlamps = 5; // pin-5 to use LED as Right side indicator 
const int Llamps = 6; // pin-6 to use LED as Left side indicator 

int i = 0; // for use in for-loop as counter 
int j = 0; // for use in for-loop as counter 

void setup() 
{
  Serial.begin(115200); // data transfer rate

  pinMode(Headlamps, OUTPUT);
  pinMode(Backlamps, OUTPUT);
  pinMode(Rlamps, OUTPUT);
  pinMode(Llamps, OUTPUT);
  pinMode(Horn,OUTPUT);
  
  if (!BLE.begin())
  {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("THOMAS_VW_KARMANN"); // local name as "THOMAS_VW_KARMANN"
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  switchCharacteristic.writeValue(0);
  BLE.advertise();
  Serial.println("VW Karmann Start");
}

void loop() 
{
  BLEDevice central = BLE.central();
  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()) 
    {
      if (switchCharacteristic.written()) 
      {
      switch (switchCharacteristic.value())
      {
        case 0:
          Serial.println("ALL OFF");
          digitalWrite(Headlamps, LOW);
          digitalWrite(Backlamps, LOW);
          digitalWrite(Rlamps, LOW);
          digitalWrite(Llamps, LOW);
          break;
          
        case 1:
          Serial.println("HEADLAMPS & BACKLIGHTS ON");
          digitalWrite(Headlamps, HIGH);
          digitalWrite(Backlamps, HIGH);
          break;
          
        case 2:    
          Serial.println("Honking");   
          tone(Horn,335, 1100);
          break; 
                 
        case 3:
          Serial.println("R_LIGHTS ON");
          for(i=0;i<3;i++)
          {
            digitalWrite(Llamps, LOW);
            digitalWrite(Rlamps, HIGH);
            delay(500);
            digitalWrite(Rlamps, LOW);
            delay(500);
          }
          i=0;
          break;   
               
          case 4:
          Serial.println("R_LIGHTS ON");
          for(j=0;j<3;j++)
          {
            digitalWrite(Rlamps, LOW);
            digitalWrite(Llamps, HIGH);
            delay(500);
            digitalWrite(Llamps, LOW);
            delay(500);
          }
          j=0;
          break; 

          case 5:
          digitalWrite(Llamps, HIGH);
          digitalWrite(Rlamps, HIGH);
          break;
            
      }
      }
    }

    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
