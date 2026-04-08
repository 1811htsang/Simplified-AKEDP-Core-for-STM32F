#ifndef __AK_H__
  #define __AK_H__

  // Khai bÃ¡o C directive cho C++ 
  #ifdef __cplusplus
  extern "C"
  {
  #endif

    // Khai bÃ¡o cÃ¡c thÆ° viá»‡n sá»­ dá»¥ng
    #include <string.h>
    #include "fsm.h"  // ThÆ° viá»‡n FSM (Finite State Machine) Ä‘á»ƒ quáº£n lÃ½ tráº¡ng thÃ¡i cá»§a há»‡ thá»‘ng
    #include "task.h" // ThÆ° viá»‡n quáº£n lÃ½ tÃ¡c vá»¥, giÃºp táº¡o vÃ  quáº£n lÃ½ cÃ¡c tÃ¡c vá»¥ trong há»‡ thá»‘ng
    #include "port.h" // ThÆ° viá»‡n quáº£n lÃ½ attribute vÃ  dynamic allocation

    // Khai bÃ¡o phiÃªn báº£n AK-EDP
    #define AK_VERSION						"1.0"

    // Khai bÃ¡o háº±ng sá»‘ chung
    #define AK_ENABLE						(0x01u)
    #define AK_DISABLE					(0x00u)
    #define AK_FLAG_ON					(0x01u)
    #define AK_FLAG_OFF					(0x00u)
    #define AK_RET_OK						(0x01u)
    #define AK_RET_NG						(0x00u)

    // Khai bÃ¡o háº±ng sá»‘ cho tÃ­n hiá»‡u ngÆ°á»i dÃ¹ng
    #define AK_USER_DEFINE_SIG				(10)

    // Khai bÃ¡o háº±ng sá»‘ cho má»©c Æ°u tiÃªn cá»§a tÃ¡c vá»¥
    #define TASK_PRI_MAX_SIZE				(8)
    #define TASK_PRI_LEVEL_0				(0) 
    #define TASK_PRI_LEVEL_1				(1)
    #define TASK_PRI_LEVEL_2				(2)
    #define TASK_PRI_LEVEL_3				(3)
    #define TASK_PRI_LEVEL_4				(4)
    #define TASK_PRI_LEVEL_5				(5)
    #define TASK_PRI_LEVEL_6				(6)
    #define TASK_PRI_LEVEL_7				(7)

    // Khai bÃ¡o háº±ng sá»‘ cho ID cá»§a tÃ¡c vá»¥
    #define AK_TASK_INTERRUPT_ID		(0xEE) // Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ ngáº¯t, giÃºp xÃ¡c Ä‘á»‹nh vÃ  xá»­ lÃ½ cÃ¡c tÃ¡c vá»¥ liÃªn quan Ä‘áº¿n ngáº¯t trong há»‡ thá»‘ng
    #define AK_TASK_IDLE_ID					(0xEF) // Quáº£n lÃ½ ID cá»§a tÃ¡c vá»¥ nhÃ n rá»—i, giÃºp xÃ¡c Ä‘á»‹nh vÃ  xá»­ lÃ½ tÃ¡c vá»¥ nhÃ n rá»—i trong há»‡ thá»‘ng

  #ifdef __cplusplus
  }
  #endif

#endif // __AK_H__

