#ifndef NO_AUDIO
void init_audio(void);
void fini_audio(void);
#else
void init_audio(void) {}
void fini_audio(void) {}
#endif
