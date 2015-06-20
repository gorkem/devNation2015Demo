// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

#include "SimpleTimer.h"

// define pins (varies per shield/board)
#define BLE_REQ   9
#define BLE_RDY   8
#define BLE_RST   7


SimpleTimer timer;
// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// create services
BLEService gattService = BLEService("1801");
BLEService tempService = BLEService("CCC0");
BLEFloatCharacteristic tempCharacteristic = BLEFloatCharacteristic("CCC1", BLERead );
BLEDescriptor tempDescriptor = BLEDescriptor("2901", "Temp Celsius");

volatile uint8_t runtimer = 0;

float thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
 float resistance=(float)(1023-RawADC)*10000/RawADC; //get the resistance of the sensor;
 float temperature=1/(log(resistance/10000)/3975+1/298.15)-273.15;//convert to temperature via datasheet ;
 return temperature;
}

void readSensorData(){
	Serial.println(F("Reading data from sensors"));
	int val = analogRead(A0);
	float tempe = thermister(val);
	Serial.print(F("temperature: "));Serial.print(tempe); Serial.print(F("C\n"));
	tempCharacteristic.setValue(tempe);
}

void blePeripheralConnectHandler(BLECentral& central) {
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
  runtimer++;
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
  runtimer--;
}


void setup() {
  Serial.begin(115200);
#if defined (__AVR_ATmega32U4__)
  delay(5000);  //5 seconds delay for enabling to see the start up comments on the serial board
#endif

  blePeripheral.setLocalName("Caster");
  blePeripheral.setAdvertisedServiceUuid(tempService.uuid()); // optional
  blePeripheral.addAttribute(gattService);
  blePeripheral.addAttribute(tempService);
  blePeripheral.addAttribute(tempCharacteristic);
  blePeripheral.addAttribute(tempDescriptor);

  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);


  // begin initialization
  blePeripheral.begin();
  timer.setInterval(2000, readSensorData);
}

void loop() {
	if(runtimer){
		timer.run();
	}
	blePeripheral.poll();
}


