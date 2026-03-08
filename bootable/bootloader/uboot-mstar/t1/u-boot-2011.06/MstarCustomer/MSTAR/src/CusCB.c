/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <command.h>
#include <common.h>
#include <MsAppCB.h>
#include <MsCoreCB.h>

void CusCallback(void)
{
   /* MsSignature Callback */
   //RegisterCBSignatureSave(SignatureSave_cb cb);
   //RegisterCBSignatureLoad(SignatureLoad_cb cb);

   /* MsUpgradeUtility Callback */
   //RegisterCBIsStorageEmpty(IsStorageEmpty cb);

   /* MsUpgrade Callback, below function is already setted in CusUpgrade.c */
   //RegisterCBShowError(show_Error_cb cb);
   //RegisterCBShowFinish(show_Finish_cb cb);
   //RegisterCBShowLoadData(show_LoadData_cb cb);
   //RegisterCBShowStartUpgrading(show_StartUpgrading_cb cb);
   //RegisterCBShowUpgrading(show_Upgrading_cb cb);
   //UnRegisterShowCb(void); */
}
