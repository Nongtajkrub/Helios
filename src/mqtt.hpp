#pragma once

#include "type.hpp"

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

namespace mqtt {
	typedef struct {
		PubSubClient mqtt;

	#ifdef DONT_SEND_OLD_PAYLOD 
		// use to prevent resending old data
		const char* old_payload;
	#endif // #ifdef CHECK_OLD_PAYLOAD
	} client_t;

	void make(client_t* cli, WiFiClient& wifi, const char* serv, u16 port);

	bool connect(client_t* cli, const char* id, const char* user, const char* pass);
	void disconnect(client_t* cli);

	bool send(client_t* cli, const char* topic, const char* payload);
}
