/*
	Name:		ControlerMatrix.ino
	Created:	18.11.2020 17:50:43
	Author:		parzival
*/

// import led matrix lib
#include <LedControl.h>

// preparing analog and digital stick pins
const int		VRx			=		0;		// analog pin connected to X output
const int		VRy			=		1;		// analog pin connected to Y output
const int		SW			=		2;		// digital pin connected to switch output

// preparing digital matrix pins
const int		DATA		=		12;		// digital pin connected to data input
const int		CS			=		11;		// digital pin connected to load input
const int		CLK			=		10;		// digital pin connected to CLK

// preparing digital mode switch pin
const int		MS			=		3;		// digital pin connected to switch

// initializing LedControl lib
LedControl lc = LedControl(DATA, CLK, CS, 1);

// options
const bool		DEBUG		=		false;	// enables or disables the debug mode
const int		REFRESH		=		100;	// point refresh rate per second
bool			THICK		=		false;	// draws a thicker point (2x2) instead a small one

// the setup function runs once when you press reset or power the board
void setup() {
	// prepare joystick switch input
	pinMode(SW, INPUT);
	digitalWrite(SW, HIGH);

	// preparing digital mode switch pin
	pinMode(MS, INPUT);
	digitalWrite(MS, HIGH);

	// initialize led matrix
	lc.shutdown(0, false);
	lc.setIntensity(0, 2);
	lc.clearDisplay(0);

	// starting serial monitor
	if (DEBUG) Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	// toggle mode on switch
	if (digitalRead(MS) == 0) THICK = !THICK;

	// collect raw data from analog stick
	int XR = analogRead(VRx);		// 1 - 1024
	int YR = analogRead(VRy);		// 1 - 1024

	// calculating led positions
	int DIVIDER = THICK ? 147 : 128;	// toggles thick mode for grid calculation
	int X = (XR / DIVIDER);				// 128 substracted to prevent overdraw
	int Y = (YR / DIVIDER);				// divided by 128 to obtain a coordinate between 1 and 8

	// debug only: send analog stick position to serial
	if (DEBUG) {
		Serial.println("\n\n\n");

		Serial.print("Switch:  ");
		Serial.println(digitalRead(SW));

		Serial.print("Mode:  ");
		Serial.println(digitalRead(MS));

		Serial.print("X:  ");
		Serial.println(X);

		Serial.print("Y:  ");
		Serial.println(Y);
	}

	// clearing old point
	lc.clearDisplay(0);

	// drawing point
	lc.setLed(0, Y, X, true);	// cord of point

	// toggles thick mode
	if (THICK) {
		lc.setLed(0, Y, (X + 1), true);
		lc.setLed(0, (Y + 1), X, true);
		lc.setLed(0, (Y + 1), (X + 1), true);
	}

	// raw exact cord if SW is pressed
	if (digitalRead(SW) == 0) {
		// draw x line
		for (int ROWx = 0; ROWx < 8; ROWx++) { lc.setLed(0, Y, ROWx, true); }

		// draw y line
		for (int ROWy = 0; ROWy < 8; ROWy++) { lc.setLed(0, ROWy, X, true); }

		// thick mode only: draw additional x & y lines
		if (THICK) {
			for (int ROWx = 0; ROWx < 8; ROWx++) { lc.setLed(0, (Y + 1), ROWx, true); }
			for (int ROWy = 0; ROWy < 8; ROWy++) { lc.setLed(0, ROWy, (X + 1), true); }
		}
	}

	delay((1000 / REFRESH));
}
