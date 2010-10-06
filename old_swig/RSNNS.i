%module RSNNS
%{

#include "RSNNSKrui.h"

extern "C" SEXP do_stuff (SEXP mat_dim);

%}

typedef int krui_err;
typedef float  FlintType;
typedef float  FlintTypeParam;

class   PosType  {
public:
  short  x,
	 y,
	 z;
};

//krui_allocNewPatternSet
%apply int *OUTPUT {int *set_no}; 

//krui_DefTrainSubPat
%apply int *OUTPUT {int *insize}; 
%apply int *OUTPUT {int *outsize}; 
%apply int *OUTPUT {int *instep}; 
%apply int *OUTPUT {int *outstep}; 
%apply int *OUTPUT {int *max_n_pos}; 

//krui_learnAllPatterns
%apply int *OUTPUT {int *NoOfOutParams}; 

//krui_getFuncParamInfo
%apply int *OUTPUT {int *no_of_input_params}; 
%apply int *OUTPUT {int *no_of_output_params}; 

//krui_learnAllPatterns
%apply int *OUTPUT {int *NoOfOutParams}; 
%apply float *OUTPUT {float *learnAllPatternsError};

%include "RSNNSKrui.h"

extern "C" SEXP do_stuff (SEXP mat_dim);
