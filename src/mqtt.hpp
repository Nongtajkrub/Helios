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
		WiFiClient wifi;
		PubSubClient mqtt;

	#ifdef DONT_SEND_OLD_PAYLOD 
		// use to prevent resending old data
		const char* old_payload;
	#endif // #ifdef CHECK_OLD_PAYLOAD
	} client_t;

	void make(client_t* cli, struct info* hint);

	bool connect(client_t* cli, struct info* hint);
	void disconnect(client_t* cli);

	bool send(client_t* cli, const char* topic, const char* payload);
}
