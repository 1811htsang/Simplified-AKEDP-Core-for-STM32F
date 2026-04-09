#ifndef XPRINTF_DEF
  #define XPRINTF_DEF

  // Khai báo directive C cho C++
  #ifdef __cplusplus
    extern "C" {
  #endif

    // Cấu hình cho xprintf
    #define XF_USE_OUTPUT	1	// Cho phép sử dụng các hàm xuất dữ liệu
    #define	XF_CRLF			  1	// Cho phép chuyển đổi LF thành CRLF trong các hàm xuất dữ liệu
    #define	XF_USE_DUMP		0	// Cho phép sử dụng hàm put_dump để hiển thị dữ liệu thô (hex dump)
    #define	XF_USE_LLI		1	// Cho phép sử dụng kiểu dữ liệu long long trong các hàm xuất dữ liệu
    #define	XF_USE_FP		  1	// Cho phép sử dụng kiểu dữ liệu float/double trong các hàm xuất dữ liệu
    #define XF_DPC			'.'	// Ký tự phân cách phần nguyên và phần thập phân trong định dạng số thực
    #define XF_USE_INPUT	0	// Cho phép sử dụng các hàm nhập dữ liệu
    #define	XF_INPUT_ECHO	0	// Cho phép echo ký tự khi nhập dữ liệu (ví dụ: hiển thị ký tự khi người dùng nhập)

    // Định nghĩa macro để tránh cảnh báo khi ép kiểu hàm con trỏ
    #if defined(__GNUC__) && __GNUC__ >= 10
      #pragma GCC diagnostic ignored "-Wcast-function-type"
    #endif

    // Khai báo các hàm xuất dữ liệu nếu được phép sử dụng
    #if XF_USE_OUTPUT
      // Macro để thiết lập thiết bị xuất dữ liệu mặc định (xfunc_output)
      #define xdev_out(func) xfunc_output = (void(*)(int))(func)

      // Khai báo các hàm xuất dữ liệu
      extern void (*xfunc_output)(int);
      void xputc (int chr);
      void xfputc (void (*func)(int), int chr);
      void xputs (const char* str);
      void xfputs (void (*func)(int), const char* str);
      void xprintf (const char* fmt, ...);
      void xsprintf (char* buff, const char* fmt, ...);
      void xfprintf (void (*func)(int), const char* fmt, ...);
      void put_dump (const void* buff, unsigned long addr, int len, int width);
    #endif

    // Khai báo các hàm nhập dữ liệu nếu được phép sử dụng
    #if XF_USE_INPUT
      // Macro để thiết lập thiết bị nhập dữ liệu mặc định (xfunc_input)
      #define xdev_in(func) xfunc_input = (int(*)(void))(func)
      
      // Khai báo các hàm nhập dữ liệu
      extern int (*xfunc_input)(void);
      int xgets (char* buff, int len);
      int xatoi (char** str, long* res);
      int xatof (char** str, double* res);
    #endif

  #ifdef __cplusplus
    }
  #endif

#endif
