#define GET_GLOBAL_SETTINGS
#include "settings.hpp"

// hidden.hpp is hide by gitignore to create pls check the example in README
#define GET_MQTT_HIDDEN
#include "hidden.hpp"

#include "netpie.hpp"
#include "func.hpp"

#define DEF_MSG_SIZE 256
#define UPDATE_SHADOW_TOPIC "@shadow/data/update"

namespace program {
	static void init_hint(struct netpie_data* netpie) {
		netpie->serv_hint.serv = MQTT_SERV;
		netpie->serv_hint.id   = MQTT_ID;
		netpie->serv_hint.user = MQTT_USER;
		netpie->serv_hint.pass = MQTT_PASS;
		netpie->serv_hint.port = MQTT_PORT;
	}

	void netpie_init(struct netpie_data* netpie,struct light_data* light) {
		init_hint(netpie);
		mqtt::make(&netpie->client, &netpie->serv_hint);

		mqtt::connect(&netpie->client, &netpie->serv_hint);

		netpie->light = light;
	}

	static void netpie_update_brightness(struct netpie_data* netpie) {
		/*
		u8 average_brightness = average_arr<u8, u8, u8>(
			light_get_brightness(netpie->light),
			NP_COUNT
			);
		*/

		u8 average_brightness = 10;
		// format msg to send
		char msg[DEF_MSG_SIZE];
		sprintf(
			msg,
			"{\"data\": {\"average_light_brightness\": %d}}",
			average_brightness
			);

		if (!mqtt::send(&netpie->client, UPDATE_SHADOW_TOPIC, msg)) {
			// TODO: handle error
		}
	}

	void netpie_loop(struct netpie_data* netpie) {
		netpie_update_brightness(netpie);
	}
}
