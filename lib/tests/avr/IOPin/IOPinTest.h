#include <cxxtest/TestSuite.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"

uint8_t	pin_state = 0;	// current port C value

static int callNum = 0;
static uint8_t expected[] = 
{
    // First set 0 to high
    0x1,
    // 7 messages come after change
    // of first pin
    0x1,
    0x1,
    0x1,
    0x1,
    0x1,
    0x1,
    0x1,
    // Set 1 to high
    0x3,
    // Set 0 to low
    0x2,
    // Set 1 to low
    0x0,
    // Testing setValue function:
    // setting 1 to high using setValue
    0x2,
    // setting 0 to high using setValue
    0x3,
    // setting 0 to low using setValue
    0x2,
    // setting 1 to low using setValue
    0x0
};
void pin_changed_hook(struct avr_irq_t * irq, uint32_t value, void * param)
{
	pin_state = (pin_state & ~(1 << irq->irq)) | (value << irq->irq);
    TS_ASSERT_EQUALS(pin_state, expected[callNum]);
    callNum++;
}

class IOPinTestSuite : public CxxTest::TestSuite
{
    public:
        // testing setHigh, setLow, setValue
        void testOutput(void)
        {
            elf_firmware_t f;
            const char * fname = "avr-output.elf";
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

            for (int i = 0; i < 10000; i++) {
		        avr_run(avr);
            }

            TS_ASSERT_EQUALS(callNum, sizeof(expected) / sizeof(expected[0]));
        }
};
