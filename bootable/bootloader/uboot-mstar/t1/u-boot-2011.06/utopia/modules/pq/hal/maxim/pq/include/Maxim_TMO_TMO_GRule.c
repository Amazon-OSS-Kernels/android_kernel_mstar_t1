
#if PQ_GRULE_TMO_MODE_ENABLE
code U8 MST_GRule_TMO_MODE_IP_Index_TMO[PQ_GRULE_TMO_MODE_IP_NUM_TMO]=
{
    PQ_IP_TMO_SOURCE_SETTING_TMO,
    PQ_IP_TMO_TARGET_SETTING_TMO,
};
#endif


#if PQ_GRULE_TMO_MODE_ENABLE
code U8 MST_GRule_TMO_MODE_TMO[QM_INPUTTYPE_NUM_TMO][PQ_GRULE_TMO_MODE_NUM_TMO][PQ_GRULE_TMO_MODE_IP_NUM_TMO]=
{
    #if PQ_QM_VR
    {//RESERVED0, 0
        {//High
            PQ_IP_TMO_SOURCE_SETTING_RESERVED0_TMO,
            PQ_IP_TMO_TARGET_SETTING_RESERVED0_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_RESERVED0_TMO,
            PQ_IP_TMO_TARGET_SETTING_RESERVED0_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_RESERVED0_TMO,
            PQ_IP_TMO_TARGET_SETTING_RESERVED0_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//BT709, 1
        {//High
            PQ_IP_TMO_SOURCE_SETTING_BT709_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT709_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_BT709_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT709_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_BT709_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT709_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//UNSPECIFIED, 2
        {//High
            PQ_IP_TMO_SOURCE_SETTING_UNSPECIFIED_TMO,
            PQ_IP_TMO_TARGET_SETTING_UNSPECIFIED_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_UNSPECIFIED_TMO,
            PQ_IP_TMO_TARGET_SETTING_UNSPECIFIED_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_UNSPECIFIED_TMO,
            PQ_IP_TMO_TARGET_SETTING_UNSPECIFIED_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//RESERVED3, 3
        {//High
            PQ_IP_TMO_SOURCE_SETTING_RESERVED3_TMO,
            PQ_IP_TMO_TARGET_SETTING_RESERVED3_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_RESERVED3_TMO,
            PQ_IP_TMO_TARGET_SETTING_RESERVED3_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_RESERVED3_TMO,
            PQ_IP_TMO_TARGET_SETTING_RESERVED3_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//GAMMA2P2, 4
        {//High
            PQ_IP_TMO_SOURCE_SETTING_GAMMA2P2_TMO,
            PQ_IP_TMO_TARGET_SETTING_GAMMA2P2_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_GAMMA2P2_TMO,
            PQ_IP_TMO_TARGET_SETTING_GAMMA2P2_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_GAMMA2P2_TMO,
            PQ_IP_TMO_TARGET_SETTING_GAMMA2P2_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//GAMMA2P8, 5
        {//High
            PQ_IP_TMO_SOURCE_SETTING_GAMMA2P8_TMO,
            PQ_IP_TMO_TARGET_SETTING_GAMMA2P8_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_GAMMA2P8_TMO,
            PQ_IP_TMO_TARGET_SETTING_GAMMA2P8_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_GAMMA2P8_TMO,
            PQ_IP_TMO_TARGET_SETTING_GAMMA2P8_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//BT601525_601625, 6
        {//High
            PQ_IP_TMO_SOURCE_SETTING_BT601525_601625_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT601525_601625_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_BT601525_601625_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT601525_601625_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_BT601525_601625_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT601525_601625_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//SMPTE240M, 7
        {//High
            PQ_IP_TMO_SOURCE_SETTING_SMPTE240M_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE240M_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_SMPTE240M_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE240M_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_SMPTE240M_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE240M_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//LINEAR, 8
        {//High
            PQ_IP_TMO_SOURCE_SETTING_LINEAR_TMO,
            PQ_IP_TMO_TARGET_SETTING_LINEAR_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_LINEAR_TMO,
            PQ_IP_TMO_TARGET_SETTING_LINEAR_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_LINEAR_TMO,
            PQ_IP_TMO_TARGET_SETTING_LINEAR_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//LOG0, 9
        {//High
            PQ_IP_TMO_SOURCE_SETTING_LOG0_TMO,
            PQ_IP_TMO_TARGET_SETTING_LOG0_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_LOG0_TMO,
            PQ_IP_TMO_TARGET_SETTING_LOG0_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_LOG0_TMO,
            PQ_IP_TMO_TARGET_SETTING_LOG0_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//LOG1, 10
        {//High
            PQ_IP_TMO_SOURCE_SETTING_LOG1_TMO,
            PQ_IP_TMO_TARGET_SETTING_LOG1_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_LOG1_TMO,
            PQ_IP_TMO_TARGET_SETTING_LOG1_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_LOG1_TMO,
            PQ_IP_TMO_TARGET_SETTING_LOG1_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//XVYCC, 11
        {//High
            PQ_IP_TMO_SOURCE_SETTING_XVYCC_TMO,
            PQ_IP_TMO_TARGET_SETTING_XVYCC_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_XVYCC_TMO,
            PQ_IP_TMO_TARGET_SETTING_XVYCC_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_XVYCC_TMO,
            PQ_IP_TMO_TARGET_SETTING_XVYCC_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//BT1361, 12
        {//High
            PQ_IP_TMO_SOURCE_SETTING_BT1361_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT1361_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_BT1361_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT1361_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_BT1361_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT1361_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//SRGB_SYCC, 13
        {//High
            PQ_IP_TMO_SOURCE_SETTING_SRGB_SYCC_TMO,
            PQ_IP_TMO_TARGET_SETTING_SRGB_SYCC_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_SRGB_SYCC_TMO,
            PQ_IP_TMO_TARGET_SETTING_SRGB_SYCC_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_SRGB_SYCC_TMO,
            PQ_IP_TMO_TARGET_SETTING_SRGB_SYCC_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//BT2020NCL, 14
        {//High
            PQ_IP_TMO_SOURCE_SETTING_BT2020NCL_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT2020NCL_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_BT2020NCL_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT2020NCL_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_BT2020NCL_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT2020NCL_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//BT2020CL, 15
        {//High
            PQ_IP_TMO_SOURCE_SETTING_BT2020CL_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT2020CL_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_BT2020CL_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT2020CL_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_BT2020CL_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT2020CL_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//SMPTE2084, 16
        {//High
            PQ_IP_TMO_SOURCE_SETTING_SMPTE2084_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE2084_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_SMPTE2084_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE2084_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_SMPTE2084_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE2084_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//SMPTE428, 17
        {//High
            PQ_IP_TMO_SOURCE_SETTING_SMPTE428_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE428_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_SMPTE428_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE428_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_SMPTE428_TMO,
            PQ_IP_TMO_TARGET_SETTING_SMPTE428_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//ADOBERGB, 18
        {//High
            PQ_IP_TMO_SOURCE_SETTING_ADOBERGB_TMO,
            PQ_IP_TMO_TARGET_SETTING_ADOBERGB_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_ADOBERGB_TMO,
            PQ_IP_TMO_TARGET_SETTING_ADOBERGB_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_ADOBERGB_TMO,
            PQ_IP_TMO_TARGET_SETTING_ADOBERGB_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//BT1886, 19
        {//High
            PQ_IP_TMO_SOURCE_SETTING_BT1886_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT1886_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_BT1886_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT1886_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_BT1886_TMO,
            PQ_IP_TMO_TARGET_SETTING_BT1886_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//DOLBYMETA, 20
        {//High
            PQ_IP_TMO_SOURCE_SETTING_DOLBYMETA_TMO,
            PQ_IP_TMO_TARGET_SETTING_DOLBYMETA_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_DOLBYMETA_TMO,
            PQ_IP_TMO_TARGET_SETTING_DOLBYMETA_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_DOLBYMETA_TMO,
            PQ_IP_TMO_TARGET_SETTING_DOLBYMETA_L_TMO,
        },
    },
    #endif
    #if PQ_QM_VR
    {//HLG, 21
        {//High
            PQ_IP_TMO_SOURCE_SETTING_HLG_TMO,
            PQ_IP_TMO_TARGET_SETTING_HLG_H_TMO,
        },
        {//Mid
            PQ_IP_TMO_SOURCE_SETTING_HLG_TMO,
            PQ_IP_TMO_TARGET_SETTING_HLG_M_TMO,
        },
        {//Low
            PQ_IP_TMO_SOURCE_SETTING_HLG_TMO,
            PQ_IP_TMO_TARGET_SETTING_HLG_L_TMO,
        },
    },
    #endif
};
#endif

