#pragma once

#include "type.hpp"
#include <LiquidCrystal_I2C.h>

typedef LiquidCrystal_I2C I2C;

namespace ui {
	/* -------------- LCD SCREEN --------------*/
	void ready_screen(I2C* screen);

	/* --------------- SELETOR ---------------*/
	typedef struct {
		u8 on;

		funcb* up_trig;
		funcb* down_trig;
		funcb* selc_trig;

		void* arg;
	} selector_t;

	void selector_make(
		selector_t* selc,
		funcb* up_trig,
		funcb* down_trig,
		funcb* selc_trig,
		void* arg
		);

	/* ---------- UI OPTION TYPE ---------- */
	typedef struct {
		const char* lable;
		func* comm;
		void* arg;
	} opt_elem_t;

	void opt_elem_make(opt_elem_t* elem, const char* lable, func* comm, void* arg);

	typedef struct {
		const char* header;
		// number of options
		u8 num;
		opt_elem_t** opts;
		selector_t* selc;

		I2C* screen;
	} opt_t;

	void opt_make(
		opt_t* opt,
		const char* header,
		I2C* screen,
		selector_t* selc,
		int num, ...
		);
	void opt_loop(opt_t* opt);
	void opt_show(opt_t* opt);
}
