#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int main()
{	
    PORTC |= 1;
    sleep_mode();
    return 0;
}
