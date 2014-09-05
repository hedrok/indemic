#include <vector>

#include <cxxtest/TestSuite.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"

class PeriodicRunnerTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * Run test that asserts that first pin of port C is
         * alternated periodically.
         * @param fname Filename of avr firmware
         * @param period Period in AVR cycles
         */
        void runTest(const char* fname, avr_cycle_count_t period)
        {
            this->_period = period;
            elf_firmware_t f;
            elf_read_firmware(fname, &f);

            avr = avr_make_mcu_by_name(f.mmcu);
            if (!avr) {
                TS_FAIL("avr_make_mcu failed");
                return;
            }

            avr_init(avr);
            avr_load_firmware(avr, &f);

            _callnum = 0;
            avr_irq_register_notify(
                avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0),
                [](struct avr_irq_t * irq, uint32_t value, void *param) {
                    PeriodicRunnerTestSuite * _this = static_cast<PeriodicRunnerTestSuite*>(param);
                    static avr_cycle_count_t prev = 0;
                    _this->_callnum++;
                    if (_this->_callnum > 2) {
                       TS_ASSERT_EQUALS(_this->avr->cycle - prev, _this->_period);
                    }
                    prev = _this->avr->cycle;
                },
                static_cast<void*>(this)
            );
//            avr_irq_register_notify(
//                avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), IOPORT_IRQ_DIRECTION_ALL),
//                [](struct avr_irq_t * irq, uint32_t value, void *param) {
//                    IOPinTestSuite *p = static_cast<IOPinTestSuite*>(param);
//                    p->ddrState = value;
//                    if (p->ddrChangesNum < p->ddrExpected.size()) {
//                        TS_ASSERT_EQUALS(
//                            p->ddrState,
//                            p->ddrExpected[p->ddrChangesNum]
//                        );
//                    }
//                    p->ddrChangesNum++;
//                },
//                static_cast<void*>(this)
//            );

            // 40 should be more than enough to start timer
            for (avr_cycle_count_t i = 0; i < period * 10 + 40; i++) {
		        avr_run(avr);
            }
            TS_ASSERT(this->_callnum >= 10);
        }

        /**
         * testing Timer0 of AT90USB
         */
        void testTimer0AT90USB()
        {
            runTest("Timer/timer0.at90usb162.avr.elf", 1000);
        }

    private:
        avr_t * avr;
        avr_cycle_count_t _period;
        int _callnum;
};
