/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of
* the GNU General Public License as published by the Free Software Foundation;
* either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/
//******************************************************************************
//******************************************************************************

/*!
 **************************************************************************************
 * \file
 *    parsetcommon.h
 * \brief
 *    Picture and Sequence Parameter Sets, structures common to encoder and decoder
 *
 * \date 25 November 2002
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *      - Stephan Wenger        <stewe@cs.tu-berlin.de>
 ***************************************************************************************
 */


// In the JVT syntax, frequently flags are used that indicate the presence of
// certain pieces of information in the NALU.  Here, these flags are also
// present.  In the encoder, those bits indicate that the values signaled to
// be present are meaningful and that this part of the syntax should be
// written to the NALU.  In the decoder, the flag indicates that information
// was received from the decoded NALU and should be used henceforth.
// The structure names were chosen as indicated in the JVT syntax

#ifndef _PARSETCOMMON_H_
#define _PARSETCOMMON_H_

#define MAXIMUMPARSETRBSPSIZE   1500UL
#define MAXIMUMPARSETNALUSIZE   1500UL

#define MAXIMUMVALUEOFcpb_cnt   32UL
typedef struct
{
	MS_U32 cpb_cnt_minus1;                                   // ue(v)
	MS_U32 bit_rate_scale;                                   // u(4)
	MS_U32 cpb_size_scale;                                   // u(4)
    MS_U32 bit_rate_value_minus1 [MAXIMUMVALUEOFcpb_cnt];  // ue(v)
    MS_U32 cpb_size_value_minus1 [MAXIMUMVALUEOFcpb_cnt];  // ue(v)
    MS_U32 cbr_flag              [MAXIMUMVALUEOFcpb_cnt];  // u(1)
	MS_U32 initial_cpb_removal_delay_length_minus1;          // u(5)
	MS_U32 cpb_removal_delay_length_minus1;                  // u(5)
	MS_U32 dpb_output_delay_length_minus1;                   // u(5)
	MS_U32 time_offset_length;                               // u(5)
} hrd_parameters_t;


typedef struct
{
	MS_BOOL      aspect_ratio_info_present_flag;                   // u(1)
    MS_U32 aspect_ratio_idc;                               // u(8)
	MS_U32 sar_width;                                    // u(16)
	MS_U32 sar_height;                                   // u(16)
	MS_BOOL      overscan_info_present_flag;                       // u(1)
    MS_BOOL      overscan_appropriate_flag;                      // u(1)
	MS_BOOL      video_signal_type_present_flag;                   // u(1)
    MS_U32 video_format;                                   // u(3)
    MS_BOOL      video_full_range_flag;                          // u(1)
    MS_BOOL      colour_description_present_flag;                // u(1)
	MS_U32 colour_primaries;                             // u(8)
	MS_U32 transfer_characteristics;                     // u(8)
	MS_U32 matrix_coefficients;                          // u(8)
	MS_BOOL      chroma_location_info_present_flag;                // u(1)
    MS_U32  chroma_sample_loc_type_top_field;               // ue(v)
    MS_U32  chroma_sample_loc_type_bottom_field;            // ue(v)
	MS_BOOL      timing_info_present_flag;                         // u(1)
    MS_U32 num_units_in_tick;                              // u(32)
    MS_U32 time_scale;                                     // u(32)
    MS_BOOL      fixed_frame_rate_flag;                          // u(1)
	MS_BOOL      nal_hrd_parameters_present_flag;                  // u(1)
    hrd_parameters_t nal_hrd_parameters;                      // hrd_paramters_t
	MS_BOOL      vcl_hrd_parameters_present_flag;                  // u(1)
    hrd_parameters_t vcl_hrd_parameters;                      // hrd_paramters_t
	// if ((nal_hrd_parameters_present_flag || (vcl_hrd_parameters_present_flag))
    MS_BOOL      low_delay_hrd_flag;                             // u(1)
	MS_BOOL      pic_struct_present_flag;                        // u(1)
	MS_BOOL      bitstream_restriction_flag;                       // u(1)
    MS_BOOL      motion_vectors_over_pic_boundaries_flag;        // u(1)
    MS_U32 max_bytes_per_pic_denom;                        // ue(v)
    MS_U32 max_bits_per_mb_denom;                          // ue(v)
    MS_U32 log2_max_mv_length_vertical;                    // ue(v)
    MS_U32 log2_max_mv_length_horizontal;                  // ue(v)
    MS_U32 num_reorder_frames;                             // ue(v)
    MS_U32 max_dec_frame_buffering;                        // ue(v)
} vui_seq_parameters_t;


#define MAXnum_slice_groups_minus1  8UL
typedef struct
{
	MS_BOOL   Valid;                  // indicates the parameter set is valid
	MS_U32 pic_parameter_set_id;                             // ue(v)
	MS_U32 seq_parameter_set_id;                             // ue(v)
	MS_BOOL   entropy_coding_mode_flag;                         // u(1)
	
	MS_BOOL   transform_8x8_mode_flag;                          // u(1)
	
	MS_BOOL   pic_scaling_matrix_present_flag;                  // u(1)
	MS_S32       pic_scaling_list_present_flag[12];                 // u(1)
	
	// if( pic_order_cnt_type < 2 )  in the sequence parameter set
	MS_BOOL      pic_order_present_flag;                           // u(1)
	MS_U32 num_slice_groups_minus1;                          // ue(v)
    MS_U32 slice_group_map_type;                        // ue(v)
    // if( slice_group_map_type = = 0 )
	MS_U32 run_length_minus1[MAXnum_slice_groups_minus1]; // ue(v)
    // else if( slice_group_map_type = = 2 )
	MS_U32 top_left[MAXnum_slice_groups_minus1];         // ue(v)
	MS_U32 bottom_right[MAXnum_slice_groups_minus1];     // ue(v)
    // else if( slice_group_map_type = = 3 || 4 || 5
	MS_BOOL   slice_group_change_direction_flag;            // u(1)
	MS_U32 slice_group_change_rate_minus1;               // ue(v)
    // else if( slice_group_map_type = = 6 )
	MS_U32 pic_size_in_map_units_minus1;                 // ue(v)
	MS_U8      *slice_group_id;                              // complete MBAmap u(v)
	
	MS_S32 num_ref_idx_l0_active_minus1;                     // ue(v)
	MS_S32 num_ref_idx_l1_active_minus1;                     // ue(v)
	MS_BOOL   weighted_pred_flag;                               // u(1)
	MS_U32  weighted_bipred_idc;                              // u(2)
	MS_S32       pic_init_qp_minus26;                              // se(v)
	MS_S32       pic_init_qs_minus26;                              // se(v)
	MS_S32       chroma_qp_index_offset;                           // se(v)
	
	MS_S32       cb_qp_index_offset;                               // se(v)
	MS_S32       cr_qp_index_offset;                               // se(v)
	
	MS_BOOL   deblocking_filter_control_present_flag;           // u(1)
	MS_BOOL   constrained_intra_pred_flag;                      // u(1)
	MS_BOOL   redundant_pic_cnt_present_flag;                   // u(1)
	MS_BOOL   vui_pic_parameters_flag;                          // u(1)
} pic_parameter_set_rbsp_t;


#define MAXnum_ref_frames_in_pic_order_cnt_cycle  256UL
typedef struct
{
	MS_BOOL   Valid;                  // indicates the parameter set is valid
	
	MS_U32 profile_idc;                                      // u(8)
	MS_BOOL   constrained_set0_flag;                            // u(1)
	MS_BOOL   constrained_set1_flag;                            // u(1)
	MS_BOOL   constrained_set2_flag;                            // u(1)
	MS_BOOL   constrained_set3_flag;                            // u(1)
	MS_U32 level_idc;                                        // u(8)
	MS_U32 seq_parameter_set_id;                             // ue(v)
	MS_U32 chroma_format_idc;                                // ue(v)
	
	MS_U32 bit_depth_luma_minus8;                            // ue(v)
	MS_U32 bit_depth_chroma_minus8;                          // ue(v)
	MS_U32 log2_max_frame_num_minus4;                        // ue(v)
	MS_U32 pic_order_cnt_type;
	// if( pic_order_cnt_type == 0 )
	MS_U32 log2_max_pic_order_cnt_lsb_minus4;                 // ue(v)
	// else if( pic_order_cnt_type == 1 )
      MS_BOOL delta_pic_order_always_zero_flag;               // u(1)
      MS_S32     offset_for_non_ref_pic;                         // se(v)
      MS_S32     offset_for_top_to_bottom_field;                 // se(v)
      MS_U32 num_ref_frames_in_pic_order_cnt_cycle;          // ue(v)
    // for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
	MS_S32   offset_for_ref_frame[MAXnum_ref_frames_in_pic_order_cnt_cycle];   // se(v)
	MS_U32 num_ref_frames;                                   // ue(v)
	MS_BOOL   gaps_in_frame_num_value_allowed_flag;             // u(1)
	MS_U32 pic_width_in_mbs_minus1;                          // ue(v)
	MS_U32 pic_height_in_map_units_minus1;                   // ue(v)
	MS_BOOL   frame_mbs_only_flag;                              // u(1)
	// if( !frame_mbs_only_flag )
      MS_BOOL   mb_adaptive_frame_field_flag;                   // u(1)
	MS_BOOL   direct_8x8_inference_flag;                        // u(1)
	MS_BOOL   frame_cropping_flag;                              // u(1)
      MS_U32 frame_cropping_rect_left_offset;                // ue(v)
      MS_U32 frame_cropping_rect_right_offset;               // ue(v)
      MS_U32 frame_cropping_rect_top_offset;                 // ue(v)
      MS_U32 frame_cropping_rect_bottom_offset;              // ue(v)
	MS_BOOL   vui_parameters_present_flag;                      // u(1)
      vui_seq_parameters_t vui_seq_parameters;                  // vui_seq_parameters_t
      MS_U32  separate_colour_plane_flag;                       // u(1)
} seq_parameter_set_rbsp_t;

#endif
