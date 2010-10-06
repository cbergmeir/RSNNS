#include <R.h>
#include <R_ext/Print.h>

#include <iostream>

#include "RSNNS_glob_typ.h"
#include "RSNNS_kr_ui.h"


char* RSNNS_getFuncInfo(int funcno, int *functype)
{
 int func_type;
 char* func_name;
 
 krui_getFuncInfo(funcno, &func_name, &func_type); 

 //Rprintf("\nfunc_no: %i\n", a); 
 //Rprintf("func_type: %i\n", functype);
 //Rprintf("func_name: %s\n\n", funcname); 
 
 functype = &func_type;
 
 return func_name;
}

void RSNNS_getUnitDefaults()
{

FlintType act, bias;
int st, subnet_no, layer_no;
char *act_func, *out_func;

krui_getUnitDefaults(&act, &bias, &st, &subnet_no, &layer_no, &act_func, &out_func);

}

krui_err RSNNS_testSinglePattern(int patternNo, float decay, float *learnAllPatternsError)
{

  float *inParams = &decay;
  int numParams = 1;
  float *outParams;
  int NoOfOutParams;

  int res = krui_testSinglePattern(patternNo, inParams, numParams, &outParams, &NoOfOutParams);
  
  *learnAllPatternsError = outParams[0];
  
 Rprintf("\nNoOfOutParams: %i\n", NoOfOutParams); 
for(int i=0;i<NoOfOutParams;i++) {
 Rprintf("outParams: %f\n", outParams[i]);
 }
 

  return res;
/*
extern "C" krui_err  krui_testSinglePattern(int pattern_no, float *parameterInArray,
				        int NoOfInParams, 
				        float **parameterOutArray, 
				        int *NoOfOutParams);
*/
}

krui_err  SNNS_updateNet(float decay)
{
  float *inParams = &decay;
  int numParams = 1;
  extern krui_err  krui_updateNet(float *parameterInArray, int NoOfInParams);
}
