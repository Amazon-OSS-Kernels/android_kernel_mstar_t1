#ifndef _IPA_POOL_H_
#define _IPA_POOL_H_

#include <linux/types.h>
#include <linux/rbtree.h>
#include <linux/scatterlist.h>
#include <mdrv_ipa_pool.h>
#include <chip_setup.h>
#include "ipa_bootargs.h"
#include <linux/atomic.h>
#include <linux/swap.h>

typedef unsigned long IPA_ADDR;
enum IPASPCMAN_HEAP_TYPE
{
    IPASPCMAN_HEAP_TYPE_PA = 0,  //directly PA space
    IPASPCMAN_HEAP_TYPE_CMA = 1, //CMA heap
    IPASPCMAN_HEAP_TYPE_MAX
};

struct IPA_heap_attr
{
    int miu;
    char * name;
    unsigned long miu_offset;
    unsigned long heap_len;
    enum IPASPCMAN_HEAP_TYPE type;
};
struct IPA_heap_op
{
    int (*heap_allocate)(unsigned long heap_off, unsigned long len, void *para);
    int (*heap_free)(unsigned long heap_off, unsigned long len, void *para);
    int (*heap_get_ipa)(unsigned long heap_off,IPA_ADDR *addr, void *para);
    int (*heap_get_attr)(struct IPA_heap_attr *info, void *para);
};

struct Heap_pm_op
{
    int (*heap_suspend)(void);
    int (*heap_resume)(void);
};

struct IPA_heap_pm_op
{   //one heap type to one pm ops
    struct Heap_pm_op *heap_pm_ops;
    enum IPASPCMAN_HEAP_TYPE type;
    struct list_head node;
    int ref;
};

struct IPA_heap
{
    struct list_head node;
    struct rb_root pool_allocation_tree_root; //interval tree root for pool allocations
    struct rb_root hal_allocation_tree_root;  //interval tree root for hal allocations

    int hid;
    unsigned long len;
    unsigned long alignmask;
    struct IPA_heap_op *heap_op;
    void * priv;
    struct dentry *debug_file;
    struct mutex lock;

    struct IPA_pool *delay_free_pool;
    struct mutex delay_free_mutex;
    struct proc_dir_entry *proc_root;
    int delay_free_timelimit;//time limit to hold dealy free pool allocation, by msec
    int delay_free_sizelimit;//size limit to determine if to delay free the pool allocation, byte byte
};

enum cpuaddr_type
{
    ca_type_uncached=0,
    ca_type_cached_writeback,
    ca_type_cached_writethrough,
    ca_type_max
};

struct IPAPool_device{
    int major;
    int minor;
    struct cdev dev;
    struct file_operations fop;
    struct mutex apilock;
    struct mutex lock;
    struct idr idr_poolid;
    struct idr idr_ipc_pool_handle;
    struct list_head pool;
    struct dentry *debug_root;
    struct class *ipapool_class;
    struct device *ipapool_dev;
};

struct IPA_ipc_handle{
    struct file * fp;
    int ipc_pool_handle;
};

struct IPA_POLL_EVENT
{
    wait_queue_head_t wait;
    unsigned long event_count;
    struct list_head   event_head;
    struct mutex lock;
};

//typedef unsigned char	                        MS_BOOL;

struct IPA_pool_ref
{
    unsigned long pool_ref_id;
    struct list_head link;  //link to pool    
    struct file *fp;
    struct IPA_pool *pool;
    struct mutex lock;
    struct list_head cpu_addr[ca_type_max];              //user map record
    struct list_head cpu_addr_kernel[ca_type_max]; //kernel map record
    unsigned int flags;
    struct vm_area_struct **vma;
    struct IPA_ipc_handle ipc_handle;
    struct dentry * dbg_entry;
    
    struct IPA_POLL_EVENT poll_event;
    bool having_polling_thread;//flag for whether having polling thread    
    
};

struct IPA_space_heap_man
{
    struct mutex lock;
    struct list_head heaps;
    struct dentry * dbg_root;
};

#define IPA_POOL_FLAG_NO_CONFLICT   (1 << 10)

struct pool_range_node
{
    struct list_head list;
    unsigned long start;
    unsigned long length;
};

typedef int (*test_t)(void *p1, void *p2);

int IPASpcMan_register_heap(int hid,unsigned long len,
       unsigned long alignmask, struct IPA_heap_op *heap_op, void *data);

int IPASpcMan_unregister_heap(int hid);
int IPASpcMan_register_heap_pm(struct Heap_pm_op *ops, enum IPASPCMAN_HEAP_TYPE type);
int IPASpcMan_unregister_heap_pm(enum IPASPCMAN_HEAP_TYPE type);

int pool_alloc(struct IPA_pool *pool, struct file *client_filp, unsigned long pool_offset, unsigned long len,struct IPA_pool_conflict *conflict_info,  unsigned long timeout);
int pool_free(struct IPA_pool *pool, struct file *client_filp, unsigned long pool_offset, unsigned long len);
int pool_check_mem(struct IPA_pool *pool, struct file *client_filp, unsigned long pool_offset,
                unsigned long len, IPA_ADDR *ipa);
int heap_get_ipa(int hid, unsigned int heap_off, IPA_ADDR *ipa);
int heap_get_attr(int hid, struct IPA_heap_attr *info);
int pa_miu_get_heap(signed int miu,unsigned long pa_offset_in_miu,struct IPA_heap_attr *info,bool *in_heap,signed int *heap_id);
struct IPA_pool * heap_check_allocation(signed int space_id,unsigned long space_length,unsigned long pa_offset_in_heap,bool *p_allocated);
int heap_suspend(void);
int heap_resume(void);
void dump_pool_allocation(struct IPA_pool *pool, struct file * client_filp, struct seq_file *s);
int IPAHeap_init(void);

int _IPAPool_create_pool(
                           int heap_id, unsigned int flags,
                           unsigned int heap_off,
                           int pool_len,char *name,
                           struct IPA_pool **pool);
int _IPAPool_release_pool(struct IPA_pool *pool);
int _IPA_pool_entry(
                           int pool_id, unsigned long pool_offset,  unsigned long len,
                           struct file **pfp,struct IPA_pool_ref **ppr,struct IPA_pool **ppool);
struct IPA_heap* _IPASpcMan_find_heap(int hid);
int pool_allocation_timeout_test(void *param1, void *param2);
int ipa_pool_search_valid_range(struct IPA_pool *pool, struct file *client_filp,
	                           unsigned long start, unsigned long length, struct list_head *head, test_t test);
int ipa_pool_alloc_from_range_list(struct IPA_pool *pool, 
	                           struct file *client_filp, struct list_head *head, test_t test);
#endif
