#pragma once

#include "mqtt.hpp"

#include "light_prog.hpp"

namespace program {
	struct netpie_data {
		struct mqtt::info serv_hint;
		mqtt::client_t client;

		// use to communicate with ligth system
		struct light_data* light;
	};

	void netpie_init(struct netpie_data* netpie,struct light_data* light);
	void netpie_loop(struct netpie_data* netpie);
}
