#ifndef __PWM_H__
#define __PWM_H__

#include "freertos/FreeRTOS.h"

#define PWM_CHANNEL 3

struct pwm_single_param {
	uint16 gpio_set;
	uint16 gpio_clear;
    uint16 h_time;
};

struct pwm_param {
    uint16 period;
    uint16 freq;
    uint8  duty[PWM_CHANNEL];
};

#define PWM_DEPTH 255

#define PWM_1S 1000000

/*#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_0_OUT_IO_NUM 12
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO12

#define PWM_1_OUT_IO_MUX PERIPHS_IO_MUX_MTDO_U
#define PWM_1_OUT_IO_NUM 15
#define PWM_1_OUT_IO_FUNC  FUNC_GPIO15

#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_2_OUT_IO_NUM 13
#define PWM_2_OUT_IO_FUNC  FUNC_GPIO13*/

void pwm_init(uint16 freq, uint8 *duty);
void pwm_start(void);

void pwm_set_duty(uint8 duty, uint8 channel);
uint8 pwm_get_duty(uint8 channel);
void pwm_set_freq(uint16 freq);
uint16 pwm_get_freq(void);

void gpio_output_set(uint32 set_mask, uint32 clear_mask, uint32 enable_mask, uint32 disable_mask);

#define ETS_FRC_TIMER1_INTR_ATTACH(func) \
    _xt_isr_attach(ETS_FRC_TIMER1_INUM, (func))
    
#define ETS_INTR_ENABLE(inum) \
    _xt_isr_unmask((1<<inum))

#define ETS_INTR_DISABLE(inum) \
    _xt_isr_mask((1<<inum))

#define ETS_FRC1_INTR_ENABLE() \
	ETS_INTR_ENABLE(ETS_FRC_TIMER1_INUM)

#define ETS_FRC1_INTR_DISABLE() \
	ETS_INTR_DISABLE(ETS_FRC_TIMER1_INUM)

#endif

