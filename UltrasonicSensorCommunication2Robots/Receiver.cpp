// Do not remove the include below
#include "Receiver.h"
#include "Wire.h"
//#include "TimerOne.h"
#include "Message.h"
//#include "FrequencyTimer2.h"









const int ULTRA_TRANS_ADDRESS    =  112;
const int ULTRA_RECEIV_ADDRESS   =  113;
<<<<<<< HEAD
const int MAX_MEASURE_TIME=1000000; //15000
=======
const int MAX_MEASURE_TIME=15000;
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git
unsigned int reading=0;
int controllerReadout=-1;
int counter=0;
int msg1_bits[]={0,1,1,0,0,1};
Message *msg;
int msg_bit=0;
const int TIMEOUT=0; //too much 15752;//old 11000
const int TIMEOUT_2=20000;
boolean flag=false;
const int WAITING_TIME=1000; //65
boolean bit_flag=false;
unsigned long start_time=0.0;
unsigned long end_time=0.0;
<<<<<<< HEAD


unsigned char MSG_TYPE_SIZE=4;
unsigned char msg_type=0;

=======
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git



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
		  //delay(2);
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
<<<<<<< HEAD
//		  Serial.print("ControllerReadoutFirmware_0: ");
//		  Serial.println(controllerReadout);
=======
		  Serial.print("ControllerReadoutFirmware_0: ");
		  Serial.println(controllerReadout);
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git
		  delay(1);


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
//	  	  Serial.print("Seconds: ");
//	  	  Serial.println(reading);
	  	  delay(2);


<<<<<<< HEAD
	  	  //Serial.print("DATA-BIT: ");
	  	  if(reading!=TIMEOUT){
	  		  //1 received
	  		 Serial.println("Possible packet is incoming");
=======
	  	  Serial.print("DATA-BIT: ");
	  	  if(reading!=TIMEOUT){
	  		 Serial.println("First 1 arrived!");
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git
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
//	  Serial.print("ControllerReadoutFirmware0: ");
//	  Serial.println(controllerReadout);
	  controllerReadout=-1;
}




unsigned char receive_bit(){
	//Serial.println("muh");


	  /*
	   *	Received Impulses
	   */
	start_time=millis();

		  while(true){

			  controllerReadout=-1;
			  activate_receiving_mode();


			  Wire.beginTransmission(113);
			  Wire.write(byte(0x00)); // Register 0 -> control register
			  Wire.endTransmission();



			  while(controllerReadout==-1)
				{

				  delay(1);
				  Wire.requestFrom(113,1);
				  controllerReadout=Wire.read();
//				  Serial.print("ControllerReadoutFirmware: ");
//				  Serial.println(controllerReadout);

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

	//	  reading = reading << 8;    // shift high byte to be high 8 bits
	//	  reading |= Wire.read();
		  reading =(reading * 256) + Wire.read();
<<<<<<< HEAD
//		  Serial.print("Seconds: ");
//		  Serial.println(reading);
=======
		  Serial.print("Seconds: ");
		  Serial.println(reading);
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git
		  delay(2);
<<<<<<< HEAD
		  // if(reading!=TIMEOUT and reading<MAX_MEASURE_TIME){
		  if(reading!=TIMEOUT){

			  //Serial.println("MUH");
			  bit_flag=true;
		  }
		  if((millis()-start_time)>=965.0){
			  break;
		  }

=======
		  if(reading!=TIMEOUT and reading<MAX_MEASURE_TIME){
			  bit_flag=true;
		  }
		  if((millis()-start_time)>=965.0){
			  break;
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git
		  }

<<<<<<< HEAD

	  //Serial.print("DATA-BIT: ");
=======
		  }


	  Serial.print("DATA-BIT: ");
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git
	  if(!bit_flag)
	  	  {
		  Serial.print("0");
		  bit_flag=false;
		  return 0;

	  	  }
	  else{
<<<<<<< HEAD
		  Serial.print("1");
		  bit_flag=false;
		  return 1;
=======
		  Serial.println("1");
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git
	  }

<<<<<<< HEAD
	  //Serial.println("*************");

=======
	  Serial.println("*************");
	  bit_flag=false;
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git

}
//void test(){Serial.println("muh!");}






void setup() {
	//Receiving state

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

  Serial.println("********************************");
  Serial.println(" RECEIVING STATE ON ");
  Serial.println("********************************");
  while(!start_synchronize()){}
<<<<<<< HEAD
=======
  //FrequencyTimer2::setOnOverflow(test);
>>>>>>> branch 'master' of https://github.com/abusimbele/acoustic_swarm.git
  delay(500);



}



unsigned char look_for_msg_type(){
	unsigned char msg_type=0;
		for(unsigned char i=0;i<MSG_TYPE_SIZE;i++){
			msg_type = msg_type << 1;
			msg_type = msg_type + receive_bit();


		}

		return msg_type;
}


void loop() {

	msg_type=look_for_msg_type();
	Serial.println();
	Serial.print("MSG_TYPE: ");
	Serial.println(msg_type);
	Serial.println();

	switch(msg_type)
	{
	//RTS
	case 0:
		break;

	//CTS
	case 1:
		break;

	//DATA
	case 2:
		break;
	}
	  Serial.println("********************************");
	  Serial.println(" RECEIVING STATE ON ");
	  Serial.println("********************************");
	while(!start_synchronize()){}
	delay(500);














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
