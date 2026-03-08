/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TA_COMM_H
#define TA_COMM_H

#define TEE_Result MS_U32
#define TEE_SUCCESS                       0x00000000
#define TEE_ERROR_CORRUPT_OBJECT          0xF0100001
#define TEE_ERROR_CORRUPT_OBJECT_2        0xF0100002
#define TEE_ERROR_STORAGE_NOT_AVAILABLE   0xF0100003
#define TEE_ERROR_STORAGE_NOT_AVAILABLE_2 0xF0100004
#define TEE_ERROR_GENERIC                 0xFFFF0000
#define TEE_ERROR_ACCESS_DENIED           0xFFFF0001
#define TEE_ERROR_CANCEL                  0xFFFF0002
#define TEE_ERROR_ACCESS_CONFLICT         0xFFFF0003
#define TEE_ERROR_EXCESS_DATA             0xFFFF0004
#define TEE_ERROR_BAD_FORMAT              0xFFFF0005
#define TEE_ERROR_BAD_PARAMETERS          0xFFFF0006
#define TEE_ERROR_BAD_STATE               0xFFFF0007
#define TEE_ERROR_ITEM_NOT_FOUND          0xFFFF0008
#define TEE_ERROR_NOT_IMPLEMENTED         0xFFFF0009
#define TEE_ERROR_NOT_SUPPORTED           0xFFFF000A
#define TEE_ERROR_NO_DATA                 0xFFFF000B
#define TEE_ERROR_OUT_OF_MEMORY           0xFFFF000C
#define TEE_ERROR_BUSY                    0xFFFF000D
#define TEE_ERROR_COMMUNICATION           0xFFFF000E
#define TEE_ERROR_SECURITY                0xFFFF000F
#define TEE_ERROR_SHORT_BUFFER            0xFFFF0010
#define TEE_ERROR_EXTERNAL_CANCEL         0xFFFF0011
#define TEE_ERROR_OVERFLOW                0xFFFF300F
#define TEE_ERROR_TARGET_DEAD             0xFFFF3024
#define TEE_ERROR_STORAGE_NO_SPACE        0xFFFF3041
#define TEE_ERROR_MAC_INVALID             0xFFFF3071
#define TEE_ERROR_SIGNATURE_INVALID       0xFFFF3072
#define TEE_ERROR_TIME_NOT_SET            0xFFFF5000
#define TEE_ERROR_TIME_NEEDS_RESET        0xFFFF5001

/* Parameter Type Constants */
#define TEE_PARAM_TYPE_NONE             0
#define TEE_PARAM_TYPE_VALUE_INPUT      1
#define TEE_PARAM_TYPE_VALUE_OUTPUT     2
#define TEE_PARAM_TYPE_VALUE_INOUT      3
#define TEE_PARAM_TYPE_MEMREF_INPUT     5
#define TEE_PARAM_TYPE_MEMREF_OUTPUT    6
#define TEE_PARAM_TYPE_MEMREF_INOUT     7

typedef unsigned int                    uint32_t;

#define __unused __attribute__((unused))

#define TEE_PARAM_TYPES(t0,t1,t2,t3) \
   ((t0) | ((t1) << 4) | ((t2) << 8) | ((t3) << 12))
typedef union {
    struct {
        void *buffer;
        MS_U32 size;
    } memref;
    struct {
        MS_U32 a;
        MS_U32 b;
    } value;
} TEE_Param;


typedef struct {
    MS_U32 timeLow;
    MS_U16 timeMid;
    MS_U16 timeHiAndVersion;
    MS_U8 clockSeqAndNode[8];
} TEE_UUID;

/* Trusted Application Function header */
typedef struct ta_func_head {
    MS_U32 cmd_id;    /* Trusted Application Function ID */
    MS_U32 start;        /* offset to start func */
} ta_func_head_t;


typedef struct {
    /* Same Prefix as ta_head_t */
    TEE_UUID uuid;
    const char *name;
    MS_U32 flags;

    /* properties */
    MS_U32 prop_datasize;
    MS_U32 prop_stacksize;
    MS_U32 prop_tracelevel;

    const ta_func_head_t *funcs;
    MS_U32 nbr_func;
     TEE_Result(*create_entry_point) (void);
    void (*destroy_entry_point) (void);
     TEE_Result(*open_session_entry_point) (MS_U32 nParamTypes,
                         TEE_Param pParams[4],
                         void **ppSessionContext);
    void (*close_session_entry_point) (void *pSessionContext);
     TEE_Result(*invoke_command_entry_point) (void *pSessionContext,
                           MS_U32 nCommandID,
                           MS_U32 nParamTypes,
                           TEE_Param pParams[4]);
     TEE_Result(*core_entries) (MS_U32 nServiceId, MS_U32 nParamTypes,
                   TEE_Param pParam[4]);
} ta_static_head_t;

#endif /* TA_COMM_H */
