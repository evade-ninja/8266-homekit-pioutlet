#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <ESP8266WebServer.h>
#include "ESPButton.h"
#include <DNSServer.h>
#include <WiFiManager.h>	// https://github.com/tzapu/WiFiManager

extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t switcher;

#include "chime.h"

#define PIN_BUTTON D5 		// D5 Button

WiFiManager wifiManager;

void setup() {
	Serial.begin(115200);
	
	// WiFiManager
	
	wifiManager.setConfigPortalTimeout(180);  // in seconds
	if(!wifiManager.autoConnect("ESP_Switch_AP")) {
    	printf("Failed to connect and hit timeout");
	} 

  Serial.println("Wifi Connected!");

	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	homekit_setup();
  Serial.println("Homekit init");
}

void loop() {
	homekit_loop();
  accessory_loop();
	delay(10);
}


void homekit_setup() {
	accessory_init();
  Serial.println("accessory init");

	pinMode(PIN_BUTTON, INPUT_PULLUP);
	
	ESPButton.add(0, PIN_BUTTON, LOW, true, true);
	
	ESPButton.setCallback([&](uint8_t id, ESPButtonEvent event) {
		printf("Button %d Event: %s\n", id, ESPButton.getButtonEventDescription(event));
		
		if (event == ESPBUTTONEVENT_SINGLECLICK) {
			change_song();
		} else if (event == ESPBUTTONEVENT_DOUBLECLICK) {
			// Free to configure
		} else if (event == ESPBUTTONEVENT_LONGCLICK) {
			printf("Rebooting...\n");
			homekit_storage_reset();
      wifiManager.resetSettings();
			ESP.restart(); // or system_restart();
    }
	});
  

	ESPButton.begin();
  Serial.println("button init");
  switcher.getter=switch_on_get;
	switcher.setter=switch_on_set;
	arduino_homekit_setup(&config);
  Serial.println("hksetup!");
}

static uint32_t next_heap_millis = 0;

void homekit_loop() {
  //Serial.println("hkloop");
	ESPButton.loop();
  //Serial.println("button loop done");
	arduino_homekit_loop();
  //Serial.println("ahkl done");
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// Show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		printf("Free heap: %d, HomeKit clients: %d\n",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}
