// TODO: implement num_t

#pragma once

#include "type.h"
#include <LiquidCrystal_I2C.h>

typedef LiquidCrystal_I2C I2C;

#define NO_WRAP 0 

namespace ui {
typedef enum {
	NONE,
	SELC,
	SELC_UP,
	SELC_DOWN,
} event_t;

inline void event_take(event_t* e) {
	*e = NONE;
}

/* -------------- LCD SCREEN --------------*/
void ready_screen(I2C* screen);

void show(I2C* screen, const char* text, u8 x, u8 y);
void show(I2C* screen, const char* text, u8 x, u8 y, u8 lcd_rows, u8 wrap);

/* --------------- SELETOR ---------------*/
typedef struct {
	u16 on;

	funcb* up_trig;
	funcb* down_trig;
	funcb* selc_trig;

	void* arg;
} selector_t;

void selector_make(selector_t* selc,
		funcb* up_trig, funcb* down_trig, funcb* selc_trig, void* arg);

inline u16 selc_on(selector_t* selc) {
	return selc->on;
}

inline void selc_reset(selector_t* selc) {
	selc->on = 0;
}

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
	event_t* e;

	I2C* screen;
	u8 rows;
} opt_t;

void opt_make(opt_t* opt, const char* header,
		I2C* screen, u8 rows, selector_t* selc, event_t* e, int num, ...);
void opt_loop(opt_t* opt);
void opt_show(opt_t* opt);

/* ---- UI NUMBER SELECTOR TYPE ---- */
typedef struct {
	const char* header;

	u16 begin;
	u16 end;
	u8 rows;

	selector_t* selc;
	event_t* e;
	I2C* screen;
} num_t;

void num_make(num_t* num, const char* header,
		u16 begin, u16 end, u8 rows, selector_t* selc, event_t* e, I2C* screen);

void num_show(num_t* num);
bool num_loop(num_t* num);

inline u16 num_result(num_t* num) {
	return num->selc->on;
}
}
