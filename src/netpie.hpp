// TODO: implement netpie

#pragma once

#define DONT_SEND_OLD_PAYLOD
#include "mqtt.hpp"

namespace program {
	struct netpie_data {
		mqtt::client_t netpie;

		struct {
			const char* serv;
			const u16 port;
			const char* id;
			const char* user;
			const char* pass;
		} info;
	};

	void netpie_init();
}
