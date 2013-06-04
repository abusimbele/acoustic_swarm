/*
 * Message.cpp
 *
 *  Created on: 28.05.2013
 *      Author: Work
 */

#include "Message.h"








Message::Message(int bits[]) {
	// TODO Auto-generated constructor stub

	this->bits[0] = bits[0];
	this->bits[1] = bits[1];
	this->bits[2] = bits[2];
	this->bits[3] = bits[3];
	this->bits[4] = bits[4];
	this->bits[5] = bits[5];


	this->pointer_message_read=0;
	MSG_LENGTH=6;

}


Message::~Message() {
	this->MSG_LENGTH=6;
	// TODO Auto-generated destructor stub

}


int* Message::get_bitvalues(){
	return this->bits;

}


 int Message::nextBit(){
	int result = bits[pointer_message_read];

	if(pointer_message_read<(MSG_LENGTH-1)){
		pointer_message_read=pointer_message_read+1;
	}
	else{
		pointer_message_read=0;
		return bits[MSG_LENGTH-1];

	}
	return result;

}

