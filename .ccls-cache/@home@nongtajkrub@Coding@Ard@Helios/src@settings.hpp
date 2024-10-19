#pragma once

// ui settings
#ifdef GET_UI_SETTINGS
	#define LCD_ADDR 0x27
	#define LCD_COLS 20
	#define LCD_ROWS 4

	#define UP_BUTT_PIN 12
	#define DOWN_BUTT_PIN 14
	#define SEL_BUTT_PIN 27

	#define BACK_UI_CHAR "<"
#endif // #ifdef GET_UI_SETTING

// light settings
#ifdef GET_LIGHT_SETTINGS
	#define LDR1_PIN 34

	#define NP_PIN 26
	#define NP_COUNT 1
	#define DEF_NP_R 255
	#define DEF_NP_G 255
	#define DEF_NP_B 255

	// BNF - BRIGHTNESS_NORMALIZATION_FACTOR
	#define CALULATE_BNF(MAX_ADC) ((float)(MAX_ADC) / 100.0f)
	#define BNF CALULATE_BNF(4063)
#endif // #ifdef GET_LIGHT_SETTINGS
