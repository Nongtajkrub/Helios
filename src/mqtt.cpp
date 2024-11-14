#include "mqtt.hpp"

#define MAX_RETRY 10

namespace mqtt {
	static void callback(char* topic, byte* payload, uint len) {
		Serial.println("Msg recv topic: ");
		Serial.println(topic);
	}

	void make(client_t* cli, struct info* hint) {
		cli->wifi = WiFiClient();
		cli->mqtt = PubSubClient(cli->wifi);
		cli->mqtt.setServer(hint->serv, hint->port);
		cli->mqtt.setCallback(callback);
		cli->old_payload = "";
	}

	bool connect(client_t* cli, struct info* hint) {
		// do nothing if already connected
		if (cli->mqtt.connected()) {
			return true;
		}

		u16 retry = 0;

		while (!cli->mqtt.connect(hint->id, hint->user, hint->pass)) {
			if (retry > MAX_RETRY) {
				return false;
			}

			retry++;
		}

		// if reach here mean connected successfully
		return true;
	}

	// TODO: handle disconnect
	bool loop(client_t* cli, struct info* hint) {
		bool loop_result= true;

		while (!is_connect(cli)) {
			loop_result = false;
			connect(cli, hint);
			delay(500);
		}
		cli->mqtt.loop();

		return loop_result;
	}

	// va_arg is topics
	void sub(client_t* cli, int count, ...) {
		if (count <= 0) {
			return;
		}

		va_list topics;
		va_start(topics, count);

		for (u8 i = 0; i < count; i++) {
			const char* topic = va_arg(topics, const char*);
			cli->mqtt.subscribe(topic);
			Serial.print("Sub to ");
			Serial.print(topic);
		}
	}

	bool send(client_t* cli, const char* topic, const char* payload) {
		// avoiding sending old payload
		if (strcmp(payload, cli->old_payload) == 0) {
			return false;
		}
		cli->old_payload = payload;

		// send and return whether send is successfully or not
		return !cli->mqtt.publish(topic, payload);
	}
}