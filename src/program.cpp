// TODO: make multithreading work

#include "program.hpp"

#define GET_MAIN_SETTINGS
#include "settings.hpp"

#include <Arduino.h>
#include <WiFi.h>

namespace program {
	static SemaphoreHandle_t light_thread_ready;
	static SemaphoreHandle_t ui_thread_ready;

	static void light_thread(void* arg) {
		struct data* data = (struct data*)arg;

		light_init(&data->light);
		xSemaphoreGive(light_thread_ready);

		for (;;) {
			light_loop(&data->light);
			vTaskDelay(LOOP_DELAY / portTICK_PERIOD_MS);
		}
	}

	static void ui_thread(void* arg) {
		struct data* data = (struct data*)arg;

		xSemaphoreTake(light_thread_ready, portMAX_DELAY);

		ui_init(&data->ui, &data->light);
		xSemaphoreGive(ui_thread_ready);
	
		for (;;) {
			ui_loop(&data->ui);
			vTaskDelay(LOOP_DELAY / portTICK_PERIOD_MS);
		}
	}

	static void netpie_thread(void* arg) {
		struct data* data = (struct data*)arg;

		xSemaphoreTake(ui_thread_ready, portMAX_DELAY);
		
		netpie_init(&data->netpie, &data->light);

		for (;;) {
			netpie_loop(&data->netpie);
			vTaskDelay(NETPIE_LOOP_DELAY / portTICK_PERIOD_MS);
		}
	}

	void setup(struct data* data) {
		// delay to make sure everything is ready
		delay(SETUP_DELAY);
		//Serial.println("-- HERE TO AVOID GRABADE DATA --");

		light_thread_ready = xSemaphoreCreateBinary();
		ui_thread_ready = xSemaphoreCreateBinary();

		xTaskCreate(
			light_thread,
			"light_thread",
			LIGHT_THREAD_STACK_SIZE,
			(void*)data,
			1,
			NULL
			);

		xTaskCreate(
			ui_thread,
			"ui_thread",
			UI_THREAD_STACK_SIZE,
			(void*)data,
			2,
			NULL
			);

		// only start netpie if wifi is connected
		if (WiFi.status() == WL_CONNECTED) {
			xTaskCreate(
				netpie_thread,
				"netpie_thread",
				NETPIE_THREAD_STACK_SIZE,
				(void*)data,
				3,
				NULL
				);
		}
	}	
}
