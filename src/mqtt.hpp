#pragma once

#include "type.hpp"

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

namespace mqtt {
	struct info {
		const char* serv;
		const char* id;
		const char* user;
		const char* pass;
		u16 port;
	};

	typedef struct {
		WiFiClient* wifi;
		PubSubClient* mqtt;

		// use to prevent resending old data
		char* old_payload;
	} client_t;

	void make(client_t* cli, struct info* hint);

	void connect(client_t* cli, struct info* hint);

	bool loop(client_t* cli, struct info* hint);

	void sub(client_t* cli, int count, ...);
	bool send(client_t* cli, const char* topic, const char* payload);

	inline void disconnect(client_t* cli) {
		cli->mqtt->disconnect();
	}

	inline bool is_connect(client_t* cli) {
		return cli->mqtt->connected();
	}

	inline void sub(client_t* cli, const char* topic) {
		cli->mqtt->subscribe(topic);
	}
}
