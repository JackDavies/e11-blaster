#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"

#include "defs.h"
#include "ammo.h"
#include "laser.h"
#include "sound.h"
#include "rpi-pico-utils/src/button.h"
#include "rpi-pico-utils/src/macros.h"
#include "rpi-pico-utils/src/bargraph.h"

#include "audiolib/audio.h"

#define SELECT_BUTTON_PIN 1

queue_t call_queue;

struct Bargraph bargraph;

void init(){
    gpio_init(TEST_LED_PIN);
    gpio_set_dir(TEST_LED_PIN, GPIO_OUT);
    
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_IN);
    gpio_set_pulls(TRIGGER_PIN, false, true);
    
    gpio_init(RELOAD_BUTTON_PIN);
    gpio_set_dir(RELOAD_BUTTON_PIN, GPIO_IN);
    gpio_set_pulls(RELOAD_BUTTON_PIN, false, true);
    
    gpio_init(SELECT_BUTTON_PIN);
    gpio_set_dir(SELECT_BUTTON_PIN, GPIO_IN);
    gpio_set_pulls(SELECT_BUTTON_PIN, false, true);
    
    //init_bargraph(&bargraph, AMMO_BAR_PIN_LIST, 10);
    //set_bargraph_value(&bargraph, AMMO_BAR_COUNT / 2);
    
    init_ammo();
    init_laser();
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    queue_init(&call_queue, sizeof(int32_t), 2);
    
    audio_init(SOUND_PIN, 22050);
}

void lights_task(){
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    uint button_state = 0;
    
    while(true){ 
        if (multicore_fifo_rvalid()){
            button_state = multicore_fifo_pop_blocking();
        }
        
        if (button_state == 1) {
            gpio_put(LED_PIN, 0);          
        }
        else{
            gpio_put(LED_PIN, 1);
        }
    }
}

void sound_task(){
    while(true){
        int count = queue_get_level(&call_queue);
        int val;
        
        if (count > 0) {
            queue_remove_blocking(&call_queue, &val);
        }
        
        if (val == 1){
            sound_test();
        }
        
        val = 0;
        audio_mixer_step();
    }
}

void fire(){
    // Play sound
    //queue_add_blocking(&call_queue, 1);
    if (get_ammo_count() > 0){
        flash_laser();        
        sound_test();
        dec_ammo();
    }
}

void reload(){
    reload_ammo();
}

void turn_on(){
     gpio_put(25, 1);
}
void turn_off(){
     gpio_put(25, 0);
}

int main(){
    init();

    //multicore_launch_core1(lights_task);
    multicore_launch_core1(sound_task);

    bool trigger_event_handled = false;
    
    bool select_event_handled = false;
    
    struct Button buttons[3];
    init_buttons(buttons, ARRAY_LENGTH(buttons)); 
    
    buttons[0].pin = TRIGGER_PIN;
    buttons[0].state = up;
    buttons[0].on_click = fire;
    buttons[0].on_down = turn_on;
    buttons[0].on_up = turn_off;
    
    buttons[1].pin = RELOAD_BUTTON_PIN;
    buttons[1].on_release = reload;
    
    buttons[2].pin = SELECT_BUTTON_PIN;
    buttons[2].on_click = change_laser;
    
    while(true) {              
        process_button_events(buttons, ARRAY_LENGTH(buttons));         
    }    
}
