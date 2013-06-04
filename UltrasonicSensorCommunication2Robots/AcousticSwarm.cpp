// Do not remove the include below
#include "AcousticSwarm.h"
#include "Wire.h"
#include "Message.h"
//#include "pt.h"







const int ULTRA_TRANS_ADDRESS    =  112;
const int ULTRA_RECEIV_ADDRESS   =  113;
int reading=0;
int controllerReadout=-1;
int counter=0;
int msg1_bits[]={0,1,1,0,0,1};
Message *msg;
int msg_bit=0;




Message send_bit_sequence(int bits_to_send[]){
	Message msg(bits_to_send);
	return msg;


}

void activate_receiving_mode(){

	/*
		   *Activate receiver to listen to the channel
		   */

		  Wire.beginTransmission(113);
		  Wire.write(byte(0x00)); // Register 0 -> control register
		  Wire.write(byte(0x58)); // look
		  Wire.endTransmission();
		  delay(2);
}


void send_bit_0(){
	//do nothing
}

void send_bit_1(){

	  /*
	   *	Send 8 Impulses
	   */


	  Serial.println("PING");
	  delay(5);
	  Wire.beginTransmission(112);
	  Wire.write(byte(0x00)); // Register 0 -> control register
	  Wire.write(byte(0x5C)); // Erzeugt einen 8 zyklischen 40khz Impuls/Ton
	  Wire.endTransmission();

	  delay(2);


}


void read_firmware(){
	  Wire.beginTransmission(113);
	  Wire.write(byte(0x00)); // Register 0 -> control register
	  Wire.endTransmission();
	  delay(2);

	  Wire.requestFrom(113,1);
	  controllerReadout=Wire.read();
	  Serial.print("ControllerReadoutFirmware0: ");
	  Serial.println(controllerReadout);
	  controllerReadout=-1;
}

void receive_bit(){
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
}





void setup() {
  (*msg)= send_bit_sequence(msg1_bits);
  Wire.begin();
  Serial.begin(19200);
  delay(1000);
  Serial.print("Msg: ");
  Serial.print((*msg).get_bitvalues()[0]);
  Serial.print((*msg).get_bitvalues()[1]);
  Serial.print((*msg).get_bitvalues()[2]);
  Serial.print((*msg).get_bitvalues()[3]);
  Serial.print((*msg).get_bitvalues()[4]);
  Serial.println((*msg).get_bitvalues()[5]);
  delay(2000);

}



void loop() {
	msg_bit=(*msg).nextBit();
	Serial.print("**p**");
	Serial.println(msg_bit);
	if(msg_bit==1){
		send_bit_1();
	}
	if(msg_bit==0){
			send_bit_0();
		}



	controllerReadout=-1;
	activate_receiving_mode();
	receive_bit();
	//delay(1000);
	counter=counter+1;
}
