#include <R.h>
#include <R_ext/Print.h>

//#include <config.h>

#include "glob_typ.h"
#include "krui_typ.h"
#include "kr_ui.h"
#include "kr_const.h"
#include "ext_typ.h"
#include "kr_typ.h"
#include "enzo_mem_typ.h"
#include "trans_f.h"
#include "kernel.h"

int glob_temp;

int testTrainNeuralNetwork (int a, int b, int *c)
{
  Rprintf("Method testTrainNeuralNetwork char** fjsldkf\n");

//  int temp;
//  temp = a+5;
//  c = &temp;

  int glob_temp;
  glob_temp = a+5;
  c = &glob_temp;

  Rprintf("value of c 1: %i", glob_temp);
  Rprintf("value of c 2: %i", (int) c);

  return(a+10);


//extern void      krui_getFuncInfo(int func_no, char **func_name,int *func_type);

}

int testTrainNeuralNetworkWrapper(void)
{
 int temp;
 int temp2 = testTrainNeuralNetwork(10,20,&temp);
 char* funcname;
 
 Rprintf("value of c 1: %i", temp);
 Rprintf("value of c 2: %i", (int) &temp);

 //krui_getFuncInfo(int func_no, char **func_name,int *func_type); 
 krui_getFuncInfo(10, &funcname, &temp); 
 
 Rprintf("functype: %i", temp);
 Rprintf("funcname: %s", funcname); 
 
 return temp;
}

char* getFuncInfoWrapper(int a)
{
 int functype;
 char* funcname;
 
 krui_getFuncInfo(a, &funcname, &functype); 

 Rprintf("\nfunc_no: %i\n", a); 
 Rprintf("func_type: %i\n", functype);
 Rprintf("func_name: %s\n\n", funcname); 
 
 return funcname;
}

char* getFuncInfoWrapper2(int a, int *b)
{
 int functype;
 char* funcname;
 
 krui_getFuncInfo(a, &funcname, &functype); 

 Rprintf("\nfunc_no: %i\n", a); 
 Rprintf("func_type: %i\n", functype);
 Rprintf("func_name: %s\n\n", funcname); 
 
 b = &functype;
 
 return funcname;
}


krui_err myBackpropLearnAllPatterns(float decay, float *learnAllPatternsError)
{
  float *inParams = &decay;
  int numParams = 1;
  float *outParams;
  int NoOfOutParams;

  int res = krui_learnAllPatterns(inParams, numParams, &outParams, &NoOfOutParams);
  
  *learnAllPatternsError = outParams[0];
  
  return res;
  
  //float error;
  //int res = krui_learnAllPatterns(&decay, num_params, &learnAllPatterns_error, NoOfOutParams);

  //int res = krui_learnAllPatterns(&inParams, numParams, &outParams, &noOutParams);

  //error = outParams[0];
//Rprintf("error: %f\n",error);  
  //&error;
//  *NoOfOutParams = noOutParams;
//Rprintf("NoOfOutParams: %i\n",noOutParams);    
  
}



void encoderExample(void)  {

krui_err res;

int x=0;
Rprintf("%i\n",x++);
krui_setLearnFunc("Std_Backpropagation");
Rprintf("%i\n",x++);
krui_setUpdateFunc("Topological_Order");
Rprintf("%i\n",x++);
krui_setUnitDefaults(1,0,INPUT,0,1,"Act_Logistic","Out_Identity");
Rprintf("%i\n",x++);
Rprintf("\nCreating the network out of thin air\n");
Rprintf("%i\n",x++);

Rprintf("Creating input layer\n");	

struct PosType pos;
pos.x = 0;
pos.y = 0;
pos.z = 0;


int inputs[9];
for (int i=1;i<10;i++) {
    Rprintf("i: %i\n",i);
	pos.x = i;
Rprintf("%i\n",x++);	
	int num = krui_createDefaultUnit();
Rprintf("%i\n",x++);	
	inputs[i-1] = num;
Rprintf("%i\n",x++);	
    char unitName[12];
	sprintf(unitName, "Input_%i",i);
	krui_setUnitName(num,unitName);
Rprintf("%i\n",x++);	
	krui_setUnitPosition(num, &pos);
Rprintf("%i\n",x++);	
};

Rprintf("Creating hidden layer\n");	

pos.y=2;
int hidden[4];
for (int i=1;i<5;i++) {
	pos.x = i + 3;
	int num = krui_createDefaultUnit();
	hidden[i-1] = num;
    char unitName[12];
	sprintf(unitName, "Hidden_%i",i);
	krui_setUnitName(num,unitName);
	krui_setUnitTType(num,HIDDEN);
	krui_setUnitPosition(num,&pos);
	krui_setCurrentUnit(num);
	for(int j=0; j<9;j++)  {
	  krui_createLink(inputs[j],0);
	};
};
	
Rprintf("Creating output layer\n");	

pos.y=4;
int outputs[9];
for (int i=1;i<10;i++) {
	pos.x = i;
	int num = krui_createDefaultUnit();
	outputs[i-1] = num;
    char unitName[12];
	sprintf(unitName, "Output_%i",i);
	krui_setUnitName(num,unitName);
	krui_setUnitTType(num,OUTPUT);
	krui_setUnitPosition(num,&pos);
	krui_setCurrentUnit(num);
	for(int j=0; j<4;j++)  {
	  krui_createLink(hidden[j],0);
	};
};

Rprintf("Creating patterns\n");

krui_deleteAllPatterns();
Rprintf("%i\n",x++);	
int patset;
krui_allocNewPatternSet(&patset);
Rprintf("%i\n",x++);	
for(int unum=0;unum<9;unum++)  {
  for(int curnum=0;curnum<9;curnum++)  {
Rprintf("%i\n",x++);	  
    krui_setUnitActivation(inputs[curnum], curnum == unum);
    krui_setUnitActivation(outputs[curnum], curnum == unum);
  }
Rprintf("new pattern:\n");	  
  krui_newPattern();
};	

Rprintf("%i\n",x++);	
float parameterInArray = -1;
res = krui_initializeNet(&parameterInArray,1);
Rprintf("Result: %i\n", res);
Rprintf("%i\n",x++);	

res = krui_shufflePatterns(1);
Rprintf("Result: %i\n", res);
Rprintf("%i\n",x++);	

int insize, outsize, instep, outstep, max_n_pos;
res = krui_DefTrainSubPat(&insize, &outsize, &instep, &outstep, &max_n_pos);
Rprintf("Result: %i\n", res);
Rprintf("%i\n",x++);	

Rprintf("Results of krui_DefTrainSubPat: %i,%i,%i,%i, %i\n",insize, outsize, instep, outstep, max_n_pos);


Rprintf("Training\n");

char buffer[200];

int i=0;
while(i < 50000) {
  float parameterInArray = 0.2;
  float* parameterOutArray;
  int NoOfOutParams;
  res = krui_learnAllPatterns(&parameterInArray, 1, &parameterOutArray, &NoOfOutParams);
  if (res != 0) Rprintf("error in learning iteration\n");		   
  else {
   sprintf(&buffer,"error: %f\n", parameterOutArray[0]);
   Rprintf(buffer);		   
  }
  //if not i % 5000 : print "Error in learning cycle %d:" % i, res[0]
  i = i + 1;
}

Rprintf("Writing pyencoder.pat\n");
res = krui_saveNewPatterns("/home/bergmeir/pyencoder_ctest.pat",patset);
Rprintf("Result: %i\n", res);

Rprintf("Writing pyencoder.net\n");
res = krui_saveNet("/home/bergmeir/pyencoder_ctest.net","pyencoder");
Rprintf("Result: %i\n", res);

/*
#!/usr/bin/python
# Construct a encoder.net like network from scratch

from snns import krui,util

krui.setLearnFunc('Std_Backpropagation')
krui.setUpdateFunc('Topological_Order')
krui.setUnitDefaults(1,0,krui.INPUT,0,1,'Act_Logistic','Out_Identity')

print "Creating the network out of thin air"

# build the input layer
pos = [0,0,0]
inputs = []
for i in range(1,9) :
	pos[0] = i
	num = krui.createDefaultUnit()
	inputs.append(num)
	krui.setUnitName(num,'Input_%i' % i)
	krui.setUnitPosition(num, pos)
# hidden layer
pos[1]=2
hidden = []
for i in range(1,4) :
	pos[0] = i + 3
	num = krui.createDefaultUnit()
	hidden.append(num)
	krui.setUnitName(num,'Hidden_%i' % i)
	krui.setUnitTType(num,krui.HIDDEN)
	krui.setUnitPosition(num,pos)
	krui.setCurrentUnit(num)
	for src in inputs :
		krui.createLink(src,0)

# output layer
pos[1]=4
outputs = []
for i in range(1,9) :
	pos[0] = i
	num = krui.createDefaultUnit()
	outputs.append(num)
	krui.setUnitName(num,'Output_%i' % i)
	krui.setUnitTType(num,krui.OUTPUT)
	krui.setUnitPosition(num,pos)
	krui.setCurrentUnit(num)
	for src in hidden :
		krui.createLink(src,0)

print "Creating patterns"

krui.deleteAllPatterns()
patset = krui.allocNewPatternSet()
for unum in range(8) :
	for curnum in range(8) :
		for row in inputs, outputs :
			krui.setUnitActivation(row[curnum], curnum == unum)
	krui.newPattern()		

krui.initializeNet(-1,1)
krui.shufflePatterns(1)
krui.DefTrainSubPat()

print "Training"

i=0
while i < 50000 :
	res = krui.learnAllPatterns(0.2,0)
	if not i % 5000 : print "Error in learning cycle %d:" % i, res[0]
	i = i + 1

print "Writing pyencoder.pat"
krui.saveNewPatterns('pyencoder.pat',patset)
print "Writing pyencoder.net"
krui.saveNet('pyencoder.net','pyencoder') */

}
