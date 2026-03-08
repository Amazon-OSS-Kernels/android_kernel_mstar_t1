#define PQ_GRULE_TMO_MODE_ENABLE 1
#define PQ_GRULE_DEFINE_AUTO_GEN 1
#if (PQ_GRULE_TMO_MODE_ENABLE)
typedef enum
{
#if PQ_GRULE_TMO_MODE_ENABLE
    PQ_GRule_TMO_MODE_TMO,
#endif

}
MST_GRule_Index_TMO;
#endif

#if PQ_GRULE_TMO_MODE_ENABLE
typedef enum
{
    PQ_GRule_TMO_MODE_High_TMO,
    PQ_GRule_TMO_MODE_Mid_TMO,
    PQ_GRule_TMO_MODE_Low_TMO,
}
MST_GRule_TMO_MODE_Index_TMO;
#endif

#if PQ_GRULE_TMO_MODE_ENABLE
typedef enum
{
    PQ_GRule_Lvl_TMO_MODE_High_TMO,
    PQ_GRule_Lvl_TMO_MODE_Mid_TMO,
    PQ_GRule_Lvl_TMO_MODE_Low_TMO,
}
MST_GRule_TMO_MODE_LvL_Index_TMO;
#endif


#define PQ_GRULE_RULE_NUM_TMO 1

#if PQ_GRULE_TMO_MODE_ENABLE
#define PQ_GRULE_TMO_MODE_IP_NUM_TMO 2
#define PQ_GRULE_TMO_MODE_NUM_TMO 3
#define PQ_GRULE_TMO_MODE_LVL_NUM_TMO 3
#endif

#if PQ_GRULE_TMO_MODE_ENABLE
extern code U8 MST_GRule_TMO_MODE_IP_Index_TMO[PQ_GRULE_TMO_MODE_IP_NUM_TMO];
extern code U8 MST_GRule_TMO_MODE_TMO[QM_INPUTTYPE_NUM_TMO][PQ_GRULE_TMO_MODE_NUM_TMO][PQ_GRULE_TMO_MODE_IP_NUM_TMO];
#endif

#if PQ_GRULE_TMO_MODE_ENABLE
extern code U8 MST_GRule_Panel_1_TMO_MODE_TMO[PQ_GRULE_TMO_MODE_LVL_NUM_TMO];
#endif

