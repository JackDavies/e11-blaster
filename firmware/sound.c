#include "sound.h"

#include "audiolib/audio.h"
#include "audiolib/snd_drum.h"
#include "audiolib/snd_synth_loop.h"
#include "audiolib/snd_blaster1.h"

void sound_test(){
    int id = audio_play_once(snd_blaster1, sizeof(snd_blaster1)); //snd_drum
    if (id >= 0){
        audio_source_set_volume(id, 1024);
    }
}
