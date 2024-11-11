#include "mqtt.hpp"

#define MAX_RETRY 10

namespace mqtt {
	void make(client_t* cli, WiFiClient& wifi, const char* serv, u16 port) {
		cli->mqtt = PubSubClient(wifi);
		cli->mqtt.setServer(serv, port);
	}

	bool connect(client_t* cli, const char* id, const char* user, const char* pass) {
		// do nothing if already connected
		if (cli->mqtt.connected()) {
			return true;
		}

		u16 retry = 0;

		while (!cli->mqtt.connect(id, user, pass)) {
			if (retry > MAX_RETRY) {
				return false;
			}

			retry++;
		}

		// if reach here mean connected successfully
		return true;
	}

	void disconnect(client_t* cli) {
		cli->mqtt.disconnect();
	}

	bool send(client_t* cli, const char* topic, const char* payload) {
	#ifdef DONT_SEND_OLD_PAYLOD
		// avoiding sending old payload
		if (strcmp(payload, cli->old_payload) == 0) {
			return false;
		}
	#endif // #ifdef DONT_SEND_OLD_PAYLOD
		
		// send and return whether send is successfully or not
		return !cli->mqtt.publish(topic, payload);
	}
}
