#include "MKS_SERVO42.h"

void MKS_SERVO42_1_1::initialize(HardwareSerial *serialPort, long const &baudRate)
{
	port_ = serialPort;
	// port_->begin(baudRate);
}

bool MKS_SERVO42_1_1::ping(byte const &stepperId)
{
	// Flush
	while (port_->read() != -1)
		;
	int send = sendMessage(stepperId, instruction::STEPPER_PING);
	if (send != 3)
	{
		Serial.println("Failed to send");
		return false;
	}
	return reciveStepperStatus();
}

int MKS_SERVO42_1_1::sendMessage(byte stepperId, byte const &commandId)
{
	stepperId += 0xe0;
	byte message[3];
	byte checksum = stepperId + commandId;
	message[0] = stepperId;
	message[1] = commandId;
	message[2] = checksum & 0xFF;
	for (size_t i = 0; i < 3; i++)
	{
		Serial.print(" ");
		Serial.print(message[i], HEX);
	}
	Serial.println();
	return port_->write(message, 3);
}

int MKS_SERVO42_1_1::reciveStepperStatus()
{
	int messageSize = 2 + sizeof(uint8_t);
	byte receivedBytes[messageSize];
	size_t rd = port_->readBytes(receivedBytes, messageSize);
	for (size_t i = 0; i < 2 + sizeof(uint8_t); i++)
	{
		Serial.print(" ");
		Serial.print(receivedBytes[i], HEX);
	}
	Serial.println();
	return receivedBytes[1] == 1;
}

long MKS_SERVO42_1_1::getCurrentPosition(byte const &stepperId)
{
	// Flush
	while (port_->read() != -1)
		;
	int send = sendMessage(stepperId, instruction::GET_ENCODER_POS);
	if (send != 3)
	{
		Serial.println("Failed to send");
		return -1;
	}
	return recieveEncoderPosition(stepperId);
}

long MKS_SERVO42_1_1::recieveEncoderPosition(byte const &stepperId)
{
	byte receivedBytes[8] = {0xe0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20};
	size_t bytesRead = port_->readBytes(receivedBytes, 8);
	if (bytesRead == 8 && receivedBytes[0] == (stepperId + 0xE0))
	{
		int32_t carry = (int32_t)receivedBytes[1] << 24 | (int32_t)receivedBytes[2] << 16 | (int32_t)receivedBytes[3] << 8 | (int32_t)receivedBytes[4];
		uint16_t value = (uint16_t)receivedBytes[5] << 8 | (uint16_t)receivedBytes[6];
		return (carry * 0xffff) + value;
	}
	else
	{
		Serial.println("Invalid response from motor controller");
		return false;
	}
}

bool MKS_SERVO42_1_1::setTargetPosition(byte stepperId, byte direction, uint8_t speed, uint32_t pulses)
{
	if (speed > 2000 || direction > 1)
	{
		Serial.println("Speed out of range or invalid directionection");
		return false;
	}
	stepperId += 0xE0;
	byte message[8];
	message[0] = stepperId;							// Slave address
	message[1] = instruction::MOVE_SPEED_PULSES;	// Function code for running the motor
	message[2] = (direction << 7) | (speed & 0x7F); // VAL byte, with directionection and speed
	message[3] = (pulses >> 24) & 0xFF;
	message[4] = (pulses >> 16) & 0xFF;
	message[5] = (pulses >> 8) & 0xFF;
	message[6] = pulses & 0xFF;
	message[7] = calculateChecksum(message, 7);
	port_->write(message, sizeof(message));

	while (port_->read() != -1)
		;
	byte response[3];
	size_t bytesRead = port_->readBytes(response, 3);
	Serial.println("Bytes read: " + String(bytesRead));
	Serial.print("Response: ");
	for (size_t i = 0; i < bytesRead; i++)
	{
		Serial.print(response[i], HEX);
		Serial.print(" ");
	}
	Serial.println();
	if (bytesRead == 3 && response[0] == stepperId)
	{
		if (response[1] == 0)
		{
			Serial.println("Run command failed");
			return false;
		}
		else if (response[1] == 1)
		{
			Serial.println("Run command starting...");
			return true;
		}
		else if (response[1] == 2)
		{
			Serial.println("Run command completed");
			return true;
		}
	}
	else
	{
		Serial.println("Invalid response from motor controller");
		return false;
	}
}

byte MKS_SERVO42_1_1::calculateChecksum(const byte *message, int length)
{
	byte checksum = 0;
	for (int i = 0; i < length; i++)
		checksum += message[i];
	return checksum & 0xFF;
}

void MKS_SERVO42_1_0::initialize(HardwareSerial *serialPort, long const &baudRate)
{
	port_ = serialPort;
	// port_->begin(baudRate);
}

bool MKS_SERVO42_1_0::ping(byte const &stepperId)
{
	// Flush
	while (port_->read() != -1)
		;
	int send = sendMessage(stepperId, instruction::STEPPER_PING);
	if (send != 3)
	{
		Serial.println("Failed to send");
		return false;
	}
	return reciveStepperStatus();
}

int MKS_SERVO42_1_0::sendMessage(byte stepperId, byte const &commandId)
{
	stepperId += 0xe0;
	byte message[2];
	message[0] = stepperId;
	message[1] = commandId;
	for (size_t i = 0; i < 2; i++)
	{
		Serial.print(" ");
		Serial.print(message[i], HEX);
	}
	Serial.println();
	return port_->write(message, 3);
}

int MKS_SERVO42_1_0::reciveStepperStatus()
{
	int messageSize = 2 + sizeof(uint8_t);
	byte receivedBytes[messageSize];
	size_t rd = port_->readBytes(receivedBytes, messageSize);
	for (size_t i = 0; i < 2 + sizeof(uint8_t); i++)
	{
		Serial.print(" ");
		Serial.print(receivedBytes[i], HEX);
	}
	Serial.println();
	return receivedBytes[1] == 1;
}

long MKS_SERVO42_1_0::getCurrentPosition(byte const &stepperId)
{
	// Flush
	while (port_->read() != -1)
		;
	int send = sendMessage(stepperId, instruction::GET_ENCODER_POS);
	if (send != 3)
	{
		Serial.println("Failed to send");
		return -1;
	}
	return recieveEncoderPosition(stepperId);
}

long MKS_SERVO42_1_0::recieveEncoderPosition(byte const &stepperId)
{
	byte receivedBytes[3] = {0x00, 0x00, 0x00};

	size_t bytesRead = port_->readBytes(receivedBytes, 3);

	if (bytesRead == 3 && receivedBytes[0] == (stepperId + 0xE0))
	{
		uint16_t value = (uint16_t)receivedBytes[1] << 4 | (uint16_t)receivedBytes[2];
		return value;
	} else {
		Serial.println("Invalid response from motor controller");
		return false;
	}
}

bool MKS_SERVO42_1_0::setTargetPosition(byte stepperId, byte direction, uint8_t speed, uint16_t pulses)
{
	if (speed > 2000 || direction > 1)
	{
		Serial.println("Speed out of range or invalid directionection");
		return false;
	}
	stepperId += 0xE0;
	byte message[5];
	message[0] = stepperId;							// Slave address
	message[1] = instruction::MOVE_SPEED_PULSES;	// Function code for running the motor
	message[2] = (direction << 7) | (speed & 0x7F); // VAL byte, with directionection and speed
	message[3] = (pulses >> 8) & 0xFF;
	message[4] = pulses & 0xFF;
	port_->write(message, sizeof(message));

	while (port_->read() != -1)
		;

	byte response[2];
	size_t bytesRead = port_->readBytes(response, 2);

	Serial.println("Bytes read: " + String(bytesRead));
	Serial.print("Response: ");
	for (size_t i = 0; i < bytesRead; i++)
	{
		Serial.print(response[i], HEX);
		Serial.print(" ");
	}
	Serial.println();

	if (bytesRead == 2 && response[0] == stepperId)
	{
		if (response[1] == 0)
		{
			Serial.println("Run command failed");
			return false;
		}
		else if (response[1] == 1)
		{
			Serial.println("Run command starting...");
			return true;
		}
		else if (response[1] == 2)
		{
			Serial.println("Run command completed");
			return true;
		}
	}
	else
	{
		Serial.println("Invalid response from motor controller");
		return false;
	}
}
