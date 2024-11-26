#include "mqtt.hpp"

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

void connect(client_t* cli, struct info* hint) {
	// do nothing if already connected
	if (cli->mqtt->connected()) {
		return;
	}

	while (!cli->mqtt->connect(hint->id, hint->user, hint->pass)) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

bool loop(client_t* cli, struct info* hint) {
	// if got disconnect try and reconnect
	if (!is_connect(cli)) {
		connect(cli, hint);
		cli->mqtt->loop();

		return false;
	}
	cli->mqtt->loop();

	return true;
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
