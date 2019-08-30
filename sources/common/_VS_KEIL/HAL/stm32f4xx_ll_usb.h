#pragma once


#define USB_OTG_EMBEDDED_PHY                   2U
#define DEP0CTL_MPS_64                         0U


typedef struct
{
    uint32_t dev_endpoints;        /*!< Device Endpoints number.
                                   This parameter depends on the used USB core.   
                                   This parameter must be a number between Min_Data = 1 and Max_Data = 15 */

    uint32_t Host_channels;        /*!< Host Channels number.
                                   This parameter Depends on the used USB core.   
                                   This parameter must be a number between Min_Data = 1 and Max_Data = 15 */

    uint32_t speed;                /*!< USB Core speed.
                                   This parameter can be any value of @ref USB_Core_Speed_                */

    uint32_t dma_enable;           /*!< Enable or disable of the USB embedded DMA.                             */

    uint32_t ep0_mps;              /*!< Set the Endpoint 0 Max Packet size. 
                                   This parameter can be any value of @ref USB_EP0_MPS_                   */

    uint32_t phy_itface;           /*!< Select the used PHY interface.
                                   This parameter can be any value of @ref USB_Core_PHY_                  */

    uint32_t Sof_enable;           /*!< Enable or disable the output of the SOF signal.                        */

    uint32_t low_power_enable;     /*!< Enable or disable the low power mode.                                  */

    uint32_t lpm_enable;           /*!< Enable or disable Link Power Management.                               */

    uint32_t battery_charging_enable; /*!< Enable or disable Battery charging.                                 */

    uint32_t vbus_sensing_enable;  /*!< Enable or disable the VBUS Sensing feature.                            */ 

    uint32_t use_dedicated_ep1;    /*!< Enable or disable the use of the dedicated EP1 interrupt.              */

    uint32_t use_external_vbus;    /*!< Enable or disable the use of the external VBUS.                        */

}USB_OTG_CfgTypeDef;
