#include "mqtt.hpp"

#define MAX_RETRY 10

namespace mqtt {
	static void callback(char* topic, byte* payload, uint len) {
		Serial.println("Msg recv topic: ");
		Serial.println(topic);
	}

	void make(client_t* cli, struct info* hint) {
		cli->wifi = new WiFiClient();
		cli->mqtt = new PubSubClient(*cli->wifi);
		cli->mqtt->setServer(hint->serv, hint->port);
		cli->mqtt->setCallback(callback);
		cli->old_payload = NULL;
	}

	bool connect(client_t* cli, struct info* hint) {
		// do nothing if already connected
		if (cli->mqtt->connected()) {
			return true;
		}

		u16 retry = 0;

		while (!cli->mqtt->connect(hint->id, hint->user, hint->pass)) {
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
			vTaskDelay(500 / portTICK_PERIOD_MS);
		}
		cli->mqtt->loop();

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
			cli->mqtt->subscribe(topic);
			Serial.print(topic);
		}
	}

	bool send(client_t* cli, const char* topic, const char* payload) {
		// avoiding sending old payload
		if (strcmp(payload, (cli->old_payload != NULL) ? cli->old_payload : "") == 0) {
			return false;
		}

		u16 payload_size = strlen(payload);

		if (cli->old_payload != NULL) {
			free(cli->old_payload);
		}

		cli->old_payload = (char*)malloc(payload_size + 1);
		memset(cli->old_payload, '\0', payload_size); 
		memcpy(cli->old_payload, payload, payload_size);

		// send and return whether send is successfully or not
		return cli->mqtt->publish(topic, payload);
	}
}
