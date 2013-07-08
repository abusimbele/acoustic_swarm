// Do not remove the include below
#include "Receiver.h"
#include "Wire.h"
#include "Message.h"



//#include <boost/dynamic_bitset.hpp>





boolean last_bit_flag=false;
boolean echo_off=false;
const int ULTRA_TRANS_ADDRESS    =  112;
const int ULTRA_RECEIV_ADDRESS   =  113;
const unsigned int MAX_MEASURE_TIME=65279;
unsigned int reading=0;
int controllerReadout=-1;
int counter=0;
int msg1_bits[]={0,1,1,0,0,1};
Message *msg;
int msg_bit=0;
const unsigned int TIMEOUT=0; //too much 15752;//old 11000
const int TIMEOUT_2=20000;
boolean flag=false;
const unsigned long WAITING_TIME=2; //65 //1000 //285 //282.9999 too fast!! 282.99999 too slow  /// 20
boolean bit_flag=false;
unsigned long start_time=0.0;
unsigned long end_time=0.0;


//************ HEADER **************

//START BITS
unsigned char HEADER_START_BITS_SIZE=1;

//DATA, RTS, CTS,...  in bits for bits!!
unsigned char MSG_TYPE_SIZE=4;

//Maximal number of individuals in a local network  in bits for bits!!
unsigned char MAC_Address_SIZE=8;

//Max DATA-size in bits for BYTES!!! (*** be carefully ***)
unsigned char DATA_SIZE=8;

//Number of Bits in one Byte
unsigned char BYTE=8;


//************ HEADER END **************



/**
 * Creates a message object with the intended information for transmission.
 */
Message send_bit_sequence(int bits_to_send[]){
	Message msg(bits_to_send);
	return msg;


}


/**
 * Sets the board in a mode to get transmission packets.
 */
void activate_receiving_mode(){

		 /*
		  *Activate receiver to listen to the channel
	      */

		  Wire.beginTransmission(113); //I2C ID
		  Wire.write(byte(0x00)); // Set pointer to -> register 0 -> control register
		  Wire.write(byte(0x58)); // operation -> look for 40k signal
		  Wire.endTransmission();
		  //delay(2);
}



/**
 * Look for a 40k signal and return after first seen a 1.
 */

bool start_synchronize(){
      while(true){
	  controllerReadout=-1;
	  activate_receiving_mode();

	  Wire.beginTransmission(113);// I2C ID
	  Wire.write(byte(0x00)); // Set pointer to -> register 0 -> control register
	  Wire.endTransmission();
	  delay(2);



	  while(controllerReadout==-1)
		{

		  //Wire.requestFrom(113,1) returns -1, if a measurement is still in progress. Otherwise a number is returned and the loop will break.
		  Wire.requestFrom(113,1); //I2C ID 113; register 1
		  controllerReadout=Wire.read();
//		  Serial.print("ControllerReadoutFirmware_0: ");
//		  Serial.println(controllerReadout);
		  delay(2);


		}

	  //Register Pointer to 0x02
	  Wire.beginTransmission(113); // I2C ID 113
	  Wire.write(byte(0x02));      // Set pointer to -> echo #1 register (0x02)
	  Wire.endTransmission();
	  delay(2);


	  //Readout low and high bit of register 0x02
	  Wire.requestFrom(113,2);
	  	  reading=Wire.read();
		  reading =(reading * 256) + Wire.read();
//	  	  reading = reading << 8;    // shift high byte to be high 8 bits
//	  	  reading |= Wire.read();
//	  	  Serial.print("Seconds1: ");
//	  	  Serial.println(reading);
	  	  delay(2);


	  	  //Serial.print("DATA-BIT: ");
	  	  if(reading!=TIMEOUT ){
	  		  //1 received
	  		 Serial.println("Possible packet is incoming");

	  		 //This delay is important to eliminate further 40k signals from one measurement process.
	  		 delay(12);//20
	  		 return true;
	  	  }
	}


}


/**
 * Read out the firmware information of the SRF02
 */
void read_firmware(){
	  Wire.beginTransmission(113); //IC2 ID 113
	  Wire.write(byte(0x00)); // Set pointer to -> register 0 -> control register
	  Wire.endTransmission();
	  delay(2);

	  Wire.requestFrom(113,1); //Readout 1 byte from register, that was before chosen.
	  controllerReadout=Wire.read();
//	  Serial.print("ControllerReadoutFirmware0: ");
//	  Serial.println(controllerReadout);
	  controllerReadout=-1;
	  delay(2);
}



/**
 * Returns 1 bit of a transmission.
 */
unsigned char receive_bit(){

	//save start-time for synchronization of receiving bit intervals.
	//start_time=millis();

	//Go into receiving mode
	activate_receiving_mode();


	/*
	*  Pointer to Register 0x02
	*/

	delay(68);
	Wire.beginTransmission(113); // transmit to device #112
	Wire.write(byte(0x02));      //Set pointer to -> register (0x02)
	Wire.endTransmission();



	/*
	* Read low and high bit from register (0x02)
	*/

	Wire.requestFrom(113,2);
	reading=Wire.read();
	reading =(reading * 256) + Wire.read();

	/**
	* +++++++++++++START DEBUGGING+++++++++++++
	*/
	//
	//			  Serial.print("Seconds: ");
	//			  Serial.println(reading);

	/**
	* ++++++++++++++END DEBUGGING++++++++++++++
	*/

	delay(2);
	//Serial.println(reading);
	if(reading<MAX_MEASURE_TIME && reading!=0 ){
		Serial.print("1");
		return 1;
	}
		else{
			return 0;
			Serial.print("0");
		}







//	  //Serial.print("DATA-BIT: ");
//	  if(!bit_flag)
//	  	  {
//		  Serial.print("0");
//		  bit_flag=false;
//		  return 0;
//
//	  	  }
//	  else{
//		  Serial.print("1");
//		  bit_flag=false;
//		  return 1;
//	  }


	  /**
	   * +++++++++++++START DEBUGGING+++++++++++++
	   */

//	  Serial.println("*************");

	  /**
	   * ++++++++++++++END DEBUGGING++++++++++++++
	   */



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


			//PARITY
		 	Serial.println();
		 	Serial.print("Parity-BIT: ");
			parity=parity_bit_check(look_for_parity_bit(),data_1_byte);
			if(parity){
				Serial.println();
				Serial.println("PARITY_CHECK: OK");
				Serial.println();
			}
			else{
				Serial.println();
				Serial.println("PARITY_CHECK: ERROR");
				Serial.println();
				//WHAT RETURN??????????????
				return "ERROR";
			}






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

	//PARITY
	Serial.print("Parity-BIT: ");
	parity=parity_bit_check(look_for_parity_bit(),msg_type );
	if(parity){
		Serial.println();
		Serial.println("PARITY_CHECK: OK");
		Serial.println();
	}
	else{
		Serial.println();
		Serial.println("PARITY_CHECK: ERROR");
		Serial.println();
		//WHAT RETURN??????????????
		return;
	}





	mac_address_from=look_for_MAC_address();

	Serial.println();
	Serial.print("MAC_FROM: ");
	Serial.println(mac_address_from);
	Serial.println();

	//PARITY
	Serial.print("Parity-BIT: ");
	parity=parity_bit_check(look_for_parity_bit(), mac_address_from);
	if(parity){
		Serial.println();
		Serial.println("PARITY_CHECK: OK");
		Serial.println();
	}
	else{
		Serial.println();
		Serial.println("PARITY_CHECK: ERROR");
		Serial.println();
		//WHAT RETURN??????????????
		return;
	}

	mac_address_to=look_for_MAC_address();



	Serial.println();
	Serial.print("MAC_TO: ");
	Serial.println(mac_address_to);
	Serial.println();

	//PARITY
	Serial.print("Parity-BIT: ");
	parity=parity_bit_check(look_for_parity_bit(), mac_address_to);
	if(parity){
		Serial.println();
		Serial.println("PARITY_CHECK: OK");
		Serial.println();
	}
	else{
		Serial.println();
		Serial.println("PARITY_CHECK: ERROR");
		Serial.println();
		//WHAT RETURN??????????????
		return;
	}





	if(msg_type==2){

		data_length=look_for_data_length();

		Serial.println();
		Serial.print("DATA SIZE: ");
		Serial.print(data_length);
		Serial.println(" bytes");
		Serial.println();


		//PARITY
		Serial.print("Parity-BIT: ");
		parity=parity_bit_check(look_for_parity_bit(),data_length);
		if(parity){
			Serial.println();
			Serial.println("PARITY_CHECK: OK");
			Serial.println();
		}
		else{
			Serial.println();
			Serial.println("PARITY_CHECK: ERROR");
			Serial.println();
			//WHAT RETURN??????????????
			return;
		}



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
