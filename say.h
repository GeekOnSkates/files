#ifndef _SAY_H_
#define _SAY_H_

#include <espeak/speak_lib.h>
#include <string.h>

#define SAY_NOW		1
#define SAY_ASYNC	2

char speech_enabled() {
	return espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0) == EE_INTERNAL_ERROR ? 0 : 1;
}

void say(const char *text, char flags) {
	if (flags & SAY_NOW) espeak_Cancel();
    espeak_Synth(text, strlen(text) + 1, 0, POS_SENTENCE, 0, espeakCHARS_AUTO, NULL, NULL);
    if (!(flags & SAY_ASYNC)) while (espeak_IsPlaying() != 0) {}
}

#define stop espeak_Cancel

#endif
