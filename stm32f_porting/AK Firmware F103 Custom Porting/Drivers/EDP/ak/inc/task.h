#ifndef __TASK_H__
	#define __TASK_H__

	// Khai bÃ¡o C directive cho C++
	#ifdef __cplusplus
	extern "C"
	{
	#endif

		// Khai bÃ¡o cÃ¡c thÆ° viá»‡n sá»­ dá»¥ng
		#include <stdint.h>
		#include <stdbool.h>
		#include "port.h"			 	// ThÆ° viá»‡n quáº£n lÃ½ attribute vÃ  dynamic allocation
		#include "log_queue.h"  // ThÆ° viá»‡n quáº£n lÃ½ hÃ ng Ä‘á»£i log

		/* Forward declaration to avoid cyclic include with ak.h/message.h. */
		typedef struct ak_msg_t ak_msg_t;

		// Khai bÃ¡o háº±ng sá»‘ vÃ  macro liÃªn quan Ä‘áº¿n tÃ¡c vá»¥
		#define LOG_QUEUE_OBJECT_SIZE			(512) // Quáº£n lÃ½ kÃ­ch thÆ°á»›c cá»§a hÃ ng Ä‘á»£i log Ä‘á»‘i tÆ°á»£ng, giÃºp xÃ¡c Ä‘á»‹nh sá»‘ lÆ°á»£ng log cÃ³ thá»ƒ chá»©a trong hÃ ng Ä‘á»£i log Ä‘á»‘i tÆ°á»£ng
		#define LOG_QUEUE_IRQ_SIZE				(128) // Quáº£n lÃ½ kÃ­ch thÆ°á»›c cá»§a hÃ ng Ä‘á»£i log ngáº¯t, giÃºp xÃ¡c Ä‘á»‹nh sá»‘ lÆ°á»£ng log cÃ³ thá»ƒ chá»©a trong hÃ ng Ä‘á»£i log ngáº¯t

		// Khai bÃ¡o cÃ¡c kiá»ƒu dá»¯ liá»‡u Ä‘á»ƒ quáº£n lÃ½ tÃ¡c vá»¥
		typedef uint8_t	task_pri_t; // Quáº£n lÃ½ kiá»ƒu dá»¯ liá»‡u cho má»©c Æ°u tiÃªn cá»§a tÃ¡c vá»¥, giÃºp xÃ¡c Ä‘á»‹nh má»©c Ä‘á»™ Æ°u tiÃªn cá»§a cÃ¡c tÃ¡c vá»¥ trong há»‡ thá»‘ng
		typedef uint8_t	task_id_t; // Quáº£n lÃ½ kiá»ƒu dá»¯ liá»‡u cho ID cá»§a tÃ¡c vá»¥, giÃºp xÃ¡c Ä‘á»‹nh vÃ  quáº£n lÃ½ cÃ¡c tÃ¡c vá»¥ trong há»‡ thá»‘ng thÃ´ng qua ID cá»§a chÃºng

			/**
		 	* Ghi chÃº:
		 	* `task_id_t` cÃ³ thá»ƒ Ä‘Æ°á»£c sá»­ dá»¥ng Ä‘á»ƒ quáº£n lÃ½ ID cá»§a cÃ¡c tÃ¡c vá»¥, bao gá»“m cáº£ tÃ¡c vá»¥ ngáº¯t vÃ  tÃ¡c vá»¥ nhÃ n rá»—i, giÃºp xÃ¡c Ä‘á»‹nh vÃ  xá»­ lÃ½ cÃ¡c tÃ¡c vá»¥ trong há»‡ thá»‘ng má»™t cÃ¡ch hiá»‡u quáº£.
		 	*/

		// Khai bÃ¡o con trá» hÃ m Ä‘á»ƒ quáº£n lÃ½ cÃ¡c hÃ m thá»±c thi cá»§a tÃ¡c vá»¥
		typedef void (*pf_task)(ak_msg_t*); // Quáº£n lÃ½ cÃ¡c task theo message-driven
		typedef void (*pf_task_polling)(); 	// Quáº£n lÃ½ cÃ¡c task theo polling-driven

		/**
		 * Ghi chÃº:
		 * `pf_task` Ä‘Æ°á»£c sá»­ dá»¥ng Ä‘á»ƒ quáº£n lÃ½ cÃ¡c hÃ m thá»±c thi cá»§a tÃ¡c vá»¥ 
		 * theo cÆ¡ cháº¿ message-driven, nÆ¡i má»—i tÃ¡c vá»¥ sáº½ Ä‘Æ°á»£c kÃ­ch hoáº¡t 
		 * vÃ  thá»±c thi dá»±a trÃªn cÃ¡c tin nháº¯n mÃ  nÃ³ nháº­n Ä‘Æ°á»£c.
		 * 
		 * `pf_task_polling` Ä‘Æ°á»£c sá»­ dá»¥ng Ä‘á»ƒ quáº£n lÃ½ cÃ¡c hÃ m thá»±c thi cá»§a tÃ¡c vá»¥ 
		 * theo cÆ¡ cháº¿ polling-driven, nÆ¡i má»—i tÃ¡c vá»¥ sáº½ Ä‘Æ°á»£c kÃ­ch hoáº¡t 
		 * vÃ  thá»±c thi liÃªn tá»¥c hoáº·c theo má»™t lá»‹ch trÃ¬nh nháº¥t Ä‘á»‹nh, 
		 * khÃ´ng phá»¥ thuá»™c vÃ o viá»‡c nháº­n tin nháº¯n.
		 */

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ thÃ´ng tin cá»§a tÃ¡c vá»¥
		typedef struct {
			task_id_t id;		// Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥, giÃºp xÃ¡c Ä‘á»‹nh vÃ  quáº£n lÃ½ cÃ¡c tÃ¡c vá»¥ trong há»‡ thá»‘ng thÃ´ng qua ID cá»§a chÃºng
			task_pri_t pri;	// Quáº£n lÃ½ má»©c Æ°u tiÃªn cá»§a tÃ¡c vá»¥, giÃºp xÃ¡c Ä‘á»‹nh má»©c Ä‘á»™ Æ°u tiÃªn cá»§a cÃ¡c tÃ¡c vá»¥ trong há»‡ thá»‘ng
			pf_task task;		// Quáº£n lÃ½ hÃ m thá»±c thi cá»§a tÃ¡c vá»¥, giÃºp xÃ¡c Ä‘á»‹nh chá»©c nÄƒng vÃ  hÃ nh vi cá»§a tÃ¡c vá»¥ khi nÃ³ Ä‘Æ°á»£c kÃ­ch hoáº¡t vÃ  thá»±c thi dá»±a trÃªn cÃ¡c tin nháº¯n mÃ  nÃ³ nháº­n Ä‘Æ°á»£c
		} task_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ thÃ´ng tin cá»§a tÃ¡c vá»¥ polling
		typedef struct {
			task_id_t id;									// Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ polling, giÃºp xÃ¡c Ä‘á»‹nh vÃ  quáº£n lÃ½ cÃ¡c tÃ¡c vá»¥ polling trong há»‡ thá»‘ng thÃ´ng qua ID cá»§a chÃºng
			uint8_t ability;							// Quáº£n lÃ½ kháº£ nÄƒng cá»§a tÃ¡c vá»¥ polling, giÃºp xÃ¡c Ä‘á»‹nh cÃ¡c kháº£ nÄƒng hoáº·c chá»©c nÄƒng mÃ  tÃ¡c vá»¥ polling cÃ³ thá»ƒ thá»±c hiá»‡n khi nÃ³ Ä‘Æ°á»£c kÃ­ch hoáº¡t vÃ  thá»±c thi liÃªn tá»¥c hoáº·c theo má»™t lá»‹ch trÃ¬nh nháº¥t Ä‘á»‹nh
			pf_task_polling task_polling;	// Quáº£n lÃ½ hÃ m thá»±c thi cá»§a tÃ¡c vá»¥ polling, giÃºp xÃ¡c Ä‘á»‹nh chá»©c nÄƒng vÃ  hÃ nh vi cá»§a tÃ¡c vá»¥ polling khi nÃ³ Ä‘Æ°á»£c kÃ­ch hoáº¡t vÃ  thá»±c thi liÃªn tá»¥c hoáº·c theo má»™t lá»‹ch trÃ¬nh nháº¥t Ä‘á»‹nh, khÃ´ng phá»¥ thuá»™c vÃ o viá»‡c nháº­n tin nháº¯n
		} task_polling_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ thÃ´ng tin cá»§a ngoáº¡i lá»‡
		typedef struct {
			uint32_t except_number;				// Quáº£n lÃ½ sá»‘ hiá»‡u cá»§a ngoáº¡i lá»‡, giÃºp xÃ¡c Ä‘á»‹nh loáº¡i ngoáº¡i lá»‡ Ä‘Ã£ xáº£y ra trong há»‡ thá»‘ng
			uint32_t timestamp;						// Quáº£n lÃ½ thá»i gian xáº£y ra ngoáº¡i lá»‡, giÃºp theo dÃµi vÃ  phÃ¢n tÃ­ch thá»i Ä‘iá»ƒm xáº£y ra ngoáº¡i lá»‡ Ä‘á»ƒ há»— trá»£ viá»‡c debug vÃ  cáº£i thiá»‡n hiá»‡u suáº¥t cá»§a há»‡ thá»‘ng
		} exception_info_t;

		// Khai bÃ¡o directive Ä‘á»ƒ quáº£n lÃ½ viá»‡c kÃ­ch hoáº¡t log Ä‘á»‘i tÆ°á»£ng tÃ¡c vá»¥
		#if defined(AK_TASK_OBJ_LOG_ENABLE)
			extern log_queue_t log_task_dbg_object_queue;
		#endif

		// Khai bÃ¡o directive Ä‘á»ƒ quáº£n lÃ½ viá»‡c kÃ­ch hoáº¡t log ngáº¯t
		#if defined(AK_IRQ_OBJ_LOG_ENABLE)
			extern log_queue_t log_irq_queue;
		#endif

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ quáº£n lÃ½ tÃ¡c vá»¥
		extern void task_create(task_t* task_tbl);													// Quáº£n lÃ½ viá»‡c táº¡o cÃ¡c tÃ¡c vá»¥ dá»±a trÃªn báº£ng tÃ¡c vá»¥ Ä‘Æ°á»£c cung cáº¥p
		extern void task_post(task_id_t des_task_id, ak_msg_t* msg);				// Quáº£n lÃ½ viá»‡c Ä‘Äƒng tin nháº¯n Ä‘áº¿n má»™t tÃ¡c vá»¥ Ä‘Ã­ch cá»¥ thá»ƒ
		extern void task_post_pure_msg(task_id_t des_task_id, uint8_t sig);	// Quáº£n lÃ½ viá»‡c Ä‘Äƒng má»™t tin nháº¯n thuáº§n tÃºy (chá»‰ chá»©a pháº§n tiÃªu Ä‘á») Ä‘áº¿n má»™t tÃ¡c vá»¥ Ä‘Ã­ch cá»¥ thá»ƒ
		extern void task_post_common_msg(
			task_id_t des_task_id, uint8_t sig, 
			uint8_t* data, uint8_t len
		);																																	// Quáº£n lÃ½ viá»‡c Ä‘Äƒng má»™t tin nháº¯n thÃ´ng thÆ°á»ng (chá»©a pháº§n tiÃªu Ä‘á» vÃ  dá»¯ liá»‡u cÃ³ kÃ­ch thÆ°á»›c cá»‘ Ä‘á»‹nh) Ä‘áº¿n má»™t tÃ¡c vá»¥ Ä‘Ã­ch cá»¥ thá»ƒ						
		extern void task_post_dynamic_msg(
			task_id_t des_task_id, uint8_t sig, 
			uint8_t* data, uint32_t len
		);																																	// Quáº£n lÃ½ viá»‡c Ä‘Äƒng má»™t tin nháº¯n Ä‘á»™ng (chá»©a pháº§n tiÃªu Ä‘á» vÃ  dá»¯ liá»‡u cÃ³ kÃ­ch thÆ°á»›c thay Ä‘á»•i Ä‘Æ°á»£c cáº¥p phÃ¡t táº¡i thá»i Ä‘iá»ƒm cháº¡y) Ä‘áº¿n má»™t tÃ¡c vá»¥ Ä‘Ã­ch cá»¥ thá»ƒ
		extern uint8_t task_remove_msg(task_id_t task_id, uint8_t sig); 		// Quáº£n lÃ½ viá»‡c xÃ³a má»™t tin nháº¯n cá»¥ thá»ƒ dá»±a trÃªn ID cá»§a tÃ¡c vá»¥ vÃ  tÃ­n hiá»‡u cá»§a tin nháº¯n
		
		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ quáº£n lÃ½ tÃ¡c vá»¥
		extern int task_init(void); // Quáº£n lÃ½ viá»‡c khá»Ÿi táº¡o há»‡ thá»‘ng tÃ¡c vá»¥, chuáº©n bá»‹ cÃ¡c cáº¥u trÃºc dá»¯ liá»‡u vÃ  tÃ i nguyÃªn cáº§n thiáº¿t Ä‘á»ƒ há»‡ thá»‘ng tÃ¡c vá»¥ hoáº¡t Ä‘á»™ng má»™t cÃ¡ch hiá»‡u quáº£
		extern int task_run(void);	// Quáº£n lÃ½ viá»‡c cháº¡y há»‡ thá»‘ng tÃ¡c vá»¥, báº¯t Ä‘áº§u thá»±c thi cÃ¡c tÃ¡c vá»¥ Ä‘Ã£ Ä‘Æ°á»£c táº¡o vÃ  quáº£n lÃ½ viá»‡c chuyá»ƒn Ä‘á»•i giá»¯a cÃ¡c tÃ¡c vá»¥ dá»±a trÃªn cÃ¡c tin nháº¯n mÃ  chÃºng nháº­n Ä‘Æ°á»£c hoáº·c theo lá»‹ch trÃ¬nh cá»§a chÃºng
		extern int task_debug_run_once(void); // Cháº¡y scheduler má»™t vÃ²ng phá»¥c vá»¥ debug há»¯u háº¡n, khÃ´ng Ä‘i vÃ o vÃ²ng láº·p vÃ´ háº¡n nhÆ° task_run
		extern void task_deinit(void); // ÄÆ°a cÃ¡c cáº¥u trÃºc ná»™i bá»™ cá»§a task manager vá» tráº¡ng thÃ¡i trÆ°á»›c khá»Ÿi táº¡o Ä‘á»ƒ phá»¥c vá»¥ test/debug

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ quáº£n lÃ½ tÃ¡c vá»¥ polling
		extern void task_polling_create(task_polling_t* task_polling_tbl);								// Quáº£n lÃ½ viá»‡c táº¡o cÃ¡c tÃ¡c vá»¥ polling dá»±a trÃªn báº£ng tÃ¡c vá»¥ polling Ä‘Æ°á»£c cung cáº¥p
		extern void task_polling_set_ability(task_id_t task_polling_id, uint8_t ability);	// Quáº£n lÃ½ viá»‡c thiáº¿t láº­p kháº£ nÄƒng cá»§a má»™t tÃ¡c vá»¥ polling cá»¥ thá»ƒ dá»±a trÃªn ID cá»§a tÃ¡c vá»¥ polling vÃ  giÃ¡ trá»‹ kháº£ nÄƒng Ä‘Æ°á»£c cung cáº¥p
		extern void task_polling_run();																										// Quáº£n lÃ½ viá»‡c cháº¡y há»‡ thá»‘ng tÃ¡c vá»¥ polling, báº¯t Ä‘áº§u thá»±c thi cÃ¡c tÃ¡c vá»¥ polling Ä‘Ã£ Ä‘Æ°á»£c táº¡o vÃ  quáº£n lÃ½ viá»‡c chuyá»ƒn Ä‘á»•i giá»¯a cÃ¡c tÃ¡c vá»¥ polling dá»±a trÃªn lá»‹ch trÃ¬nh cá»§a chÃºng hoáº·c theo kháº£ nÄƒng cá»§a chÃºng

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ quáº£n lÃ½ ngoáº¡i lá»‡
		extern void task_entry_interrupt(); // Quáº£n lÃ½ viá»‡c xá»­ lÃ½ khi má»™t tÃ¡c vá»¥ ngáº¯t Ä‘Æ°á»£c kÃ­ch hoáº¡t, chuáº©n bá»‹ cÃ¡c tÃ i nguyÃªn vÃ  cáº¥u trÃºc dá»¯ liá»‡u cáº§n thiáº¿t Ä‘á»ƒ xá»­ lÃ½ ngáº¯t má»™t cÃ¡ch hiá»‡u quáº£
		extern void task_exit_interrupt(); 	// Quáº£n lÃ½ viá»‡c xá»­ lÃ½ khi má»™t tÃ¡c vá»¥ ngáº¯t káº¿t thÃºc, giáº£i phÃ³ng cÃ¡c tÃ i nguyÃªn vÃ  cáº¥u trÃºc dá»¯ liá»‡u Ä‘Ã£ Ä‘Æ°á»£c sá»­ dá»¥ng trong quÃ¡ trÃ¬nh xá»­ lÃ½ ngáº¯t Ä‘á»ƒ Ä‘áº£m báº£o há»‡ thá»‘ng hoáº¡t Ä‘á»™ng má»™t cÃ¡ch hiá»‡u quáº£ vÃ  á»•n Ä‘á»‹nh sau khi ngáº¯t káº¿t thÃºc

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ láº¥y thÃ´ng tin vá» tÃ¡c vá»¥ hiá»‡n táº¡i
		extern task_id_t task_self(); 								// Quáº£n lÃ½ viá»‡c láº¥y ID cá»§a tÃ¡c vá»¥ hiá»‡n táº¡i
		extern task_id_t get_current_task_id(); 			// Quáº£n lÃ½ viá»‡c láº¥y ID cá»§a tÃ¡c vá»¥ hiá»‡n táº¡i
		extern task_t* get_current_task_info(); 			// Quáº£n lÃ½ viá»‡c láº¥y thÃ´ng tin cá»§a tÃ¡c vá»¥ hiá»‡n táº¡i, bao gá»“m ID, má»©c Æ°u tiÃªn, vÃ  hÃ m thá»±c thi cá»§a tÃ¡c vá»¥ hiá»‡n táº¡i
		extern ak_msg_t* get_current_active_object(); // Quáº£n lÃ½ viá»‡c láº¥y con trá» Ä‘áº¿n Ä‘á»‘i tÆ°á»£ng tin nháº¯n hiá»‡n táº¡i Ä‘ang Ä‘Æ°á»£c xá»­ lÃ½ bá»Ÿi tÃ¡c vá»¥ hiá»‡n táº¡i

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ quáº£n lÃ½ viá»‡c kÃ­ch hoáº¡t vÃ  káº¿t thÃºc tráº¡ng thÃ¡i xá»­ lÃ½ cá»§a tÃ¡c vá»¥ ngáº¯t
		extern void task_irq_io_entry_trigger(); // Quáº£n lÃ½ viá»‡c kÃ­ch hoáº¡t khi má»™t tÃ¡c vá»¥ ngáº¯t vÃ o tráº¡ng thÃ¡i xá»­ lÃ½, chuáº©n bá»‹ cÃ¡c tÃ i nguyÃªn vÃ  cáº¥u trÃºc dá»¯ liá»‡u cáº§n thiáº¿t Ä‘á»ƒ xá»­ lÃ½ ngáº¯t má»™t cÃ¡ch hiá»‡u quáº£ khi má»™t sá»± kiá»‡n ngáº¯t xáº£y ra
		extern void task_irq_io_exit_trigger() ; // Quáº£n lÃ½ viá»‡c kÃ­ch hoáº¡t khi má»™t tÃ¡c vá»¥ ngáº¯t káº¿t thÃºc tráº¡ng thÃ¡i xá»­ lÃ½, giáº£i phÃ³ng cÃ¡c tÃ i nguyÃªn vÃ  cáº¥u trÃºc dá»¯ liá»‡u Ä‘Ã£ Ä‘Æ°á»£c sá»­ dá»¥ng trong quÃ¡ trÃ¬nh xá»­ lÃ½ ngáº¯t Ä‘á»ƒ Ä‘áº£m báº£o há»‡ thá»‘ng hoáº¡t Ä‘á»™ng má»™t cÃ¡ch hiá»‡u quáº£ vÃ  á»•n Ä‘á»‹nh sau khi ngáº¯t káº¿t thÃºc

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ quáº£n lÃ½ viá»‡c xÃ³a táº¥t cáº£ tin nháº¯n cá»§a má»™t tÃ¡c vá»¥ cá»¥ thá»ƒ
		extern void task_pri_queue_dump(); 

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ láº¥y kÃ­ch thÆ°á»›c cá»§a báº£ng tÃ¡c vá»¥
		extern uint8_t get_task_table_size(); 

	#ifdef __cplusplus
	}
	#endif

#endif //__TASK_H__

