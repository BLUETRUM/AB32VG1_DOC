#include <rtthread.h>

#ifdef BSP_USING_IRRX

uint8_t ab32_get_irkey(uint16_t *addr, uint16_t *cmd);
void irrx_test(void)
{
    uint16_t addr = 0;
    uint16_t cmd = 0;

    while (1) {
        if (ab32_get_irkey(&addr, &cmd)) {
            rt_kprintf("get_irkey: %04x, %04x\n", addr, cmd);
        } else {
            rt_thread_mdelay(10);
        }
    }
}
MSH_CMD_EXPORT(irrx_test, irrx_test);

#endif
