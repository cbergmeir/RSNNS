/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/cc_modify.h,v $
  SHORTNAME      : cc_modify.h
  SNNS VERSION   : 4.2

  PURPOSE        : Header file of correspondent '.c' file
  NOTES          : 

  AUTHOR         : Juergen Gatter
  DATE           : 

  CHANGED BY     : 
  RCS VERSION    : $Revision: 1.4 $
  LAST CHANGE    : 

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _CC_MODIFY_DEFINED_
#define  _CC_MODIFY_DEFINED_

/* begin global definition section */

extern krui_err  cc_InitModificationVariables(void);
   /* inits some global Varibales. */

extern krui_err cc_TestWhetherParametersAreValid(void);
   /* is the Parameter-Input in the Windows valid ? */

extern krui_err cc_allocateMemoryForModifications(void);
   /* allocates Memory needed by some of the Modifications    */
   /* i.E. ccr_ListOfChosenUnits, ccs_GroupList,ccs_GroupData */

extern void cc_deallocateMemory(void);
   /* deallocates the above allocated Memory                              */
   /* gets and returns ErrorCode                                          */  
   /* Routine is called by the Makro ERROR_CHECK_WITH_MEMORY_DEALLOCATION */

extern double cc_modifyHighScore(struct Unit* SpecialUnitPtr, int s, double old);
   /* modifies the HighScore according the actual Modification       */
   /* s is the No (0..Max) of the Special Unit, old is the unchanged */
   /* HighScore. The calling routine uses the changed HighScore to   */
   /* determine the best Special Unit.                               */

extern void cc_actualizeGroupHighscores(double Score,int s,struct Unit* UnitPtr);
   /* if more then one group is used, this routine actualises the  */
   /* according datas. Parameters see above.                       */

extern int cc_calculateNoOfNewUnits(void);
   /* How much Units do we have to install ? */

extern struct Unit*  cc_getModifiedBestSpecialUnitPtr(int GroupNo);
   /* get the best SpecialUnit according to the actual GroupNo */

extern bool cc_UnitsHaveToBeConnected(int SpecialUnitNo,struct Unit *UnitPtr);
   /* should the be a connection between the two units ?  */

extern krui_err cc_calculateConnections(int SpecUnitNo);
   /* calculate a connection table, if this must be done before */
   /* all units where checked.                                  */

extern void cc_MakeMiscCalculationsForModifications(void);
   /* make some misc calculations for the Modifications.    */
   /* Actually : CCM --> Calculate Layer Height.            */
   /*            CCS --> Calculate Correlation Output Units */
   /*                    and Groups.                        */
extern int* ccs_GroupList;
   /* dynamic Array [0..NoOfOutputUnits]. the Group in which */
   /* the specified Output-Unit is.                          */


/* end   global definition section */

/* begin privat definition section */
/* end privat definition section */

#endif /* _CC_MODIFY_DEFINED_ */
