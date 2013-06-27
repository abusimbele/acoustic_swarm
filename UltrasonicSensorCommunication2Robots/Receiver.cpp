// Do not remove the include below
#include "Receiver.h"
#include "Wire.h"
//#include "TimerOne.h"
#include "Message.h"
//#include "FrequencyTimer2.h"









const int ULTRA_TRANS_ADDRESS    =  112;
const int ULTRA_RECEIV_ADDRESS   =  113;
unsigned int reading=0;
int controllerReadout=-1;
int counter=0;
int msg1_bits[]={0,1,1,0,0,1};
Message *msg;
int msg_bit=0;
const int TIMEOUT=0; //too much 15752;//old 11000
const int TIMEOUT_2=20000;
boolean flag=false;
const int WAITING_TIME=65; //65



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



bool start_synchronize(){
      while(true){
	  controllerReadout=-1;
	  activate_receiving_mode();

	  Wire.beginTransmission(113);
	  Wire.write(byte(0x00)); // Register 0 -> control register
	  Wire.endTransmission();
	  delay(2);



	  while(controllerReadout==-1)
		{


		  Wire.requestFrom(113,1);
		  controllerReadout=Wire.read();
		  Serial.print("ControllerReadoutFirmware_0: ");
		  Serial.println(controllerReadout);


		}

	  Wire.beginTransmission(113); // transmit to device #112
	  Wire.write(byte(0x02));      // sets register pointer to echo #1 register (0x02)
	  Wire.endTransmission();      // stop transmitting
	  delay(2);

	  Wire.requestFrom(113,2);
	  	  reading=Wire.read();
		  reading =(reading * 256) + Wire.read();
//	  	  reading = reading << 8;    // shift high byte to be high 8 bits
//	  	  reading |= Wire.read();
	  	  Serial.print("Seconds: ");
	  	  Serial.println(reading);
	  	  delay(2);


	  	  Serial.print("DATA-BIT: ");
	  	  if(reading!=TIMEOUT and reading<10000){
	  		 Serial.println("First 1 arrived!");
	  		 return true;
	  	  }
	}


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
	//Serial.println("muh");


	  /*
	   *	Received Impulses
	   */


		  controllerReadout=-1;
		  activate_receiving_mode();
		  delay(WAITING_TIME);

//		  Wire.beginTransmission(113);
//		  Wire.write(byte(0x00)); // Register 0 -> control register
//		  Wire.endTransmission();
//
//
//
//		  while(controllerReadout==-1)
//			{
//
//
//			  Wire.requestFrom(113,1);
//			  controllerReadout=Wire.read();
//			  Serial.print("ControllerReadoutFirmware: ");
//			  Serial.println(controllerReadout);
//
//			}


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

//	  reading = reading << 8;    // shift high byte to be high 8 bits
//	  reading |= Wire.read();
	  reading =(reading * 256) + Wire.read();
	  Serial.print("Seconds: ");
	  Serial.println(reading);
	  delay(2);


	  Serial.print("DATA-BIT: ");
	  if(reading==TIMEOUT)
	  	  {
		  Serial.println("0");

	  	  }
	  if (reading>TIMEOUT_2)
	  {
		  Serial.println("01");

  	  }
	  if (reading<=TIMEOUT_2 and reading != TIMEOUT) {
		  Serial.println("1");
	  }

	  Serial.println("*************");

}
//void test(){Serial.println("muh!");}






void setup() {
  //FrequencyTimer2::setPeriod(30000);
  //FrequencyTimer2::enable();
  (*msg)= send_bit_sequence(msg1_bits);
  Wire.begin();
  Serial.begin(19200);
//  delay(1000);
//  Serial.print("Msg: ");
//  Serial.print((*msg).get_bitvalues()[0]);
//  Serial.print((*msg).get_bitvalues()[1]);
//  Serial.print((*msg).get_bitvalues()[2]);
//  Serial.print((*msg).get_bitvalues()[3]);
//  Serial.print((*msg).get_bitvalues()[4]);
//  Serial.println((*msg).get_bitvalues()[5]);

  while(!start_synchronize()){}
  //FrequencyTimer2::setOnOverflow(test);



}



void loop() {
	receive_bit();






//	msg_bit=(*msg).nextBit();
//	Serial.print("**p**");
//	Serial.println(msg_bit);
//	if(msg_bit==1){
//		send_bit_1();
//	}
//	if(msg_bit==0){
//			send_bit_0();
//		}





	//activate_receiving_mode();
	//receive_bit();
	//delay(1000);
	counter=counter+1;

}
