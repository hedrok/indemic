#include <cxxtest/TestSuite.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"

uint8_t	pin_state = 0;	// current port C value

void pin_changed_hook(struct avr_irq_t * irq, uint32_t value, void * param)
{
	pin_state = (pin_state & ~(1 << irq->irq)) | (value << irq->irq);
}

class IOPinTestSuite : public CxxTest::TestSuite
{
    public:
        void testOutputHigh(void)
        {
            elf_firmware_t f;
            const char * fname = "avr-output-high.elf";
            elf_read_firmware(fname, &f);

            avr_t * avr = NULL;

            avr = avr_make_mcu_by_name(f.mmcu);
            if (!avr) {
                TS_FAIL("avr_make_mcu failed");
                return;
            }

            avr_init(avr);
            avr_load_firmware(avr, &f);

            for (int i = 0; i < 8; i++)
                avr_irq_register_notify(
                    avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), i),
                    pin_changed_hook, 
                    NULL);

            for (int i = 0; i < 100; i++) {
		        avr_run(avr);
            }

            TS_ASSERT_EQUALS(pin_state, 1);
        }
};
