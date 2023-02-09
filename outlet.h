#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>

#define PIN_LED_BUILD_IN D4  // Build in LED 
#define PIN_SW1_REQ D5
#define PIN_SW2_REQ D6
#define PIN_RELAY1 D2
#define PIN_RELAY2 D1

bool switch1_power = false;
bool switch2_power = false;

int sw1_req = LOW;
int sw2_req = LOW;

unsigned long last_read = 0;
#define READ_INTERVAL = 1000;

void switch1_on_set(homekit_value_t value);
void switch2_on_set(homekit_value_t value);

homekit_value_t switch1_on_get();
homekit_value_t switch2_on_get();

void accessory_init();
void accessory_identify(homekit_value_t _value);
void builtInLed_set_status(bool on);
void accessory_loop();
void switch1_on();
void switch1_off();
void switch2_on();
void switch2_off();



void accessory_init() {

  // Set the pinmode for the built-in LED
  pinMode(PIN_LED_BUILD_IN, OUTPUT);

  // Set inputs so we can talk to the Pi
  pinMode(PIN_SW1_REQ, INPUT_PULLUP);
  pinMode(PIN_SW2_REQ, INPUT_PULLUP);

  // Set the outputs so we can talk to the relay board
  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
}

void accessory_loop(){
  if(millis() > last_read + READ_INTERVAL){
    int sw1new = digitalRead(PIN_SW1_REQ);
    if(sw1new != ){

      }
  }
  
  

}

void builtInLed_set_status(bool on) {
	digitalWrite(PIN_LED_BUILD_IN, on ? LOW : HIGH);
}

void builtInLed_blink(int intervalInMs, int count) {
	for (int i = 0; i < count; i++) {
		builtInLed_set_status(true);
		delay(intervalInMs);
		builtInLed_set_status(false);
		delay(intervalInMs);
	}
}

void accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
    builtInLed_blink(500, 3);
}

homekit_value_t switch1_on_get() {
	return HOMEKIT_BOOL(switch1_power);
}

homekit_value_t switch2_on_get(){
  return HOMEKIT_BOOL(switch2_power);
}

// Function that executes instructions
void switch1_on_set(homekit_value_t value) {
	if (value.format != homekit_format_bool) {
		printf("Invalid on-value format: %d\n", value.format);
		return;
	}

    switch1_power = value.bool_value; // Sync value

	if (switch1_power) {
    // This switch is on!
		printf("ON1\n");
    digitalWrite(PIN_RELAY1, HIGH);

	} else {
    // Turn off
		printf("OFF1\n");
		digitalWrite(PIN_RELAY1, LOW);
	}
}

// Homekit Event Handlers
void switch1_on() {
  switcher1.value.bool_value = true;
	switcher1.setter(switcher1.value);
	homekit_characteristic_notify(&switcher1, switcher1.value);
}

void switch1_off() {
  switcher1.value.bool_value = false;
	switcher1.setter(switcher1.value);
	homekit_characteristic_notify(&switcher1, switcher1.value);
}

void switch2_on_set(homekit_value_t value) {
	if (value.format != homekit_format_bool) {
		printf("Invalid on-value format: %d\n", value.format);
		return;
	}

    switch2_power = value.bool_value; // Sync value

	if (switch2_power) {
    // This switch is on!
		printf("ON1\n");
    digitalWrite(PIN_RELAY2, HIGH);

	} else {
    // Turn off
		printf("OFF1\n");
		digitalWrite(PIN_RELAY2, LOW);
	}
}

// Homekit Event Handlers
void switch2_on() {
  switcher2.value.bool_value = true;
	switcher2.setter(switcher2.value);
	homekit_characteristic_notify(&switcher2, switcher2.value);
}

void switch2_off() {
  switcher2.value.bool_value = false;
	switcher2.setter(switcher2.value);
	homekit_characteristic_notify(&switcher2, switcher2.value);
}

