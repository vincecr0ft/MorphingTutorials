/* -*- mode: c++ -*- *********************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * authors:                                                                  *
 *  Lydia Brenner (lbrenner@cern.ch), Carsten Burgard (cburgard@cern.ch)     *
 *  Katharina Ecker (kecker@cern.ch), Adam Kaluza      (akaluza@cern.ch)     *
 *****************************************************************************/

#ifndef ROO_LAGRANGIAN_MORPH
#define ROO_LAGRANGIAN_MORPH

#include "RooAbsPdf.h"
#include "RooRealSumPdf.h"
#include "RooRealSumFunc.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooProduct.h"
#include "TMatrixD.h"
#include "RooAbsArg.h"
   
class RooWorkspace;
class RooParamHistFunc;
class TPair;
class TFolder;

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace RooLagrangianMorphing {
  typedef std::map<const std::string,double> ParamSet;
  typedef std::map<const std::string,int> FlagSet;  
  typedef std::map<const std::string,RooLagrangianMorphing::ParamSet > ParamMap;
  typedef std::map<const std::string,RooLagrangianMorphing::FlagSet > FlagMap;  
  extern bool gAllowExceptions;
  double implementedPrecision();
  RooWorkspace* makeCleanWorkspace(RooWorkspace* oldWS, const char* newName = 0, const char* mcname = "ModelConfig", bool keepData = false);
  void importToWorkspace(RooWorkspace* ws, const RooAbsReal* object);
  void importToWorkspace(RooWorkspace* ws, RooAbsData* object);  
  
  // these are a couple of helper functions for use with the Higgs Characterization (HC) Model
  // arXiv: 1306.6464
  RooArgSet makeHCggFCouplings(RooAbsCollection& kappas);
  RooArgSet makeHCVBFCouplings(RooAbsCollection& kappas);
  RooArgSet makeHCHWWCouplings(RooAbsCollection& kappas);
  RooArgSet makeHCHZZCouplings(RooAbsCollection& kappas);
  RooArgSet makeHCHllCouplings(RooAbsCollection& kappas);

  // these are a couple of helper functions for use with the Standard Model Effective Field Theory (SMEFT) Model
  // arXiv: 1709.06492
  RooArgSet makeSMEFTCouplings(RooAbsCollection& operators);
  RooArgSet makeSMEFTggFCouplings(RooAbsCollection& operators);
  RooArgSet makeSMEFTVBFCouplings(RooAbsCollection& operators);
  RooArgSet makeSMEFTHWWCouplings(RooAbsCollection& operators);
  RooArgSet makeSMEFTHyyCouplings(RooAbsCollection& operators);
  
  void writeMatrixToFile(const TMatrixD& matrix, const char* fname);
  void writeMatrixToStream(const TMatrixD& matrix, std::ostream& stream);
  TMatrixD readMatrixFromFile(const char* fname);
  TMatrixD readMatrixFromStream(std::istream& stream);

  RooDataHist* makeDataHistogram(TH1* hist, RooRealVar* observable, const char* histname = NULL);
  void setDataHistogram(TH1* hist, RooRealVar* observable, RooDataHist* dh);
  void printDataHistogram(RooDataHist* hist, RooRealVar* obs);

  int countSamples(std::vector<RooArgList*>& vertices);
  int countSamples(int nprod, int ndec, int nboth);

  TPair* makeCrosssectionContainer(double xs, double unc);
  RooArgSet createWeights(const RooLagrangianMorphing::ParamMap& inputs, const std::vector<RooArgList*>& vertices, RooArgList& couplings, const RooLagrangianMorphing::FlagMap& inputFlags, const RooArgList& flags, const std::vector<RooArgList*>& nonInterfering);
  RooArgSet createWeights(const RooLagrangianMorphing::ParamMap& inputs, const std::vector<RooArgList*>& vertices, RooArgList& couplings);

  // some helpers to make the template mapping work
  template<class Base> struct Internal;
  template<> struct Internal<RooAbsReal> { typedef RooRealSumFunc Type; };
  template<> struct Internal<RooAbsPdf>  { typedef RooRealSumPdf  Type; };
 
  template<class Base>
  class RooLagrangianMorphBase : public Base {
    using InternalType = typename Internal<Base>::Type;
  public:
    RooLagrangianMorphBase();
    template<class T> RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    template<class T> RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    template<class T> RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const char* objFilter, bool allowNegativeYields=true);
    template<class T> RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    template<class T> RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    template<class T> RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const char *name, const char *title, const char* fileName, const char* obsName, const char* objFilter = 0, bool allowNegativeYields=true);
    RooLagrangianMorphBase(const RooLagrangianMorphBase& other, const char *newName);
 
    virtual ~RooLagrangianMorphBase();

    virtual std::list<Double_t>* binBoundaries(RooAbsRealLValue& /*obs*/, Double_t /*xlo*/, Double_t /*xhi*/) const override;
    virtual std::list<Double_t>* plotSamplingHint(RooAbsRealLValue& /*obs*/, Double_t /*xlo*/, Double_t /*xhi*/) const override;
    virtual Bool_t isBinnedDistribution(const RooArgSet& obs) const override;
    virtual Double_t evaluate() const override;
    virtual TObject* clone(const char* newname) const override;
    virtual Double_t getValV(const RooArgSet* set=0) const override;

    virtual Bool_t checkObservables(const RooArgSet *nset) const override;
    virtual Bool_t forceAnalyticalInt(const RooAbsArg &arg) const override;
    virtual Int_t getAnalyticalIntegralWN(RooArgSet &allVars, RooArgSet &numVars, const RooArgSet *normSet, const char *rangeName = 0) const override;
    virtual Double_t analyticalIntegralWN(Int_t code, const RooArgSet *normSet, const char *rangeName = 0) const override;
    virtual void printMetaArgs(std::ostream &os) const override;
    virtual RooAbsArg::CacheMode canNodeBeCached() const override;
    virtual void setCacheAndTrackHints(RooArgSet &) override;

    void insert(RooWorkspace* ws);
    
    void setParameters(const char* foldername);
    void setParameters(TH1* paramhist);
    void setParameter(const char* name, double value);
    void setParameters(const ParamSet& params);
    void setParameters(const RooArgList* list);
    double getParameterValue(const char* name) const;
    RooRealVar* getParameter(const char* name) const;
    bool hasParameter(const char* paramname) const;
    bool isParameterUsed(const char* paramname) const;
    bool isParameterConstant(const char* paramname) const;
    void setParameterConstant(const char* paramname, bool constant) const;
    void setParameter(const char* name, double value, double min, double max);
    void setParameter(const char* name, double value, double min, double max, double error);
    void randomizeParameters(double z);
    const RooArgList* getParameterSet() const;
    using RooAbsArg::getParameters;
    ParamSet getParameters(const char* foldername) const;
    ParamSet getParameters() const;

    int nParameters() const;
    int nSamples() const;
    int nPolynomials() const;

    bool isCouplingUsed(const char* couplname) const;
    const RooArgList* getCouplingSet() const;
    ParamSet getCouplings() const;

    TMatrixD getMatrix() const;
    TMatrixD getInvertedMatrix() const;
    double getCondition() const;
    
    RooRealVar* getObservable() const;
    RooRealVar* getBinWidth() const;
 
    void printEvaluation() const;
    void printCouplings() const;
    void printParameters() const;
    void printParameters(const char* samplename) const;
    void printSamples() const;
    void printPhysics() const;

    RooProduct* getSumElement(const char* name) const;
  
    const std::vector<std::string>& getSamples() const;
  
    double expectedUncertainty() const;
    TH1* createTH1(const std::string& name, RooFitResult* r = NULL);
    TH1* createTH1(const std::string& name, bool correlateErrors, RooFitResult* r = NULL);
  
  protected:

    class CacheElem;

    void init();
    void printAuthors() const;

    void setup(const RooArgSet& operators, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, bool ownParams = true);
    template<class T> void setup(const RooArgSet& operators, const std::vector<T>& vertices, bool ownParams = true);
    bool _ownParameters = false; 
  
    void disableInterference(const std::vector<const char*>& nonInterfering);
    void disableInterferences(const std::vector<std::vector<const char*> >& nonInterfering);

    mutable RooObjCacheManager _cacheMgr; //! The cache manager
 
    void addFolders(const RooArgList& folders);

    InternalType* getInternal() const;
  
    bool hasCache() const;
    RooLagrangianMorphBase<Base>::CacheElem* getCache(const RooArgSet* nset) const;
    void readParameters(TDirectory* f);
    void collectInputs(TDirectory* f);
    void updateSampleWeights();
    RooRealVar* setupObservable(const char* obsname,TClass* mode,TObject* inputExample);
    
  public:
  
    bool updateCoefficients();
    bool useCoefficients(const TMatrixD& inverse);
    bool useCoefficients(const char* filename);
    bool writeCoefficients(const char* filename);
  
    int countContributingFormulas() const;
    RooParamHistFunc* getBaseTemplate();
    RooAbsReal* getSampleWeight(const char* name);
    void printSampleWeights();
  
  protected:
    std::string _fileName;
    std::string _obsName;
    std::string _objFilter;
    std::string _baseFolder;
    bool _allowNegativeYields;
    std::vector<std::string>  _folderNames;
    ParamMap _paramCards;
    FlagMap _flagValues;    
    std::map<std::string,int>  _sampleMap;
    RooListProxy _physics;
    RooListProxy _operators;
    RooListProxy _observables ; 
    RooListProxy _binWidths ; 
    RooListProxy _flags;
    std::vector<RooListProxy*> _vertices;
    std::vector<RooListProxy*> _nonInterfering;

    mutable const RooArgSet* _curNormSet ; //! 

  public:

    ClassDefT(RooLagrangianMorphBase<Base>,4)
  
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // RooLagrangianMorphBase
    //
    // The RooLagrangianMorphBase is a type of RooAbsPdf that allows to morph
    // different input EFT samples to some arbitrary output EFT
    // sample, as long as the desired set of output parameters lie
    // within the realm spanned by the input samples. More
    // specifically, it expects as an input a TFile (or TDirectory)
    // with the following layout:
    //
    // TDirectory 
    //  |-sample1
    //  | |-param_card     // a TH1 which encodes the EFT parameter values used for this sample
    //  | | histogram      // a TH1 with a distribution of some physical observable 
    //  | |-subfolder1     // a subfolder (optional)
    //  | | |-histogram1   // another TH1 with a distribution of some physical observable 
    //  | | |-histogram2   // another TH1 with a distribution of some physical observalbe 
    //  | | ...            // more of these
    //  |-sample2
    //  | |-param_card     // a TH1 which encodes the EFT parameter values used for this sample
    //  | | histogram      // a TH1 with a distribution of the same physical observable as above
    //  | | ...
    //  | ...
    //
    // The RooLagrangianMorphBase operates on this structure, extracts data
    // and meta-data and produces a morphing result as a RooRealSum
    // consisting of the input histograms with appropriate prefactors.
    //
    // The histograms to be morphed can be accessed via their paths in
    // the respective sample, e.g. using
    //    "histogram"
    // or "subfolder1/histogram1"
    // or "some/deep/path/to/some/subfolder/histname"
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////

  };

  
  ClassDefT2(RooLagrangianMorphBase,Base)

}

class RooLagrangianMorphFunc : public RooLagrangianMorphing::RooLagrangianMorphBase<RooAbsReal> {
public:
  template<class T> RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,basefolder,folders,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,folders,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const char* objFilter, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,nonInterfering,basefolder,folders,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,nonInterfering,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const char* objFilter = 0, bool allowNegativeYields=true){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,nonInterfering,basefolder,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,nonInterfering,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,nonInterfering,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,basefolder,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,basefolder,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const char *name, const char *title, const char* fileName, const char* obsName, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,objFilter,allowNegativeYields){}
  RooLagrangianMorphFunc(const RooLagrangianMorphFunc& other, const char* name) : RooLagrangianMorphBase(other,name){}
  RooLagrangianMorphFunc() : RooLagrangianMorphBase(){}
  
  RooRealSumFunc* getFunc() const;
  RooRealSumFunc* cloneFunc() const;
  ClassDefOverride(RooLagrangianMorphFunc,2)
};
class RooLagrangianMorphPdf : public RooLagrangianMorphing::RooLagrangianMorphBase<RooAbsPdf> {
public:
  template<class T> RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,basefolder,folders,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,folders,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const char* objFilter, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,nonInterfering,basefolder,folders,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,vertices,nonInterfering,folders,objFilter,allowNegativeYields){}
  template<class T> RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const std::vector<T>& vertices, const std::vector<std::vector<const char*> >& nonInterfering, const char* objFilter = 0, bool allowNegativeYields=true){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,nonInterfering,basefolder,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,nonInterfering,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const std::vector<std::vector<const char*> >& nonInterfering, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,nonInterfering,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,basefolder,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const RooAbsCollection& prodCouplings, const RooAbsCollection& decCouplings, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,prodCouplings,decCouplings,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const char* basefolder, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,basefolder,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const RooArgList& folders, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,folders,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const char *name, const char *title, const char* fileName, const char* obsName, const char* objFilter = 0, bool allowNegativeYields=true) : RooLagrangianMorphBase(name,title,fileName,obsName,objFilter,allowNegativeYields){}
  RooLagrangianMorphPdf(const RooLagrangianMorphPdf& other, const char* name) : RooLagrangianMorphBase(other,name){}
  RooLagrangianMorphPdf() : RooLagrangianMorphBase(){}

  virtual RooAbsPdf::ExtendMode extendMode() const override;
  virtual Double_t expectedEvents(const RooArgSet* nset) const override;
  virtual Double_t expectedEvents(const RooArgSet& nset) const override;
  virtual Double_t expectedEvents() const;
  RooRealSumPdf* getPdf() const;
  RooRealSumPdf* clonePdf() const;
  virtual Bool_t selfNormalized() const override;
  ClassDefOverride(RooLagrangianMorphPdf,2)
};



#define MAKE_ROOLAGRANGIANMORPH(BASE,CLASSNAME) public:			\
  CLASSNAME(const char *name, const char *title, const char* fileName, const char* obsName, const char* basefolder, const RooArgList& folders, const char* objFilter=0, bool allowNegativeYields=true): RooLagrangianMorph ## BASE(name,title,fileName,obsName,basefolder,folders,objFilter,allowNegativeYields) {this->makeCouplings();}; \
  CLASSNAME(const char *name, const char *title, const char* fileName, const char* obsName, const RooArgList& folders, const char* objFilter=0, bool allowNegativeYields=true) : RooLagrangianMorph ## BASE(name,title,fileName,obsName,folders,objFilter,allowNegativeYields){this->makeCouplings();}; \
  CLASSNAME(const char *name, const char *title, const char* fileName, const char* obsName, const char* objFilter=0, bool allowNegativeYields=true) : RooLagrangianMorph ## BASE(name,title,fileName,obsName,objFilter,allowNegativeYields){this->makeCouplings();}; \
  CLASSNAME(const CLASSNAME& other, const char* newname) : RooLagrangianMorph ## BASE(other,newname){ }; \
  CLASSNAME():RooLagrangianMorph ## BASE(){ };                          \
  virtual ~CLASSNAME(){};						\
  virtual TObject* clone(const char* newname) const override { return new CLASSNAME(*this,newname); };

////////////////////////////////////////////////////////////////////////////////////////////////
// DERIVED CLASSES to implement specific PHYSICS ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class RooHCggfWWMorphFunc : public RooLagrangianMorphFunc {
  MAKE_ROOLAGRANGIANMORPH(Func,RooHCggfWWMorphFunc)
  ClassDefOverride(RooHCggfWWMorphFunc,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("ggfWW");
    this->setup(kappas,RooLagrangianMorphing::makeHCggFCouplings(kappas),RooLagrangianMorphing::makeHCHWWCouplings(kappas),true);
  }
};
class RooHCggfWWMorphPdf : public RooLagrangianMorphPdf {
  MAKE_ROOLAGRANGIANMORPH(Pdf,RooHCggfWWMorphPdf)
  ClassDefOverride(RooHCggfWWMorphPdf,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("ggfWW");
    this->setup(kappas,RooLagrangianMorphing::makeHCggFCouplings(kappas),RooLagrangianMorphing::makeHCHWWCouplings(kappas),true);
  }
};

class RooHCvbfWWMorphFunc : public RooLagrangianMorphFunc {
  MAKE_ROOLAGRANGIANMORPH(Func,RooHCvbfWWMorphFunc)
  ClassDefOverride(RooHCvbfWWMorphFunc,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("vbfWW");
    this->setup(kappas,RooLagrangianMorphing::makeHCVBFCouplings(kappas),RooLagrangianMorphing::makeHCHWWCouplings(kappas),true);
  }
};
class RooHCvbfWWMorphPdf : public RooLagrangianMorphPdf {
  MAKE_ROOLAGRANGIANMORPH(Pdf,RooHCvbfWWMorphPdf)
  ClassDefOverride(RooHCvbfWWMorphPdf,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("vbfWW");
    this->setup(kappas,RooLagrangianMorphing::makeHCVBFCouplings(kappas),RooLagrangianMorphing::makeHCHWWCouplings(kappas),true);
  }
};

class RooHCggfZZMorphFunc : public RooLagrangianMorphFunc {
  MAKE_ROOLAGRANGIANMORPH(Func,RooHCggfZZMorphFunc)
  ClassDefOverride(RooHCggfZZMorphFunc,1) 
  protected:
  void makeCouplings(){
    RooArgSet kappas("ggfZZ");
    this->setup(kappas,RooLagrangianMorphing::makeHCggFCouplings(kappas),RooLagrangianMorphing::makeHCHZZCouplings(kappas),true);
  }
};
class RooHCggfZZMorphPdf : public RooLagrangianMorphPdf {
  MAKE_ROOLAGRANGIANMORPH(Pdf,RooHCggfZZMorphPdf)
  ClassDefOverride(RooHCggfZZMorphPdf,1) 
  protected:
  void makeCouplings(){
    RooArgSet kappas("ggfZZ");
    this->setup(kappas,RooLagrangianMorphing::makeHCggFCouplings(kappas),RooLagrangianMorphing::makeHCHZZCouplings(kappas),true);
  }
};

class RooHCvbfZZMorphFunc : public RooLagrangianMorphFunc {
  MAKE_ROOLAGRANGIANMORPH(Func,RooHCvbfZZMorphFunc)
  ClassDefOverride(RooHCvbfZZMorphFunc,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("vbfZZ");
    this->setup(kappas,RooLagrangianMorphing::makeHCVBFCouplings(kappas),RooLagrangianMorphing::makeHCHZZCouplings(kappas),true);
  }  
};
class RooHCvbfZZMorphPdf : public RooLagrangianMorphPdf {
  MAKE_ROOLAGRANGIANMORPH(Pdf,RooHCvbfZZMorphPdf)
  ClassDefOverride(RooHCvbfZZMorphPdf,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("vbfZZ");
    this->setup(kappas,RooLagrangianMorphing::makeHCVBFCouplings(kappas),RooLagrangianMorphing::makeHCHZZCouplings(kappas),true);
  }  
};

class RooHCvbfMuMuMorphFunc : public RooLagrangianMorphFunc {
  MAKE_ROOLAGRANGIANMORPH(Func,RooHCvbfMuMuMorphFunc)
  ClassDefOverride(RooHCvbfMuMuMorphFunc,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("vbfMuMu");
    this->setup(kappas,RooLagrangianMorphing::makeHCVBFCouplings(kappas),RooLagrangianMorphing::makeHCHllCouplings(kappas),true);
  }    
};
class RooHCvbfMuMuMorphPdf : public RooLagrangianMorphPdf {
  MAKE_ROOLAGRANGIANMORPH(Pdf,RooHCvbfMuMuMorphPdf)
  ClassDefOverride(RooHCvbfMuMuMorphPdf,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("vbfMuMu");
    this->setup(kappas,RooLagrangianMorphing::makeHCVBFCouplings(kappas),RooLagrangianMorphing::makeHCHllCouplings(kappas),true);
  }    
};

#ifndef __CINT__
ClassImp(RooHCggfWWMorphFunc)
ClassImp(RooHCvbfWWMorphFunc) 
ClassImp(RooHCggfZZMorphFunc) 
ClassImp(RooHCvbfZZMorphFunc) 
ClassImp(RooHCvbfMuMuMorphFunc) 
ClassImp(RooHCggfWWMorphPdf)
ClassImp(RooHCvbfWWMorphPdf) 
ClassImp(RooHCggfZZMorphPdf) 
ClassImp(RooHCvbfZZMorphPdf) 
ClassImp(RooHCvbfMuMuMorphPdf) 
#endif


////////////////////////////////////////////////////////////////////////////////////////////////
// DERIVED CLASSES to implement specific PHYSICS ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class RooSMEFTggfWWMorphFunc : public RooLagrangianMorphFunc {
  MAKE_ROOLAGRANGIANMORPH(Func,RooSMEFTggfWWMorphFunc)
  ClassDefOverride(RooSMEFTggfWWMorphFunc,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("ggfWW");
    this->setup(kappas,RooLagrangianMorphing::makeSMEFTggFCouplings(kappas),RooLagrangianMorphing::makeSMEFTHWWCouplings(kappas),true);
  }
};
class RooSMEFTggfWWMorphPdf : public RooLagrangianMorphPdf {
  MAKE_ROOLAGRANGIANMORPH(Pdf,RooSMEFTggfWWMorphPdf)
  ClassDefOverride(RooSMEFTggfWWMorphPdf,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("ggfWW");
    this->setup(kappas,RooLagrangianMorphing::makeSMEFTggFCouplings(kappas),RooLagrangianMorphing::makeSMEFTHWWCouplings(kappas),true);
  }
};

class RooSMEFTvbfWWMorphFunc : public RooLagrangianMorphFunc {
  MAKE_ROOLAGRANGIANMORPH(Func,RooSMEFTvbfWWMorphFunc)
  ClassDefOverride(RooSMEFTvbfWWMorphFunc,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("vbfWW");
    this->setup(kappas,RooLagrangianMorphing::makeSMEFTVBFCouplings(kappas),RooLagrangianMorphing::makeSMEFTHWWCouplings(kappas),true);
  }
};
class RooSMEFTvbfWWMorphPdf : public RooLagrangianMorphPdf {
  MAKE_ROOLAGRANGIANMORPH(Pdf,RooSMEFTvbfWWMorphPdf)
  ClassDefOverride(RooSMEFTvbfWWMorphPdf,1)
  protected:
  void makeCouplings(){
    RooArgSet kappas("vbfWW");
    this->setup(kappas,RooLagrangianMorphing::makeSMEFTVBFCouplings(kappas),RooLagrangianMorphing::makeSMEFTHWWCouplings(kappas),true);
  }
};

#ifndef __CINT__
ClassImp(RooSMEFTggfWWMorphFunc)
ClassImp(RooSMEFTvbfWWMorphFunc) 
ClassImp(RooSMEFTggfWWMorphPdf)
ClassImp(RooSMEFTvbfWWMorphPdf) 
#endif

#endif

