/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/bn_assoz.c,v $
  SHORTNAME      : bn_assoz
  SNNS VERSION   : 4.2

  PURPOSE        : Bignet generator of n-Component Kohonen Networks 
  NOTES          :

  AUTHOR         : Guenter Mamier
  DATE           : February 11, 1994

  CHANGED BY     : 
  RCS VERSION    : $Revision: 2.11 $
  LAST CHANGE    : $Date: 1998/03/03 14:09:42 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#include "SnnsCLib.h"

#define CHECK_RETURN(ret_code)  if (ret_code != KRERR_NO_ERROR) \
                                   return (ret_code)

/*
#include <config.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>

#include "ui.h"
#include "kr_ui.h"
#include "ui_utilP.h"
#include "ui_textP.h"
#include "ui_mainP.h"
#include "ui_xWidgets.h"
#include "ui_confirmer.h"
#include "ui_control.h"
#include "bn_basics.h"

#include "bn_assoz.ph"
*/

/*****************************************************************************
  FUNCTION : bn_create_assoz

  PURPOSE  : create the autoassoziative bignet window.
  NOTES    :
  RETURNS  :

  UPDATE   : 
******************************************************************************/
/*
void bn_create_assoz (void)
{
  Widget   box, panel, create_assoz, done, dummy;
  Arg      arg[25];
  Cardinal n;
  char     buf[40];
  

    if(assoz_widget_open) {
	XRaiseWindow (XtDisplay (baseWidget), XtWindow (baseWidget));
	return;
    }

    sprintf (buf, "SNNS BigNet (Auto assoziative Networks)");
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
    
    dummy= ui_xCreateLabelItem("X-size:", panel, 80, NULL, NULL);
    Xsize= ui_xCreateDialogItem(" ", panel, "", 30, dummy, NULL);	
    
    dummy= ui_xCreateLabelItem("Y-size:", panel, 80, Xsize, NULL);
    Ysize= ui_xCreateDialogItem(" ", panel, "", 30, dummy, NULL);
    
    create_assoz = ui_xCreateButtonItem ("create_net",box,NULL,NULL);
    XtAddCallback(create_assoz,XtNcallback,(XtCallbackProc)create_net_PROC,NULL);
    done = ui_xCreateButtonItem ("done",box,create_assoz,NULL);
    XtAddCallback(done,XtNcallback,(XtCallbackProc)done_PROC,NULL);
    
    XtPopup (baseWidget, XtGrabNone);
    assoz_widget_open = 1;
    

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
    assoz_widget_open = 0;
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

  X     = ui_xIntFromAsciiWidget(Xsize);
  Y     = ui_xIntFromAsciiWidget(Ysize);

  HUnits= X*Y;
  IUnits= X*Y;

  if ((X>0) && (Y>0)) {
    if(krui_getNoOfUnits() != 0)
      create= ui_confirmYes("Create will erase current network. Create?");
    if (create) {
      krui_deleteNet();
      bn_assoz_createNet();
      bn_basics_refresh();
      ui_confirmOk("Network created.");
    }
  } else ui_confirmOk("Wrong parameters. Use positive integers.");

} 
*/

/*****************************************************************************
  FUNCTION : errChk

  PURPOSE  : check whether an error occured during a process 
  NOTES    : 
  RETURNS  :

  UPDATE   : june 6 1993
******************************************************************************/
/*
void bn_assoz_errChk(krui_err ret)
{
  if (ret != 0) ui_tw_errorMessage(krui_error(ret));
} 
*/

/*****************************************************************************
  FUNCTION : bn_assoz_createNet

  PURPOSE  : create the Assoz Network 
  NOTES    : 
  RETURNS  :

  UPDATE   : june 6 1993
******************************************************************************/

krui_err SnnsCLib::bn_assoz_createNet(int X, int Y)
{
  int i,j,unit_no;
  struct PosType    unit_pos;
  krui_err ret = KRERR_NO_ERROR;

  int HUnits= X*Y;
  int IUnits= X*Y;


  /*  Allocate units */
  
  ret = krui_allocateUnits( IUnits + HUnits );
  CHECK_RETURN (ret);


  /* Create standard input Units. The size of the input layer is X*Y */
  
  for (i = 1; i <= Y; i++)
    for (j = 1; j <= X; j++) {
      unit_pos.x = j;
      unit_no = krui_createDefaultUnit();
      if (unit_no < 0)  CHECK_RETURN (unit_no);
      ret = krui_setUnitTType( unit_no, INPUT );
      CHECK_RETURN (ret);
      ret = krui_setUnitActFunc( unit_no, "Act_RM" );
      CHECK_RETURN (ret);
      
      unit_pos.y = i;
      krui_setUnitPosition( unit_no, &unit_pos );
    }

  /* Create standard hidden Units. The size of the output layer is X*Y */
  
  for (i = 1; i <= Y; i++)
    for (j = 1; j <= X; j++) {
      unit_pos.x = X+4+j;
      unit_no = krui_createDefaultUnit();
      if (unit_no < 0)  CHECK_RETURN (unit_no);
      ret = krui_setUnitTType( unit_no, HIDDEN );
      CHECK_RETURN (ret);
      ret = krui_setUnitActFunc( unit_no, "Act_RM" );
      CHECK_RETURN (ret);
      
      unit_pos.y = i;
      krui_setUnitPosition( unit_no, &unit_pos );
    }
  

  /* Make connections between input units and hidden units  */

  for (i = IUnits + 1; i <= IUnits + HUnits; i++) {

      /*  Make hidden unit to current unit  */
      ret = krui_setCurrentUnit( i );
      CHECK_RETURN (ret);
    
      /* (backward) connect current (hidden) unit with input unit */
      /* set all link weights from the input units to one */
      ret = krui_createLink( i-IUnits, 1.0);
      CHECK_RETURN (ret);

      /* Make connections from all hidden units  */
      /* set link weights to zero */
      for (j = IUnits + 1; j <= IUnits + HUnits; j++) {
	  if (j != i){
	      ret = krui_createLink( j, 0.0);
	      CHECK_RETURN (ret);
	  }
      }
  } 

  /*  set the update function  */
  ret = krui_setUpdateFunc (ASSOZ_UPDATE_FUNC_NAME);
  CHECK_RETURN (ret);

  /* set the learning function */
  ret = krui_setLearnFunc (ASSOZ_LEARN_FUNC_NAME);
  CHECK_RETURN (ret);

  /* set the init function */
  ret = krui_setInitialisationFunc (ASSOZ_INIT_FUNC_NAME);
  //CHECK_RETURN (ret);

  return(ret);
  //ui_NumberOfLearnParamsChanged();
  //ui_NumberOfUpdateParamsChanged();
  //ui_NumberOfInitParamsChanged();
} /* bn_assoz_createNet */



/*****************************************************************************
 
******************************************************************************/

/* end of file */
/* lines: 283 */
