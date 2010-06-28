/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      derivator.h
 * Project:   SOT
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



#ifndef __SOT_DERIVATOR_H__
#define __SOT_DERIVATOR_H__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* Matrix */
#include <MatrixAbstractLayer/boost.h>
namespace ml = maal::boost;

/* SOT */
#include <sot-core/flags.h>
#include <sot-core/pool.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/all-signals.h>
#include <sot-core/vector-quaternion.h>

/* STD */
#include <string>

namespace sot {
namespace dg = dynamicgraph;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

template< class T >
class Derivator
:public dg::Entity
{
 protected:
  T memory;
  bool initialized;
  double timestep;
  static const double TIMESTEP_DEFAULT ; //= 1.;

 public: /* --- CONSTRUCTION --- */

  static std::string getTypeName( void ) { return "Unknown"; }
  static const std::string CLASS_NAME;

  Derivator( const std::string& name )
    : dg::Entity(name)
    ,memory(),initialized(false)
    ,timestep(TIMESTEP_DEFAULT)
    ,SIN(NULL,"sotDerivator<"+getTypeName()+">("+name+")::input("+getTypeName()+")::in") 
    ,SOUT( boost::bind(&Derivator<T>::computeDerivation,this,_1,_2), 
	   SIN,"sotDerivator<"+getTypeName()+">("+name+")::output("+getTypeName()+")::out")
    ,timestepSIN("sotDerivator<"+getTypeName()+">("+name+")::input(double)::dt")
    {
      signalRegistration( SIN<<SOUT<<timestepSIN );
      timestepSIN.setReferenceNonConstant( &timestep );
      timestepSIN.setKeepReference(true);
    }


  virtual ~Derivator( void ) {};

 public: /* --- SIGNAL --- */

  dg::SignalPtr<T,int> SIN;
  dg::SignalTimeDependent<T,int> SOUT;
  dg::Signal<double,int> timestepSIN;

 protected:
  T& computeDerivation( T& res,int time )
    {
      if(initialized)
	{
	  res = memory; res *= -1;
	  memory = SIN(time);
	  res += memory;
	  if( timestep!=1. ) res*=timestep;
	} else {
	  initialized = true; 
	  memory = SIN(time); 
	  res = memory; 
	  res *= 0; 
	}
      return res;
    }

 public: /* --- PARAMS --- */
/*   virtual void commandLine( const std::string& cmdLine,std::istringstream& cmdArgs, */
/* 			    std::ostream& os ) {} */
    

};

} // using namespace sot



#endif // #ifndef __SOT_DERIVATOR_H__
