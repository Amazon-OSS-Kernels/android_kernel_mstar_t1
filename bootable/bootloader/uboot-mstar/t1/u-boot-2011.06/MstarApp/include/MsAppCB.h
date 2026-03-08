/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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
#ifndef __MS_APP_CB_H__
#define __MS_APP_CB_H__
#include <MsSecureCommon.h>


/******************************************************
 * MsUpgrade.c
 *****************************************************/
typedef int (*show_LoadData_cb)(unsigned int);
typedef int (*show_StartUpgrading_cb)(unsigned int);
typedef int (*show_Finish_cb)(unsigned int);
typedef int (*show_Error_cb)(unsigned int);
typedef int (*show_Upgrading_cb)(unsigned int var1,unsigned int var2);
//-------------------------------------------------------------------------------------------------
/// Register the display function in Error phase
/// @param  cb                                  \b IN: call back function for handling in finish phase
//-------------------------------------------------------------------------------------------------
void RegisterCBShowError(show_Error_cb cb);
//-------------------------------------------------------------------------------------------------
/// Register the display function in finish phase
/// @param  cb                                  \b IN: call back function for handling in finish phase
//-------------------------------------------------------------------------------------------------
void RegisterCBShowFinish(show_Finish_cb cb);
//-------------------------------------------------------------------------------------------------
/// Register the display function in load data phase
/// @param  cb                                  \b IN: call back function for handling in load data phase
//-------------------------------------------------------------------------------------------------
void RegisterCBShowLoadData(show_LoadData_cb cb);
//-------------------------------------------------------------------------------------------------
/// Register the display function in the beginning phase
/// @param  cb                                  \b IN: call back function for handling in beginning phase
//-------------------------------------------------------------------------------------------------
void RegisterCBShowStartUpgrading(show_StartUpgrading_cb cb);
//-------------------------------------------------------------------------------------------------
/// Register the display of upgrading phase on software upgradeing
/// @param  cb                                  \b IN: call back function for handling in upgrading phase
//-------------------------------------------------------------------------------------------------
void RegisterCBShowUpgrading(show_Upgrading_cb cb);
//-------------------------------------------------------------------------------------------------
/// Unregister all call back function on software upgradeing
//-------------------------------------------------------------------------------------------------
void UnRegisterShowCb(void);


/******************************************************
 * MsSignature.c
 *****************************************************/
typedef int (*SignatureSave_cb)(SECURITY_INFO *);
typedef int (*SignatureLoad_cb)(SECURITY_INFO *);
void RegisterCBSignatureSave(SignatureSave_cb cb);
void RegisterCBSignatureLoad(SignatureLoad_cb cb);


/******************************************************
 * MsUpgradeUtility.c
 *****************************************************/
typedef int (*IsStorageEmpty)(void);
//---------------------------------------------------------------- ---------------------------------
/// Register the call backfunction for detecting if the storage is empty or not during usb upgrade
/// @param  cb                                  \b IN: call back function
//-------------------------------------------------------------------------------------------------
void RegisterCBIsStorageEmpty(IsStorageEmpty cb);

#endif
