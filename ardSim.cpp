// Do not remove the include below
#include "ardSim.h"
#include "Timer.h"

#define TIME_STEP 10

#define LED 13
#define BUT 8

int but;
Timer tim;
int tim_event;
long t;
int x, y, z;

void takeReading()
{
    x = (x+3) % 512;
    y = (y+5) % 512;
    z = (z+7) % 512;

	// envia uma mensagem "t ttt xxx yyy zzz\n"
	// o ttt e' o tempo em milisegundos
	Serial.print("t ");
	Serial.print(t);
	Serial.print(" ");
	Serial.print(x);
	Serial.print(" ");
	Serial.print(y);
	Serial.print(" ");
	Serial.print(z);
	Serial.print("\n"); // O println envia "\r\n" no fim!

	t = t + TIME_STEP;
}

void setup() {
	t = x = y = z = 0;

	pinMode(BUT, INPUT);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);

	Serial.begin(115200);
}

#define CMD_SIZE 10

char *getMsg(void) {
	int c;
	int static index;
	char static cmd[CMD_SIZE+1];

	if ((c = Serial.read()) != -1) {
		if (index && ((char) c == '\r')) {
			cmd[index] = '\0';
			index = 0;
			return cmd;
		}
		else {
			if (index < CMD_SIZE)
				cmd[index++] = (char) c;
			return 0;
		}
	}
	return 0;
}

void procMsg(char *msg) {
	switch(msg[0]) {
		case 'l':
			if (msg[1] == '1') {
				digitalWrite(LED, HIGH);
				Serial.println("LED ON");
			}
			else {
				digitalWrite(LED, LOW);
				Serial.println("LED OFF");
			}
			break;
		case 'b':
			if (!but)
				Serial.println("BUT ON");
			else
				Serial.println("BUT OFF");
			break;
		case 'a':
			if (msg[1] == '1') {
				tim_event = tim.every(TIME_STEP, takeReading);
				Serial.println("Timer ON");
			}
			else {
				tim.stop(tim_event);
				Serial.println("Timer OFF");
			}
			break;
	}
}

// the loop routine runs over and over again forever:
void loop() {
	char *msg;
	if ((msg = getMsg()) != 0) {
		Serial.println(msg);
		procMsg(msg);
	}

	int b = digitalRead(BUT);
	if (b != but) {
		but = b;
		Serial.println((!but) ? "BUT ON" : "BUT OFF");
	}

	tim.update();
}

