#include "hwreg_frc_map.h"
#include "Maserati_FRC_ACT_4K0_5K_LLRR_240.h"

// FRC_ACT_4K0_5K_LLRR_240
// 3D_FHD_TB
void MFC_3D_FRC_ACT_4K0_5K_LLRR_240_3D_FHD_TB(void)
{
// FSC_TOP
    MDrv_WriteByteMask( REG_FSC_BK20_A2, 0x00, 0xff); // reg_splt_gb_in_size_r
    MDrv_WriteByteMask( REG_FSC_BK20_A3, 0x00, 0x01); // reg_splt_gb_in_size_r
    MDrv_WriteByteMask( REG_FSC_BK20_A4, 0x00, 0xff); // reg_splt_h_size
    MDrv_WriteByteMask( REG_FSC_BK20_A5, 0x00, 0x1f); // reg_splt_h_size
    MDrv_WriteByteMask( REG_FSC_BK20_A6, 0x00, 0xff); // reg_splt_h_size_l
    MDrv_WriteByteMask( REG_FSC_BK20_A7, 0x00, 0x0f); // reg_splt_h_size_l
    MDrv_WriteByteMask( REG_FSC_BK20_A8, 0x00, 0x80); // reg_mcm_lr_en
    MDrv_WriteByteMask( REG_FSC_BK20_49, 0x00, 0x0f); // reg_vertical_limit_cnt
    MDrv_WriteByteMask( REG_FSC_BK20_48, 0x00, 0xff); // reg_vertical_limit_cnt
    MDrv_WriteByteMask( REG_FSC_BK20_1D, 0x00, 0x1f); // reg_hde_st
    MDrv_WriteByteMask( REG_FSC_BK20_1C, 0x00, 0xff); // reg_hde_st
    MDrv_WriteByteMask( REG_FSC_BK20_1F, 0x00, 0x0f); // reg_vde_st
    MDrv_WriteByteMask( REG_FSC_BK20_1E, 0x00, 0xff); // reg_vde_st
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x00, 0x04); // reg_de_h_mask_en
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x00, 0x08); // reg_de_v_mask_en
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x00, 0x10); // reg_hvmask_en
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x00, 0x20); // reg_mask_hcnt_sel
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x00, 0x40); // reg_mask_vcnt_sel
    MDrv_WriteByteMask( REG_FSC_BK20_CA, 0x01, 0x01); // reg_path_selection
    MDrv_WriteByteMask( REG_FSC_BK20_05, 0x01, 0x10); // reg_ckg_ficlk
    MDrv_WriteByteMask( REG_FSC_BK20_04, 0x01, 0x1f); // reg_ckg_odclk
// CSC
// DD
    MDrv_WriteByteMask( REG_FSC_BK1D_02, 0x00, 0xff); // reg_dd_pixel_num (H)
    MDrv_WriteByteMask( REG_FSC_BK1D_03, 0x00, 0x0f); // reg_dd_pixel_num
    MDrv_WriteByteMask( REG_FSC_BK1D_04, 0x00, 0xff); // reg_dd_line_num (V)
    MDrv_WriteByteMask( REG_FSC_BK1D_05, 0x00, 0x0f); // reg_dd_line_num
    MDrv_WriteByteMask( REG_FSC_BK1D_0A, 0x00, 0x7f); // reg_flh_last_ctr
    MDrv_WriteByteMask( REG_FSC_BK1D_0B, 0x00, 0x7f); // reg_flh_req_ctr
    MDrv_WriteByteMask( REG_FSC_BK1D_0D, 0x00, 0x40); // reg_blk_md
// MCNR
    MDrv_WriteByteMask( REG_FSC_BK20_20, 0x00, 0xff); // reg_fetch_num
    MDrv_WriteByteMask( REG_FSC_BK20_21, 0x00, 0xff); // reg_fetch_num
    MDrv_WriteByteMask( REG_FSC_BK20_22, 0x00, 0xff); // reg_vlen
    MDrv_WriteByteMask( REG_FSC_BK20_23, 0x00, 0xff); // reg_vlen
// MCM
    MDrv_WriteByteMask( REG_FSC_BK12_1C, 0x00, 0xff); // reg_mcm_offset
    MDrv_WriteByteMask( REG_FSC_BK12_1D, 0x00, 0x1f); // reg_mcm_offset
    MDrv_WriteByteMask( REG_FSC_BK12_1E, 0x00, 0xff); // reg_mcm_fetch
    MDrv_WriteByteMask( REG_FSC_BK12_1F, 0x00, 0x1f); // reg_mcm_fetch
    MDrv_WriteByteMask( REG_FSC_BK12_60, 0x00, 0x0C); // reg_mcm_fi
    MDrv_WriteByteMask( REG_FSC_BK12_2F, 0x00, 0x80); // reg_ipm_cut_en
// VSU
    MDrv_WriteByteMask( REG_FSC_BK23_12, 0x00, 0xff); // vsp_scl_fac0
    MDrv_WriteByteMask( REG_FSC_BK23_13, 0x00, 0xff); // vsp_scl_fac1
    MDrv_WriteByteMask( REG_FSC_BK23_14, 0x00, 0xff); // vsp_scl_fac2
    MDrv_WriteByteMask( REG_FSC_BK23_15, 0x00, 0xff); // vsp_scl_en
    MDrv_WriteByteMask( REG_FSC_BK23_15, 0x00, 0x02); // vsp_shift_mode_en
    MDrv_WriteByteMask( REG_FSC_BK23_0A, 0x00, 0xff); // vsp_vsize_in0
    MDrv_WriteByteMask( REG_FSC_BK23_0B, 0x00, 0xff); // vsp_vsize_in1
    MDrv_WriteByteMask( REG_FSC_BK23_0C, 0x00, 0xff); // vsp_vsize_out0
    MDrv_WriteByteMask( REG_FSC_BK23_0D, 0x00, 0xff); // vsp_vsize_out1
// HSU
    MDrv_WriteByteMask( REG_FSC_BK23_0E, 0x00, 0xff); // hsp_scl_fac0
    MDrv_WriteByteMask( REG_FSC_BK23_0F, 0x00, 0xff); // hsp_scl_fac1
    MDrv_WriteByteMask( REG_FSC_BK23_10, 0x00, 0xff); // hsp_scl_fac2
    MDrv_WriteByteMask( REG_FSC_BK23_11, 0x00, 0xff); // hsp_scl_en
    MDrv_WriteByteMask( REG_FSC_BK23_11, 0x00, 0x02); // hsp_shift_mode_en
    MDrv_WriteByteMask( REG_FSC_BK23_34, 0x00, 0xff); // hsp_hsize_in0
    MDrv_WriteByteMask( REG_FSC_BK23_35, 0x00, 0xff); // hsp_hsize_in1
    MDrv_WriteByteMask( REG_FSC_BK23_36, 0x00, 0xff); // hsp_hsize_out0
    MDrv_WriteByteMask( REG_FSC_BK23_37, 0x00, 0xff); // hsp_hsize_out1
    MDrv_WriteByteMask( REG_FSC_BK23_38, 0x00, 0xff); // gb_in_size
    MDrv_WriteByteMask( REG_FSC_BK23_39, 0x00, 0x0f); // gb_in_size
    MDrv_WriteByteMask( REG_FSC_BK23_39, 0x00, 0x10); // gb_in_en
    MDrv_WriteByteMask( REG_FSC_BK23_3A, 0x00, 0xff); // gb_out_size
    MDrv_WriteByteMask( REG_FSC_BK23_3B, 0x00, 0x0f); // gb_out_size
    MDrv_WriteByteMask( REG_FSC_BK23_3B, 0x00, 0x10); // gb_out_en
    MDrv_WriteByteMask( REG_FSC_BK23_3E, 0x00, 0x04); // hsize_half_en
    MDrv_WriteByteMask( REG_FSC_BK23_3C, 0x00, 0xff); // hsp_mask_size
    MDrv_WriteByteMask( REG_FSC_BK23_3D, 0x00, 0x01); // hsp_mask_size
// VIP
    MDrv_WriteByteMask( REG_FSC_BK1B_1C, 0x00, 0xFF); // reg_vip_horizontal_num_lsb
    MDrv_WriteByteMask( REG_FSC_BK1B_1D, 0x00, 0x0F); // reg_vip_horizontal_num_msb
    MDrv_WriteByteMask( REG_FSC_BK1B_1E, 0x00, 0xFF); // reg_vip_vertical_num_lsb
    MDrv_WriteByteMask( REG_FSC_BK1B_1F, 0x00, 0x1F); // reg_vip_vertical_num_msb
    MDrv_WriteByteMask( REG_FSC_BK1B_1F, 0x00, 0x80); // reg_vip_tb_3d_en
// VIP_444to422
    MDrv_WriteByteMask( REG_FSC_BK1B_10, 0x00, 0x01); // reg_main_444to422_en
    MDrv_WriteByteMask( REG_FSC_BK1B_10, 0x00, 0x02); // reg_main_h_mirror_en
    MDrv_WriteByteMask( REG_FSC_BK1B_10, 0x00, 0x40); // reg_main_444to422_filter
    MDrv_WriteByteMask( REG_FSC_BK1B_11, 0x00, 0x01); // reg_sub_444to422_en
    MDrv_WriteByteMask( REG_FSC_BK1B_11, 0x00, 0x02); // reg_sub_h_mirror_en
    MDrv_WriteByteMask( REG_FSC_BK1B_11, 0x00, 0x40); // reg_sub_444to422_en
// SPTF_D2LR
    MDrv_WriteByteMask( REG_FSC_BK00_60, 0x00, 0xff); // gb_cut_st_l
    MDrv_WriteByteMask( REG_FSC_BK00_61, 0x00, 0xff); // gb_cut_st_l
    MDrv_WriteByteMask( REG_FSC_BK00_62, 0x00, 0xff); // gb_cut_end_l
    MDrv_WriteByteMask( REG_FSC_BK00_63, 0x00, 0xff); // gb_cut_end_l
    MDrv_WriteByteMask( REG_FSC_BK00_64, 0x00, 0xff); // gb_cut_st_r
    MDrv_WriteByteMask( REG_FSC_BK00_65, 0x00, 0xff); // gb_cut_st_r
    MDrv_WriteByteMask( REG_FSC_BK00_66, 0x00, 0xff); // gb_cut_end_r
    MDrv_WriteByteMask( REG_FSC_BK00_67, 0x00, 0xff); // gb_cut_end_r
    MDrv_WriteByteMask( REG_FSC_BK00_50, 0x00, 0x01); // d2lr_bypass
    MDrv_WriteByteMask( REG_FSC_BK00_50, 0x00, 0x02); // d2lr_eo
    MDrv_WriteByteMask( REG_FSC_BK00_50, 0x00, 0x04); // d2lr_lr
    MDrv_WriteByteMask( REG_FSC_BK00_52, 0x00, 0xff); // d2lr_output_h
    MDrv_WriteByteMask( REG_FSC_BK00_53, 0x00, 0xff); // d2lr_output_h
    MDrv_WriteByteMask( REG_FSC_BK00_40, 0x00, 0xff); // d2lr_w0_st
    MDrv_WriteByteMask( REG_FSC_BK00_41, 0x00, 0xff); // d2lr_w0_st
    MDrv_WriteByteMask( REG_FSC_BK00_42, 0x00, 0xff); // d2lr_w0_end
    MDrv_WriteByteMask( REG_FSC_BK00_43, 0x00, 0xff); // d2lr_w0_end
    MDrv_WriteByteMask( REG_FSC_BK00_44, 0x00, 0xff); // d2lr_w1_st
    MDrv_WriteByteMask( REG_FSC_BK00_45, 0x00, 0xff); // d2lr_w1_st
    MDrv_WriteByteMask( REG_FSC_BK00_46, 0x00, 0xff); // d2lr_w1_end
    MDrv_WriteByteMask( REG_FSC_BK00_47, 0x00, 0xff); // d2lr_w1_end
    MDrv_WriteByteMask( REG_FSC_BK00_48, 0x00, 0xff); // d2lr_r0_st
    MDrv_WriteByteMask( REG_FSC_BK00_49, 0x00, 0xff); // d2lr_r0_st
    MDrv_WriteByteMask( REG_FSC_BK00_4A, 0x00, 0xff); // d2lr_r0_end
    MDrv_WriteByteMask( REG_FSC_BK00_4B, 0x00, 0xff); // d2lr_r0_end
    MDrv_WriteByteMask( REG_FSC_BK00_4C, 0x00, 0xff); // d2lr_r1_st
    MDrv_WriteByteMask( REG_FSC_BK00_4D, 0x00, 0xff); // d2lr_r1_st
    MDrv_WriteByteMask( REG_FSC_BK00_4E, 0x00, 0xff); // d2lr_r1_end
    MDrv_WriteByteMask( REG_FSC_BK00_4F, 0x00, 0xff); // d2lr_r1_end
// FSC_FBL
    MDrv_WriteByteMask( REG_FSC_BK20_0E, 0x00, 0x01); // fbl_en
    MDrv_WriteByteMask( REG_FSC_BK20_0C, 0x00, 0x03); // reg_ipm_lr_en
    MDrv_WriteByteMask( REG_FSC_BK20_60, 0x00, 0x04); // 3d_sbs_en
    MDrv_WriteByteMask( REG_FSC_BK20_60, 0x00, 0x01); // 3d_top_bot_en
// IPM_OPM
    MDrv_WriteByteMask( REG_FSC_BK00_1C, 0x00, 0xff); // ipm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_1D, 0x00, 0xff); // ipm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_1E, 0x00, 0xff); // ipm_fetch_f2
    MDrv_WriteByteMask( REG_FSC_BK00_1F, 0x00, 0xff); // ipm_fetch_f2
    MDrv_WriteByteMask( REG_FSC_BK00_04, 0x00, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FSC_BK00_05, 0x00, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FSC_BK00_04, 0x00, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FSC_BK00_05, 0x00, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FSC_BK00_84, 0x00, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FSC_BK00_85, 0x00, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FSC_BK00_9C, 0x00, 0xff); // ipm_fetch_f1
    MDrv_WriteByteMask( REG_FSC_BK00_9D, 0x00, 0xff); // ipm_fetch_f1
    MDrv_WriteByteMask( REG_FSC_BK00_9E, 0x00, 0xff); // ipm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_9F, 0x00, 0xff); // ipm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_84, 0x00, 0xff); // ipm_mem_config_f1
    MDrv_WriteByteMask( REG_FSC_BK00_85, 0x00, 0xff); // ipm_mem_config_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2C, 0x00, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2D, 0x00, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2E, 0x00, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2F, 0x00, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FSC_BK00_30, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_31, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2C, 0x00, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2D, 0x00, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2E, 0x00, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2F, 0x00, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FSC_BK00_30, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_31, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2C, 0x00, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2D, 0x00, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2E, 0x00, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2F, 0x00, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FSC_BK00_30, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_31, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2C, 0x00, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2D, 0x00, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2E, 0x00, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2F, 0x00, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FSC_BK00_30, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_31, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2C, 0x00, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2D, 0x00, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2E, 0x00, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2F, 0x00, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FSC_BK00_30, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_31, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2C, 0x00, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2D, 0x00, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2E, 0x00, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2F, 0x00, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FSC_BK00_30, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_31, 0x00, 0xff); // reg_ipm_vcnt_limit_num_f2
// IPM_OPM_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FSC_BK00_62, 0x00, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_63, 0x00, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_62, 0x00, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_63, 0x00, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_62, 0x00, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_63, 0x00, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_62, 0x00, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_63, 0x00, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_62, 0x00, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_63, 0x00, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_62, 0x00, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_63, 0x00, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_CC, 0x00, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CC, 0x00, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CC, 0x00, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CC, 0x00, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CC, 0x00, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CC, 0x00, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_CD, 0x00, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_BA, 0x00, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BB, 0x00, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BA, 0x00, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BB, 0x00, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BA, 0x00, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BB, 0x00, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BA, 0x00, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BB, 0x00, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BA, 0x00, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BB, 0x00, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BA, 0x00, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_BB, 0x00, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FSC_BK00_C8, 0x00, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C9, 0x00, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C8, 0x00, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C9, 0x00, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C8, 0x00, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C9, 0x00, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C8, 0x00, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C9, 0x00, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C8, 0x00, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C9, 0x00, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C8, 0x00, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FSC_BK00_C9, 0x00, 0x1f); // reg_opm_meds_vlen_new
// IPM_OPM_DSmode
    MDrv_WriteByteMask( REG_FSC_BK00_1C, 0x00, 0xff); // reg_ipm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_1D, 0x00, 0xff); // reg_ipm_offset_f2
    MDrv_WriteByteMask( REG_FSC_BK00_1E, 0x00, 0xff); // reg_ipm_fetch_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_1F, 0x00, 0xff); // reg_ipm_fetch_num_f2
    MDrv_WriteByteMask( REG_FSC_BK00_9C, 0x00, 0xff); // reg_ipm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_9D, 0x00, 0xff); // reg_ipm_offset_f1
    MDrv_WriteByteMask( REG_FSC_BK00_9E, 0x00, 0xff); // reg_ipm_fetch_num_f1
    MDrv_WriteByteMask( REG_FSC_BK00_9F, 0x00, 0xff); // reg_ipm_fetch_num_f1
    MDrv_WriteByteMask( REG_FSC_BK00_BC, 0x00, 0xff); // reg_opm_meds_offset
    MDrv_WriteByteMask( REG_FSC_BK00_BD, 0x00, 0xff); // reg_opm_meds_offset
    MDrv_WriteByteMask( REG_FSC_BK00_BE, 0x00, 0xff); // reg_opm_meds_fetch_num
    MDrv_WriteByteMask( REG_FSC_BK00_BF, 0x00, 0xff); // reg_opm_meds_fetch_num
// IPM_3D
    MDrv_WriteByteMask( REG_FSC_BK00_1C, 0x00, 0xff); // reg_v_toggle_value
    MDrv_WriteByteMask( REG_FSC_BK00_1D, 0x00, 0x0f); // reg_v_toggle_value
    MDrv_WriteByteMask( REG_FSC_BK00_04, 0x00, 0x30); // reg_v_toggle_en
    MDrv_WriteByteMask( REG_FSC_BK00_27, 0x00, 0x80); // reg_ipm_ud_en
    MDrv_WriteByteMask( REG_FSC_BK00_27, 0x00, 0x80); // reg_ipm_meds_ud_en
    MDrv_WriteByteMask( REG_FSC_BK00_26, 0x00, 0xff); // reg_ipm_turn_back_line
    MDrv_WriteByteMask( REG_FSC_BK00_27, 0x00, 0x1f); // reg_ipm_turn_back_line
    MDrv_WriteByteMask( REG_FSC_BK00_26, 0x00, 0xff); // reg_ipm_turn_back_line_meds
    MDrv_WriteByteMask( REG_FSC_BK00_27, 0x00, 0x1f); // reg_ipm_turn_back_line_meds
// OPM_3D
    MDrv_WriteByteMask( REG_FSC_BK00_60, 0x00, 0x01); // reg_ipm_3d_en_f2
    MDrv_WriteByteMask( REG_FSC_BK00_E0, 0x00, 0x01); // reg_ipm_3d_en_f1
    MDrv_WriteByteMask( REG_FSC_BK00_60, 0x00, 0x01); // reg_ipm_3d_en_mef3f4_f2
    MDrv_WriteByteMask( REG_FSC_BK00_E0, 0x00, 0x01); // reg_ipm_3d_en_mef3f4_f1
    MDrv_WriteByteMask( REG_FSC_BK00_66, 0x00, 0x80); // reg_opm_3d_en_f2
    MDrv_WriteByteMask( REG_FSC_BK00_66, 0x00, 0x80); // reg_opm_3d_en_f1
    MDrv_WriteByteMask( REG_FSC_BK00_66, 0x00, 0x80); // reg_opm_3d_en_mef3f4_f2
    MDrv_WriteByteMask( REG_FSC_BK00_66, 0x00, 0x80); // reg_opm_3d_en_mef3f4_f1
    MDrv_WriteByteMask( REG_FSC_BK00_66, 0x00, 0x80); // reg_opm_3d_en_mif3_f2
    MDrv_WriteByteMask( REG_FSC_BK00_66, 0x00, 0x80); // reg_opm_3d_en_mif3_f1
    MDrv_WriteByteMask( REG_FSC_BK00_67, 0x00, 0x04); // reg_opm_passive_en_f2
    MDrv_WriteByteMask( REG_FSC_BK00_67, 0x00, 0x04); // reg_opm_passive_en_f1
    MDrv_WriteByteMask( REG_FSC_BK00_67, 0x00, 0x04); // reg_opm_passive_en_f2
    MDrv_WriteByteMask( REG_FSC_BK00_67, 0x00, 0x04); // reg_opm_passive_en_f1
    MDrv_WriteByteMask( REG_FSC_BK00_67, 0x00, 0x04); // reg_opm_passive_en_f2
    MDrv_WriteByteMask( REG_FSC_BK00_67, 0x00, 0x04); // reg_opm_passive_en_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2B, 0x00, 0x20); // reg_opm_line_repeat_en_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2B, 0x00, 0x20); // reg_opm_line_repeat_en_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2B, 0x00, 0x20); // reg_opm_line_repeat_en_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2B, 0x00, 0x20); // reg_opm_line_repeat_en_f1
    MDrv_WriteByteMask( REG_FSC_BK00_2B, 0x00, 0x20); // reg_opm_line_repeat_en_f2
    MDrv_WriteByteMask( REG_FSC_BK00_2B, 0x00, 0x20); // reg_opm_line_repeat_en_f1
// OPMRM
    MDrv_WriteByteMask( REG_FSC_BK00_E8, 0x00, 0x01); // reg_opm_ml_en
    MDrv_WriteByteMask( REG_FSC_BK00_E9, 0x00, 0x08); // reg_rbk_free
    MDrv_WriteByteMask( REG_FSC_BK00_E9, 0x00, 0x70); // reg_rbk_free_diff
    MDrv_WriteByteMask( REG_FSC_BK00_32, 0x00, 0x80); // reg_opm_2f_md
    MDrv_WriteByteMask( REG_FSC_BK00_32, 0x00, 0x80); // reg_opm_2f_md
    MDrv_WriteByteMask( REG_FSC_BK00_33, 0x00, 0x01); // reg_opm_3f_md
    MDrv_WriteByteMask( REG_FSC_BK00_33, 0x00, 0x01); // reg_opm_3f_md
    MDrv_WriteByteMask( REG_FSC_BK00_CE, 0x00, 0x0f); // reg_opm_memc_md
    MDrv_WriteByteMask( REG_FSC_BK00_CF, 0x00, 0x07); // reg_opm_memc_md
// HSD_MEDS
    MDrv_WriteByteMask( REG_FSC_BK00_C2, 0x00, 0xff); // reg_frc_ipm_hvsd_la_mode
    MDrv_WriteByteMask( REG_FSC_BK00_C3, 0x00, 0xff); // reg_frc_ipm_hvsd_drop_mode
// MEMC_FSC
    MDrv_WriteByteMask( REG_FSC_BK00_A4, 0x00, 0xff); // reg_frc_xxxx
    MDrv_WriteByteMask( REG_FSC_BK00_A5, 0x00, 0xff); // reg_frc_xxxx
    MDrv_WriteByteMask( REG_FSC_BK00_A6, 0x00, 0xff); // reg_frc_xxxx
    MDrv_WriteByteMask( REG_FSC_BK00_A7, 0x00, 0xff); // reg_frc_xxxx
// MLB
    MDrv_WriteByteMask( REG_FSC_BK00_F4, 0x00, 0xff); // reg_sadmvRangeL
    MDrv_WriteByteMask( REG_FSC_BK00_F5, 0x00, 0x03); // reg_sadmvRangeL
    MDrv_WriteByteMask( REG_FSC_BK00_F8, 0x00, 0xff); // reg_sadmvRangeU
    MDrv_WriteByteMask( REG_FSC_BK00_F9, 0x00, 0x01); // reg_sadmvRangeU
    MDrv_WriteByteMask( REG_FSC_BK00_F6, 0x00, 0xff); // reg_sadmvRangeR
    MDrv_WriteByteMask( REG_FSC_BK00_F7, 0x00, 0x03); // reg_sadmvRangeR
    MDrv_WriteByteMask( REG_FSC_BK00_F2, 0x00, 0xff); // reg_sadmvRangeD
    MDrv_WriteByteMask( REG_FSC_BK00_F3, 0x00, 0x01); // reg_sadmvRangeD
    MDrv_WriteByteMask( REG_FSC_BK00_02, 0x00, 0x02); // reg_pass3d_la
    MDrv_WriteByteMask( REG_FSC_BK00_11, 0x00, 0xc0); // reg_mask_en
// MV_PREPROC
    MDrv_WriteByteMask( REG_FSC_BK00_92, 0x00, 0x03); // reg_mv_preprocess
// MFC_pipectrl
    MDrv_WriteByteMask( REG_FSC_BK00_3E, 0x00, 0xff); // reg_ppctr_h_pixl_num_me
    MDrv_WriteByteMask( REG_FSC_BK00_3F, 0x00, 0x1f); // reg_ppctr_h_pixl_num_me
    MDrv_WriteByteMask( REG_FSC_BK00_40, 0x00, 0xff); // reg_ppctr_v_line_num_me
    MDrv_WriteByteMask( REG_FSC_BK00_41, 0x00, 0x1f); // reg_ppctr_v_line_num_me
    MDrv_WriteByteMask( REG_FSC_BK00_46, 0x00, 0xff); // reg_me_h_pixel_num_mi
    MDrv_WriteByteMask( REG_FSC_BK00_47, 0x00, 0x1f); // reg_me_h_pixel_num_mi
    MDrv_WriteByteMask( REG_FSC_BK00_48, 0x00, 0xff); // reg_me_v_pixel_num_mi
    MDrv_WriteByteMask( REG_FSC_BK00_49, 0x00, 0x1f); // reg_me_v_pixel_num_mi
    MDrv_WriteByteMask( REG_FSC_BK00_F8, 0x00, 0xff); // reg_h_pixel_num_mlb
    MDrv_WriteByteMask( REG_FSC_BK00_F9, 0x00, 0x1f); // reg_h_pixel_num_mlb
    MDrv_WriteByteMask( REG_FSC_BK00_FA, 0x00, 0xff); // reg_v_pixel_num_mlb
    MDrv_WriteByteMask( REG_FSC_BK00_FB, 0x00, 0x1f); // reg_v_pixel_num_mlb
    MDrv_WriteByteMask( REG_FSC_BK00_3A, 0x00, 0xff); // reg_time_gen_sw_h_width
    MDrv_WriteByteMask( REG_FSC_BK00_3B, 0x00, 0x1f); // reg_time_gen_sw_h_width
    MDrv_WriteByteMask( REG_FSC_BK00_3C, 0x00, 0xff); // reg_time_gen_sw_v_width
    MDrv_WriteByteMask( REG_FSC_BK00_3D, 0x00, 0x1f); // reg_time_gen_sw_v_width
    MDrv_WriteByteMask( REG_FSC_BK00_42, 0x00, 0xff); // reg_mlb_disp_pixel_latch
    MDrv_WriteByteMask( REG_FSC_BK00_43, 0x00, 0x1f); // reg_mlb_disp_pixel_latch
    MDrv_WriteByteMask( REG_FSC_BK00_A8, 0x00, 0xff); // reg_gmv_vertical_active_window_height
    MDrv_WriteByteMask( REG_FSC_BK00_A9, 0x00, 0x01); // reg_gmv_vertical_active_window_height
// MFC_VDUP
    MDrv_WriteByteMask( REG_FSC_BK00_04, 0x00, 0xFF); // reg_VDUP_width
    MDrv_WriteByteMask( REG_FSC_BK00_05, 0x00, 0xFF); // reg_VDUP_width
    MDrv_WriteByteMask( REG_FSC_BK00_06, 0x00, 0xFF); // reg_VDUP_height
    MDrv_WriteByteMask( REG_FSC_BK00_07, 0x00, 0xFF); // reg_VDUP_height
// MFC
    MDrv_WriteByteMask( REG_FSC_BK00_61, 0x00, 0xff); // reg_MFC_enable
    MDrv_WriteByteMask( REG_FSC_BK00_70, 0x00, 0x20); //
    MDrv_WriteByteMask( REG_FSC_BK00_73, 0x00, 0x1f); // reg_h_pix_num_3D
    MDrv_WriteByteMask( REG_FSC_BK00_72, 0x00, 0xff); // reg_h_pix_num_3D
    MDrv_WriteByteMask( REG_FSC_BK00_75, 0x00, 0x1f); // reg_v_lin_num_3D
    MDrv_WriteByteMask( REG_FSC_BK00_74, 0x00, 0xff); // reg_v_lin_num_3D
    MDrv_WriteByteMask( REG_FSC_BK00_80, 0x00, 0x08); //
    MDrv_WriteByteMask( REG_FSC_BK00_05, 0x00, 0xFF); // reg_h_pix_num_ME
    MDrv_WriteByteMask( REG_FSC_BK00_04, 0x00, 0xFF); // reg_h_pix_num_ME
    MDrv_WriteByteMask( REG_FSC_BK00_07, 0x00, 0x0F); // reg_v_lin_num_ME
    MDrv_WriteByteMask( REG_FSC_BK00_06, 0x00, 0xFF); // reg_v_lin_num_ME
    MDrv_WriteByteMask( REG_FSC_BK00_91, 0x00, 0xFF); // reg_h_pix_num_MI
    MDrv_WriteByteMask( REG_FSC_BK00_90, 0x00, 0xFF); // reg_h_pix_num_MI
    MDrv_WriteByteMask( REG_FSC_BK00_93, 0x00, 0xFF); // reg_v_lin_num_MI
    MDrv_WriteByteMask( REG_FSC_BK00_92, 0x00, 0xFF); // reg_v_lin_num_MI
    MDrv_WriteByteMask( REG_FSC_BK00_67, 0x00, 0x01); // reg_422to444_en
    MDrv_WriteByteMask( REG_FSC_BK00_D0, 0x00, 0x02); // reg_rgb_bypass
    MDrv_WriteByteMask( REG_FSC_BK00_C4, 0x00, 0xFF); // reg_hr_miu_req
    MDrv_WriteByteMask( REG_FSC_BK00_60, 0x00, 0x80); // reg_c_drop
    MDrv_WriteByteMask( REG_FSC_BK00_9C, 0x00, 0x80); // reg_422_avgmode
    MDrv_WriteByteMask( REG_FSC_BK00_80, 0x00, 0x01); // reg_pipectrl_bypass
// MFC_Halo
    MDrv_WriteByteMask( REG_FSC_BK00_93, 0x00, 0x80); // reg_halo_buf_frame_end_en
    MDrv_WriteByteMask( REG_FSC_BK00_A7, 0x00, 0x02); // reg_halo_buf23_force_ready
// MFC_GMV
    MDrv_WriteByteMask( REG_FSC_BK00_2A, 0x00, 0x01); // reg_gmv_in_ud_mode
// SNR
    MDrv_WriteByteMask( REG_FSC_BK00_E0, 0x00, 0x08); // reg_snr_bypass_en
    MDrv_WriteByteMask( REG_FSC_BK00_E0, 0x00, 0x04); // reg_snr_la_en
    MDrv_WriteByteMask( REG_FSC_BK00_E2, 0x00, 0xff); // reg_snr_pix_num_LSB
    MDrv_WriteByteMask( REG_FSC_BK00_E3, 0x00, 0x1f); // reg_snr_pix_num_MSB
    MDrv_WriteByteMask( REG_FSC_BK00_E8, 0x00, 0xff); // reg_snr_line_num_LSB
    MDrv_WriteByteMask( REG_FSC_BK00_E9, 0x00, 0x0f); // reg_snr_line_num_MSB
// SNR_3D
    MDrv_WriteByteMask( REG_FSC_BK00_65, 0x00, 0x20); // reg_line_switch
// SNR_VSU2X
    MDrv_WriteByteMask( REG_FSC_BK00_E0, 0x00, 0x10); // reg_lbi_vsu2x_en
// T3D
    MDrv_WriteByteMask( REG_FSC_BKC6_C0, 0x00, 0xff); // reg_pipe_guard_cycle_pb
    MDrv_WriteByteMask( REG_FSC_BKC6_C2, 0x00, 0xff); // reg_ln_width
    MDrv_WriteByteMask( REG_FSC_BKC6_C3, 0x00, 0x3f); // reg_ln_width
    MDrv_WriteByteMask( REG_FSC_BKC6_C4, 0x00, 0xff); // reg_col_height
    MDrv_WriteByteMask( REG_FSC_BKC6_C5, 0x00, 0x1f); // reg_col_height
    MDrv_WriteByteMask( REG_FSC_BKC6_C5, 0x00, 0x60); // reg_lb_mode_control
    MDrv_WriteByteMask( REG_FSC_BKC6_C6, 0x00, 0x01); // reg_srclb_en
    MDrv_WriteByteMask( REG_FSC_BKC6_C6, 0x00, 0x02); // reg_depthlb_en
    MDrv_WriteByteMask( REG_FSC_BKC6_D0, 0x00, 0x01); // reg_render_control
    MDrv_WriteByteMask( REG_FSC_BKC6_D0, 0x00, 0x02); // reg_lr_control_l
    MDrv_WriteByteMask( REG_FSC_BKC6_D0, 0x00, 0x04); // reg_lr_control_r
    MDrv_WriteByteMask( REG_FSC_BKC6_10, 0x00, 0x10); // reg_mtv_bypass_en
    MDrv_WriteByteMask( REG_FSC_BKC6_C5, 0x00, 0x40); // reg_lineswap
// FO_HSU
    MDrv_WriteByteMask( REG_FSC_BK00_30, 0x00, 0xff); // hfac_smd0
    MDrv_WriteByteMask( REG_FSC_BK00_31, 0x00, 0xff); // hfac_smd1
    MDrv_WriteByteMask( REG_FSC_BK00_32, 0x00, 0x3f); // hfac_smd2
    MDrv_WriteByteMask( REG_FSC_BK00_33, 0x00, 0x01); // hsp_bypass_en
    MDrv_WriteByteMask( REG_FSC_BK00_33, 0x00, 0x02); // hsp_bypass2_en
    MDrv_WriteByteMask( REG_FSC_BK00_0C, 0x00, 0xff); // hsp_size_in0
    MDrv_WriteByteMask( REG_FSC_BK00_0D, 0x00, 0x1f); // hsp_size_in1
    MDrv_WriteByteMask( REG_FSC_BK00_0E, 0x00, 0xff); // hsp_size_out0
    MDrv_WriteByteMask( REG_FSC_BK00_0F, 0x00, 0x1f); // hsp_size_out1
// SPTP
    MDrv_WriteByteMask( REG_FSC_BK00_80, 0x00, 0xff); // reg_sptp_mfc_dc_m1
    MDrv_WriteByteMask( REG_FSC_BK00_81, 0x00, 0x0f); // reg_sptp_mfc_dc_m1
    MDrv_WriteByteMask( REG_FSC_BK00_82, 0x00, 0x80); // reg_sptp_fbl_en
    MDrv_WriteByteMask( REG_FSC_BK00_84, 0x00, 0x01); // reg_sptp_usr_en
    MDrv_WriteByteMask( REG_FSC_BK00_88, 0x00, 0xff); // reg_sptp_f0_st
    MDrv_WriteByteMask( REG_FSC_BK00_89, 0x00, 0x01); // reg_sptp_f0_st
    MDrv_WriteByteMask( REG_FSC_BK00_8A, 0x00, 0xff); // reg_sptp_f0_end
    MDrv_WriteByteMask( REG_FSC_BK00_8B, 0x00, 0x01); // reg_sptp_f0_end
    MDrv_WriteByteMask( REG_FSC_BK00_8C, 0x00, 0xff); // reg_sptp_f1_st
    MDrv_WriteByteMask( REG_FSC_BK00_8D, 0x00, 0x01); // reg_sptp_f1_st
    MDrv_WriteByteMask( REG_FSC_BK00_8E, 0x00, 0xff); // reg_sptp_f1_end
    MDrv_WriteByteMask( REG_FSC_BK00_8F, 0x00, 0x01); // reg_sptp_f1_end
    MDrv_WriteByteMask( REG_FSC_BK00_86, 0x00, 0xff); // reg_sptp_gb_en
// CSC
}



/********************************************/
