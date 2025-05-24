#include "laser.h"
#include "defs.h"
#include "pico/stdlib.h"

#define LASER_ON_TIME 200

int _selected_laser = 0;

void init_laser(){
    init_laser_pins();
    init_indicator_pins();
    _selected_laser = 0;
    update_colour_indicator();
}
 
void init_laser_pins(){    
    for (int i = 0; i <= LASER_PIN_COUNT - 1; i++){
        gpio_init(LASER_PIN_LIST[i]);
        gpio_set_dir(LASER_PIN_LIST[i], GPIO_OUT);  
    }
}

void init_indicator_pins(){    
    for (int i = 0; i <= LASER_IND_PIN_COUNT - 1; i++){
        gpio_init(LASER_IND_PIN_LIST[i]);
        gpio_set_dir(LASER_IND_PIN_LIST[i], GPIO_OUT);  
    }
}

void change_laser(){
    if (_selected_laser == LASER_PIN_COUNT - 1) {
        _selected_laser = 0;
    }
    else{
        _selected_laser++;
    }
    update_colour_indicator();
}

void flash_laser(){
    gpio_put(LASER_PIN_LIST[_selected_laser], 1);
    sleep_ms(LASER_ON_TIME);
    gpio_put(LASER_PIN_LIST[_selected_laser], 0);
}

void update_colour_indicator(){
    for (int i = 0; i <= LASER_IND_PIN_COUNT - 1; i++){
        if (i == _selected_laser){
            gpio_put(LASER_IND_PIN_LIST[i], 1);    
        }
        else{
            gpio_put(LASER_IND_PIN_LIST[i], 0);
        }
    }
}
