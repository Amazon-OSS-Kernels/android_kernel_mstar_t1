#ifndef _IPA_POOL_DELAY_FREE_H_
#define _IPA_POOL_DELAY_FREE_H_

int ipa_heap_delay_free_init(struct IPA_heap *heap);
int IPA_pool_delay_free(int pool_id, unsigned long pool_offset, unsigned long length);

#endif