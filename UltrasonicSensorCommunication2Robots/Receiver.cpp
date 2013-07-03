// Do not remove the include below
#include "Receiver.h"
#include "Wire.h"
#include "Message.h"



//#include <boost/dynamic_bitset.hpp>





boolean last_bit_flag=false;
boolean echo_off=false;
const int ULTRA_TRANS_ADDRESS    =  112;
const int ULTRA_RECEIV_ADDRESS   =  113;
const int MAX_MEASURE_TIME=1000000; //15000
unsigned int reading=0;
int controllerReadout=-1;
int counter=0;
int msg1_bits[]={0,1,1,0,0,1};
Message *msg;
int msg_bit=0;
const unsigned int TIMEOUT=0; //too much 15752;//old 11000
const int TIMEOUT_2=20000;
boolean flag=false;
const unsigned long WAITING_TIME=290; //65 //1000 //285 //282.9999 too fast!! 282.99999 too slow
boolean bit_flag=false;
unsigned long start_time=0.0;
unsigned long end_time=0.0;


//************ HEADER **************

//START BITS

unsigned char HEADER_START_BITS_SIZE=1;
//DATA, RTS, CTS,...  in bits for bits!!
unsigned char MSG_TYPE_SIZE=4;

//maximal number of individuals in a local network  in bits for bits!!
unsigned char MAC_Address_SIZE=8;

//max DATA-size in bits for BYTES!!! (*** be carefully ***)
unsigned char DATA_SIZE=8;

unsigned char BYTE=8;




//************ HEADER END **************




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
//		  Serial.print("ControllerReadoutFirmware_0: ");
//		  Serial.println(controllerReadout);
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


	  	  //Serial.print("DATA-BIT: ");
	  	  if(reading!=TIMEOUT){
	  		  //1 received
	  		 Serial.println("Possible packet is incoming");
	  		 delay(15);//10
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
			  delay(2);



			  while(controllerReadout==-1)
				{
				  delay(2);
				  if((millis()-start_time)>=WAITING_TIME){ //965
				 			  break;
				  }

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
		  delay(2);
		  reading=Wire.read();
		  delay(2);

	//	  reading = reading << 8;    // shift high byte to be high 8 bits
	//	  reading |= Wire.read();
		  reading =(reading * 256) + Wire.read();
//		  Serial.print("Seconds: ");
//		  Serial.println(reading);
		  delay(2);
		  // if(reading!=TIMEOUT and reading<MAX_MEASURE_TIME){
		  if(reading!=TIMEOUT){

			  //Serial.println("MUH");
			  if(echo_off){
				  echo_off=false;
			  }
			  else{
				  bit_flag=true;

			  }

			  last_bit_flag=true;
		  }
		  else{
			  echo_off=false;

		  }
		  if((millis()-start_time)>=WAITING_TIME){ //965
			  if(last_bit_flag){
				  echo_off=true;
				  last_bit_flag=false;
			  }

			  //Serial.println("****************");
			  break;
		  }

		  }


	  //Serial.print("DATA-BIT: ");
	  if(!bit_flag)
	  	  {
		  Serial.print("0");
		  bit_flag=false;
		  return 0;

	  	  }
	  else{
		  Serial.print("1");
		  bit_flag=false;
		  return 1;
	  }

	  //Serial.println("*************");


}
//void test(){Serial.println("muh!");}




void start_receiving_mode(){


	Serial.println("********************************");
	Serial.println(" RECEIVING STATE ON ");
	Serial.println("********************************");
	while(!start_synchronize()){}
	//delay(500);
}


void setup() {
	//Receiving state

  (*msg)= send_bit_sequence(msg1_bits);
  Wire.begin();
  Serial.begin(19200);
  delay(4);
//  delay(1000);
//  Serial.print("Msg: ");
//  Serial.print((*msg).get_bitvalues()[0]);
//  Serial.print((*msg).get_bitvalues()[1]);
//  Serial.print((*msg).get_bitvalues()[2]);
//  Serial.print((*msg).get_bitvalues()[3]);
//  Serial.print((*msg).get_bitvalues()[4]);
//  Serial.println((*msg).get_bitvalues()[5]);
  start_receiving_mode();


}

/**
 *Reads packet information for "number_of_bits" places.
 */
unsigned char receive_bits(unsigned char number_of_bits){
	unsigned char number=0;
		for(unsigned char i=0;i<number_of_bits;i++){
			number = number << 1;
			number = number + receive_bit();
		}

		return number;


}


unsigned char look_for_msg_type(){
	return receive_bits(MSG_TYPE_SIZE);
}

unsigned char look_for_MAC_address(){
	return receive_bits(MAC_Address_SIZE);
}


unsigned char look_for_data_length(){
	return receive_bits(DATA_SIZE);
}

unsigned char look_for_1_byte_data(){
	return receive_bits(BYTE);

}

//type to boolean?!
unsigned char look_for_parity_bit(){
	return receive_bits(1);
}

boolean parity_bit_check(boolean paritybit, unsigned char bits){
	bits ^= bits>>4;
	bits ^= bits>>2;
	bits ^= bits>>1;
	bits &= 1;

	if(!bits==paritybit){
		return true;

	}
	else{
		return false;
	}

}





 String look_for_data(unsigned char number_bytes){
	 String data="";
	 unsigned char data_1_byte;
	 boolean parity =false;
	 for(unsigned char i=0;i<number_bytes;i++){

		 data_1_byte=look_for_1_byte_data();


//			//PARITY
//		 	Serial.println();
//		 	Serial.print("Parity-BIT: ");
//			parity=parity_bit_check(look_for_parity_bit(),data_1_byte);
//			if(parity){
//				Serial.println();
//				Serial.println("PARITY_CHECK: OK");
//				Serial.println();
//			}
//			else{
//				Serial.println();
//				Serial.println("PARITY_CHECK: ERROR");
//				Serial.println();
//				//WHAT RETURN??????????????
//				return "ERROR";
//			}






		 data=data+data_1_byte;




		 data=data+"_";


	 }

	 return data;


}





unsigned char msg_type=0;
unsigned char mac_address_from=0;
unsigned char mac_address_to=0;
unsigned char data_length=0;
String data="";
boolean parity=false;
unsigned char header_start=0;


unsigned char header_start_bits(){
	return receive_bits(HEADER_START_BITS_SIZE);

}



void receive_packet(){

	Serial.println();
	Serial.print("HEADER START BITS: ");
	Serial.println("1");
	Serial.println();




	msg_type=look_for_msg_type();


	Serial.println();
	Serial.print("MSG_TYPE: ");
	Serial.println(msg_type);
	Serial.println();

//	//PARITY
//	Serial.print("Parity-BIT: ");
//	parity=parity_bit_check(look_for_parity_bit(),msg_type );
//	if(parity){
//		Serial.println();
//		Serial.println("PARITY_CHECK: OK");
//		Serial.println();
//	}
//	else{
//		Serial.println();
//		Serial.println("PARITY_CHECK: ERROR");
//		Serial.println();
//		//WHAT RETURN??????????????
//		return;
//	}





	mac_address_from=look_for_MAC_address();

	Serial.println();
	Serial.print("MAC_FROM: ");
	Serial.println(mac_address_from);
	Serial.println();

//	//PARITY
//	Serial.print("Parity-BIT: ");
//	parity=parity_bit_check(look_for_parity_bit(), mac_address_from);
//	if(parity){
//		Serial.println();
//		Serial.println("PARITY_CHECK: OK");
//		Serial.println();
//	}
//	else{
//		Serial.println();
//		Serial.println("PARITY_CHECK: ERROR");
//		Serial.println();
//		//WHAT RETURN??????????????
//		return;
//	}

	mac_address_to=look_for_MAC_address();



	Serial.println();
	Serial.print("MAC_TO: ");
	Serial.println(mac_address_to);
	Serial.println();

//	//PARITY
//	Serial.print("Parity-BIT: ");
//	parity=parity_bit_check(look_for_parity_bit(), mac_address_to);
//	if(parity){
//		Serial.println();
//		Serial.println("PARITY_CHECK: OK");
//		Serial.println();
//	}
//	else{
//		Serial.println();
//		Serial.println("PARITY_CHECK: ERROR");
//		Serial.println();
//		//WHAT RETURN??????????????
//		return;
//	}





	if(msg_type==2){

		data_length=look_for_data_length();

		Serial.println();
		Serial.print("DATA SIZE: ");
		Serial.print(data_length);
		Serial.println(" bytes");
		Serial.println();


//		//PARITY
//		Serial.print("Parity-BIT: ");
//		parity=parity_bit_check(look_for_parity_bit(),data_length);
//		if(parity){
//			Serial.println();
//			Serial.println("PARITY_CHECK: OK");
//			Serial.println();
//		}
//		else{
//			Serial.println();
//			Serial.println("PARITY_CHECK: ERROR");
//			Serial.println();
//			//WHAT RETURN??????????????
//			return;
//		}



		data= look_for_data(data_length);


		Serial.println();
		Serial.print("DATA: ");
		Serial.println(data);
		Serial.println();



	}
	else{
		Serial.println();
		Serial.print("NO DATA INCLUDED");
		Serial.println();

	}





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
}


void loop() {
	receive_packet();
	start_receiving_mode();

}
