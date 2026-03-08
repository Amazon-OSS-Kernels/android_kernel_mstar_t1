/**
* Copyright (c) 2006 ?2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef _DRV_AESDMA_H_
#define _DRV_AESDMA_H_

#ifdef CONFIG_MSTAR_EDISON
#define AESDMA_HAL_EDISON       1
#endif

#ifdef CONFIG_MSTAR_EIFFEL
#define AESDMA_HAL_EIFFEL       1
#endif

#ifdef CONFIG_MSTAR_NIKE
#define AESDMA_HAL_NIKE         1
#endif

#ifdef CONFIG_MSTAR_MADISON
#define AESDMA_HAL_MADISON      1
#endif

#ifdef CONFIG_MSTAR_CLIPPERS
#define AESDMA_HAL_CLIPPERS     1
#endif

#ifdef CONFIG_MSTAR_MIAMI
#define AESDMA_HAL_MIAMI        1
#endif

#ifdef CONFIG_MSTAR_MONACO
#define AESDMA_HAL_MONACO       1
#endif

#ifdef CONFIG_MSTAR_MUJI
#define AESDMA_HAL_MUJI       1
#endif

#ifdef CONFIG_MSTAR_MANHATTAN
#define AESDMA_HAL_MANHATTAN     1
#endif

#ifdef CONFIG_MSTAR_MASERATI
#define AESDMA_HAL_MASERATI      1
#endif

#ifdef CONFIG_MSTAR_CELTICS
#define AESDMA_HAL_CELTICS       1
#endif

#ifdef CONFIG_MSTAR_MACAN
#define AESDMA_HAL_MACAN        1
#endif

#ifdef CONFIG_MSTAR_MUSTANG
#define AESDMA_HAL_MUSTANG        1
#endif

#ifdef CONFIG_MSTAR_NADAL
#define AESDMA_HAL_NADAL        1
#endif

#ifdef CONFIG_MSTAR_NUGGET
#define AESDMA_HAL_NUGGET       1
#endif

#ifdef CONFIG_MSTAR_EINSTEIN
#define AESDMA_HAL_EINSTEIN     1
#endif

#ifdef CONFIG_MSTAR_EINSTEIN3
#define AESDMA_HAL_EINSTEIN3    1
#endif

#ifdef CONFIG_MSTAR_NAPOLI
#define AESDMA_HAL_NAPOLI       1
#endif

#ifdef CONFIG_MSTAR_MONET
#define AESDMA_HAL_MONET        1
#endif

#ifdef CONFIG_MSTAR_MESSI
#define AESDMA_HAL_MESSI        1
#endif

#ifdef CONFIG_MSTAR_MUNICH
#define AESDMA_HAL_MUNICH       1
#endif


//=====================================================
// do NOT edit the following content.
//=====================================================
#if defined(AESDMA_HAL_EDISON) && AESDMA_HAL_EDISON
  #include "hal/edison/halAESDMA.h"
#elif defined(AESDMA_HAL_EIFFEL) && AESDMA_HAL_EIFFEL
  #include "hal/eiffel/halAESDMA.h"
#elif defined(AESDMA_HAL_NIKE) && AESDMA_HAL_NIKE
  #include "hal/nike/halAESDMA.h"
#elif defined(AESDMA_HAL_MADISON) && AESDMA_HAL_MADISON
  #include "hal/madison/halAESDMA.h"
#elif defined(AESDMA_HAL_CLIPPERS) && AESDMA_HAL_CLIPPERS
  #include "hal/clippers/halAESDMA.h"
#elif defined(AESDMA_HAL_MIAMI) && AESDMA_HAL_MIAMI
  #include "hal/miami/halAESDMA.h"
#elif defined(AESDMA_HAL_MONACO) && AESDMA_HAL_MONACO
  #include "hal/monaco/halAESDMA.h"
#elif defined(AESDMA_HAL_MUJI) && AESDMA_HAL_MUJI
  #include "hal/muji/halAESDMA.h"
#elif defined(AESDMA_HAL_CELTICS) && AESDMA_HAL_CELTICS
  #include "hal/celtics/halAESDMA.h"
#elif defined(AESDMA_HAL_NADAL) && AESDMA_HAL_NADAL
    #include "hal/nadal/halAESDMA.h"
#elif defined(AESDMA_HAL_NUGGET) && AESDMA_HAL_NUGGET
  #include "hal/nugget/halAESDMA.h"
#elif defined(AESDMA_HAL_EINSTEIN) && AESDMA_HAL_EINSTEIN
  #include "hal/einstein/halAESDMA.h"
#elif defined(AESDMA_HAL_EINSTEIN3) && AESDMA_HAL_EINSTEIN3
  #include "hal/einstein3/halAESDMA.h"
#elif defined(AESDMA_HAL_NAPOLI) && AESDMA_HAL_NAPOLI
  #include "hal/napoli/halAESDMA.h"
#elif defined(AESDMA_HAL_MONET) && AESDMA_HAL_MONET
  #include "hal/monet/halAESDMA.h"
#elif defined(AESDMA_HAL_MESSI) && AESDMA_HAL_MESSI
  #include "hal/messi/halAESDMA.h"
#elif defined(AESDMA_HAL_MUNICH) && AESDMA_HAL_MUNICH
  #include "hal/munich/halAESDMA.h"
#elif defined(AESDMA_HAL_MANHATTAN) && AESDMA_HAL_MANHATTAN
  #include "hal/manhattan/halAESDMA.h"
#elif defined(AESDMA_HAL_MASERATI) && AESDMA_HAL_MASERATI
  #include "hal/maserati/halAESDMA.h"
#elif defined(AESDMA_HAL_MACAN) && AESDMA_HAL_MACAN
  #include "hal/macan/halAESDMA.h"
#elif defined(AESDMA_HAL_MUSTANG) && AESDMA_HAL_MUSTANG
  #include "hal/mustang/halAESDMA.h"
#else
  #error "Error! no platform selected."
#endif



void MDrv_AESDMA_SecureAuthen(U32 u32SHAInBuf, U32 u32Size, U32 u32RSAInBuf, U32 u32SHAOutBuf, U32 u32RSAOutBuf, U32 u32SramSel);
U32 MDrv_AESDMA_SecureMain(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U32 u32SramSel);

#ifdef CONFIG_MSTAR_NS_UBOOT
U32 MDrv_AESDMA_Decrypt(U32 addr, U32 len, U8* key);
U32 MDrv_AESDMA_Encrypt(U32 addr, U32 len, U8* key);
U32 MDrv_AESDMA_EncDec(U32 addr, U32 len, U8* key, U32 isDec);


#endif

//-------------------------------------------------------------------------------------------------
/// Get HMAC with SHA256
/// @ingroup G_AES_EnDecrypt
/// @param text     \b IN: pointer to data stream
/// @param text_len \b IN: length of data stream
/// @param key      \b IN: pointer to authentication key
/// @param key_len  \b IN: length of authentication key
/// @param digest   \b OUT: caller digest to be filled in
/// @return n/a
//-------------------------------------------------------------------------------------------------
void MDrv_AESDMA_Hmac_sha256(const U8 *text, U32 text_len, const U8 *key, U32 key_len, void *digest);

//-------------------------------------------------------------------------------------------------
/// Get HMAC Key
/// @ingroup G_AES_EnDecrypt
/// @param u32KeyBufPA \b OUT: Physcial address of the key buffer.
/// @param u32key_len  \b IN: length of key (In the case, >=16 bytes is required)
/// @return 0 : Success
//-------------------------------------------------------------------------------------------------
U32 GetHmacKey(U32 u32KeyBufPA, U32 u32key_len);

//-------------------------------------------------------------------------------------------------
/// Get Hash by SHA1/SHA256
/// @ingroup G_AES_EnDecrypt
/// @param u32SrcPA     \b IN: Physcial address of data.
/// @param u32Size      \b IN: length of data
/// @param eMode        \b IN: 1: SHA256, 0:SHA1
/// @param u32ShaOutPA  \b IN: Physcial address of hash
/// @return 0 : Success
//-------------------------------------------------------------------------------------------------
U32 MDrv_AESDMA_GetSHA (U32 u32SrcPA, U32 u32Size, U8 eMode, U32 u32ShaOutPA);

void MDrv_AESDMA_DisableHwKey(void);
#endif
