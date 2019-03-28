/* -*- mode: c++ -*- *********************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * authors:                                                                  *
 *  Lydia Brenner (lbrenner@cern.ch), Carsten Burgard (cburgard@cern.ch)     *
 *  Katharina Ecker (kecker@cern.ch), Adam Kaluza      (akaluza@cern.ch)     *
 *****************************************************************************/

#ifndef ROO_LAGRANGIAN_MORPH_LC
#define ROO_LAGRANGIAN_MORPH_LC
#include <limits>

#ifdef USE_UBLAS
#include <vector>
#include <list>
#include <ostream>

#include <boost/multiprecision/cpp_dec_float.hpp>

#include "RooListProxy.h"
#include "RooAbsReal.h"
#include "RooArgSet.h"


namespace RooLagrangianMorphing {
  typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<100> > SuperFloat;
  typedef std::numeric_limits< SuperFloat > SuperFloatPrecision;

  class LinearCombination : public RooAbsReal {
    RooListProxy _actualVars ;
    std::vector<SuperFloat> _coefficients;
    mutable RooArgSet* _nset; //!

  public:
    LinearCombination();
    LinearCombination(const char* name);
    LinearCombination(const LinearCombination& other, const char* name);
    virtual void printArgs(std::ostream& os) const override;
    ~LinearCombination();
    virtual TObject* clone(const char* newname) const override;
    void add(SuperFloat c,RooAbsReal* t);
    void setCoefficient(size_t idx,SuperFloat c);
    SuperFloat getCoefficient(size_t idx);
    virtual Double_t evaluate() const override;    
    virtual std::list<Double_t>* binBoundaries(RooAbsRealLValue& obs, Double_t xlo, Double_t xhi) const override;
    virtual std::list<Double_t>* plotSamplingHint(RooAbsRealLValue& obs, Double_t xlo, Double_t xhi) const override;

    ClassDefOverride(LinearCombination,1)
    
  };
}

#else
namespace RooLagrangianMorphing {
   typedef double SuperFloat;
   typedef std::numeric_limits<double> SuperFloatPrecision;
}
#endif

#endif
