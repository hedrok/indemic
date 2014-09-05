#include <vector>

#include <cxxtest/TestSuite.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"

class ExternalInterruptTestSuite : public CxxTest::TestSuite
{
    public:
        void setUp()
        {
            this->portState = 0;
            this->portChangesNum = 0;
            this->portExpected.clear();
        }

        void runTest(const char* fname, int cycles)
        {
            elf_firmware_t f;
            elf_read_firmware(fname, &f);

            avr_t * avr = NULL;

            avr = avr_make_mcu_by_name(f.mmcu);
            if (!avr) {
                TS_FAIL("avr_make_mcu failed");
                return;
            }

            avr_init(avr);
            avr_load_firmware(avr, &f);

            for (int i = 0; i < 8; i++) {
                avr_irq_register_notify(
                    avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), i),
                    [](struct avr_irq_t * irq, uint32_t value, void *param) {
                        ExternalInterruptTestSuite *p = static_cast<ExternalInterruptTestSuite*>(param);
                        if (value) {
                            p->portState |= (1 << irq->irq);
                        } else {
                            p->portState &= ~(1 << irq->irq);
                        }
                        if (p->portChangesNum < p->portExpected.size()) {
                            TS_ASSERT_EQUALS(
                                p->portState,
                                p->portExpected[p->portChangesNum]
                            );
                        }
                        p->portChangesNum++;
                    },
                    static_cast<void*>(this)
                );
            }

            for (int i = 0; i < cycles; i++) {
		        avr_run(avr);
            }

            TS_ASSERT_EQUALS(portChangesNum, this->portExpected.size());
        }

        /**
         * testing External Interrupt
         */
        void testInt()
        {
            // Set pins 0 and 1 output
            this->portExpected.push_back(0x0);
            this->portExpected.push_back(0x1);

            runTest("ExternalInterrupt/external.at90usb162.avr.elf", 100);
        }
    private:
        uint8_t portState;
        size_t portChangesNum;
        std::vector<uint8_t> portExpected;
};
