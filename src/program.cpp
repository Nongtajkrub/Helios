// TODO: make multithreading work

#include "program.hpp"

#define GET_MAIN_SETTINGS
#include "settings.hpp"

namespace program {
	static void light_thread(void* arg) {
		/*
		struct light_data* light = (struct light_data*)arg;

		light_init(light);
		while (true) {
			light_loop(light);
			vTaskDelay(LOOP_DELAY / portTICK_PERIOD_MS);
		}
		*/

		while (1) {
			Serial.println("light");
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}

	static void ui_thread(void* arg) {
		/*
		struct data* data = (struct data*)arg;

		ui_init(&data->ui, &data->light);
		while (true) {
			ui_loop(&data->ui);
			vTaskDelay(LOOP_DELAY / portTICK_PERIOD_MS);
		}
		*/
		while (1) {
			Serial.println("UI");
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}

	void setup(struct data* data) {
		light_init(&data->light);
		//ui_init(&data->ui, &data->light);
		/*
		xTaskCreate(
			ui_thread,
			"ui_thread",
			UI_THREAD_STACK_SIZE,
			NULL,
			1,
			NULL
			);

		xTaskCreate(
			light_thread,
			"light_thread",
			UI_THREAD_STACK_SIZE,
			(void*)data,
			1,
			NULL
			);

		vTaskStartScheduler();

		Serial.println(esp_get_free_heap_size());
		*/
	}	

	void loop(struct data* data) {
		light_loop(&data->light);
		//ui_loop(&data->ui);
		delay(LOOP_DELAY);
		//vTaskDelay(LOOP_DELAY / portTICK_PERIOD_MS);
	}
}
