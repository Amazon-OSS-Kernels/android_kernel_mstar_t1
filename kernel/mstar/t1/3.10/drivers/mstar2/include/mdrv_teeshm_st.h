#ifndef _MDRV_TEESHM_ST_H_
#define _MDRV_TEESHM_ST_H_

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
	char *name;				// in
	u32 size;				// in
	u16 m_shm_id;			// out
} DrvTeeShmResourceCreate;

typedef struct
{
	u16 m_shm_id;			// in
} DrvTeeShmResourceDestroy;

typedef struct
{
	u16 m_shm_id;			// in
} DrvTeeShmResourceObtain;

typedef struct
{
	u16 m_shm_id;			// in
} DrvTeeShmResourceRelease;

typedef struct
{
	u16 m_shm_id;			// in
	void *va;				// out
} DrvTeeShmResourceMap;

typedef struct
{
	u16 m_shm_id;			// in
	void *va;				// in
} DrvTeeShmResourceUnmap;

#ifdef CONFIG_COMPAT
typedef struct
{
	compat_uptr_t name;				// in, change * to compat_uptr_t
	compat_ulong_t size;			// in
	__compat_uid16_t m_shm_id;		// out
} DrvTeeShmResourceCreate_32;

typedef struct
{
	__compat_uid16_t m_shm_id;		// in
	compat_uptr_t va;				// out
} DrvTeeShmResourceMap_32;

typedef struct
{
	__compat_uid16_t m_shm_id;		// in
	compat_uptr_t va;				// in
} DrvTeeShmResourceUnmap_32;
#endif

#endif // _MDRV_TEESHM_ST_H_
