#ifndef __APP_H__
	#define __APP_H__

	// Khai báo C directive cho C++
	#ifdef __cplusplus
		extern "C"
		{
	#endif

		// Khai báo thư viện sử dụng 
		#include "ak.h"

		// Khai báo tín hiệu cho các interface task
		enum {
			AC_IF_PURE_MSG_IN = AK_USER_DEFINE_SIG, // Tín hiệu tin nhắn thuần túy vào
			AC_IF_PURE_MSG_OUT,											// Tín hiệu tin nhắn thuần túy ra
			AC_IF_COMMON_MSG_IN,										// Tín hiệu tin nhắn chung vào 
			AC_IF_COMMON_MSG_OUT,										// Tín hiệu tin nhắn chung ra
			AC_IF_DYNAMIC_MSG_IN,										// Tín hiệu tin nhắn động vào
			AC_IF_DYNAMIC_MSG_OUT,									// Tín hiệu tin nhắn động ra
		};

		// Khai báo thông tin ứng dụng
		#define APP_MAGIC_NUMBER	0x18112005 	// Số ma thuật để xác định ứng dụng
		#define APP_VER				{0, 0, 1, 1}		// Phiên bản ứng dụng: major, minor, patch, build

		// Khai báo cấu trúc thông tin ứng dụng
		typedef struct {
			uint32_t magic_number;
			uint8_t version[4];
			const char *author = "EPCB + Shang Huang";
		} app_info_t;

		// Khai báo biến toàn cục chứa thông tin khởi động ứng dụng
		extern const app_info_t app_info;

		// Khai báo chương trình chính của ứng dụng
		extern int main_app();

	#ifdef __cplusplus
		}
	#endif

#endif //__APP_H__

