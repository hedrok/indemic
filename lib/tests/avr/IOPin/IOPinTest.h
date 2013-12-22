#include <vector>

#include <cxxtest/TestSuite.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"

class IOPinTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * testing IOPin::setHigh, 
         *         IOPin::setLow,
         *         IOPin::setValue
         */
        void testOutput()
        {
            this->portState = 0;
            this->changesNum = 0;
            this->portExpected.clear();

            // First set 0 to high
            this->portExpected.push_back(0x1);
            // 7 messages come after change
            // of first pin
            this->portExpected.push_back(0x1);
            this->portExpected.push_back(0x1);
            this->portExpected.push_back(0x1);
            this->portExpected.push_back(0x1);
            this->portExpected.push_back(0x1);
            this->portExpected.push_back(0x1);
            this->portExpected.push_back(0x1);
            // Set 1 to high
            this->portExpected.push_back(0x3);
            // Set 0 to low
            this->portExpected.push_back(0x2);
            // Set 1 to low
            this->portExpected.push_back(0x0);
            // Testing setValue function:
            // setting 1 to high using setValue
            this->portExpected.push_back(0x2);
            // setting 0 to high using setValue
            this->portExpected.push_back(0x3);
            // setting 0 to low using setValue
            this->portExpected.push_back(0x2);
            // setting 1 to low using setValue
            this->portExpected.push_back(0x0);

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

            for (int i = 0; i < 8; i++) {
                avr_irq_register_notify(
                    avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), i),
                    [](struct avr_irq_t * irq, uint32_t value, void *param) {
                        IOPinTestSuite *p = static_cast<IOPinTestSuite*>(param);
                        if (value) {
                            p->portState |= (1 << irq->irq);
                        } else {
                            p->portState &= ~(1 << irq->irq);
                        }
                        if (p->changesNum < p->portExpected.size()) {
                            TS_ASSERT_EQUALS(
                                p->portState,
                                p->portExpected[p->changesNum]
                            );
                        }
                        p->changesNum++;
                    },
                    static_cast<void*>(this)
                );
            }

            for (int i = 0; i < 100; i++) {
		        avr_run(avr);
            }

            TS_ASSERT_EQUALS(changesNum, this->portExpected.size());
        }

    private:
        uint8_t portState;
        int changesNum;
        std::vector<uint8_t> portExpected;
};
