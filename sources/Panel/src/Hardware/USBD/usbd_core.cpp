#include "usbd_core.h"
#include "defines.h"
#include "Hardware/VCP.h"



USBD_StatusTypeDef USBD_Init(USBD_HandleTypeDef *pdev, USBD_DescriptorsTypeDef *pdesc, uint8_t id)
{
    if (pdev == NULL)
    {
        USBD_ErrLog("Invalid Device handle");
        return USBD_FAIL;
    }

    if (pdev->pClass != NULL)
    {
        pdev->pClass = NULL;
    }

    if (pdesc != NULL)
    {
        pdev->pDesc = pdesc;
    }

    pdev->dev_state = USBD_STATE_DEFAULT;
    pdev->id = id;
    USBD_LL_Init(pdev);

    return USBD_OK;
}


USBD_StatusTypeDef USBD_DeInit(USBD_HandleTypeDef *pdev)
{
    pdev->dev_state = USBD_STATE_DEFAULT;
    pdev->pClass->DeInit(pdev, (uint8)pdev->dev_config);
    USBD_LL_Stop(pdev);
    USBD_LL_DeInit(pdev);

    return USBD_OK;
}


USBD_StatusTypeDef  USBD_RegisterClass(USBD_HandleTypeDef *pdev, USBD_ClassTypeDef *pclass)
{
    USBD_StatusTypeDef   status = USBD_OK;
    if (pclass != 0)
    {
        pdev->pClass = pclass;
        status = USBD_OK;
    }
    else
    {
        USBD_ErrLog("Invalid Class handle");
        status = USBD_FAIL;
    }

    return status;
}


USBD_StatusTypeDef  USBD_Start  (USBD_HandleTypeDef *pdev)
{
    USBD_LL_Start(pdev);

    return USBD_OK;
}


USBD_StatusTypeDef  USBD_Stop   (USBD_HandleTypeDef *pdev)
{
    pdev->pClass->DeInit(pdev, (uint8)pdev->dev_config);
    USBD_LL_Stop(pdev);

    return USBD_OK;
}


USBD_StatusTypeDef  USBD_RunTestMode (USBD_HandleTypeDef  *) 
{
    return USBD_OK;
}


USBD_StatusTypeDef USBD_SetClassConfig(USBD_HandleTypeDef  *pdev, uint8_t cfgidx)
{
    USBD_StatusTypeDef   ret = USBD_FAIL;

    if (pdev->pClass != NULL)
    {
        if (pdev->pClass->Init(pdev, cfgidx) == 0)
        {
            ret = USBD_OK;
        }
    }
    return ret;
}


USBD_StatusTypeDef USBD_ClrClassConfig(USBD_HandleTypeDef  *pdev, uint8_t cfgidx)
{
    pdev->pClass->DeInit(pdev, cfgidx);
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_SetupStage(USBD_HandleTypeDef *pdev, uint8_t *psetup)
{
    USBD_ParseSetupRequest(&pdev->request, psetup);

    pdev->ep0_state = USBD_EP0_SETUP;
    pdev->ep0_data_len = pdev->request.wLength;

    switch (pdev->request.bmRequest & 0x1F)
    {
    case USB_REQ_RECIPIENT_DEVICE:
        USBD_StdDevReq(pdev, &pdev->request);
        break;

    case USB_REQ_RECIPIENT_INTERFACE:
        USBD_StdItfReq(pdev, &pdev->request);
        CONNECTED_TO_USB = false;
        break;

    case USB_REQ_RECIPIENT_ENDPOINT:
        USBD_StdEPReq(pdev, &pdev->request);
        break;

    default:
        USBD_LL_StallEP(pdev, (uint8)(pdev->request.bmRequest & 0x80));
        break;
    }
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_DataOutStage(USBD_HandleTypeDef *pdev , uint8_t epnum, uint8_t *pdata)
{
    USBD_EndpointTypeDef    *pep;

    if (epnum == 0)
    {
        pep = &pdev->ep_out[0];

        if (pdev->ep0_state == USBD_EP0_DATA_OUT)
        {
            if (pep->rem_length > pep->maxpacket)
            {
                pep->rem_length -= pep->maxpacket;

                USBD_CtlContinueRx(pdev, pdata, (uint16)(MIN(pep->rem_length, pep->maxpacket)));
            }
            else
            {
                if ((pdev->pClass->EP0_RxReady != NULL) && (pdev->dev_state == USBD_STATE_CONFIGURED))
                {
                    pdev->pClass->EP0_RxReady(pdev);
                }
                USBD_CtlSendStatus(pdev);
            }
        }
    }
    else if ((pdev->pClass->DataOut != NULL) && (pdev->dev_state == USBD_STATE_CONFIGURED))
    {
        pdev->pClass->DataOut(pdev, epnum);
    }
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_DataInStage(USBD_HandleTypeDef *pdev ,uint8_t epnum, uint8_t *pdata)
{
    USBD_EndpointTypeDef    *pep;

    if (epnum == 0)
    {
        pep = &pdev->ep_in[0];

        if (pdev->ep0_state == USBD_EP0_DATA_IN)
        {
            if (pep->rem_length > pep->maxpacket)
            {
                pep->rem_length -= pep->maxpacket;

                USBD_CtlContinueSendData(pdev, pdata, (uint16)pep->rem_length);

                USBD_LL_PrepareReceive(pdev, 0, NULL, 0);
            }
            else
            {
                if ((pep->total_length % pep->maxpacket == 0) &&
                    (pep->total_length >= pep->maxpacket) &&
                    (pep->total_length < pdev->ep0_data_len))
                {

                    USBD_CtlContinueSendData(pdev, NULL, 0);
                    pdev->ep0_data_len = 0;

                    USBD_LL_PrepareReceive(pdev, 0, NULL, 0);
                }
                else
                {
                    if ((pdev->pClass->EP0_TxSent != NULL) &&
                        (pdev->dev_state == USBD_STATE_CONFIGURED))
                    {
                        pdev->pClass->EP0_TxSent(pdev);
                    }
                    USBD_CtlReceiveStatus(pdev);
                }
            }
        }
        if (pdev->dev_test_mode == 1)
        {
            USBD_RunTestMode(pdev);
            pdev->dev_test_mode = 0;
        }
    }
    else if ((pdev->pClass->DataIn != NULL) &&
        (pdev->dev_state == USBD_STATE_CONFIGURED))
    {
        pdev->pClass->DataIn(pdev, epnum);
    }
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_Reset(USBD_HandleTypeDef  *pdev)
{
    USBD_LL_OpenEP(pdev,
        0x00,
        USBD_EP_TYPE_CTRL,
        USB_MAX_EP0_SIZE);

    pdev->ep_out[0].maxpacket = USB_MAX_EP0_SIZE;

    USBD_LL_OpenEP(pdev,
        0x80,
        USBD_EP_TYPE_CTRL,
        USB_MAX_EP0_SIZE);

    pdev->ep_in[0].maxpacket = USB_MAX_EP0_SIZE;
    pdev->dev_state = USBD_STATE_DEFAULT;

    if (pdev->pClassData)
    {
        pdev->pClass->DeInit(pdev, (uint8)pdev->dev_config);
    }

    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_SetSpeed(USBD_HandleTypeDef  *pdev, USBD_SpeedTypeDef speed)
{
    pdev->dev_speed = speed;
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_Suspend(USBD_HandleTypeDef  *pdev)
{
    CABLE_USB_IS_CONNECTED = false;
    CONNECTED_TO_USB = false;
    pdev->dev_old_state = pdev->dev_state;
    pdev->dev_state = USBD_STATE_SUSPENDED;
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_Resume(USBD_HandleTypeDef  *pdev)
{
    pdev->dev_state = pdev->dev_old_state;
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_SOF(USBD_HandleTypeDef  *pdev)
{
    if (pdev->dev_state == USBD_STATE_CONFIGURED)
    {
        if (pdev->pClass->SOF != NULL)
        {
            pdev->pClass->SOF(pdev);
        }
    }
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_IsoINIncomplete(USBD_HandleTypeDef  *, uint8_t)
{
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_IsoOUTIncomplete(USBD_HandleTypeDef  *, uint8_t)
{
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_DevConnected(USBD_HandleTypeDef  *)
{
    CABLE_USB_IS_CONNECTED = true;
    CONNECTED_TO_USB = false;
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_DevDisconnected(USBD_HandleTypeDef  *pdev)
{
    pdev->dev_state = USBD_STATE_DEFAULT;
    pdev->pClass->DeInit(pdev, (uint8)pdev->dev_config);

    return USBD_OK;
}
