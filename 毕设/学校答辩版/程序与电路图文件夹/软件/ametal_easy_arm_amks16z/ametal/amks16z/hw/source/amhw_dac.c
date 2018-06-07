/*******************************************************************************
*                                 AMetal
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief DAC 操作接口
 *
 * 1. 片上可编程的输出产生器(电压输出从 1/4096Vin 到 Vin Vin 步长是 1/4096Vin)
 * 2. Vin 可以从两个参考电源中选择
 * 3. 在正常停机模式的静态操作
 * 4. 支持 16 位的数据缓冲区
 * 5. 支持 DMA
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-22  xiongyanming, first implementation.
 * \endinternal
 */
#include "amhw_dac.h"
#include "amhw_amks16z.h"

/**
 * \brief DAC数据缓存区初始化
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] p_buf    : 指向DAC缓存区配置结构体
 *
 * \return 无
 */
void amhw_dac_dat_buf_init (amhw_dac_t *p_hw_dac,amhw_dac_buffer_config_t *p_buf)
{
    amhw_dac_c0_cfg(p_hw_dac,AMHW_DAC_C0_VREF_SEL(AMHW_DAC_VREF_2) |
                    AMHW_DAC_C0_POWER_SEL(AMHW_DAC_POWER_LOW));

    if(p_buf->buffer_enable == TRUE ){
        amhw_dac_buf_enable(p_hw_dac);
    }else{
        amhw_dac_buf_disable(p_hw_dac);
    }
    amhw_dac_trg_sel(p_hw_dac,p_buf->trig_mode);

    if( p_buf->start_int_enable == TRUE){
       amhw_dac_bufbot_int_enable(p_hw_dac);

   }else{
       amhw_dac_bufbot_int_disable(p_hw_dac);
   }
   if( p_buf->upper_int_enable == TRUE){
       amhw_dac_buftop_int_enable(p_hw_dac);

   }else{
       amhw_dac_buftop_int_disable(p_hw_dac);
   }
   if(p_buf->dma_enable == TRUE){

       amhw_dac_dma_enable(p_hw_dac);
   }else{
       amhw_dac_dma_disable(p_hw_dac);
   }

   amhw_dac_bufmode_set(p_hw_dac,AMHW_DAC_BUFMODE_NORMAL);


   amhw_dac_c2_buf_up(p_hw_dac,p_buf->upperidx);
}

/* end of file */

