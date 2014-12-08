#ifndef __NAT_LIBS_H__
#define __NAT_LIBS_H__

#include "ets_sys.h"

void ets_bzero(void *, int);

void ets_isr_attach(int i, void (*f)(void*), void*arg);
void ets_isr_mask(int i);
void ets_isr_unmask(int i);
void ets_install_putc1(void*arg);
void ets_delay_us(long us);

int ets_sprintf(char*buff, const char*, ...);
void os_printf(const char*s);

void uart_div_modify(int i, int v);

void ets_timer_arm_new(volatile ETSTimer*t, int a, int b, int c);
void ets_timer_setfn(volatile ETSTimer*t, void (*f)(void*), void*ar);
void ets_timer_disarm(volatile ETSTimer*t);

int ets_strlen(const void*);
void ets_strcpy(unsigned char*, const char*, int);

void ets_memcpy(void*, void*, int);
void ets_memset(void*, int, int);

#endif