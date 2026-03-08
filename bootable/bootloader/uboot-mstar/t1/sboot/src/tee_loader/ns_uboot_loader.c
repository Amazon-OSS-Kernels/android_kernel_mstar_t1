
#include "../../../u-boot-2011.06/MstarApp/include/MsTrustZone.h"
#include "../seal/src/drvSEAL.h"
#include "../mmc/inc/common/eMMC.h"
#include "../aesdma/src/drvAESDMA.h"
#include "../part_emmc.h"

#include "../MMAPInfo.h"

#if defined(__aarch64__)
#define UINT2PTR(x) ((void *)(MS_U64)x)
#define PTR2UINT(x) ((void *)(MS_U64)x)
#else
#define UINT2PTR(x) (x)
#define PTR2UINT(x) (x)
#endif

#define ALIGN(a,s) (((a)-((s) & ((a)-1))) & ((a)-1))


#define CONFIG_OPTEE_HEADER_LEN (sizeof(mstar_tee_t))
#define OPTEE_HEADER_MAGIC (0x4554504f)
#define EMMC_BLOCK_SIZE (512)
#define SEAL_ALIGN_BIT_WIDTH (12)

#define SEAL_ATTR_SECURE_RW (0x03)
#define SEAL_ATTR_ALL_RW (0x0F)

#define MIU0_PA_BASE               0x00000000UL
#define MIU1_PA_BASE               0x80000000UL
#define MIU2_PA_BASE               0xC0000000UL


#define SIGNATURE_LEN               256
#define RSA_PUBLIC_KEY_N_LEN 256
#define RSA_PUBLIC_KEY_E_LEN 4
#define RSA_PUBLIC_KEY_LEN          (RSA_PUBLIC_KEY_N_LEN+RSA_PUBLIC_KEY_E_LEN)

#define TEEKEYBANK_OFFSET (0x2000)
#define REEKEYBANK_OFFSET (0x3000)

#define AES_IV_LEN 16
#define AES_KEY_LEN 16
#define HMAC_KEY_LEN 32

struct rpmb_fs_partition {
    U32 rpmb_fs_magic;
    U32 fs_version;
    U32 write_counter;
    U32 fat_start_address;
    /* Do not use reserved[] for other purpose than partition data. */
    U8 reserved[92];
    U32 sboot_version;
    U32 uboot_version;
    U32 optee_version;
    U32 armfw_version;
    U32 optee_config_version;
};

typedef struct
{
    unsigned int ATFBase;
    unsigned int ATFSize;
    unsigned int ATFEnrty;
    unsigned int ATFFlag;
    unsigned char ATFMAC[16];
    unsigned int ATFDummy[4];
    unsigned int TLBase;
    unsigned int TLSize;
    unsigned int TLEnrty;
    unsigned int TLDummy;
} warmboot_table;

typedef struct
{
    U32 u32Num;
    U32 u32Size;
}IMAGE_INFO;

typedef struct
{
  U8 u8SecIdentify[8];
  IMAGE_INFO info;
  U8 u8Signature[SIGNATURE_LEN];
}_SUB_SECURE_INFO;

typedef struct
{
    _SUB_SECURE_INFO customer;
    U8 u8RSABootPublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAUpgradePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8RSAImagePublicKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESBootKey[AES_KEY_LEN];
    U8 u8AESUpgradeKey[AES_KEY_LEN];
    U8 u8MagicID[16];
    U8 crc[4];
}CUSTOMER_KEY_BANK;

typedef struct
{
    _SUB_SECURE_INFO SecureInfo;
    U8 u8MSID[4];
    U8 u8RSATEEKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESTEEKey[AES_KEY_LEN];
} TEE_KEY_BANK;

typedef struct
{
    _SUB_SECURE_INFO SecureInfo;
    U8 u8RSAKey[RSA_PUBLIC_KEY_LEN];
    U8 u8AESKey[AES_KEY_LEN];
} REE_KEY_BANK;


typedef struct
{
    unsigned char MagicID[12];
    unsigned int NumberOfConfig;
    unsigned int TotalConfigSize; // without descriptor
    unsigned int ConfigVersion;
    unsigned char Reserved[8];
} OPTEE_CONFIG_HEADER;

typedef struct
{
    unsigned int ConfigID;
    unsigned int Offset;
    unsigned int ConfigSize;
    unsigned int Reserved;
} OPTEE_CONFIG_DESCRIPTOR;

typedef struct
{
    unsigned int NumberOfSecureGroup;
    unsigned int NumberOfMmapID;
} OPTEE_CONFIG_MMAP_HEADER;

typedef struct
{
    unsigned int addr;
    unsigned int length;
} ST_MMAP_GROUP_ITEM;

/*
typedef struct
{
    unsigned char valid;
    unsigned int addr;
    unsigned int length;
    unsigned char MiuSel;
    unsigned int index;
} ST_MMAP_DB_ITEM;
*/
typedef struct
{
    unsigned char Dummy1[10];
    unsigned short index;
    unsigned char Dummy2[2];
} ST_MMAP_DB_ITEM;


void ns_uboot_boot(unsigned int uboot_addr);
unsigned int ns_uboot_load_optee(void);
unsigned int ns_uboot_load_armfw(void);
unsigned long long __BA2PA(unsigned long long u64BusAddr);
unsigned int ns_uboot_get_optee_addr(void);
unsigned int ns_uboot_nonsecure_handler(void);

// For STR
static int _gIsStr = 0;
static int _gStrMode = 0;
static int _gAuthMode = 0;

static unsigned long long BIN0_entry_point;
static unsigned long Fireware_entry;
static char _gOpteeBootArgs[36];
static unsigned int _gKeybankRomOffset;
static CUSTOMER_KEY_BANK _gKeybank;
static TEE_KEY_BANK _gTEEKeybank;
static REE_KEY_BANK _gREEKeybank;
extern const U8 EmbeddedTeeRsaPub[];
extern const U8 EmbeddedReeRsaPub[];
extern U8 EmbeddedAESTEEUniformKey[];
extern U8 EmbeddedAESREEUniformKey[];
static const U8 u8CusKeyMagicID[16]={0x4D,0x73,0x74,0x61,0x72,0x2E,0x4B,0x65,0x79,0x2E,0x42,0x61,0x6E,0x6B,0x2E,0x2E};
static const U8 u8SecurityIdentify[8]={'S','E','C','U','R','I','T','Y'};


static const U8 u8TmpBuf[512*7] __attribute__((aligned(16)));

static U8 u8ATF_MAC[16] __attribute__((aligned(16)));

// AESboot.bin key, this key will not change, so for now i just hard-code it here
static U8 aesBootKey[AES_KEY_LEN]={0xE0, 0x10, 0x01, 0xFF, 0x0F, 0xAA, 0x55, 0xFC, 0x92, 0x4D, 0x53, 0x54, 0x41, 0xFF, 0x07, 0x00}; // SW key, ported from hwKey in MstarApp/src/secure/crypto_aes.c
static U8 cus_aesBootKey[AES_KEY_LEN]; // this key will be set when ns_uboot_read_keybank() is called

//use efuse to encrypt puCommonTaString
static U8 puCommonTaString[AES_KEY_LEN] = {'M', 'S', 't', 'a', 'r', 'C', 'o', 'm', 'm', 'o', 'n', 'T', 'A', 'K', 'e', 'y'};

static int authFailType = 0;
#define AUTH_KEYBANK_FAIL 1
#define AUTH_ARMFW_FAIL 2
#define AUTH_OPTEE_FAIL 3

static unsigned int _gOpteeVersion = 0;
static unsigned int _gArmFwVersion = 0;
static unsigned int * _gUbootVersionAddr = NULL;
// ***** ANTI_ROLLBACK SBOOT VERSION *****
static const unsigned int _gSbootVersion = 2;

#define IS_MBOOTBAK() ((*(volatile unsigned int*)(CONFIG_RIU_BASE_ADDRESS + (0x103380<<1)))&0x0100)

#define IS_SECURE_CHIP()    ((*(volatile unsigned short*)(CONFIG_RIU_BASE_ADDRESS + (0x038E0<<1)))&0x01)

static void *_memcpy(void *d, void *s, unsigned int n)
{
    char *dest = (char*)d;
    char *src = (char*)s;

    while (n-- > 0)
    {
        *dest++ = *src++;
    }
    return d;
}


static void ns_uboot_print(char *str)
{
    extern void uart_putc(char c);
    while (*str)
    {
        uart_putc(*str);
        str++;
    }
}

void array_reverse(char* ary, unsigned int len)
{
    int i, j;
    char tmp;
    for (i=0, j=len-1; i<j; ++i, --j)
    {
        tmp = ary[i];
        ary[i] = ary[j];
        ary[j] = tmp;
    }
}

unsigned long long __BA2PA(unsigned long long u64BusAddr)
{
    unsigned long long u64PhyAddr = 0x0;
#ifdef CONFIG_MSTAR_MASERATI
    if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) && (u64BusAddr < CONFIG_MIU1_BUSADDR) ) // MIU0
        u64PhyAddr = u64BusAddr - CONFIG_MIU0_BUSADDR + MIU0_PA_BASE;
    else if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) && (u64BusAddr < CONFIG_MIU2_BUSADDR) )// MIU1
        u64PhyAddr = u64BusAddr - CONFIG_MIU1_BUSADDR + MIU1_PA_BASE;
    else                                                                        //MIU2
        u64PhyAddr = u64BusAddr - CONFIG_MIU2_BUSADDR + MIU2_PA_BASE;
#else
    if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) && (u64BusAddr < CONFIG_MIU1_BUSADDR) ) // MIU0
        u64PhyAddr = u64BusAddr - CONFIG_MIU0_BUSADDR + MIU0_PA_BASE;
    else if( (u64BusAddr >= CONFIG_MIU0_BUSADDR) )// MIU1
        u64PhyAddr = u64BusAddr - CONFIG_MIU1_BUSADDR + MIU1_PA_BASE;
#endif
    return u64PhyAddr;
}

unsigned long long __PA2BA(unsigned long long u64PhyAddr)
{
    unsigned long long u64BusAddr = 0x0;
#ifdef CONFIG_MSTAR_MASERATI
    if( (u64PhyAddr >= MIU0_PA_BASE) && (u64PhyAddr < MIU1_PA_BASE) ) // MIU0
        u64BusAddr = u64PhyAddr - MIU0_PA_BASE + CONFIG_MIU0_BUSADDR;
    else if ( (u64PhyAddr >= MIU1_PA_BASE) && (u64PhyAddr < MIU2_PA_BASE) ) // MIU1
        u64BusAddr = u64PhyAddr - MIU1_PA_BASE + CONFIG_MIU1_BUSADDR;
    else
        u64BusAddr = u64PhyAddr - MIU2_PA_BASE + CONFIG_MIU2_BUSADDR;
#else
    if( (u64PhyAddr >= MIU0_PA_BASE) && (u64PhyAddr < MIU1_PA_BASE) ) // MIU0
        u64BusAddr = u64PhyAddr - MIU0_PA_BASE + CONFIG_MIU0_BUSADDR;
    else if ( (u64PhyAddr >= MIU1_PA_BASE) ) // MIU1
        u64BusAddr = u64PhyAddr - MIU1_PA_BASE + CONFIG_MIU1_BUSADDR;
#endif
    return u64BusAddr;
}

unsigned int ns_uboot_anti_rollback(void)
{
#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
    extern void uart_putc(char c);
    U8 buf[0x100];
    struct rpmb_fs_partition *ptr;
    ptr = (struct rpmb_fs_partition*)buf;

    if(*(volatile unsigned short *)(0x1f000c08) == 0xBABE)
    {
        uart_putc('P');
        eMMC_RPMB_program_auth_key();
        /* set the register to indicate that key is provisioned */
        *(volatile unsigned short *)(0x1f000c08) = 0xEBAB;
        goto write_ver;
    }

    /* clear the register */
    *(volatile unsigned short *)(0x1f000c08) = 0x0;
    U32 ret = eMMC_RPMB_IfKeyWritten();
    uart_putc('0'+ret);
    if (ret == 0)
        return TRUE; /* key not written*/
    /* set the register to indicate that key is provisioned */
    *(volatile unsigned short *)(0x1f000c08) = 0xEBAB;

    ret = eMMC_RPMB_Read_data(buf, 0x100, 0);

    uart_putc('0' + ret);
    uart_putc('a' + ptr->optee_version);
    uart_putc('a' + ptr->armfw_version);
    uart_putc('a' + ptr->uboot_version);
    uart_putc('a' + ptr->sboot_version);
    uart_putc('a' + _gOpteeVersion);
    uart_putc('a' + _gArmFwVersion);
    uart_putc('a' + *_gUbootVersionAddr);
    uart_putc('a' + _gSbootVersion);

    /* if read error, or any kept version is larger, stop booting */
    if (ret || ptr->optee_version > _gOpteeVersion ||
        ptr->armfw_version > _gArmFwVersion ||
        ptr->uboot_version > (*_gUbootVersionAddr) ||
        ptr->sboot_version > _gSbootVersion)
    {
        uart_putc('R');
        while(1);
    }

    if (ptr->optee_version == _gOpteeVersion &&
        ptr->armfw_version == _gArmFwVersion &&
        ptr->uboot_version == *_gUbootVersionAddr &&
        ptr->sboot_version == _gSbootVersion)
    {
        return TRUE; /* versions are all same */
    }

write_ver:
    uart_putc('W');
    ptr->optee_version = _gOpteeVersion;
    ptr->armfw_version = _gArmFwVersion;
    ptr->uboot_version = *_gUbootVersionAddr;
    ptr->sboot_version = _gSbootVersion;
    eMMC_RPMB_Write_data(buf, 0x100, 0);
#endif

    return TRUE;
}

void ns_uboot_set_uboot_version(unsigned int uboot_start, unsigned int uboot_size)
{
    _gUbootVersionAddr = uboot_start + uboot_size - 0x200;
}

static unsigned int ns_uboot_is_str(void)
{
    return ((*(volatile unsigned short*)(0x1F000000 + (0x0E70<<1))) & 0xF000) != 0xF000;
}

unsigned int ns_uboot_support_faststr(void)
{
    return ns_uboot_is_str() && (_gStrMode == FAST_BOOT);
}

unsigned int ns_uboot_load_optee(void)
{
    mstar_tee_t* header = NULL;
    emmc_partition_t mpart;
    OPTEE_CONFIG_HEADER* opteeConfigHeader = NULL;
    int auth_ok;
    int encrypted = 0;
    int alignment;
    U32 tmpBuf = (U32)u8TmpBuf;
#ifdef CONFIG_MSTAR_NS_UBOOT_BRINGUP
    unsigned int u32HeadSize = sizeof(mstar_tee_t);
#else
    unsigned int u32HeadSize = sizeof(mstar_tee_t) + sizeof(OPTEE_CONFIG_HEADER) + sizeof(_SUB_SECURE_INFO);
#endif
    unsigned int u32HeadeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, u32HeadSize);
    unsigned int u32HeadeMMCBlockNum = (u32HeadSize + u32HeadeMMCAlign) / EMMC_BLOCK_SIZE;

    if (ns_uboot_load_partition(5, "optee", &header, &mpart))
    {
        eMMC_ReadData_MIU((U8*)UINT2PTR(tmpBuf), u32HeadSize + u32HeadeMMCAlign, mpart.start_block);
        // verify header
        {
#ifndef CONFIG_MSTAR_NS_UBOOT_BRINGUP
            int auth_ok = MDrv_AESDMA_SecureMain_v2(tmpBuf, CONFIG_OPTEE_HEADER_LEN+sizeof(OPTEE_CONFIG_HEADER), ((_SUB_SECURE_INFO*)(tmpBuf+CONFIG_OPTEE_HEADER_LEN+sizeof(OPTEE_CONFIG_HEADER)))->u8Signature, _gTEEKeybank.u8RSATEEKey);
            if (!auth_ok)
            {
                // optee header auth fail
                header = NULL;
                authFailType = AUTH_OPTEE_FAIL;
                return FALSE;
            }
            else
#endif
            {
                header = (mstar_tee_t*)UINT2PTR(tmpBuf);
                opteeConfigHeader = (OPTEE_CONFIG_HEADER*)(tmpBuf+CONFIG_OPTEE_HEADER_LEN);
            }
        }
    }
    else
    {
        // partition 'optee' not found
        return FALSE;
    }

    BIN0_entry_point = header->init_load_addr_lo;
    _gOpteeVersion = header->cus_version;

    if ( _gIsStr && _gStrMode )
    {
        MDrv_SEAL_SetSecureRange(0, __BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_SECURE_RW);
        return TRUE;
    }

    MDrv_SEAL_SetSecureRange(0, __BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_ALL_RW);

#if defined(CONFIG_RUN_WITH_V3_TEE_IMAGE)
    // copy header part to load address
    _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(tmpBuf), u32HeadSize + u32HeadeMMCAlign);
#else
    // copy body data that in the same emmc block with head data
    if (u32HeadeMMCAlign != 0)
    {
        _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(tmpBuf + u32HeadSize), u32HeadeMMCAlign);
    }
#endif

    // read other body data
#ifdef CONFIG_MSTAR_NS_UBOOT_BRINGUP
    unsigned int u32BodyAlign = ALIGN(16,header->init_size) ; // Body dummy data
    unsigned int u32BodyeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, header->init_size + u32BodyAlign - u32HeadeMMCAlign);
    unsigned int u32BodyOtherSizeeMMC = header->init_size + u32BodyAlign - u32HeadeMMCAlign + u32BodyeMMCAlign;
#else
    unsigned int u32BodyAlign = ALIGN(16,header->init_size) + ALIGN(16,opteeConfigHeader->TotalConfigSize) + opteeConfigHeader->NumberOfConfig*sizeof(OPTEE_CONFIG_DESCRIPTOR) + opteeConfigHeader->TotalConfigSize; // Body dummy data
    unsigned int u32BodyeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, header->init_size + u32BodyAlign + sizeof(_SUB_SECURE_INFO) - u32HeadeMMCAlign);
    unsigned int u32BodyOtherSizeeMMC = header->init_size + u32BodyAlign + sizeof(_SUB_SECURE_INFO) - u32HeadeMMCAlign + u32BodyeMMCAlign;
#endif

#if defined(CONFIG_RUN_WITH_V3_TEE_IMAGE)
    eMMC_ReadData_MIU(UINT2PTR(header->init_load_addr_lo + u32HeadSize + u32HeadeMMCAlign), u32BodyOtherSizeeMMC, mpart.start_block + u32HeadeMMCBlockNum);
#else
    eMMC_ReadData_MIU(UINT2PTR(header->init_load_addr_lo + u32HeadeMMCAlign), u32BodyOtherSizeeMMC, mpart.start_block + u32HeadeMMCBlockNum);
#endif

    //verify optee body
    {
#ifndef CONFIG_MSTAR_NS_UBOOT_BRINGUP
#if defined(CONFIG_RUN_WITH_V3_TEE_IMAGE)
        int auth_ok = MDrv_AESDMA_SecureMain_v2(header->init_load_addr_lo, header->init_size + u32BodyAlign + u32HeadSize, ((_SUB_SECURE_INFO*)(header->init_load_addr_lo + header->init_size + u32BodyAlign + u32HeadSize))->u8Signature, _gTEEKeybank.u8RSATEEKey);
#else
        int auth_ok = MDrv_AESDMA_SecureMain_v2(header->init_load_addr_lo, header->init_size + u32BodyAlign, ((_SUB_SECURE_INFO*)(header->init_load_addr_lo + header->init_size + u32BodyAlign))->u8Signature, _gTEEKeybank.u8RSATEEKey);
#endif
        if (!auth_ok)
        {
            // optee body auth fail
            authFailType = AUTH_OPTEE_FAIL;
            return FALSE;
        }

#if defined(CONFIG_RUN_WITH_V3_TEE_IMAGE)
        _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(header->init_load_addr_lo + u32HeadSize), header->init_size + u32BodyAlign);
        if (MDrv_AESDMA_Decrypt(__BA2PA(header->init_load_addr_lo), header->init_size + u32BodyAlign, _gTEEKeybank.u8AESTEEKey, AESDMA_ENGINE_CBC) == FALSE)
#else
        if (MDrv_AESDMA_Decrypt(__BA2PA(header->init_load_addr_lo), header->init_size + u32BodyAlign, _gTEEKeybank.u8AESTEEKey, AESDMA_ENGINE_ECB) == FALSE)
#endif
        {
            // optee body decrypt fail
            authFailType = AUTH_OPTEE_FAIL;
            return FALSE;
        }
#endif
        MDrv_SEAL_SetSecureRange(0, __BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_SECURE_RW);

#ifndef CONFIG_MSTAR_NS_UBOOT_BRINGUP

    #if 0 // no need to do this AMZN
    #if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
    // config data anti-rollback
    {
        unsigned char buf[0x100];
        struct rpmb_fs_partition *ptr;

        eMMC_RPMB_Read_data(buf, 0x100, 0);
        ptr = (struct rpmb_fs_partition*)buf;

        if ( (ptr->optee_config_version != 0xffffffffU) && (ptr->optee_config_version > opteeConfigHeader->ConfigVersion) )
        {
            authFailType = AUTH_OPTEE_FAIL;
            return FALSE;
        }
        else
        {
            ptr->optee_config_version = opteeConfigHeader->ConfigVersion;
            eMMC_RPMB_Write_data(buf, 0x100, 0);
        }
   }
   #endif
   #endif // no need to do this AMZN

        // parsing config data
        OPTEE_CONFIG_DESCRIPTOR* desc = (OPTEE_CONFIG_DESCRIPTOR*)(header->init_load_addr_lo + header->init_size + ALIGN(16, header->init_size) );
        unsigned char* dataOffset = header->init_load_addr_lo + header->init_size + ALIGN(16, header->init_size) + opteeConfigHeader->NumberOfConfig*sizeof(OPTEE_CONFIG_DESCRIPTOR);

        int i;
        for (i=0; i<opteeConfigHeader->NumberOfConfig; i++, desc++)
        {
            if (desc->ConfigID==1) // mmap
            {
                OPTEE_CONFIG_MMAP_HEADER* mmapHeader = (OPTEE_CONFIG_MMAP_HEADER*)(dataOffset + desc->Offset);
                ST_MMAP_GROUP_ITEM* groupItem = (ST_MMAP_GROUP_ITEM*)(dataOffset + desc->Offset + sizeof(OPTEE_CONFIG_MMAP_HEADER));
                ST_MMAP_DB_ITEM* dbItem = (ST_MMAP_DB_ITEM*)(dataOffset + desc->Offset + sizeof(OPTEE_CONFIG_MMAP_HEADER) + mmapHeader->NumberOfSecureGroup*sizeof(ST_MMAP_GROUP_ITEM));
                int j;

                // move data to HW AES BUF for secure OS
                if (dbItem[E_MMAP_ID_HW_AES_BUF].index != 0)
                {
                    unsigned char* dst;
                    unsigned char* src;

                    // get HW AES buffer information
                    groupItem = groupItem + dbItem[E_MMAP_ID_HW_AES_BUF].index - 1;

#ifdef CONFIG_SECURITY_BOOT
                    MDrv_SEAL_SetSecureRange(0x13, groupItem->addr, groupItem->addr + groupItem->length, SEAL_ATTR_ALL_RW);
#else
                    MDrv_SEAL_SetSecureRange(0, groupItem->addr, groupItem->addr + groupItem->length, SEAL_ATTR_ALL_RW);
#endif

                    // copy customer keybank
                    dst = (unsigned char*)UINT2PTR(__PA2BA(groupItem->addr));
                    src = (unsigned char*)&_gKeybank;
                    _memcpy(dst, src, sizeof(CUSTOMER_KEY_BANK));

                    // copy mmapDB.ini for backward-compatible
                    dst += 0x1000;
                    src = (unsigned char*)(dataOffset + desc->Offset);
                    _memcpy(dst, src, 0x1000);

                    // copy optee config header
                    dst += 0x1000;
                    src = (unsigned char*)(tmpBuf+CONFIG_OPTEE_HEADER_LEN);
                    _memcpy(dst, src, sizeof(OPTEE_CONFIG_HEADER));

                    // copy optee config decriptor and config data
                    dst += sizeof(OPTEE_CONFIG_HEADER);
                    src = (unsigned char*)(header->init_load_addr_lo + header->init_size + ALIGN(16, header->init_size));
                    _memcpy(dst, src, opteeConfigHeader->NumberOfConfig*sizeof(OPTEE_CONFIG_DESCRIPTOR) + opteeConfigHeader->TotalConfigSize);

#ifdef CONFIG_SECURITY_BOOT
                    MDrv_SEAL_SetSecureRange(0x13, groupItem->addr, groupItem->addr + groupItem->length, SEAL_ATTR_SECURE_RW);
#else
                    MDrv_SEAL_SetSecureRange(0, groupItem->addr, groupItem->addr + groupItem->length, SEAL_ATTR_SECURE_RW);
#endif
                }

#if 0 // reserve for future use
                groupItem = (ST_MMAP_GROUP_ITEM*)(dataOffset + desc->Offset + sizeof(OPTEE_CONFIG_MMAP_HEADER));

                for (j=0; j<mmapHeader->NumberOfSecureGroup; j++, groupItem++)
                {
                    if (groupItem->length != 0)
                    {
                        MDrv_SEAL_SetSecureRange(0, groupItem->addr, groupItem->addr + groupItem->length, SEAL_ATTR_SECURE_RW);
                    }
                }
#endif
            }
            else if (desc->ConfigID==2) // secure MIU host
            {
                unsigned char* data = (unsigned char*)(dataOffset + desc->Offset);
                unsigned int size = desc->ConfigSize;
                int i, j;

                for (i=0; i<size; i++)
                {
                    for (j=0; j<8; j++)
                    {
                        if ( (*(data+i)) & (1<<j) )
                        {
                            MDrv_SEAL_SetMIUHost(i*8+j, 1);
                        }
                    }
                }
            }
            else if (desc->ConfigID==3) // secure RIU bank
            {
                unsigned char* data = (unsigned char*)(dataOffset + desc->Offset);
                unsigned int size = desc->ConfigSize;
                int i, j;

                for (i=0; i<size; i++)
                {
                    for (j=0; j<8; j++)
                    {
                        if ( (*(data+i)) & (1<<j) )
                        {
                            MDrv_SEAL_SetRIUBank(i*8+j, 1);
                        }
                    }
                }
            }
            else if (desc->ConfigID==4) // nonsecure processor, useless because all RIU hosts are set to non-secure in init stage
            {
//                unsigned char* data = (unsigned char*)(dataOffset + desc->Offset);
//                unsigned int size = desc->ConfigSize;
//                int i, j;
//
//                LDR_PUTS("SProc:");LDR_PUTS("\n");
//                for (i=0; i<size; i++)
//                {
//                    for (j=0; j<8; j++)
//                    {
//                        if ( (*(data+i)) & (1<<j) )
//                        {
//                            LDR_PUTS("\t");LDR_PUTDW(i*8 + j);LDR_PUTS("\n");
//                        }
//                    }
//                }
            }
            else if (desc->ConfigID==5) // secure RIU register
            {
                unsigned char* data = (unsigned char*)(dataOffset + desc->Offset);
                unsigned int size = desc->ConfigSize;
                int i, j;

                for (i=0; i<size; i++)
                {
                    for (j=0; j<8; j++)
                    {
                        if ( (*(data+i)) & (1<<j) )
                        {
                            MDrv_SEAL_SetRIURegister(i*8+j, 1);
                        }
                    }
                }
            }
        }
#endif
    }


    return header->init_load_addr_lo;
}

unsigned int ns_uboot_load_armfw(void)
{
    mstar_tee_t* header = NULL;
    emmc_partition_t mpart;
    int auth_ok;
    int encrypted = 0;
    int alignment;
#if defined(CONFIG_MSTAR_NS_UBOOT_BRINGUP)
    unsigned int u32HeadSize = CONFIG_OPTEE_HEADER_LEN;
#else
    unsigned int u32HeadSize = CONFIG_OPTEE_HEADER_LEN + sizeof(_SUB_SECURE_INFO);
#endif
    unsigned int u32HeadeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, u32HeadSize);
    unsigned int u32HeadeMMCBlockNum = (u32HeadSize + u32HeadeMMCAlign) / EMMC_BLOCK_SIZE;
    U8* tmpBuf = (U8*)u8TmpBuf;

    if (ns_uboot_load_partition(5, "armfw", &header, &mpart))
    {
        eMMC_ReadData_MIU(tmpBuf, u32HeadSize + u32HeadeMMCAlign, mpart.start_block);
        // verify header
        {
#ifndef CONFIG_MSTAR_NS_UBOOT_BRINGUP
            int auth_ok = MDrv_AESDMA_SecureMain_v2(tmpBuf, CONFIG_OPTEE_HEADER_LEN, ((_SUB_SECURE_INFO*)(tmpBuf+CONFIG_OPTEE_HEADER_LEN))->u8Signature, _gTEEKeybank.u8RSATEEKey);
            if (!auth_ok)
            {
                header = NULL;
                authFailType = AUTH_ARMFW_FAIL;
                return 0;
            }
            else
#endif
            {
                header = (mstar_tee_t*)tmpBuf;
            }
        }
    }
    else
    {
        // load partition failed
        authFailType = AUTH_ARMFW_FAIL;
        return 0;
    }

    Fireware_entry = header->init_load_addr_lo;
    _gArmFwVersion = header->cus_version;

    // STR mode depends on armfw header
    _gStrMode = ( header->warmboot_mode & STR_MODE_MASK );
    _gAuthMode = ( header->warmboot_mode & AUTH_MODE_MASK );


    if ( _gIsStr && _gStrMode )
    {
        MDrv_SEAL_SetSecureRange(1, __BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_SECURE_RW);

        if ( _gAuthMode == AESCBC_MAC )
        {
            int i;
            unsigned int result;
            warmboot_table* tbl;

            if ( MDrv_SYS_Query(E_SYS_QUERY_MOBF_KEY_SAVED, &result) == FALSE )
            {
                authFailType = AUTH_ARMFW_FAIL;
                return FALSE;
            }

            if ( result ) // support mobf
            {
                do
                {
                    if ( MDrv_SYS_Query(E_SYS_QUERY_MOBF_ENABLED, &result) == FALSE )
                    {
                        authFailType = AUTH_ARMFW_FAIL;
                        return FALSE;
                    }

                    if ( result ) // MOBF init done
                    {
                        break;
                    }
                } while ( 1 );
            }

            if ( MDrv_SYS_Query(E_SYS_QUERY_TRNG_KEY_SAVED, &result) == FALSE )
            {
                authFailType = AUTH_ARMFW_FAIL;
                return FALSE;
            }

            if ( !result ) // no TRNG key, CBC-MAC check fail
            {
                authFailType = AUTH_ARMFW_FAIL;
                return FALSE;
            }

            // calculate CBC-MAC with TRNG key
            tbl = (warmboot_table*)(header->secure_range_end - sizeof(warmboot_table));
            MDrv_AESDMA_STR_CBCMAC(__BA2PA(tbl->ATFBase), tbl->ATFSize, __BA2PA(u8ATF_MAC));

            for ( i=0 ; i<16 ; i++ )
            {
                if ( u8ATF_MAC[i] != tbl->ATFMAC[i] )
                {
                    authFailType = AUTH_ARMFW_FAIL;
                    return FALSE;
                }
            }
        }

        return header->init_load_addr_lo;
    }

    MDrv_SEAL_SetSecureRange(1, __BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_ALL_RW);

#if defined(CONFIG_RUN_WITH_V3_TEE_IMAGE)
    // copy header part to load address
    _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(tmpBuf), u32HeadSize + u32HeadeMMCAlign);
#else
    // copy body data that in the same emmc block with head data
    if (u32HeadeMMCAlign != 0)
    {
        _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(tmpBuf + u32HeadSize), u32HeadeMMCAlign);
    }
#endif

    // read other body data
    unsigned int u32BodyAlign = ALIGN(16,header->init_size); // Body dummy data

#if defined(CONFIG_MSTAR_NS_UBOOT_BRINGUP)
    unsigned int u32BodyeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, header->init_size + u32BodyAlign - u32HeadeMMCAlign);
    unsigned int u32BodyOtherSizeeMMC = header->init_size + u32BodyAlign - u32HeadeMMCAlign + u32BodyeMMCAlign;
#else
    unsigned int u32BodyeMMCAlign = ALIGN(EMMC_BLOCK_SIZE, header->init_size + u32BodyAlign + sizeof(_SUB_SECURE_INFO) - u32HeadeMMCAlign);
    unsigned int u32BodyOtherSizeeMMC = header->init_size + u32BodyAlign + sizeof(_SUB_SECURE_INFO) - u32HeadeMMCAlign + u32BodyeMMCAlign;
#endif

#if defined(CONFIG_RUN_WITH_V3_TEE_IMAGE)
    eMMC_ReadData_MIU(UINT2PTR(header->init_load_addr_lo + u32HeadSize + u32HeadeMMCAlign), u32BodyOtherSizeeMMC, mpart.start_block + u32HeadeMMCBlockNum);
#else
    eMMC_ReadData_MIU(UINT2PTR(header->init_load_addr_lo + u32HeadeMMCAlign), u32BodyOtherSizeeMMC, mpart.start_block + u32HeadeMMCBlockNum);
#endif
    // verify body
    {
#ifndef CONFIG_MSTAR_NS_UBOOT_BRINGUP
#if defined(CONFIG_RUN_WITH_V3_TEE_IMAGE)
        int auth_ok = MDrv_AESDMA_SecureMain_v2(header->init_load_addr_lo, header->init_size + u32BodyAlign + u32HeadSize, ((_SUB_SECURE_INFO*)(header->init_load_addr_lo + header->init_size + u32BodyAlign + u32HeadSize))->u8Signature, _gTEEKeybank.u8RSATEEKey);
#else
        int auth_ok = MDrv_AESDMA_SecureMain_v2(header->init_load_addr_lo, header->init_size + u32BodyAlign, ((_SUB_SECURE_INFO*)(header->init_load_addr_lo + header->init_size + u32BodyAlign))->u8Signature, _gTEEKeybank.u8RSATEEKey);
#endif
        if (!auth_ok)
        {
            // armfw body auth fail
            authFailType = AUTH_ARMFW_FAIL;
            return FALSE;
        }


#if defined(CONFIG_RUN_WITH_V3_TEE_IMAGE)
        _memcpy(UINT2PTR(header->init_load_addr_lo), UINT2PTR(header->init_load_addr_lo + u32HeadSize), header->init_size + u32BodyAlign);

        if (MDrv_AESDMA_Decrypt(__BA2PA(header->init_load_addr_lo), header->init_size + u32BodyAlign, _gTEEKeybank.u8AESTEEKey, AESDMA_ENGINE_CBC) == FALSE)
#else
        if (MDrv_AESDMA_Decrypt(__BA2PA(header->init_load_addr_lo), header->init_size + u32BodyAlign, _gTEEKeybank.u8AESTEEKey, AESDMA_ENGINE_ECB) == FALSE)
#endif
        {
            // armfw body decrypt fail
            authFailType = AUTH_ARMFW_FAIL;
            return FALSE;
        }
#endif
        MDrv_SEAL_SetSecureRange(1, __BA2PA(header->init_load_addr_lo), __BA2PA(header->secure_range_end), SEAL_ATTR_SECURE_RW);
    }

    return header->init_load_addr_lo;
}

int ns_uboot_load_partition(unsigned int len, char* name, mstar_tee_t** header, emmc_partition_t* mpart)
{
    int u32_i, u32_j;
    int found;
    U8* tmpBuf = (U8*)u8TmpBuf;

    // iterate to find specific partition
    for(u32_i=0;u32_i<= EMMC_RESERVED_FOR_MAP_V2;u32_i++)
    {
        if(eMMC_ReadData_MIU((U8*)mpart, EMMC_BLOCK_SIZE, u32_i))
            break; // emmc read fail

        found = 1;

        for (u32_j=0; u32_j<len; u32_j++)
        {
            if (mpart->name[u32_j]!=name[u32_j])
            {
                found = 0;
                break;
            }
        }

        if (found)
        {
            eMMC_ReadData_MIU(tmpBuf, EMMC_BLOCK_SIZE, mpart->start_block);
            *header = (mstar_tee_t*)tmpBuf;
            return 1;
        }
    }

    return 0;
}

void to_hex_string(U8* src, char* dst, unsigned int len)
{
    static char hex_tbl[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int i, j;
    U8 val;
    for (i=0, j=0; i<len; ++i, j+=2)
    {
        val = src[i];
        dst[j] = hex_tbl[((val>>4) & 0x0F)];
        dst[j+1] = hex_tbl[(val & 0x0F)];
    }
}

static mstar_boot_prameters_t boot;

void ns_uboot_boot(unsigned int uboot_addr)
{
    mstar_boot_prameters_t* bootp = &boot;

    switch(authFailType)
    {
        case AUTH_KEYBANK_FAIL:
            ns_uboot_print("\nAUTH_KEYBANK_FAIL\n");
            ns_uboot_nonsecure_handler();
            return;
            break;
        case AUTH_ARMFW_FAIL:
            ns_uboot_print("\nAUTH_ARMFW_FAIL\n");
            ns_uboot_nonsecure_handler();
            return;
            break;
        case AUTH_OPTEE_FAIL:
            ns_uboot_print("\nAUTH_OPTEE_FAIL\n");
            ns_uboot_nonsecure_handler();
            return;
            break;
        default:
            break;
    }

    if ( _gIsStr && _gStrMode )
    {
        __asm__ __volatile__(
        ".arch_extension sec\n\t"
        "ldr  r1,[%0]  \n\t"
        "mov  r0,%1  \n\t"
        "smc #0  \n\t"
        :
        :"r"(&Fireware_entry),"r"(&bootp)
        :"r0","r1"
        );
    }

    bootp->BIN0_entry_point = BIN0_entry_point;
	bootp->BIN0_RW64 = MODE_RW_32;
	bootp->BIN1_entry_point = uboot_addr;
	bootp->BIN1_RW64 = MODE_RW_32;
#ifdef CONFIG_ARMv8_64BIT_KERNEL
	bootp->BIN2_RW64 = MODE_RW_64;
#else
	bootp->BIN2_RW64 = MODE_RW_32;
#endif

    { // construct OPTEE boot args
        U8 emmc_CID[16];
        emmc_CID[15] = 0;
        eMMC_GetCID(emmc_CID);

        _gOpteeBootArgs[0] = 'C';
        _gOpteeBootArgs[1] = 'I';
        _gOpteeBootArgs[2] = 'D';
        _gOpteeBootArgs[3] = '=';
        to_hex_string(emmc_CID, _gOpteeBootArgs+4, 15);
        _gOpteeBootArgs[34] = '\0';

        bootp->BIN0_bootargs = _gOpteeBootArgs;
    }

	__asm__ __volatile__(
        ".arch_extension sec\n\t"
		"ldr  r1,[%0]  \n\t"
		"mov  r0,%1  \n\t"
		"smc #0  \n\t"
		:
		:"r"(&Fireware_entry),"r"(bootp)
		:"r0","r1"
	);
}


unsigned int ns_uboot_read_keybank(unsigned int keybank_rom_offset)
{
    int i, auth_ok = 0, encrypted = 0, Keybank_encrypt_enable = 0;
    U8* tmpBuf = (U8*)u8TmpBuf;
    U8* bakBuf = (U8*)(u8TmpBuf+512*3);
    _gKeybankRomOffset = keybank_rom_offset;
#ifdef CONFIG_DOUBLE_MBOOT
    U32 mbootEmmcBase = IS_MBOOTBAK() ? 0x8000 : 0x1000;
#else
    U32 mbootEmmcBase = 0x1000;
#endif
    {
        // read customer keybank
        if (eMMC_ReadData_MIU((U8*)tmpBuf, 512 * 3, (_gKeybankRomOffset>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return 0; // emmc read fail
        }

        // read keybank backup
        if (eMMC_ReadData_MIU((U8*)bakBuf, 512 * 3, ((_gKeybankRomOffset+0x1000)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return 0; // emmc read fail
        }
    }
    _memcpy(&_gKeybank,tmpBuf,sizeof(CUSTOMER_KEY_BANK));

/*
    The flow below is ported from MstarApp/src/secure/MsSecureBoot.c - _LoadCustomerKeyBank()
*/
#if !defined (CONFIG_SECURE_BY_PASS_VERIFICATION)
    MDrv_AESDMA_Encrypt(__BA2PA((unsigned int)puCommonTaString), AES_KEY_LEN, NULL);
    array_reverse((char*)puCommonTaString, AES_KEY_LEN);

    for (i=0; i<16; ++i)
    {
        if (_gKeybank.u8MagicID[i] != u8CusKeyMagicID[i])
        {
            encrypted = 1;
        }
    }

    if (!encrypted)
    {
        // encrypt customer keybank and write-back to flash
        /*
            Below condition is ported from MDrv_SYS_Query(E_SYS_QUERY_SECURED_IC_SUPPORTED);
            Do we need a SYS driver in sboot?
        */
        if( IS_SECURE_CHIP() )
        {
            MDrv_AESDMA_Encrypt(__BA2PA((unsigned int)tmpBuf), sizeof(CUSTOMER_KEY_BANK), puCommonTaString);
        }
        else // Not Secure IC
        {
            MDrv_AESDMA_Encrypt(__BA2PA((unsigned int)tmpBuf), sizeof(CUSTOMER_KEY_BANK), aesBootKey);
        }

        {
            // write back encrypted customer keybank
            if (eMMC_WriteData_MIU((U8*)tmpBuf, 512 * 3, (_gKeybankRomOffset>>9) + mbootEmmcBase ))
            {
                authFailType = AUTH_KEYBANK_FAIL;
                return 0; // emmc write fail
            }

            // write back encrypted customer keybank backup
            _memcpy(bakBuf, tmpBuf, sizeof(CUSTOMER_KEY_BANK));
            if (eMMC_WriteData_MIU((U8*)bakBuf, 512 * 3, ((_gKeybankRomOffset+0x1000)>>9) + mbootEmmcBase ))
            {
                authFailType = AUTH_KEYBANK_FAIL;
                return 0; // emmc write fail
            }
        }
    }
    else
    {
        /*
            Below condition is ported from MDrv_SYS_Query(E_SYS_QUERY_SECURED_IC_SUPPORTED);
            Do we need a SYS driver in sboot?
        */
        if( IS_SECURE_CHIP() )
        {
            MDrv_AESDMA_Decrypt(__BA2PA((unsigned int)&_gKeybank), sizeof(CUSTOMER_KEY_BANK), puCommonTaString, AESDMA_ENGINE_ECB);
        }
        else // Not Secure IC
        {
            MDrv_AESDMA_Decrypt(__BA2PA((unsigned int)&_gKeybank), sizeof(CUSTOMER_KEY_BANK), aesBootKey, AESDMA_ENGINE_ECB);
        }
    }
#endif
    auth_ok = MDrv_AESDMA_SecureMain(&_gKeybank.u8RSABootPublicKey, sizeof(CUSTOMER_KEY_BANK)-sizeof(_SUB_SECURE_INFO), &_gKeybank.customer.u8Signature, 0x0);
    if (!auth_ok)
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return 0;
    }

    _memcpy(cus_aesBootKey, &_gKeybank.u8AESBootKey, AES_KEY_LEN);
    array_reverse(cus_aesBootKey, AES_KEY_LEN);

    //Copy and Auth TEE keybank
    {
        if (eMMC_ReadData_MIU(tmpBuf, 0x1000, ((_gKeybankRomOffset + TEEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return 0; // emmc read fail
        }
    }
    //Trying to EFuse key to decrypt TEEKeybank
    Keybank_encrypt_enable = 0;
    if (MDrv_AESDMA_Decrypt(__BA2PA(tmpBuf), 0x1000, NULL, AESDMA_ENGINE_ECB) == FALSE)
    {
            // optee body decrypt fail
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
    }
    //Check if TEEKeybank's magicID
    for (i=0; i<8; i++)
    {
        if (tmpBuf[i] != u8SecurityIdentify[i])
        {
            //Use EmbeddedAESTEEUniformKey instead
            //Copy and Auth TEE keybank again
            {
                if (eMMC_ReadData_MIU(tmpBuf, 0x1000, ((_gKeybankRomOffset + TEEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
                {
                    authFailType = AUTH_KEYBANK_FAIL;
                    return 0; // emmc read fail
                }
            }
            array_reverse(EmbeddedAESTEEUniformKey, AES_KEY_LEN);
            if (MDrv_AESDMA_Decrypt(__BA2PA(tmpBuf), 0x1000, cus_aesBootKey, AESDMA_ENGINE_ECB) == FALSE)
            {
                // optee body decrypt fail
                authFailType = AUTH_KEYBANK_FAIL;
                return FALSE;
            }
            Keybank_encrypt_enable = 1;
            break;
        }
    }
    //
    auth_ok = MDrv_AESDMA_SecureMain_v2((U8*)((TEE_KEY_BANK*)(tmpBuf))->u8MSID, sizeof(TEE_KEY_BANK)-sizeof(_SUB_SECURE_INFO), (U8*)((TEE_KEY_BANK*)(tmpBuf))->SecureInfo.u8Signature, _gKeybank.u8RSABootPublicKey);
    if (!auth_ok)
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return 0;
    }
    _memcpy(&_gTEEKeybank,tmpBuf,sizeof(_gTEEKeybank));
    array_reverse(_gTEEKeybank.u8AESTEEKey, AES_KEY_LEN);

    //Re-encrypt TEEKeybank using EFuse Key
    if(Keybank_encrypt_enable)
    {
        if( MDrv_AESDMA_Encrypt(__BA2PA(tmpBuf), 0x1000, NULL) == FALSE )
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }

        {
            if (eMMC_WriteData_MIU(tmpBuf, 0x1000, ((_gKeybankRomOffset + TEEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
            {
                authFailType = AUTH_KEYBANK_FAIL;
                return 0; // emmc read fail
            }
        }
    }

#ifdef CONFIG_TEE_LOADER
    //Copy and Auth REE keybank
    {
        if (eMMC_ReadData_MIU(tmpBuf, 0x1000, ((_gKeybankRomOffset + REEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return 0; // emmc read fail
        }
    }
    //Trying to EFuse key to decrypt REEKeybank
    Keybank_encrypt_enable = 0;
    if (MDrv_AESDMA_Decrypt(__BA2PA(tmpBuf), 0x1000, NULL, AESDMA_ENGINE_ECB) == FALSE)
    {
            // optee body decrypt fail
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
    }
    //Check if REEKeybank's magicID
    for (i=0; i<8; i++)
    {
        if (tmpBuf[i] != u8SecurityIdentify[i])
        {
            //Use EmbeddedAESREEUniformKey instead
            //Copy and Auth REE keybank again
            {
                if (eMMC_ReadData_MIU(tmpBuf, 0x1000, ((_gKeybankRomOffset + REEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
                {
                    authFailType = AUTH_KEYBANK_FAIL;
                    return 0; // emmc read fail
                }
            }
            array_reverse(EmbeddedAESREEUniformKey, AES_KEY_LEN);
            if (MDrv_AESDMA_Decrypt(__BA2PA(tmpBuf), 0x1000, EmbeddedAESREEUniformKey, AESDMA_ENGINE_ECB) == FALSE)
            {
                // optee body decrypt fail
                authFailType = AUTH_KEYBANK_FAIL;
                return FALSE;
            }
            Keybank_encrypt_enable = 1;
            break;
        }
    }
    //
    auth_ok = MDrv_AESDMA_SecureMain_v2((U8*)((REE_KEY_BANK*)(tmpBuf))->u8RSAKey, sizeof(REE_KEY_BANK)-sizeof(_SUB_SECURE_INFO), (U8*)((REE_KEY_BANK*)(tmpBuf))->SecureInfo.u8Signature, EmbeddedReeRsaPub);
    if (!auth_ok)
    {
        authFailType = AUTH_KEYBANK_FAIL;
        return 0;
    }
    _memcpy(&_gREEKeybank,tmpBuf,sizeof(_gREEKeybank));
    array_reverse(_gREEKeybank.u8AESKey, AES_KEY_LEN);

    //Re-encrypt REEKeybank using EFuse Key
    if(Keybank_encrypt_enable)
    {
        if( MDrv_AESDMA_Encrypt(__BA2PA(tmpBuf), 0x1000, NULL) == FALSE )
        {
            authFailType = AUTH_KEYBANK_FAIL;
            return FALSE;
        }

        {
            if (eMMC_WriteData_MIU(tmpBuf, 0x1000, ((_gKeybankRomOffset + REEKEYBANK_OFFSET)>>9) + mbootEmmcBase ))
            {
                authFailType = AUTH_KEYBANK_FAIL;
                return 0; // emmc read fail
            }
        }
    }
#endif

    return 1;
}

unsigned int ns_uboot_get_optee_addr(void)
{
    return BIN0_entry_point;
}

unsigned int ns_uboot_nonsecure_handler(void)
{
    // set hosts of PM RIU bridge to non-secure
    // set hosts of NONPM RIU bridge to non-secure (except ARM)
    // set hosts of SEC RIU bridge to non-secure
    // set ARM to non-secure mode (must be the last step of this stage)
    MDrv_SEAL_DisableRIUBridges();

    return 1;
}

void ns_uboot_init(void)
{
    _gIsStr = ns_uboot_is_str() ? 1 : 0;
    _gStrMode = 0;

#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
    /* no need to do this AMZN */
    //eMMC_RPMB_Check_Program_Key();
#endif
    MDrv_SEAL_Init();
}

#ifdef CONFIG_TEE_LOADER
unsigned int ns_uboot_SecureMain(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U8 KeySel)
{
    unsigned int auth_ok;

    if (KeySel == 0x0)
        auth_ok = MDrv_AESDMA_SecureMain_v2(u32PlaintextAddr, u32Size,
                                 u32SignatureAddr, _gTEEKeybank.u8RSATEEKey);
    else
        auth_ok = MDrv_AESDMA_SecureMain_v2(u32PlaintextAddr, u32Size,
                                 u32SignatureAddr, _gREEKeybank.u8RSAKey);

    return auth_ok;
}

void ns_uboot_copy_ReeKeybank(U32 addr)
{
    memcpy(addr, _gREEKeybank.u8RSAKey, RSA_PUBLIC_KEY_LEN + AES_KEY_LEN);
}
#endif
