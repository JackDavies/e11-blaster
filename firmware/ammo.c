#include "ammo.h"
#include "defs.h"
#include "pico/stdlib.h"

#define ANIMATION_SLEEP_DELAY 100

uint ammo_count = 0;

void animate_reload(){
    clear_ammo_lights();
    
    for (int i = 0; i <= AMMO_BAR_COUNT - 1; i++){
        sleep_ms(ANIMATION_SLEEP_DELAY / 1.3);
        gpio_put(AMMO_BAR_PIN_LIST[i], 1);
    }
    
    sleep_ms(ANIMATION_SLEEP_DELAY);
    clear_ammo_lights();   
    sleep_ms(ANIMATION_SLEEP_DELAY);
    set_ammo_lights();
    
    sleep_ms(ANIMATION_SLEEP_DELAY);
    clear_ammo_lights();   
    sleep_ms(ANIMATION_SLEEP_DELAY);
    set_ammo_lights();
}

void clear_ammo_lights(){
    for (int i = 0; i<= AMMO_BAR_COUNT - 1; i++){
        gpio_put(AMMO_BAR_PIN_LIST[i], 0);
    }
}

void dec_ammo(){
    if (ammo_count > 0){
        ammo_count--;
        set_ammo_lights();
    }
}

void init_ammo(){
    init_ammo_bar_pins();
    ammo_count = AMMO_BAR_COUNT;
    set_ammo_lights();
}
 
void init_ammo_bar_pins(){    
    for (int i = 0; i <= AMMO_BAR_COUNT - 1; i++){
        gpio_init(AMMO_BAR_PIN_LIST[i]);
        gpio_set_dir(AMMO_BAR_PIN_LIST[i], GPIO_OUT);  
    }
}

void set_ammo_lights(){
    for (int i = 0; i <= AMMO_BAR_COUNT - 1; i++){                
        if ((i + 1) <= ammo_count){
            gpio_put(AMMO_BAR_PIN_LIST[i], 1);
        }
        else{
            gpio_put(AMMO_BAR_PIN_LIST[i], 0);
        }
    }
}

void turn_on_ammo_light(int lightNumber){   
    if (lightNumber > 0 && lightNumber < AMMO_BAR_COUNT - 1) {
        gpio_put(AMMO_BAR_PIN_LIST[lightNumber],1);
    }
}

int get_ammo_count(){
    return ammo_count;
}

void reload_ammo(){
    ammo_count = AMMO_BAR_COUNT;
    animate_reload();
    set_ammo_lights();
}
