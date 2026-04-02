#ifndef __BUZZER_H__
#define __BUZZER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef struct {
	uint16_t frequency_hz;
	uint16_t duration_ms;
} buzzer_tone_t;

extern const buzzer_tone_t tones_startup[];
extern const buzzer_tone_t tones_3beep[];
extern const buzzer_tone_t tones_SMB[];
extern const buzzer_tone_t tones_merryChrismast[];

void BUZZER_Init(void);
void BUZZER_PlayTones(const buzzer_tone_t* tones);

#ifdef __cplusplus
}
#endif

#endif