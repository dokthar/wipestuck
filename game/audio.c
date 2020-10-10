#define STB_VORBIS_HEADER_ONLY
#include "miniaudio/stb_vorbis.c"    /* Enables Vorbis decoding. */
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
/* stb_vorbis implementation must come after the implementation of miniaudio. */
#undef STB_VORBIS_HEADER_ONLY
#include "miniaudio/stb_vorbis.c"

#ifndef NO_AUDIO
/* Miniaudio device callback */
void
audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	ma_bool32 isLooping = MA_TRUE;
	ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;

	if (pDecoder == NULL) {
		return;
	}

	/*
	  A decoder is a data source which means you can seemlessly plug it into the ma_data_source API. We can therefore take advantage
	  of the "loop" parameter of ma_data_source_read_pcm_frames() to handle looping for us.
	*/
	ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL, isLooping);

	(void)pInput;
}

ma_result audio_ma_result;
ma_decoder audio_ma_decoder;
ma_device audio_ma_device;

void
fini_audio(void)
{
	if (audio_ma_result == MA_SUCCESS) {
		ma_device_uninit(&audio_ma_device);
		ma_decoder_uninit(&audio_ma_decoder);
	}
}

void
init_audio(void)
{
	ma_device_config deviceConfig;

	/* Audio playback init */
	audio_ma_result = ma_decoder_init_file("./audio/Stuck.ogg", NULL, &audio_ma_decoder);
	if (audio_ma_result != MA_SUCCESS) {
		printf("Failed to open audio file\n");
	} else {
		deviceConfig = ma_device_config_init(ma_device_type_playback);
		deviceConfig.playback.format   = audio_ma_decoder.outputFormat;
		deviceConfig.playback.channels = audio_ma_decoder.outputChannels;
		deviceConfig.sampleRate        = audio_ma_decoder.outputSampleRate;
		deviceConfig.dataCallback      = audio_callback;
		deviceConfig.pUserData         = &audio_ma_decoder;

		if (ma_device_init(NULL, &deviceConfig, &audio_ma_device) != MA_SUCCESS) {
			printf("Failed to open playback device.\n");
			ma_decoder_uninit(&audio_ma_decoder);
			return;
		}

		if (ma_device_start(&audio_ma_device) != MA_SUCCESS) {
			printf("Failed to start playback device.\n");
			fini_audio();
			return;
		}
	}
}
#endif
