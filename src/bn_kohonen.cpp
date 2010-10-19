/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_kohonen.c,v $
  SHORTNAME      : bn_kohonen
  SNNS VERSION   : 4.2

  PURPOSE        : Bignet generator of n-Component Kohonen Networks 
  NOTES          :

  AUTHOR         : Marcus Ritt, Marc Seemann 
  DATE           : July 13 1993

  CHANGED BY     : Guenter Mamier
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:45 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#include "SnnsCLib.h"

/*
#include <config.h>

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"
#include "ui_utilP.h"
#include "ui_textP.h"
#include "ui_fileP.h"
#include "ui_confirmer.h"
#include "ui_control.h"
#include "bn_basics.h"

#include "bn_kohonen.ph"
*/

/*****************************************************************************
  FUNCTION : bn_createKOHONEN

  PURPOSE  : create the kohonen bignet window.
  NOTES    :
  RETURNS  :

  UPDATE   : 
******************************************************************************/
/*
void bn_createKOHONEN (void)
{
  Widget   box, panel, create_kohonen, done, dummy;
  Arg      arg[25];
  Cardinal n;
  char     buf[40];
  

    if(kohonen_widget_open) {
	XRaiseWindow (XtDisplay (baseWidget), XtWindow (baseWidget));
	return;
    }

    sprintf (buf, "SNNS BigNet (Kohonen)");
    n = 0;  
    
    XtSetArg(arg[n],XtNminHeight,82); n++;
    XtSetArg(arg[n],XtNminWidth,254); n++;  
    XtSetArg(arg[n],XtNmaxHeight,82); n++;
    XtSetArg(arg[n],XtNmaxWidth,254); n++;
    
    
    baseWidget = 
      XtCreatePopupShell (buf, topLevelShellWidgetClass, ui_toplevel, arg, n); 
    n = 0;  
    box = XtCreateManagedWidget ("box", boxWidgetClass, baseWidget, arg, n);
    panel = XtCreateManagedWidget ("form", formWidgetClass, box, NULL, 0);
    
    dummy= ui_xCreateLabelItem("Components:",panel,80,NULL,NULL);
    Comp = ui_xCreateDialogItem(" ", panel, "", 30, dummy, NULL);
    
    dummy= ui_xCreateLabelItem("X-size:", panel, 80, NULL, dummy);
    Xsize= ui_xCreateDialogItem(" ", panel, "", 30, dummy, Comp);	
    
    dummy= ui_xCreateLabelItem("Y-size:", panel, 80, Xsize, Comp);
    Ysize= ui_xCreateDialogItem(" ", panel, "", 30, dummy, Comp);
    
    create_kohonen = ui_xCreateButtonItem ("create_net",box,NULL,NULL);
    XtAddCallback(create_kohonen,XtNcallback,
		  (XtCallbackProc)create_net_PROC,NULL);
    done = ui_xCreateButtonItem ("done",box,create_kohonen,NULL);
    XtAddCallback(done,XtNcallback,(XtCallbackProc)done_PROC,NULL);
    
    XtPopup (baseWidget, XtGrabNone);
    kohonen_widget_open = 1;
    

}

*/

/*****************************************************************************
  FUNCTION : done_PROC

  PURPOSE  : callback function of the done-button. 
  NOTES    :
  RETURNS  :

  UPDATE   : june 11 1993
******************************************************************************/
/*
static void done_PROC (void)
{
    XtDestroyWidget (baseWidget);
    kohonen_widget_open = 0;
}
*/
/*****************************************************************************
  FUNCTION : create_net_PROC

  PURPOSE  : callback function of the create-button. 
  NOTES    :
  RETURNS  :

  UPDATE   : june 11 1993
******************************************************************************/
/*
static void create_net_PROC(void)
{
  int create= TRUE;

  IUnits= ui_xIntFromAsciiWidget(Comp);
  X     = ui_xIntFromAsciiWidget(Xsize);
  Y     = ui_xIntFromAsciiWidget(Ysize);
  OUnits= 0;
  HUnits= X*Y;

  if ((IUnits>0) && (X>0) && (Y>0)) {
    if(krui_getNoOfUnits() != 0)
      create= ui_confirmYes("Create will erase current network. Create?");
    if (create) {
      krui_deleteNet();
      bn_kohonen_createNet();
      bn_basics_refresh();
      ui_confirmOk("Network created.");
    }
  } else ui_confirmOk("Wrong parameters. Use positive integers.");

} */
/* create_net_PROC */


/*****************************************************************************
  FUNCTION : errChk

  PURPOSE  : check whether an error occured during a process 
  NOTES    : 
  RETURNS  :

  UPDATE   : june 6 1993
******************************************************************************/


void SnnsCLib::errChk(krui_err ret)
{
  //if (ret != 0) ui_tw_errorMessage(krui_error(ret));
}


/*****************************************************************************
  FUNCTION : bn_kohonen_createNet

  PURPOSE  : create the Kohonen Network 
  NOTES    : 
  RETURNS  :

  UPDATE   : june 6 1993
******************************************************************************/

void SnnsCLib::bn_kohonen_createNet(int X, int Y, int IUnits, int HUnits)
{
  int i,j,unit_no;
  struct PosType    unit_pos;
  krui_err ret;


  /*  Allocate units */
  
  ret = krui_allocateUnits( HUnits + IUnits );
  errChk( ret );


  /*  Create standard (input) Units  */

  unit_pos.x = 1;
  for (i = 1; i <= IUnits; i++) {
    unit_no = krui_createDefaultUnit();
    if (unit_no < 0)  errChk( unit_no );
    ret = krui_setUnitTType( unit_no, INPUT );
    errChk( ret );
    
    unit_pos.y = (IUnits<Y)?i+(Y-IUnits)/2:i;
    krui_setUnitPosition( unit_no, &unit_pos );
  }


  /* Create standard hidden Units. The size of the feature map is X*Y */
  
  for (i = 1; i <= Y; i++)
    for (j = 1; j <= X; j++) {
      unit_pos.x = 4+j;
      unit_no = krui_createDefaultUnit();
      if (unit_no < 0)  errChk( unit_no );
      ret = krui_setUnitTType( unit_no, HIDDEN );
      errChk( ret );
      
      unit_pos.y = i;
      krui_setUnitPosition( unit_no, &unit_pos );
    }
  

  /* Make connections between input units and hidden units  */

  /* set all link weights to zero */

  for (i = IUnits + 1; i <= IUnits + HUnits; i++) {

      /*  Make hidden unit to current unit  */
      ret = krui_setCurrentUnit( i );
      errChk( ret );
    
      /* (backward) connect current (hidden) unit with input unit */
      for (j = 1; j <= IUnits; j++) {
	  ret = krui_createLink( j,0.0);
	  errChk( ret );
      }
  } 
         

  /*  set the update function  */

  ret = krui_setUpdateFunc (KOHONEN_UPDATE_FUNC_NAME);
  errChk( ret );


  /* set the learning function */

  ret = krui_setLearnFunc (KOHONEN_LEARN_FUNC_NAME);
  errChk( ret );


  /* set the init function */

  ret = krui_setInitialisationFunc (KOHONEN_INIT_FUNC_NAME);
  errChk( ret );

  /*
  ui_NumberOfLearnParamsChanged();
  ui_NumberOfUpdateParamsChanged();
  ui_NumberOfInitParamsChanged();
  */

} /* bn_kohonen_createNet */

/* end of file */
