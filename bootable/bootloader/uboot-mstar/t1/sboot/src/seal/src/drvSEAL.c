#include "drvSEAL.h"

int MDrv_SEAL_SetSecureRange(unsigned int rangeId, unsigned long long startAddr, unsigned long long endAddr, int attr)
{
    return HAL_SEAL_SetSecureRange(rangeId, startAddr, endAddr, attr);
}

int MDrv_SEAL_DisableRIUBridges(void)
{
    return HAL_SEAL_DisableRIUBridges();
}

int MDrv_SEAL_Init(void)
{
    return HAL_SEAL_Init();
}

int MDrv_SEAL_SetMIUHost(int swIdx, int bSecure)
{
    return HAL_SEAL_SetMIUHost(swIdx, bSecure);
}

int MDrv_SEAL_SetRIUBank(int swIdx, int bSecure)
{
    return HAL_SEAL_SetRIUBank(swIdx, bSecure);
}

int MDrv_SEAL_SetRIURegister(int swIdx, int bSecure)
{
    return HAL_SEAL_SetRIURegister(swIdx, bSecure);
}
