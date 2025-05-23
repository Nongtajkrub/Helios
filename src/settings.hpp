// ui settings
#ifdef GET_UI_SETTINGS
	#define LCD_ADDR 0x27
	#define LCD_COLS 20
	#define LCD_ROWS 4

	#define UP_BUTT_PIN 12
	#define DOWN_BUTT_PIN 14
	#define SELC_BUTT_PIN 26

	#define BACK_INDICATOR "<"
	#define NOT_MANU_ERR_MSG "Enter manual mode first"
#endif // #ifdef GET_UI_SETTING

// light settings
#ifdef GET_LIGHT_SETTINGS
	// prevent redefinition
	#ifndef GET_GLOBAL_SETTINGS
		// NP_COUNT and LDR_COUNT always have to be the same 
		#define NP_COUNT 4
		#define LDR_COUNT NP_COUNT
	#endif // #ifndef GET_GLOBAL_SETTINGS

	// constexpr u8 LDR_PINS[] = {32, 35, 34, 33};
	// constexpr u8 NP_PINS[] = {19, 15, 18, 17};
	
	constexpr u8 LDR_PINS[] = {33, 34, 35, 32};
	constexpr u8 NP_PINS[] = {17, 18, 15, 19};

	#define NP_ROWS 2
	#define NP_COLS 2
	#define DEF_NP_R 100
	#define DEF_NP_G 100
	#define DEF_NP_B 100

	#define LDR_MAX_ADC 4096

	// RNF - READING_TO_BRIGHTNESS_NORMALIZATION_FACTOR
	#define RTBNF ((float)(LDR_MAX_ADC) / 100.0f)

	// BPWM - BRIGHTNESS_PROPAGATION_WEIGHT_MULTIPLIER
	#define BPWM 0.1
#endif // #ifdef GET_LIGHT_SETTINGS

#ifdef GET_NETPIE_SETTINGS
	#define DEF_MSG_SIZE 256
	#define UPDATE_SHADOW_TOPIC "@shadow/data/update"

	#define SUB_TOPICS "@msg/operator"
	#define SUB_COUNT 1
#endif // #ifdef GET_NETPIE_SETTINGS

#ifdef GET_MAIN_SETTINGS
	#define SETUP_DELAY 3000
	#define LOOP_DELAY 50
	#define NETPIE_LOOP_DELAY 2500

	#define UI_THREAD_STACK_SIZE 1536  
	#define LIGHT_THREAD_STACK_SIZE 2048 
	#define NETPIE_THREAD_STACK_SIZE 2048 
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

#ifdef GET_DEBUG_SETTINGS
	#define INVERT_SIGNAL 1
#endif // #ifdef DEBUG
