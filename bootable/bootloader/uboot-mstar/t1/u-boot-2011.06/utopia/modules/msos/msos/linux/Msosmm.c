#include <stdio.h>
#include <string.h>
#include "drvIPAPool.h"
#include "Msosmm.h"
#include "ULog.h"

#define VPRINTF(format,args...) ULOGI("MSOSMM", format, ##args)



MS_S32 __attribute__((weak)) MSOS_MMPool_Module_Init(void)
{
	return (MS_S32)0;
}


MS_S32 __attribute__((weak)) MSOS_MMPool_Module_DeInit(void)
{
	return (MS_S32)0;
}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Init(struct MSOS_MMPool_Init_Param * Init_Param)
{
	struct IPA_Pool_Init_Param ipaInitParam;
	struct PA_In_IPA_Pool_Param in_ipa_pool_info;
	MS_BOOL ret = false;

     if(Init_Param == NULL)
        goto done;

     memset(&ipaInitParam, 0, sizeof(ipaInitParam));

      in_ipa_pool_info.PA = Init_Param->PoolStartPa;

      if ((ret =PA_in_IPA_POOL_info(&in_ipa_pool_info)) == false ||  !in_ipa_pool_info.in_heap)
      {
             VPRINTF("\033[1;31m[%s] %d  PA_in_IPA_POOL_info fail !!! \033[m\n",__FUNCTION__,__LINE__);
             goto done;
      }


      ipaInitParam.space_id = in_ipa_pool_info.space_id;
      ipaInitParam.pool_name = Init_Param->pool_name;
      ipaInitParam.offset_in_heap = in_ipa_pool_info.pa_offset_in_heap;
      ipaInitParam.len = Init_Param->len;


      ret = MApi_IPA_Pool_Init(&ipaInitParam);

      if(ret == false)
      {
           Init_Param->error_code = in_ipa_pool_info.error_code;
           VPRINTF("\033[1;31m[%s] %d  MSOS_MMPool_Init fail !!! \033[m\n",__FUNCTION__,__LINE__);
      }
      else
      {
          Init_Param->pool_handle_id =  in_ipa_pool_info.space_id;
          Init_Param->miu = in_ipa_pool_info.miu;
          Init_Param->space_type = in_ipa_pool_info.space_type;
      }

done:
     return ret;

}


MS_BOOL __attribute__((weak)) MSOS_MMPool_Release(MS_U32 pool_handle_id)
{

     MS_BOOL ret;

     ret = MApi_IPA_Pool_Release(pool_handle_id);

      if((ret = MApi_IPA_Pool_Release(pool_handle_id)) == false)
      {
            VPRINTF("\033[1;41m[%s] %d  MSOS_MMPool_Release Pool Id %d FAIL\033[m\n",__FUNCTION__,__LINE__,pool_handle_id);
      }

      return ret;

}


MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem(struct MSOS_MMPool_GetMem_Param * get_param)
{
	struct IPA_Pool_GetMem_Param IPA_get_param;
	MS_BOOL ret = false;

      if(get_param == NULL)
      {
            goto done;
      }

      memset(&IPA_get_param, 0, sizeof(IPA_get_param));

      IPA_get_param.pool_handle_id = get_param->chunk.pool_handle_id;
      IPA_get_param.offset_in_pool = get_param->chunk.offset_in_pool;
      IPA_get_param.length = get_param->chunk.length;

      if((ret=MApi_IPA_Pool_GetMem(&IPA_get_param)) == false)
      {
            get_param->error_code = IPA_get_param.error_code;
            VPRINTF("\033[1;31m[%s] %d  MSOS_MMPool_GetMem Pool Id %d fail !!! \033[m\n",__FUNCTION__,__LINE__,IPA_get_param.pool_handle_id);
      }


done:

     return ret;
}



MS_BOOL __attribute__((weak)) MSOS_MMPool_GetMem_Timeout(struct MSOS_MMPool_GetMem_Param* get_param,MS_U32 timeout)
{
	struct IPA_Pool_GetMem_Param IPA_get_param;
	MS_BOOL ret = false;

      if(get_param == NULL)
      {
            goto done;
      }

      memset(&IPA_get_param, 0, sizeof(IPA_get_param));

      IPA_get_param.pool_handle_id = get_param->chunk.pool_handle_id;
      IPA_get_param.offset_in_pool = get_param->chunk.offset_in_pool;
      IPA_get_param.length = get_param->chunk.length;

      if((ret=MApi_IPA_Pool_GetMem_Timeout(&IPA_get_param , timeout)) == false)
      {
            get_param->error_code = IPA_get_param.error_code;
            VPRINTF("\033[1;31m[%s] %d  MSOS_MMPool_GetMem_Timeout Pool Id %d fail !!! \033[m\n",__FUNCTION__,__LINE__,IPA_get_param.pool_handle_id);
      }


done:

      return ret;

}


MS_BOOL __attribute__((weak)) MSOS_MMPool_MapVA(struct MSOS_MMPool_Map_Param * map_param)
{
	struct IPA_Pool_Map_Param IPA_map_param;
	MS_BOOL ret = false;

	memset(&IPA_map_param, 0, sizeof(IPA_map_param));

     if(map_param == NULL)
         goto done;

     IPA_map_param.pool_handle_id = map_param->chunk.pool_handle_id ;
     IPA_map_param.offset_in_pool = map_param->chunk.offset_in_pool;
     IPA_map_param.length =  map_param->chunk.length ;
     IPA_map_param.cache_type = map_param->cache_type;

      if((ret = MApi_IPA_Pool_MapVA(&IPA_map_param)) == false)
      {
           map_param->error_code = IPA_map_param.error_code;
          VPRINTF("\033[1;31m[%s] %d  MSOS_MMPool_MapVA Pool Id %d  fail !!! \033[m\n",__FUNCTION__,__LINE__,IPA_map_param.pool_handle_id);
      }

done:

      return ret;
}

void    __attribute__((weak)) MSOS_MMPool_UnmapVA(struct MSOS_MMPool_Unmap_Param * unmap_param)
{
	struct IPA_Pool_Unmap_Param IPA_unmap_param;

	if(unmap_param == NULL)
		goto done;

	memset(&IPA_unmap_param, 0, sizeof(IPA_unmap_param));

	IPA_unmap_param.virt_addr = (MS_VIRT)unmap_param->virt_addr;
	IPA_unmap_param.length = unmap_param->length;

	MApi_IPA_Pool_UnmapVA(&IPA_unmap_param);
done:
	return;
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Notify(struct MSOS_MMPool_Notify_Param *notify_param)
{
	struct IPA_Pool_Polling_Param stIPAPoolPollingParam;
	MS_BOOL ret = false;

	memset(&stIPAPoolPollingParam, 0, sizeof(stIPAPoolPollingParam));

     if(notify_param == NULL)
         goto done;

     stIPAPoolPollingParam.pool_handle_id = notify_param->pool_handle_id;
     stIPAPoolPollingParam.polling_callback = notify_param->notify_callback;

     if ((ret =MApi_IPA_Pool_Notify(&stIPAPoolPollingParam)) == true)
     {
           VPRINTF("sucess to call MSOS_MMPool_Notify.\n");
     }
     else
     {
           VPRINTF("fail to call MSOS_MMPool_Notify.\n");
     }


done:

     return ret;
}

MS_BOOL __attribute__((weak)) MSOS_MMPool_Locate_Chunk(struct MSOS_MMPool_Locate_Chunk_Param * chunk_param)
{
      return false;
}
