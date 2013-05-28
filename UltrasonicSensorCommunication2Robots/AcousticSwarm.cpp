// Do not remove the include below
#include "AcousticSwarm.h"
#include "Wire.h"
//#include "pt.h"







const int ULTRA_TRANS_ADDRESS    =  112;
const int ULTRA_RECEIV_ADDRESS   =  113;
int reading=0;
int controllerReadout=-1;
int counter=0;




void setup() {
  Wire.begin();
  Serial.begin(19200);
  delay(1000);

}


void loop() {

	  Wire.beginTransmission(113);
	  Wire.write(byte(0x00)); // Register 0 -> control register
	  Wire.endTransmission();
	  delay(2);

	  Wire.requestFrom(113,1);
	  controllerReadout=Wire.read();
	  Serial.print("ControllerReadoutFirmware0: ");
	  Serial.println(controllerReadout);
	  controllerReadout=-1;



	  /*
	   *Activate receiver to listen to the channel
	   */

	  Wire.beginTransmission(113);
	  Wire.write(byte(0x00)); // Register 0 -> control register
	  Wire.write(byte(0x58)); // look
	  Wire.endTransmission();
	  delay(2);

	  /*
	   *	Send 8 Impulses
	   */

	  if((counter%2)==0){
		  Serial.println("PING");
		  delay(5);
		  Wire.beginTransmission(112);
		  Wire.write(byte(0x00)); // Register 0 -> control register
		  Wire.write(byte(0x5C)); // Erzeugt einen 8 zyklischen 40khz Impuls/Ton
		  Wire.endTransmission();
	  }
	  delay(2);



	  Wire.beginTransmission(113);
	  Wire.write(byte(0x00)); // Register 0 -> control register
	  Wire.endTransmission();
	  delay(2);

	  Wire.requestFrom(113,1);
	  controllerReadout=Wire.read();
	  Serial.print("ControllerReadoutFirmware1: ");
	  Serial.println(controllerReadout);
	  controllerReadout=-1;





	  /*
	   *	Received Impulses
	   */

	  Wire.beginTransmission(113);
	  Wire.write(byte(0x00)); // Register 0 -> control register
	  Wire.endTransmission();
	  delay(2);


	  while(controllerReadout==-1)
	    {


		  Wire.requestFrom(113,1);
		  controllerReadout=Wire.read();
		  Serial.print("ControllerReadoutFirmware: ");
		  Serial.println(controllerReadout);

	    }


	  /*
	   *  Pointer to Register 0x02
	   */

	  Wire.beginTransmission(113); // transmit to device #112
	  Wire.write(byte(0x02));      // sets register pointer to echo #1 register (0x02)
	  Wire.endTransmission();      // stop transmitting
	  delay(2);

	  /*
	   * Read values
	   */

	  Wire.requestFrom(113,2);
	  reading=Wire.read();
	  reading = reading << 8;    // shift high byte to be high 8 bits
	  reading |= Wire.read();
	  Serial.print("Seconds: ");
	  Serial.println(reading);

	  Serial.print("DATA-BIT: ");
	  if(reading>11000)
	  	  {
		  Serial.println("0");

	  	  }
	  else
	  {Serial.println("1");
	  }

	  Serial.println("*************");
	  //delay(2000);
	  counter=counter+1;
}
