// no pragma once just have to be careful

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
	// prevent redefinition
	#ifndef GET_GLOBAL_SETTINGS
		// NP_COUNT and LDR_COUNT always have to be the same 
		#define NP_COUNT 4
		#define LDR_COUNT NP_COUNT
	#endif // #ifndef GET_GLOBAL_SETTINGS

	const u8 LDR_PINS[LDR_COUNT] = {34, 35, 32, 33};
	const u8 NP_PINS[NP_COUNT] = {19, 16, 18, 17};

	#define NP_ROWS 2
	#define NP_COLS 2
	#define DEF_NP_R 255
	#define DEF_NP_G 0 
	#define DEF_NP_B 0 

	// RNF - READING_NORMALIZATION_FACTOR
	#define CALULATE_RNF(MAX_ADC) ((float)(MAX_ADC) / 100.0f)
	#define RNF CALULATE_RNF(4063)

	// BPWM - READING_PROPAGATION_WEIGHT_MULTIPLIER
	#define RPWM 0.05
#endif // #ifdef GET_LIGHT_SETTINGS

#ifdef GET_MAIN_SETTINGS
	#define LOOP_DELAY 50

	#define UI_THREAD_STACK_SIZE 128
#endif //#ifdef GET_MAIN_SETTINGS

// use when including setting.hpp in a header file
#ifdef GET_GLOBAL_SETTINGS
	// prevent redefinition
	#ifndef GET_LIGHT_SETTINGS
		// NP_COUNT and LDR_COUNT always have to be the same 
		#define NP_COUNT 4
		#define LDR_COUNT NP_COUNT
	#endif // #ifndef GET_LIGHT_SETTINGS
#endif // #ifdef GET_GLOBAL_SETTING
