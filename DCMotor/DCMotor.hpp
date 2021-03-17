#pragma once

#ifndef DCMotor_h
#define DCMotor_h
#include <Arduino.h>
#include <assert.h>

typedef enum __Command__ :uint8_t {
	RELEASE,
	BRAKE,
	FORWARD,
	BACKWARD
}Command;

class IMotor {
protected:
	uint8_t speed;
	Command command;

public:
	IMotor() :speed(0), command(RELEASE) {}

	virtual void release() = 0;

	virtual void brake() = 0;

	virtual void moveForward(uint8_t) = 0;

	virtual void moveBackward(uint8_t) = 0;

	void run(uint8_t speed, Command command) {
		switch (command) {
		case RELEASE:
			this->release();
			break;
		case BRAKE:
			this->brake();
			break;
		case FORWARD:
			this->moveForward(speed);
			break;
		case BACKWARD:
			this->moveBackward(speed);
			break;
		}
	}

	inline uint8_t cuurentSpeed() const {
		return this->speed;
	}

	inline Command currentCommand() const {
		return this->command;
	}
};

template<uint8_t digitalPin, uint8_t PWMPin>
class DCMotorSinglePWM :public IMotor {
	static_assert(digitalPinHasPWM(PWMPin));
public:
	DCMotorSinglePWM() :IMotor() {
		pinMode(digitalPin, OUTPUT);
		pinMode(PWMPin, OUTPUT);
	}

	void release() {
		digitalWrite(digitalPin, LOW);
		digitalWrite(PWMPin, LOW);
		this->speed = 0;
		this->command = RELEASE;
	}

	void brake() {
		digitalWrite(digitalPin, HIGH);
		digitalWrite(PWMPin, HIGH);
		this->speed = 0;
		this->command = BRAKE;
	}

	void moveForward(uint8_t speed) {
		digitalWrite(digitalPin, LOW);
		analogWrite(PWMPin, speed);
		this->speed = speed;
		this->command = FORWARD;
	}

	void moveBackward(uint8_t speed) {
		digitalWrite(digitalPin, HIGH);
		analogWrite(PWMPin, 255 - speed);
		this->speed = 255 - speed;
		this->command = BACKWARD;
	}
};

template<uint8_t InputPin1, uint8_t InputPin2>
class DCMotorTwoPWM : public IMotor {
	static_assert(digitalPinHasPWM(InputPin1) && digitalPinHasPWM(InputPin2));
public:
	DCMotorTwoPWM() :IMotor() {
		pinMode(InputPin1, OUTPUT);
		pinMode(InputPin2, OUTPUT);
	}

	void release() {
		digitalWrite(InputPin1, LOW);
		digitalWrite(InputPin2, LOW);
		this->speed = 0;
		this->command = RELEASE;
	}

	void brake() {
		digitalWrite(InputPin1, HIGH);
		digitalWrite(InputPin2, HIGH);
		this->speed = 0;
		this->command = BRAKE;
	}

	void moveForward(uint8_t speed) {
		digitalWrite(InputPin1, LOW);
		analogWrite(InputPin2, speed);
		this->speed = speed;
		this->command = FORWARD;
	}

	void moveBackward(uint8_t speed) {
		digitalWrite(InputPin2, LOW);
		analogWrite(InputPin1, speed);
		this->speed = speed;
		this->command = BACKWARD;
	}
};

template<uint8_t digitalPin1, uint8_t digitalPin2, uint8_t PWMPin>
class DCMotorTB6612 :public IMotor {
	static_assert(digitalPinHasPWM(PWMPin));
public:
	DCMotorTB6612() :IMotor() {
		pinMode(digitalPin1, OUTPUT);
		pinMode(digitalPin2, OUTPUT);
		pinMode(PWMPin, OUTPUT);
	}

	void release() {
		digitalWrite(digitalPin1, LOW);
		digitalWrite(digitalPin2, LOW);
		digitalWrite(PWMPin, HIGH);
		this->speed = 0;
		this->command = RELEASE;
	}

	void brake() {
		digitalWrite(digitalPin1, HIGH);
		digitalWrite(digitalPin2, HIGH);
		this->speed = 0;
		this->command = BRAKE;
	}

	void moveForward(uint8_t speed) {
		digitalWrite(digitalPin1, LOW);
		digitalWrite(digitalPin2, HIGH);
		analogWrite(PWMPin, speed);
		this->speed = speed;
		this->command = FORWARD;
	}

	void moveBackward(uint8_t speed) {
		digitalWrite(digitalPin1, HIGH);
		digitalWrite(digitalPin2, LOW);
		analogWrite(PWMPin, speed);
		this->speed = speed;
		this->command = BACKWARD;
	}
};
#endif