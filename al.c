#include <unistd.h>
#include <fcntl.h>

#include <alsa/asoundlib.h>

#define VOLUME_BOUND 10000

typedef enum {
	AUDIO_VOLUME_SET,
	AUDIO_VOLUME_GET,
} audio_volume_action;

/*
 *   Drawbacks. Sets volume on both channels but gets volume on one. Can be easily adapted.
 *    */
int audio_volume(long* outvol)
{
	int ret = 0;
	snd_mixer_t* handle;
	snd_mixer_elem_t* elem;
	snd_mixer_selem_id_t* sid;
	audio_volume_action action = AUDIO_VOLUME_GET;

	static const char* mix_name = "Master";
	static const char* card = "default";
	static int mix_index = 0;


	snd_mixer_selem_id_alloca(&sid);

	snd_mixer_selem_id_set_index(sid, mix_index);
	snd_mixer_selem_id_set_name(sid, mix_name);

	if ((snd_mixer_open(&handle, 0)) < 0)
		return -1;
	if ((snd_mixer_attach(handle, card)) < 0) {
		snd_mixer_close(handle);
		return -2;
	}
	if ((snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
		snd_mixer_close(handle);
		return -3;
	}
	ret = snd_mixer_load(handle);
	if (ret < 0) {
		snd_mixer_close(handle);
		return -4;
	}
	elem = snd_mixer_find_selem(handle, sid);
	if (!elem) {
		snd_mixer_close(handle);
		return -5;
	}

	long minv, maxv;

	snd_mixer_selem_get_playback_volume_range (elem, &minv, &maxv);
	fprintf(stderr, "Volume range <%i,%i>\n", minv, maxv);

	if(action == AUDIO_VOLUME_GET) {
		if(snd_mixer_selem_get_playback_volume(elem, 0, outvol) < 0) {
			snd_mixer_close(handle);
			return -6;
		}

		//fprintf(stderr, "Get volume %i with status %i\n", *outvol, ret);
		*outvol -= minv;
		maxv -= minv;
		minv = 0;
		*outvol = 100 * (*outvol) / maxv; // make the value bound from 0 to 100
	}

	snd_mixer_close(handle);
	return 0;
}

