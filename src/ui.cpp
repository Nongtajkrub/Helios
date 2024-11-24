#include "ui.hpp"
#include <stdlib.h>

#define SELC_INDICATOR ">"

namespace ui {
	constexpr u16 OPT_ELEM_SIZE = sizeof(opt_elem_t);

	/* -------------- LCD SCREEN --------------*/
	void ready_screen(I2C* screen) {
		screen->init();
		screen->backlight();
	}

	static void show(I2C* screen, const char* text, u8 x, u8 y) {
		screen->setCursor(x, y);
		screen->print(text);
	}

	/* --------------- SELETOR ---------------*/
	void selector_make(
		selector_t* selc,
		funcb* up_trig,
		funcb* down_trig,
		funcb* selc_trig,
		void* arg
		) {
		selc->up_trig = up_trig;
		selc->down_trig = down_trig;
		selc->selc_trig = selc_trig;
		selc->arg = arg;
	}

	static void selc_up(selector_t* selc) {
		if (selc->on > 0) {
			selc->on--;
		}
	}

	static void selc_down(selector_t* selc, u8 limit) {
		if (selc->on < limit) {
			selc->on++;
		}
	}

	static void selc_up_down_control_loop(selector_t* selc, u8 down_limit) {
		if (selc->up_trig(selc->arg)) {
			selc_up(selc);
		} else if (selc->down_trig(selc->arg)) {
			selc_down(selc, down_limit);
		}
	}

	/* ---------- UI OPTION TYPE ---------- */
	void opt_elem_make(opt_elem_t* elem, const char* lable, func* comm, void* arg) {
		elem->lable = lable;
		elem->comm = comm;
		elem->arg = arg;
	}

	static void opt_init_opts(opt_t* opt, va_list* opts) {
		opt->opts = (opt_elem_t**)calloc(opt->num, sizeof(opt_elem_t*));

		for (u8 i = 0; i < opt->num; i++) {
			opt->opts[i] = va_arg(*opts, opt_elem_t*);
		}
	}

	void opt_make(
		opt_t* opt,
		const char* header,
		I2C* screen,
		selector_t* selc,
		int num, ...
		) {
		va_list opts;
		va_start(opts, num);

		opt->header = header;
		opt->num = num;
		opt_init_opts(opt, &opts);

		opt->selc = selc;

		opt->screen = screen;
	}

	void opt_show(opt_t* opt) {
		opt->screen->clear();

		// show header
		show(opt->screen, opt->header, 0, 0);

		// show options
		for (u8 i = 0; i < opt->num; i++) {
			// i + 1 to not overide header
			if (i == opt->selc->on) {
				// print the selector indicator first than the option label 
				show(opt->screen, SELC_INDICATOR, 0, i + 1);
				show(opt->screen, opt->opts[i]->lable, 1, i + 1);
			} else {
				// just print the option label 
				show(opt->screen, opt->opts[i]->lable, 0, i + 1);
			}
		}
	}


	static inline void opt_handle_selc(opt_t* opt) {
		(*opt->opts[opt->selc->on]->comm)(opt->selc->arg);
	}

	void opt_loop(opt_t* opt) {
		selc_up_down_control_loop(opt->selc, opt->num - 1);

		if (opt->selc->selc_trig(opt->selc->arg)) {
			opt_handle_selc(opt);
		}
	}
}
