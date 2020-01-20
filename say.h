#ifndef _SAY_H_
#define _SAY_H_

#include <espeak/speak_lib.h>
#include <string.h>

#define SAY_NOW		1
#define SAY_ASYNC	2

void say(const char *text, char flags) {
	if (flags & SAY_NOW) espeak_Cancel();
    espeak_Synth(text, strlen(text) + 1, 0, POS_SENTENCE, 0, espeakCHARS_AUTO, NULL, NULL);
    if (!(flags & SAY_ASYNC)) while (espeak_IsPlaying() != 0) {}
}

#define stop espeak_Cancel

#endif
