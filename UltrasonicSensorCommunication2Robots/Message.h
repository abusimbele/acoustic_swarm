/*
 * Message.h
 *
 *  Created on: 28.05.2013
 *      Author: Work
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

class Message {
public:
	Message(int bits[]);
	virtual ~Message();
	int* get_bitvalues();
	int nextBit();


private:
	int bits[6];
	int pointer_message_read;
	int MSG_LENGTH;
};

#endif /* MESSAGE_H_ */
