
#include <Rcpp.h>

//using namespace Rcpp ;

#include "SnnsCLib.h"


RcppExport SEXP SnnsCLib__new(){
 return Rcpp::XPtr<SnnsCLib>( new SnnsCLib, true ) ;
}

RcppExport SEXP SnnsCLib__getVersion( SEXP xp ) {
 Rcpp::XPtr<SnnsCLib> w(xp) ;
 const char* ret = w->krui_getVersion();
 return Rcpp::wrap(ret);
}


/** invoke the set method */
/*RcppExport SEXP World__set( SEXP xp, SEXP msg ){
 Rcpp::XPtr<World> w(xp) ;
 w->set( Rcpp::as<std::string>( msg ) ) ;
 return R_NilValue ;
}*/

/*
RCPP_MODULE(yada){
 using namespace Rcpp ;
class_<SnnsCLib>( "SnnsCLib" )
  .method( "getVersion", &SnnsCLib::krui_getVersion )
 // .method( "set", &World::set )
 // .method( "clear", &clearWorld )
 ;
}
*/
