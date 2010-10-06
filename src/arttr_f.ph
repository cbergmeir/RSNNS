/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/kernel/sources/arttr_f.ph,v $
  SHORTNAME      : 
  SNNS VERSION   : 4.2

  PURPOSE        : SNNS-Kernel special transfer functions for ART-networks 
  NOTES          :

  AUTHOR         : Sven Doering
  DATE           : 

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:25:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _ARTTR_F_DEFINED_
#define  _ARTTR_F_DEFINED_

/* begin global definition section */

FlintType  OUT_ART2_Noise_PLin (register FlintType activation);
FlintType  OUT_ART2_Noise_ContDiff (FlintType activation);
FlintType  ACT_ART1_NC  (struct Unit *unit_ptr);
FlintType ACT_ART2_Linear (struct Unit *unit_ptr);
FlintType ACT_ART2_NormP  (struct Unit *unit_ptr);
FlintType ACT_ART2_NormV  (struct Unit *unit_ptr);
FlintType ACT_ART2_NormW  (struct Unit *unit_ptr);
FlintType ACT_ART2_NormIP  (struct Unit *unit_ptr);
FlintType ACT_ART2_Rec  (struct Unit *unit_ptr);
FlintType ACT_ART2_Rst  (struct Unit *unit_ptr);
FlintType  ACT_ARTMAP_NCa  (struct Unit *unit_ptr);
FlintType  ACT_ARTMAP_NCb  (struct Unit *unit_ptr);
FlintType  ACT_ARTMAP_DRho (struct Unit *unit_ptr);


/* end global definition section */

/* begin private definition section */

/* end private definition section */

#endif 
