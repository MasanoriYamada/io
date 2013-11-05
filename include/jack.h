//--------------------------------------------------------------------------
/**
 * @File io.h
 * @brief data in out class (text and binary)
 * @ingroup YAMADA
 * @author  M.YAMADA 
 * @date    Thu Jun 13 22:10:03 2013
 */
//--------------------------------------------------------------------------
#ifndef JACK_CALC_YAMADA_20130613
#define JACK_CALC_YAMADA_20130613

#include<iostream>
#include<complex>
#include<memory.h>

class JACK{
 public:
  JACK();
  ~JACK();
  void		set(int confSize, int binsize, int DataSize);
  double*	calcAve();
  double*	calcErr();
  void		aveErr(double* ave,double* err);
 private:
  void		makeBin();
  void		jackAveCalc();
  void		jackErrCalc();
  void		checkErr();

 public:
  template <typename DATA>
    void	setData(DATA in, int iconf);
  void		setData(std::complex<double>* in, int iconf);
  template <typename DATA>
    void	setBinData(DATA in, int iconf);
  void		setBinData(std::complex<double>* in, int iconf);
 private:
  inline double confData(int id, int j){
    return ConfData[(id) + dataSize *(j)];
      }
  inline double binData(int id, int b){
    return BinData[(id) + dataSize *(b)];
      }
  inline double doubleBinData(int id, int b){
    return DoubleBinData[(id) + dataSize *(b)];
      }
  double* ConfData;
  double* BinData;
  double* DoubleBinData;
  double* ave_;
  double* err_;
  double* doubleAve_;

  int Confsize;
  int binsize;
  int binnumber;
  int dataSize;

};
//constracta
JACK::JACK(){
  binsize   = 0;
  Confsize  = 0;
  dataSize  = 0;
  binnumber = 0;
}
//destracta
JACK::~JACK(){
  delete [] DoubleBinData;
  delete [] doubleAve_;
  delete [] BinData;
  delete [] ConfData;
  delete [] ave_;
  delete [] err_;
}

void JACK::set(int confSize, int BinSize, int DataSize){
  binsize	= BinSize;
  Confsize	= confSize;
  dataSize	= DataSize;
  binnumber	= confSize/binsize;
  ConfData	= new double[Confsize*dataSize]();
  BinData	= new double[binnumber*dataSize]();
  DoubleBinData = new double[binnumber*dataSize]();
  doubleAve_	= new double[dataSize]();
  ave_		= new double[dataSize]();
  err_		= new double[dataSize]();
}

double* JACK::calcAve(){
  jackAveCalc();
  return ave_;
}
double* JACK::calcErr(){
  jackAveCalc();
  jackErrCalc();
  return err_;
}
void JACK::aveErr(double* Ave, double* Err){
  Ave = calcAve();
  Err = calcErr();
}
template <typename DATA>     void JACK::setData(DATA in, int iconf){
  checkErr();
  double* tmp =new double[dataSize]();
  for(int id = 0; id <dataSize; id++){
    tmp[id] = (double)in[id];
}
  memcpy(ConfData + iconf*dataSize,tmp,sizeof(tmp) * dataSize);
  delete[] tmp;
  makeBin();
}

void JACK::setData(std::complex<double>* in, int iconf){
  checkErr();
  double* tmp = new double[dataSize]();
  for(int id = 0; id <dataSize; id++){
    tmp[id] = (double)in[id].real();
}
  memcpy(ConfData + iconf*dataSize,tmp,sizeof(tmp)*dataSize);
  delete [] tmp;
  makeBin();
}
template <typename DATA>     void JACK::setBinData(DATA in, int iconf){
  checkErr();
  double* tmp = new double[dataSize]();
  for(int id = 0; id <dataSize; id++){
    tmp[id] = (double)in[id];
}
  memcpy(BinData + iconf*dataSize,tmp,sizeof(tmp)*dataSize);
  delete[] tmp;
}

void JACK::setBinData(std::complex<double>* in, int iconf){
  checkErr();
  double*	tmp = new double[dataSize]();
  for(int id = 0; id <dataSize; id++){
    tmp[id] = (double)in[id].real();
}
  memcpy(BinData + iconf*dataSize,tmp,sizeof(tmp)*dataSize);
  delete[] tmp;
}

void JACK::makeBin(){
  for(int id = 0; id<dataSize; id++){
    double	full = 0.0;
    for (int j=0; j<Confsize; j++) {
      full=full+confData(id,j);
    }
    for (int b=0; b<binnumber; b++) {
      double subfull[Confsize];
      double localfull=0.0;
      for (int j=b*binsize; j<(b+1)*binsize; j++) {
	localfull = localfull + confData(id,j);
      }
      subfull[b] = full - localfull;
      BinData[(id) + dataSize *(b)] = subfull[b]/((double)Confsize-(double)binsize);
    }
  }
}
void JACK::jackAveCalc(){
  for(int id = 0; id<dataSize; id++){
    
    double	buffer = 0.0;
    for (int b=0; b<binnumber; b++) {
      buffer = buffer+binData(id,b);
    }
    ave_[id] = buffer/(double)binnumber;
     for (int b=0; b<binnumber; b++) {
      DoubleBinData[id + dataSize *b] = binData(id,b)*binData(id,b);
    }

    double	buffer1 = 0.0;
    for (int b=0; b<binnumber; b++) {
      buffer1 = buffer1+doubleBinData(id,b);
    }
    doubleAve_[id] = buffer1/(double)binnumber;
  }
}
void JACK::jackErrCalc(){
  for(int id = 0; id<dataSize; id++){
    err_[id] = sqrt(((double)binnumber -1.0)*((doubleAve_[id])-(ave_[id])*(ave_[id])));
  }
}
void JACK::checkErr(){
  if(  binsize == 0 || Confsize==0 || dataSize==0)
    {
      std::cout <<"ERR set jack info by using set(int confSize, int binsize, int DataSize); "<<std::endl;
    }
}


#endif
