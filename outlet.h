#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_NeoPixel.h>
#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>

#define MP3_RX D1
#define MP3_TX D3
#define PIN_LED_BUILD_IN D4  // Build in LED 

#define LIGHT_TIME 10000
#define NEOPIN D2
#define NUMPIXELS 1

SoftwareSerial mp3Serial(MP3_RX, MP3_TX); // RX, TX
DFRobotDFPlayerMini mp3;

bool switch_power = false;
unsigned long turn_off_after = 0;

int rgb_colors[3] = {0,32,255};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

int mp3_volume = 30; //30 might be max?
int mp3_num_songs = 0;
int mp3_current_song = 1;

void mp3_setup();
void mp3_get_songs();
void mp3_loop();
void switch_on_set(homekit_value_t value);
homekit_value_t switch_on_get();
void accessory_init();
void accessory_identify(homekit_value_t _value);
void builtInLed_set_status(bool on);
void accessory_loop();
void switch_on();
void switch_off();
void change_song();


void accessory_init() {

  pinMode(PIN_LED_BUILD_IN, OUTPUT);
  Serial.println("neopixel start");
  // Init the NeoPixel(s)
	pixels.begin();
  // Set the pixel color  
  pixels.setPixelColor(0,pixels.Color(rgb_colors[0], rgb_colors[1], rgb_colors[2]));
  pixels.setBrightness(0);
  pixels.show();
  // Setup the MP3 player
  mp3_setup();
}

void accessory_loop(){

  if((turn_off_after < millis()) && switch_power){
    Serial.println("time up-turn off");
    switch_off();
  }
  if(mp3.available()){
    if(mp3.readType() == DFPlayerCardInserted){
      mp3_get_songs();
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

homekit_value_t switch_on_get() {
	return HOMEKIT_BOOL(switch_power);
}

// Function that executes instructions
void switch_on_set(homekit_value_t value) {
	if (value.format != homekit_format_bool) {
		printf("Invalid on-value format: %d\n", value.format);
		return;
	}

    switch_power = value.bool_value; // Sync value

	if (switch_power) {
    // This switch is on!
		printf("ON\n");

    // Set this to turn off after LIGHT_TIME
		turn_off_after = millis() + LIGHT_TIME;

    // Turn on the LED
    pixels.setBrightness(255);
    pixels.setPixelColor(0,pixels.Color(rgb_colors[0], rgb_colors[1], rgb_colors[2]));
    pixels.show();

    // Play the sound
    mp3.play(mp3_current_song);

    builtInLed_set_status(true);
	} else {
    // Turn off
		printf("OFF\n");
		
    // Stop the sound
    //mp3.stop();

    // Turn off the LED
    pixels.setBrightness(0);
    pixels.show();
    builtInLed_set_status(false);
	}
}

// Homekit Event Handlers
void switch_on() {
  switcher.value.bool_value = true;
	switcher.setter(switcher.value);
	homekit_characteristic_notify(&switcher, switcher.value);
}

void switch_off() {
  switcher.value.bool_value = false;
	switcher.setter(switcher.value);
	homekit_characteristic_notify(&switcher, switcher.value);
}

/*

  MP3 Player Functions

*/
void mp3_setup(){
  mp3Serial.begin(9600);
  mp3.begin(mp3Serial);
  mp3.setTimeOut(500);
  mp3_get_songs();
}

void mp3_get_songs(){
  yield();
  delay(500);
  yield();
  delay(500);
  mp3_num_songs = mp3.readFileCounts();
  #ifdef DEBUG
  Serial.printf("There are %i songs\n", mp3_num_songs);
  #endif
}

void change_song() {
  // Change to the next song. If we are at the last song, then go back to #1
  if(mp3_current_song + 1 <= mp3_num_songs){
    mp3_current_song++;
  }else{
    mp3_current_song = 1;
  }

  // Play the song
  mp3.play(mp3_current_song);
}