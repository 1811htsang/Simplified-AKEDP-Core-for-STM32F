// Khai bÃ¡o thÆ° viá»‡n sá»­ dá»¥ng
#include "fsm.h"

// Äá»‹nh nghÄ©a hÃ m Ä‘á»ƒ xá»­ lÃ½ tin nháº¯n trong FSM
void fsm_dispatch(fsm_t* me, ak_msg_t* msg) {
	me->state(msg); // Gá»i hÃ m xá»­ lÃ½ tráº¡ng thÃ¡i hiá»‡n táº¡i cá»§a FSM vá»›i tin nháº¯n Ä‘Æ°á»£c truyá»n vÃ o
}

