/*Timer handler for our OS*/
#include "low-io.h"
static int timer_ticks=0;
/* to set timer frequency IRQ0 frequency */
void timer_phase(int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outportb(0x43, 0x36);             /* Set our command byte 0x36 */
    outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    /*if (timer_ticks % 18 == 0)
    {
        kprintf("One second has passed\n");
    }*/
}

void install_timer()
{
    /* Installs 'timer_handler' to IRQ0 */
	timer_phase(1000);
    irq_install_handler(0, timer_handler);
}
int get_tick()
{
	return timer_ticks;
}
void sleep(int ms)
{
	int cur;
	cur=get_tick();
	while(cur+ms-get_tick());
}
