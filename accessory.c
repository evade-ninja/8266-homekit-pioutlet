#include <Arduino.h>
#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>

#define HK_ACC_NAME ("DoorbellChime")
#define HK_MFR ("NinjaHome")
#define HK_SERIAL ("NHDBC0001B")
#define HK_MODEL ("DB8266")
#define HK_FW ("0.1")
#define HK_PWD ("111-11-111")
#define HK_SETUP ("HKDB")

void accessory_identify(){};

homekit_characteristic_t switcher = HOMEKIT_CHARACTERISTIC_(ON, false);

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "DoorbellChime"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "NinjaHome"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "NHDBC0001B"),
            HOMEKIT_CHARACTERISTIC(MODEL, "DB8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.1"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, accessory_identify),
            NULL
		}),
        HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
            &switcher,
			HOMEKIT_CHARACTERISTIC(NAME, "DoorbellChime"),
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "111-11-111",
    .setupId = "HKDB"
};
