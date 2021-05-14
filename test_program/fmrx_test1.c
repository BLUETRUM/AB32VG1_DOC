#include <rtthread.h>
#include <rtdevice.h>
#include "api_fmrx.h"

#define FMRXSIZE (256u)
#define SOUND_DEVICE_NAME    "sound0"    /* Audio 设备名称 */
static rt_device_t snd_dev;              /* Audio 设备句柄 */

int fmrx_test(void)
{
    rt_err_t ret = RT_EOK;

    snd_dev = rt_device_find(SOUND_DEVICE_NAME);
    ret = rt_device_open(snd_dev, RT_DEVICE_OFLAG_WRONLY);
    if (ret == RT_EOK) {
        rt_kprintf("sound0 open successful\n");
    }

    struct rt_audio_caps caps = 
    {
        .main_type = AUDIO_TYPE_OUTPUT,
        .sub_type = AUDIO_DSP_SAMPLERATE,
        .udata.config.samplerate = 38000,
    };
    rt_device_control(snd_dev, AUDIO_CTL_CONFIGURE, (void *)&caps);
    
    caps.main_type = AUDIO_TYPE_MIXER;
    caps.sub_type  = AUDIO_MIXER_EXTEND;
    caps.udata.value = RT_TRUE;
    rt_device_control(snd_dev, AUDIO_CTL_CONFIGURE, (void *)&caps);

    int stream = AUDIO_STREAM_REPLAY;
    rt_device_control(snd_dev, AUDIO_CTL_START, (void *)&stream);

    struct rt_audio_device *audio = (struct rt_audio_device *)(snd_dev);
    uint32_t buf_size = audio->replay->buf_info.total_size;
    rt_kprintf("total size:%d\n", buf_size);

    fmrx_power_on(0);
    fmrx_dma_to_aubuf(RT_TRUE);

    while(1)
    {
        fmrx_dac_sync(buf_size/4);
        rt_thread_mdelay(100);
    }
}
MSH_CMD_EXPORT(fmrx_test, fmrx_test);
