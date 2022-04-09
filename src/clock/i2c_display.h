// Controls the I2C 14seg display sampler
#ifndef _I2C_DISPLAY_H_
#define _I2C_DISPLAY_H_

// start the thread that interact with 14seg display
void Display_start(void);

// display number on the i2c display
void Display_update_number(int number);

// display rectangle on the i2c display
void Display_show_rectangle(void);

// stop the thread that interact with 14seg display
void Display_stop(void);

#endif
