#ifndef _LV_DRAW_WIFI_TIPS_
#define _LV_DRAW_WIFI_TIPS_

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

extern void lv_draw_wifi_tips(void);
extern void lv_clear_wifi_tips();

typedef enum
{
	TIPS_TYPE_JOINING,
	TIPS_TYPE_TAILED_JOIN,
	TIPS_TYPE_WIFI_CONECTED
}TIPS_TYPE;
extern TIPS_TYPE wifi_tips_type;

typedef struct
{
	unsigned char timer;
	unsigned int timer_count;
}TIPS_DISP;
extern TIPS_DISP tips_disp;

#define TIPS_TIMER_START 1
#define TIPS_TIMER_STOP 0


//extern void disp_temp_ready_print();
#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif

#endif

