#include <vector>

#include <cxxtest/TestSuite.h>

#include <simavr/avr_uart.h>
#include <simavr/sim_elf.h>

class UartTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * Run test that checks Uart - each sent symbol should
         * be increased by one
         * @param fname Filename of avr firmware
         * @param send Characters to send via UART
         * @param expect Expected string to receive via UART
         */
        void runTest(const char* fname, const char * send, const char * expect)
        {
            elf_firmware_t f;
            elf_read_firmware(fname, &f);
            this->_curSend = send;
            this->_curExpect = expect;

            avr = avr_make_mcu_by_name(f.mmcu);
            if (!avr) {
                TS_FAIL("avr_make_mcu failed");
                return;
            }

            avr_init(avr);
            avr_load_firmware(avr, &f);

	        avr_irq_t * uart_output = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('1'), UART_IRQ_OUTPUT);
	        avr_irq_t * uart_xon = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('1'), UART_IRQ_OUT_XON);
            avr_irq_register_notify(
                uart_output,
                [](struct avr_irq_t * irq, uint32_t value, void *param) {
                    UartTestSuite * _this = static_cast<UartTestSuite*>(param);
                    if (*_this->_curExpect == '\0') {
                        TS_FAIL("Too many bytes received\n");
                        return;
                    }
                    TS_ASSERT_EQUALS(value, *_this->_curExpect++);
                },
                static_cast<void*>(this)
            );
            avr_irq_register_notify(
                uart_xon,
                [](struct avr_irq_t * irq, uint32_t value, void *param) {
                    UartTestSuite * _this = static_cast<UartTestSuite*>(param);
                    if (*_this->_curSend == '\0') {
                        return;
                    }
	                avr_irq_t * uart_input = avr_io_getirq(_this->avr, AVR_IOCTL_UART_GETIRQ('1'), UART_IRQ_INPUT);
		            avr_raise_irq(uart_input, *_this->_curSend++);
                },
                static_cast<void*>(this)
            );

            // For reaction to input symbol
            for (avr_cycle_count_t i = 0; i < 2000; i++) {
                avr_run(avr);
            }
            // Check that all Uart character were sent/received
            TS_ASSERT_EQUALS(*this->_curExpect, '\0');
            TS_ASSERT_EQUALS(*this->_curSend, '\0');
        }

        /**
         * testing blocking Uart of AT90USB
         */
        void testUartBlocking()
        {
            runTest("Uart/uart.at90usb162.avr.elf", "abc", "bcd");
        }
        /**
         * testing Uart of AT90USB implemented via interrupts
         */
        void testUartInterrupts()
        {
            runTest("Uart/uart-interrupts.at90usb162.avr.elf", "aA0", "abcABC012");
        }

    private:
        avr_t * avr;
        const char * _curSend;
        const char * _curExpect;
};
