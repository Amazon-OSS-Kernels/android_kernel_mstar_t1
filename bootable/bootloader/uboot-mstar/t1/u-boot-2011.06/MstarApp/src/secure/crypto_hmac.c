/*
 * $Log: hmac_sha256.c,v $
 * Revision 2.1  2011-12-10 15:13:38+05:30  Cprogrammer
 * hmac_sha256() function
 *
 */
 #define CRYPTO_HMAC_C

#include <secure/crypto_hmac.h>
#include <secure/crypto_sha.h>
#include <secure/crypto_aes.h>
#include <secure/MsSecureCommon.h>
#include <common.h>
#include <MsSystem.h>
#include <MsString.h>
#include <MsDebug.h>

#define SHA256_DIGEST_LENGTH 32
#define HMAC_KEY_LENGTH 32
#define HMAC_DRM_ID "MStarHmacKey"
#define DRM_ID_LEN 16


void
hmac_sha256(
    const unsigned char *text,      /* pointer to data stream        */
    int                 text_len,   /* length of data stream         */
    const unsigned char *key,       /* pointer to authentication key */
    int                 key_len,    /* length of authentication key  */
    void                *digest)    /* caller digest to be filled in */
{
    unsigned char k_ipad[65];   /* inner padding -
                                 * key XORd with ipad
                                 */
    unsigned char k_opad[65];   /* outer padding -
                                 * key XORd with opad
                                 */
    unsigned char tk[SHA256_DIGEST_LENGTH];
    unsigned char tk2[SHA256_DIGEST_LENGTH];
    unsigned char bufferIn[1024];
    unsigned char bufferOut[1024];
    int           i;

    /* if key is longer than 64 bytes reset it to key=sha256(key) */
    if ( key_len > 64 ) {
#ifdef MSTAR_SHA256_QUIRK
        mstar_sha256( key, key_len, tk );
#else
        sha256( key, key_len, tk );
#endif
        key     = tk;
        key_len = SHA256_DIGEST_LENGTH;
    }

    /*
     * the HMAC_SHA256 transform looks like:
     *
     * SHA256(K XOR opad, SHA256(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */

    /* start out by storing key in pads */
    memset( k_ipad, 0, sizeof k_ipad );
    memset( k_opad, 0, sizeof k_opad );
    memcpy( k_ipad, key, key_len );
    memcpy( k_opad, key, key_len );

    /* XOR key with ipad and opad values */
    for ( i = 0; i < 64; i++ ) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    /*
     * perform inner SHA256
     */
    memset( bufferIn, 0x00, 1024 );
    memcpy( bufferIn, k_ipad, 64 );
    memcpy( bufferIn + 64, text, text_len );

#ifdef MSTAR_SHA256_QUIRK
    mstar_sha256( bufferIn, 64 + text_len, tk2 );
#else
    sha256( bufferIn, 64 + text_len, tk2 );
#endif

    /*
     * perform outer SHA256
     */
    memset( bufferOut, 0x00, 1024 );
    memcpy( bufferOut, k_opad, 64 );
    memcpy( bufferOut + 64, tk2, SHA256_DIGEST_LENGTH );

#ifdef MSTAR_SHA256_QUIRK
    mstar_sha256( bufferOut, 64 + SHA256_DIGEST_LENGTH, digest );
#else
    sha256( bufferOut, 64 + SHA256_DIGEST_LENGTH, digest );
#endif

}

int GetHmacKey(unsigned char *u8pHmacKey)
{
    unsigned char VirtAddr[DRM_ID_LEN]={0};
    unsigned char u8Key[AES_KEY_LEN];

    UBOOT_TRACE("IN\n");
    if(strlen(HMAC_DRM_ID)<sizeof(VirtAddr))
    {
        memcpy(VirtAddr, HMAC_DRM_ID, strlen(HMAC_DRM_ID));
    }
    else
    {
        memcpy(VirtAddr, HMAC_DRM_ID, sizeof(VirtAddr));
    }

    UBOOT_DEBUG("HMAC_DRM_ID\n\n");
    UBOOT_DUMP((U32)VirtAddr, DRM_ID_LEN);

    Secure_Get_DeviceKey(u8Key);
    Secure_AES_ECB_Encrypt((U32)VirtAddr,DRM_ID_LEN,u8Key);

    memcpy(u8pHmacKey,(void*)VirtAddr, DRM_ID_LEN);
    array_reverse((char*)VirtAddr,DRM_ID_LEN);
    memcpy(&u8pHmacKey[DRM_ID_LEN],(void*)VirtAddr, DRM_ID_LEN);

    UBOOT_DEBUG("HmacKey\n\n");
    UBOOT_DUMP((U32)u8pHmacKey, HMAC_KEY_LEN);

    UBOOT_TRACE("OK\n");
    return TRUE;
}
#undef CRYPTO_HMAC_C
