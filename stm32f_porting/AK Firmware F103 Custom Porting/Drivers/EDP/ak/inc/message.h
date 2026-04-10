#ifndef __MESSAGE_H__
	#define __MESSAGE_H__

	// Khai bÃ¡o C directive cho C++
	#ifdef __cplusplus
	extern "C"
	{
	#endif

		// Khai bÃ¡o cÃ¡c thÆ° viá»‡n sá»­ dá»¥ng
		#include <stdint.h>
		#include <stdlib.h>
		#include "ak.h"
		#include "port.h"

		// Khai bÃ¡o háº±ng sá»‘ tin nháº¯n
		#define AK_MSG_NULL ((ak_msg_t*)0) // Quáº£n lÃ½ con trá» null cho tin nháº¯n
		#define AK_MSG_NG		(0x0u) // Quáº£n lÃ½ tráº¡ng thÃ¡i khÃ´ng há»£p lá»‡ cá»§a tin nháº¯n
		#define AK_MSG_OK		(0x1u) // Quáº£n lÃ½ tráº¡ng thÃ¡i há»£p lá»‡ cá»§a tin nháº¯n

		// Khai bÃ¡o directive quáº£n lÃ½ kÃ­ch thÆ°á»›c cá»§a cÃ¡c pool tin nháº¯n
		#ifndef AK_COMMON_MSG_POOL_SIZE
			#define AK_COMMON_MSG_POOL_SIZE		(8u)
		#endif

		// Khai bÃ¡o directive quáº£n lÃ½ kÃ­ch thÆ°á»›c dá»¯ liá»‡u cá»§a tin nháº¯n thÃ´ng thÆ°á»ng
		#ifndef AK_COMMON_MSG_DATA_SIZE
			#define AK_COMMON_MSG_DATA_SIZE		(64u)
		#endif

		// Khai bÃ¡o directive quáº£n lÃ½ kÃ­ch thÆ°á»›c cá»§a pool tin nháº¯n thuáº§n tÃºy
		#ifndef AK_PURE_MSG_POOL_SIZE
			#define AK_PURE_MSG_POOL_SIZE			(32u)
		#endif

		// Khai bÃ¡o directive quáº£n lÃ½ kÃ­ch thÆ°á»›c cá»§a pool tin nháº¯n Ä‘á»™ng
		#ifndef AK_DYNAMIC_MSG_POOL_SIZE
			#define AK_DYNAMIC_MSG_POOL_SIZE	(8u)
		#endif

		// Khai bÃ¡o directive quáº£n lÃ½ kÃ­ch thÆ°á»›c cá»§a dá»¯ liá»‡u trong tin nháº¯n Ä‘á»™ng
		#ifndef AK_DYNAMIC_DATA_POOL_SIZE
			#define AK_DYNAMIC_DATA_POOL_SIZE	(128u)
		#endif

		// Khai bÃ¡o directive quáº£n lÃ½ kÃ­ch thÆ°á»›c cá»§a PDU Ä‘á»™ng trong tin nháº¯n Ä‘á»™ng
		#ifndef AK_DYNAMIC_PDU_SIZE
			#define AK_DYNAMIC_PDU_SIZE				(4u)

			/**
			 * Ghi chÃº:
			 * PDU lÃ  Ä‘Æ¡n vá»‹ dá»¯ liá»‡u truyá»n táº£i trong tin nháº¯n Ä‘á»™ng,
			 * cÃ³ kÃ­ch thÆ°á»›c cá»‘ Ä‘á»‹nh Ä‘Æ°á»£c xÃ¡c Ä‘á»‹nh bá»Ÿi AK_DYNAMIC_PDU_SIZE.
			 */
		#endif

		// Khai bÃ¡o cÃ¡c háº±ng sá»‘ vÃ  macro liÃªn quan Ä‘áº¿n tin nháº¯n
		#define AK_MSG_TYPE_MASK						(0xC0) // Quáº£n lÃ½ máº·t náº¡ Ä‘á»ƒ xÃ¡c Ä‘á»‹nh loáº¡i tin nháº¯n (thuáº§n tÃºy, thÃ´ng thÆ°á»ng, Ä‘á»™ng)
		#define AK_MSG_REF_COUNT_MASK				(0x3F) // Quáº£n lÃ½ máº·t náº¡ Ä‘á»ƒ xÃ¡c Ä‘á»‹nh sá»‘ lÆ°á»£ng tham chiáº¿u cá»§a tin nháº¯n, giÃºp quáº£n lÃ½ bá»™ nhá»› hiá»‡u quáº£

		#define AK_MSG_REF_COUNT_MAX				(7u)   // Quáº£n lÃ½ sá»‘ lÆ°á»£ng tham chiáº¿u tá»‘i Ä‘a cá»§a tin nháº¯n, giÃºp Ä‘áº£m báº£o ráº±ng tin nháº¯n khÃ´ng bá»‹ sá»­ dá»¥ng quÃ¡ má»©c vÃ  gÃ¢y ra lá»—i

		// Khai bÃ¡o cÃ¡c macro Ä‘á»ƒ quáº£n lÃ½ sá»‘ lÆ°á»£ng tham chiáº¿u vÃ  loáº¡i tin nháº¯n
		// Tráº£ vá» sá»‘ lÆ°á»£ng tham chiáº¿u cá»§a tin nháº¯n, giÃºp theo dÃµi sá»‘ lÆ°á»£ng tÃ¡c vá»¥ Ä‘ang sá»­ dá»¥ng tin nháº¯n Ä‘á»ƒ quáº£n lÃ½ bá»™ nhá»› hiá»‡u quáº£
		#define get_msg_ref_count(x)		((((ak_msg_t*)x)->ref_count) & AK_MSG_REF_COUNT_MASK)
		// Quáº£n lÃ½ viá»‡c Ä‘áº·t láº¡i sá»‘ lÆ°á»£ng tham chiáº¿u cá»§a tin nháº¯n vá» 0, giÃºp chuáº©n bá»‹ tin nháº¯n Ä‘á»ƒ sá»­ dá»¥ng láº¡i hoáº·c giáº£i phÃ³ng khi khÃ´ng cÃ²n cáº§n thiáº¿t ná»¯a
		#define reset_msg_ref_count(x)		((((ak_msg_t*)x)->ref_count) = \
			(((ak_msg_t*)x)->ref_count) & (~AK_MSG_REF_COUNT_MASK))
		// Tráº£ vá» loáº¡i cá»§a tin nháº¯n, giÃºp xÃ¡c Ä‘á»‹nh loáº¡i tin nháº¯n (thuáº§n tÃºy, thÃ´ng thÆ°á»ng, Ä‘á»™ng) Ä‘á»ƒ xá»­ lÃ½ phÃ¹ há»£p
		#define get_msg_type(x)				((((ak_msg_t*)x)->ref_count) & AK_MSG_TYPE_MASK)

		// Khai báo cấu trúc quản lý debug
		typedef struct {
			uint32_t	start_post; // Thời gian bắt đầu post message, giúp theo dõi thời gian để phân tích hiệu suất và debug
			uint32_t	start_exe;  // Thời gian bắt đầu thực thi handler của message, giúp theo dõi thời gian để phân tích hiệu suất và debug
			uint32_t	stop_exe;   // Quản lý thời gian kết thúc thực thi tin nhắn, giúp theo dõi thời gian thực thi của tin nhắn để phân tích hiệu suất và debug
		} dbg_handler_t;

		// Khai báo cấu trúc tin nhắn cơ bản, được sử dụng cho cả 3 loại tin nhắn (thuần túy, thông thường, động)
		typedef struct ak_msg_t {
			// Quản lý liên kết tin nhắn trong pool và queue
			struct ak_msg_t*	next; // Quản lý con trỏ đến tin nhắn tiếp theo trong pool hoặc queue, giúp xây dựng danh sách liên kết cho việc quản lý bộ nhớ và hàng đợi tin nhắn

			// Quản lý debug handler
			#if (AK_TASK_DEBUG == AK_ENABLE) 
				dbg_handler_t		dbg_handler; // Quản lý thông tin debug cho handler của tin nhắn
			#endif

			// Quản lý thông tin cơ bản của tin nhắn, bao gồm ID của tác vụ nguồn và đích, số lượng tham chiếu, tín hiệu, và thông tin giao diện bên ngoài
			uint8_t				src_task_id; 	// Quản lý ID của tác vụ nguồn gởi tin nhắn
			uint8_t				des_task_id; 	// Quản lý ID của tác vụ đích nhận tin nhắn
			uint8_t				ref_count; 	 	// Quản lý số lượng tham chiếu đến tin nhắn, giúp quản lý bộ nhớ hiệu quả
			uint8_t				sig; 					// Quản lý tín hiệu của tin nhắn, giúp xác định loại tin nhắn và cách xử lý nó
			uint8_t				if_src_task_id; // Quản lý ID của tác vụ nguồn gởi tin nhắn qua giao diện bên ngoài
			uint8_t				if_des_task_id; // Quản lý ID của tác vụ đích nhận tin nhắn qua giao diện bên ngoài
			uint8_t				if_src_type; // Quản lý loại nguồn của tin nhắn qua giao diện bên ngoài, giúp xác định nguồn gốc của tin nhắn
			uint8_t				if_des_type; // Quản lý loại đích của tin nhắn qua giao diện bên ngoài, giúp xác định đích đến của tin nhắn
			uint8_t				if_sig; // Quản lý tín hiệu của tin nhắn qua giao diện bên ngoài, giúp xác định loại tin nhắn và cách xử lý nó
		} ak_msg_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ tin nháº¯n thÃ´ng thÆ°á»ng
		typedef struct {
			ak_msg_t	msg_header; // Quáº£n lÃ½ pháº§n tiÃªu Ä‘á» cá»§a tin nháº¯n, chá»©a thÃ´ng tin cÆ¡ báº£n vá» tin nháº¯n nhÆ° ID cá»§a tÃ¡c vá»¥ nguá»“n vÃ  Ä‘Ã­ch, sá»‘ lÆ°á»£ng tham chiáº¿u, tÃ­n hiá»‡u, vÃ  thÃ´ng tin giao diá»‡n bÃªn ngoÃ i
			uint8_t		len; // Quáº£n lÃ½ Ä‘á»™ dÃ i cá»§a dá»¯ liá»‡u trong tin nháº¯n thÃ´ng thÆ°á»ng, giÃºp xÃ¡c Ä‘á»‹nh kÃ­ch thÆ°á»›c cá»§a dá»¯ liá»‡u Ä‘Æ°á»£c chá»©a trong tin nháº¯n
			uint8_t		data[AK_COMMON_MSG_DATA_SIZE]; // Quáº£n lÃ½ dá»¯ liá»‡u cá»§a tin nháº¯n thÃ´ng thÆ°á»ng, chá»©a dá»¯ liá»‡u cÃ³ kÃ­ch thÆ°á»›c cá»‘ Ä‘á»‹nh Ä‘Æ°á»£c xÃ¡c Ä‘á»‹nh bá»Ÿi AK_COMMON_MSG_DATA_SIZE
		} ak_msg_common_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ tin nháº¯n thuáº§n tÃºy
		typedef struct {
			ak_msg_t	msg_header; // Quáº£n lÃ½ pháº§n tiÃªu Ä‘á» cá»§a tin nháº¯n, chá»©a thÃ´ng tin cÆ¡ báº£n vá» tin nháº¯n nhÆ° ID cá»§a tÃ¡c vá»¥ nguá»“n vÃ  Ä‘Ã­ch, sá»‘ lÆ°á»£ng tham chiáº¿u, tÃ­n hiá»‡u, vÃ  thÃ´ng tin giao diá»‡n bÃªn ngoÃ i
			
			/**
			 * Ghi chÃº:
			 * Tin nháº¯n thuáº§n tÃºy chá»‰ chá»©a pháº§n tiÃªu Ä‘á» vÃ  khÃ´ng chá»©a dá»¯ liá»‡u,
			 * thÆ°á»ng Ä‘Æ°á»£c sá»­ dá»¥ng Ä‘á»ƒ truyá»n tÃ­n hiá»‡u hoáº·c 
			 * thÃ´ng tin Ä‘Æ¡n giáº£n giá»¯a cÃ¡c tÃ¡c vá»¥ mÃ  khÃ´ng cáº§n dá»¯ liá»‡u Ä‘i kÃ¨m, 
			 * giÃºp tiáº¿t kiá»‡m bá»™ nhá»› vÃ  tÄƒng hiá»‡u suáº¥t khi chá»‰ cáº§n truyá»n thÃ´ng tin cÆ¡ báº£n. 
			 */

		} ak_msg_pure_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ tin nháº¯n Ä‘á»™ng
		typedef struct {
			ak_msg_t	msg_header; // Quáº£n lÃ½ pháº§n tiÃªu Ä‘á» cá»§a tin nháº¯n, chá»©a thÃ´ng tin cÆ¡ báº£n vá» tin nháº¯n nhÆ° ID cá»§a tÃ¡c vá»¥ nguá»“n vÃ  Ä‘Ã­ch, sá»‘ lÆ°á»£ng tham chiáº¿u, tÃ­n hiá»‡u, vÃ  thÃ´ng tin giao diá»‡n bÃªn ngoÃ i
			uint32_t	len; 				// Quáº£n lÃ½ Ä‘á»™ dÃ i cá»§a dá»¯ liá»‡u trong tin nháº¯n Ä‘á»™ng, giÃºp xÃ¡c Ä‘á»‹nh kÃ­ch thÆ°á»›c cá»§a dá»¯ liá»‡u Ä‘Æ°á»£c chá»©a trong tin nháº¯n
			uint8_t*	data;				// Quáº£n lÃ½ dá»¯ liá»‡u cá»§a tin nháº¯n Ä‘á»™ng, chá»©a dá»¯ liá»‡u cÃ³ kÃ­ch thÆ°á»›c thay Ä‘á»•i Ä‘Æ°á»£c cáº¥p phÃ¡t táº¡i thá»i Ä‘iá»ƒm cháº¡y, giÃºp linh hoáº¡t trong viá»‡c truyá»n dá»¯ liá»‡u lá»›n hoáº·c khÃ´ng xÃ¡c Ä‘á»‹nh trÆ°á»›c kÃ­ch thÆ°á»›c giá»¯a cÃ¡c tÃ¡c vá»¥.

			/**
			 * Ghi chÃº:
			 * Quáº£n lÃ½ tin nháº¯n Ä‘á»™ng, chá»©a dá»¯ liá»‡u cÃ³ kÃ­ch thÆ°á»›c thay Ä‘á»•i 
			 * vÃ  Ä‘Æ°á»£c cáº¥p phÃ¡t táº¡i thá»i Ä‘iá»ƒm cháº¡y. 
			 * Tin nháº¯n nÃ y thÆ°á»ng Ä‘Æ°á»£c sá»­ dá»¥ng khi cáº§n truyá»n dá»¯ liá»‡u lá»›n 
			 * hoáº·c khÃ´ng xÃ¡c Ä‘á»‹nh trÆ°á»›c kÃ­ch thÆ°á»›c cá»§a dá»¯ liá»‡u giá»¯a cÃ¡c tÃ¡c vá»¥.
			 */

		} ak_msg_dynamic_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ tiÃªu Ä‘á» cá»§a tin nháº¯n giao diá»‡n
		typedef struct {
			uint8_t type;					// Quáº£n lÃ½ loáº¡i cá»§a tin nháº¯n giao diá»‡n, giÃºp xÃ¡c Ä‘á»‹nh loáº¡i tin nháº¯n giao diá»‡n (thuáº§n tÃºy, thÃ´ng thÆ°á»ng, Ä‘á»™ng) Ä‘á»ƒ xá»­ lÃ½ phÃ¹ há»£p
			uint8_t src_task_id;	// Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ nguá»“n gá»­i tin nháº¯n giao diá»‡n, giÃºp xÃ¡c Ä‘á»‹nh nguá»“n gá»‘c cá»§a tin nháº¯n giao diá»‡n
			uint8_t des_task_id;	// Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ Ä‘Ã­ch nháº­n tin nháº¯n giao diá»‡n, giÃºp xÃ¡c Ä‘á»‹nh Ä‘Ã­ch Ä‘áº¿n cá»§a tin nháº¯n giao diá»‡n
			uint8_t sig;					// Quáº£n lÃ½ tÃ­n hiá»‡u cá»§a tin nháº¯n giao diá»‡n, giÃºp xÃ¡c Ä‘á»‹nh loáº¡i tin nháº¯n giao diá»‡n vÃ  cÃ¡ch xá»­ lÃ½ nÃ³
			uint8_t if_src_type;	// Quáº£n lÃ½ loáº¡i nguá»“n cá»§a tin nháº¯n giao diá»‡n, giÃºp xÃ¡c Ä‘á»‹nh nguá»“n gá»‘c cá»§a tin nháº¯n giao diá»‡n
			uint8_t if_des_type;	// Quáº£n lÃ½ loáº¡i Ä‘Ã­ch cá»§a tin nháº¯n giao diá»‡n, giÃºp xÃ¡c Ä‘á»‹nh Ä‘Ã­ch Ä‘áº¿n cá»§a tin nháº¯n giao diá»‡n

			/**
			 * Ghi chÃº:
			 * `if` nghÄ©a lÃ  interface.
			 */

		} __AK_PACKETED ak_msg_if_header_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ tin nháº¯n giao diá»‡n thuáº§n tÃºy
		typedef struct {
			ak_msg_if_header_t header; // Quáº£n lÃ½ pháº§n tiÃªu Ä‘á» cá»§a tin nháº¯n giao diá»‡n, chá»©a thÃ´ng tin vá» loáº¡i tin nháº¯n giao diá»‡n, ID cá»§a tÃ¡c vá»¥ nguá»“n vÃ  Ä‘Ã­ch, tÃ­n hiá»‡u, vÃ  thÃ´ng tin giao diá»‡n bÃªn ngoÃ i
		} ak_msg_pure_if_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ tin nháº¯n giao diá»‡n thÃ´ng thÆ°á»ng
		typedef struct {
			ak_msg_if_header_t header;							// Quáº£n lÃ½ pháº§n tiÃªu Ä‘á» cá»§a tin nháº¯n giao diá»‡n, chá»©a thÃ´ng tin vá» loáº¡i tin nháº¯n giao diá»‡n, ID cá»§a tÃ¡c vá»¥ nguá»“n vÃ  Ä‘Ã­ch, tÃ­n hiá»‡u, vÃ  thÃ´ng tin giao diá»‡n bÃªn ngoÃ i
			uint8_t len;														// Quáº£n lÃ½ Ä‘á»™ dÃ i cá»§a dá»¯ liá»‡u trong tin nháº¯n giao diá»‡n thÃ´ng thÆ°á»ng, giÃºp xÃ¡c Ä‘á»‹nh kÃ­ch thÆ°á»›c cá»§a dá»¯ liá»‡u Ä‘Æ°á»£c chá»©a trong tin nháº¯n giao diá»‡n
			uint8_t data[AK_COMMON_MSG_DATA_SIZE];	// Quáº£n lÃ½ dá»¯ liá»‡u cá»§a tin nháº¯n giao diá»‡n thÃ´ng thÆ°á»ng, chá»©a dá»¯ liá»‡u cÃ³ kÃ­ch thÆ°á»›c cá»‘ Ä‘á»‹nh Ä‘Æ°á»£c xÃ¡c Ä‘á»‹nh bá»Ÿi AK_COMMON_MSG_DATA_SIZE
		} __AK_PACKETED ak_msg_common_if_t;

		// Khai bÃ¡o cáº¥u trÃºc quáº£n lÃ½ tin nháº¯n giao diá»‡n Ä‘á»™ng	
		typedef struct {
			ak_msg_if_header_t header;						// Quáº£n lÃ½ pháº§n tiÃªu Ä‘á» cá»§a tin nháº¯n giao diá»‡n, chá»©a thÃ´ng tin vá» loáº¡i tin nháº¯n giao diá»‡n, ID cá»§a tÃ¡c vá»¥ nguá»“n vÃ  Ä‘Ã­ch, tÃ­n hiá»‡u, vÃ  thÃ´ng tin giao diá»‡n bÃªn ngoÃ i
			uint32_t len;													// Quáº£n lÃ½ Ä‘á»™ dÃ i cá»§a dá»¯ liá»‡u trong tin nháº¯n giao diá»‡n Ä‘á»™ng, giÃºp xÃ¡c Ä‘á»‹nh kÃ­ch thÆ°á»›c cá»§a dá»¯ liá»‡u Ä‘Æ°á»£c chá»©a trong tin nháº¯n giao diá»‡n
			uint8_t *data;												// Quáº£n lÃ½ dá»¯ liá»‡u cá»§a tin nháº¯n giao diá»‡n Ä‘á»™ng, chá»©a dá»¯ liá»‡u cÃ³ kÃ­ch thÆ°á»›c thay Ä‘á»•i Ä‘Æ°á»£c cáº¥p phÃ¡t táº¡i thá»i Ä‘iá»ƒm cháº¡y, giÃºp linh hoáº¡t trong viá»‡c truyá»n dá»¯ liá»‡u lá»›n hoáº·c khÃ´ng xÃ¡c Ä‘á»‹nh trÆ°á»›c kÃ­ch thÆ°á»›c giá»¯a cÃ¡c tÃ¡c vá»¥.
		} __AK_PACKETED ak_msg_dynamic_if_t;

		// Khai bÃ¡o cÃ¡c macro Ä‘á»ƒ quáº£n lÃ½ viá»‡c thiáº¿t láº­p thÃ´ng tin cá»§a tin nháº¯n
		#define set_msg_sig(m, s)				(((ak_msg_t*)m)->sig = s) 						// Quáº£n lÃ½ tÃ­n hiá»‡u cá»§a tin nháº¯n, giÃºp xÃ¡c Ä‘á»‹nh loáº¡i tin nháº¯n vÃ  cÃ¡ch xá»­ lÃ½ nÃ³
		#define set_msg_src_task_id(m, t)		(((ak_msg_t*)m)->src_task_id = t) // Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ nguá»“n gá»­i tin nháº¯n
		#define set_msg_des_task_id(m, t)		(((ak_msg_t*)m)->des_task_id = t) // Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ Ä‘Ã­ch nháº­n tin nháº¯n

		// Khai bÃ¡o cÃ¡c macro Ä‘á»ƒ quáº£n lÃ½ viá»‡c thiáº¿t láº­p thÃ´ng tin cá»§a tin nháº¯n giao diá»‡n
		#define set_if_src_task_id(m, t)		(((ak_msg_t*)m)->if_src_task_id = t) 	// Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ nguá»“n gá»­i tin nháº¯n giao diá»‡n
		#define set_if_des_task_id(m, t)		(((ak_msg_t*)m)->if_des_task_id = t) 	// Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ Ä‘Ã­ch nháº­n tin nháº¯n giao diá»‡n
		#define set_if_src_type(m, t)			(((ak_msg_t*)m)->if_src_type = t) 		 	// Quáº£n lÃ½ loáº¡i cá»§a tÃ¡c vá»¥ nguá»“n
		#define set_if_des_type(m, t)			(((ak_msg_t*)m)->if_des_type = t) 			// Quáº£n lÃ½ loáº¡i cá»§a tÃ¡c vá»¥ Ä‘Ã­ch
		#define set_if_sig(m, s)				(((ak_msg_t*)m)->if_sig = s) 							// Quáº£n lÃ½ tÃ­n hiá»‡u cá»§a tin nháº¯n giao diá»‡n
		// Quáº£n lÃ½ dá»¯ liá»‡u cá»§a tin nháº¯n thÃ´ng thÆ°á»ng, giÃºp thiáº¿t láº­p dá»¯ liá»‡u vÃ  kÃ­ch thÆ°á»›c cá»§a nÃ³
		#define set_if_data_common_msg(m, d, s) set_data_common_msg(m, d, s)
		// Quáº£n lÃ½ dá»¯ liá»‡u cá»§a tin nháº¯n Ä‘á»™ng, giÃºp thiáº¿t láº­p dá»¯ liá»‡u vÃ  kÃ­ch thÆ°á»›c cá»§a nÃ³
		#define set_if_data_dynamic_msg(m, d, s) set_data_dynamic_msg(m, d, s)

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ quáº£n lÃ½ tin nháº¯n
		extern void msg_init(); 											// Quáº£n lÃ½ viá»‡c khá»Ÿi táº¡o há»‡ thá»‘ng tin nháº¯n, chuáº©n bá»‹ cÃ¡c pool tin nháº¯n vÃ  cáº¥u trÃºc dá»¯ liá»‡u cáº§n thiáº¿t Ä‘á»ƒ quáº£n lÃ½ tin nháº¯n hiá»‡u quáº£
		extern void msg_free(ak_msg_t* msg); 					// Quáº£n lÃ½ viá»‡c giáº£i phÃ³ng tin nháº¯n, kiá»ƒm tra sá»‘ lÆ°á»£ng tham chiáº¿u trÆ°á»›c khi giáº£i phÃ³ng Ä‘á»ƒ Ä‘áº£m báº£o an toÃ n bá»™ nhá»›
		extern void msg_force_free(ak_msg_t* msg);  	// Quáº£n lÃ½ viá»‡c giáº£i phÃ³ng tin nháº¯n mÃ  khÃ´ng kiá»ƒm tra sá»‘ lÆ°á»£ng tham chiáº¿u, sá»­ dá»¥ng khi cáº§n thiáº¿t Ä‘á»ƒ giáº£i phÃ³ng ngay láº­p tá»©c mÃ  khÃ´ng quan tÃ¢m Ä‘áº¿n sá»‘ lÆ°á»£ng tham chiáº¿u
		extern void msg_inc_ref_count(ak_msg_t* msg); // Quáº£n lÃ½ viá»‡c tÄƒng sá»‘ lÆ°á»£ng tham chiáº¿u cá»§a tin nháº¯n, giÃºp theo dÃµi sá»‘ lÆ°á»£ng tÃ¡c vá»¥ Ä‘ang sá»­ dá»¥ng tin nháº¯n Ä‘á»ƒ quáº£n lÃ½ bá»™ nhá»› hiá»‡u quáº£
		extern void msg_dec_ref_count(ak_msg_t* msg); // Quáº£n lÃ½ viá»‡c giáº£m sá»‘ lÆ°á»£ng tham chiáº¿u cá»§a tin nháº¯n, giÃºp theo dÃµi sá»‘ lÆ°á»£ng tÃ¡c vá»¥ Ä‘ang sá»­ dá»¥ng tin nháº¯n vÃ  giáº£i phÃ³ng bá»™ nhá»› khi khÃ´ng cÃ²n tÃ¡c vá»¥ nÃ o sá»­ dá»¥ng tin nháº¯n ná»¯a

		/**
		 * Ghi chÃº:
		 * Sá»­ dá»¥ng `extern` Ä‘á»ƒ khai bÃ¡o vÃ  Ä‘Æ°á»£c Ä‘á»‹nh nghÄ©a á»Ÿ nÆ¡i khÃ¡c.
		 */

		// Khai bÃ¡o cÃ¡c hÃ m Ä‘á»ƒ quáº£n lÃ½ bá»™ nhá»› Ä‘á»™ng, giÃºp cáº¥p phÃ¡t vÃ  giáº£i phÃ³ng bá»™ nhá»› má»™t cÃ¡ch hiá»‡u quáº£ trong há»‡ thá»‘ng
		extern void* ak_malloc(size_t); // Quáº£n lÃ½ viá»‡c cáº¥p phÃ¡t bá»™ nhá»› Ä‘á»™ng, cho phÃ©p cáº¥p phÃ¡t bá»™ nhá»› vá»›i kÃ­ch thÆ°á»›c Ä‘Æ°á»£c xÃ¡c Ä‘á»‹nh táº¡i thá»i Ä‘iá»ƒm cháº¡y
		extern void ak_free(void*); 		// Quáº£n lÃ½ viá»‡c giáº£i phÃ³ng bá»™ nhá»› Ä‘á»™ng, cho phÃ©p giáº£i phÃ³ng bá»™ nhá»› Ä‘Ã£ Ä‘Æ°á»£c cáº¥p phÃ¡t khi khÃ´ng cÃ²n cáº§n thiáº¿t ná»¯a

		// Khai bÃ¡o tá»« khÃ³a quáº£n lÃ½ loáº¡i tin nháº¯n
		#define PURE_MSG_TYPE					(0x80)
		#define COMMON_MSG_TYPE				(0xC0)
		#define DYNAMIC_MSG_TYPE			(0x40)

		// Khai bÃ¡o cÃ¡c hÃ m quáº£n lÃ½ tin nháº¯n thuáº§n tÃºy
		extern ak_msg_t* get_pure_msg();							// Quáº£n lÃ½ viá»‡c láº¥y má»™t tin nháº¯n thuáº§n tÃºy tá»« pool
		extern uint32_t get_pure_msg_pool_used();		  // Quáº£n lÃ½ viá»‡c láº¥y sá»‘ lÆ°á»£ng tin nháº¯n thuáº§n tÃºy Ä‘ang Ä‘Æ°á»£c sá»­ dá»¥ng trong pool
		extern uint32_t get_pure_msg_pool_used_max();	// Quáº£n lÃ½ viá»‡c láº¥y sá»‘ lÆ°á»£ng tin nháº¯n thuáº§n tÃºy Ä‘Ã£ Ä‘Æ°á»£c sá»­ dá»¥ng tá»‘i Ä‘a trong pool

		// Khai bÃ¡o cÃ¡c hÃ m quáº£n lÃ½ tin nháº¯n thÃ´ng thÆ°á»ng
		extern ak_msg_t* get_common_msg();																							// Quáº£n lÃ½ viá»‡c láº¥y má»™t tin nháº¯n thÃ´ng thÆ°á»ng tá»« pool		
		extern uint32_t get_common_msg_pool_used();																			// Quáº£n lÃ½ viá»‡c láº¥y sá»‘ lÆ°á»£ng tin nháº¯n thÃ´ng thÆ°á»ng Ä‘ang Ä‘Æ°á»£c sá»­ dá»¥ng trong pool
		extern uint32_t get_common_msg_pool_used_max();																	// Quáº£n lÃ½ viá»‡c láº¥y sá»‘ lÆ°á»£ng tin nháº¯n thÃ´ng thÆ°á»ng Ä‘Ã£ Ä‘Æ°á»£c sá»­ dá»¥ng tá»‘i Ä‘a trong pool
		extern uint8_t set_data_common_msg(ak_msg_t* msg, uint8_t* data, uint8_t size);	// Quáº£n lÃ½ viá»‡c thiáº¿t láº­p dá»¯ liá»‡u cá»§a tin nháº¯n thÃ´ng thÆ°á»ng
		extern uint8_t* get_data_common_msg(ak_msg_t* msg);															// Quáº£n lÃ½ viá»‡c láº¥y dá»¯ liá»‡u cá»§a tin nháº¯n thÃ´ng thÆ°á»ng
		extern uint8_t get_data_len_common_msg(ak_msg_t* msg);													// Quáº£n lÃ½ viá»‡c láº¥y Ä‘á»™ dÃ i dá»¯ liá»‡u cá»§a tin nháº¯n thÃ´ng thÆ°á»ng

		/**
		 * Ghi chÃº:
		 * KÃ­ch thÆ°á»›c dá»¯ liá»‡u khÃ´ng Ä‘Æ°á»£c vÆ°á»£t quÃ¡ AK_COMMON_MSG_DATA_SIZE Ä‘á»ƒ Ä‘áº£m báº£o an toÃ n bá»™ nhá»›
		 * Tráº£ vá» AK_MSG_OK náº¿u thiáº¿t láº­p thÃ nh cÃ´ng, hoáº·c AK_MSG_NG náº¿u kÃ­ch thÆ°á»›c dá»¯ liá»‡u vÆ°á»£t quÃ¡ giá»›i háº¡n
		 */

		// Khai bÃ¡o cÃ¡c hÃ m quáº£n lÃ½ tin nháº¯n Ä‘á»™ng
		extern ak_msg_t* get_dynamic_msg();																									// Quáº£n lÃ½ viá»‡c láº¥y má»™t tin nháº¯n Ä‘á»™ng tá»« pool
		extern uint32_t get_dynamic_msg_pool_used();																				// Quáº£n lÃ½ viá»‡c láº¥y sá»‘ lÆ°á»£ng tin nháº¯n Ä‘á»™ng Ä‘ang Ä‘Æ°á»£c sá»­ dá»¥ng trong pool
		extern uint32_t get_dynamic_msg_pool_used_max();																		// Quáº£n lÃ½ viá»‡c láº¥y sá»‘ lÆ°á»£ng tin nháº¯n Ä‘á»™ng Ä‘Ã£ Ä‘Æ°á»£c sá»­ dá»¥ng tá»‘i Ä‘a trong pool
		extern uint8_t set_data_dynamic_msg(ak_msg_t* msg, uint8_t* data, uint32_t size);		// Quáº£n lÃ½ viá»‡c thiáº¿t láº­p dá»¯ liá»‡u cá»§a tin nháº¯n Ä‘á»™ng
		extern uint8_t* get_data_dynamic_msg(ak_msg_t* msg);																// Quáº£n lÃ½ viá»‡c láº¥y dá»¯ liá»‡u cá»§a tin nháº¯n Ä‘á»™ng
		extern uint32_t get_data_len_dynamic_msg(ak_msg_t* msg);														// Quáº£n lÃ½ viá»‡c láº¥y Ä‘á»™ dÃ i dá»¯ liá»‡u cá»§a tin nháº¯n Ä‘á»™ng							
		extern uint8_t msg_is_valid_ptr(const ak_msg_t* msg);																// Quáº£n lÃ½ viá»‡c kiá»ƒm tra tÃ­nh há»£p lá»‡ cá»§a con trá» tin nháº¯n
		extern uint8_t msg_is_valid_or_null(const ak_msg_t* msg);														// Quáº£n lÃ½ viá»‡c kiá»ƒm tra tÃ­nh há»£p lá»‡ hoáº·c null cá»§a con trá» tin nháº¯n

		 /**
		  * Ghi chÃº:
		  * Tráº£ vá» AK_MSG_OK náº¿u con trá» tin nháº¯n há»£p lá»‡, AK_MSG_NG náº¿u con trá» tin nháº¯n khÃ´ng há»£p lá»‡, vÃ  AK_MSG_OK náº¿u con trá» tin nháº¯n lÃ  null trong trÆ°á»ng há»£p cá»§a msg_is_valid_or_null
		  */

		// Khai bÃ¡o hÃ m quáº£n lÃ½ debug cá»§a tin nháº¯n
		extern void msg_dbg_dum(ak_msg_t* msg);

	#ifdef __cplusplus
	}
	#endif

#endif //__MESSAGE_H__

