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
#include<vector>
#include<algorithm>

class JACK{
 public:
  JACK();
  ~JACK();
  void		set(int confSize, int binsize, int DataSize);
  void	        calcAve(double* );
  void	        calcErr(double* );
  void		aveErr(double* ave,double* err);
  void          percentileErrCalc(double* errMin, double* errMax, const double percent ); //percent 0~1
  void          makeBinCalc(double*);

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
  bool aveInit_;

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
  aveInit_ = false;
}
//destracta
JACK::~JACK(){
  delete [] DoubleBinData; DoubleBinData = NULL;
  delete [] doubleAve_; doubleAve_ = NULL;
  delete [] BinData; BinData = NULL;
  delete [] ConfData; ConfData = NULL;
  delete [] ave_; ave_ = NULL;
  delete [] err_; err_ = NULL;
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

void JACK::calcAve(double* out_ave){
  jackAveCalc();
  memcpy(out_ave,ave_,sizeof(*ave_) * dataSize);
}
void JACK::calcErr(double* out_err){
  if(!aveInit_) jackAveCalc();
  jackErrCalc();
  memcpy(out_err,err_,sizeof(*err_) * dataSize);
}
void JACK::aveErr(double* Ave, double* Err){
  calcAve(Ave);
  calcErr(Err);
}
void JACK::makeBinCalc(double* BinData_out)
{
  makeBin();
  memcpy(BinData_out,BinData,sizeof(*BinData) * dataSize * binnumber);
}
template <typename DATA>     void JACK::setData(DATA in, int iconf){
  checkErr();
  double* tmp =new double[dataSize]();
  for(int id = 0; id <dataSize; id++){
    tmp[id] = (double)in[id];
}
  memcpy(ConfData + iconf*dataSize,tmp,sizeof(*tmp) * dataSize);
  delete[] tmp;
  if(Confsize ==iconf + 1)  makeBin();
}

void JACK::setData(std::complex<double>* in, int iconf){
  checkErr();
  double* tmp = new double[dataSize]();
  for(int id = 0; id <dataSize; id++){
    tmp[id] = (double)in[id].real();
}
  memcpy(ConfData + iconf*dataSize,tmp,sizeof(*tmp)*dataSize);
  delete [] tmp;
  if(Confsize ==iconf + 1)  makeBin();
}
template <typename DATA>     void JACK::setBinData(DATA in, int iconf){
  checkErr();
  double* tmp = new double[dataSize]();
  for(int id = 0; id <dataSize; id++){
    tmp[id] = (double)in[id];
}
  memcpy(BinData + iconf*dataSize,tmp,sizeof(*tmp)*dataSize);
  delete[] tmp;
}

void JACK::setBinData(std::complex<double>* in, int iconf){
  checkErr();
  double*	tmp = new double[dataSize]();
  for(int id = 0; id <dataSize; id++){
    tmp[id] = (double)in[id].real();
}
  memcpy(BinData + iconf*dataSize,tmp,sizeof(*tmp)*dataSize);
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
  aveInit_ = true;
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
void JACK::percentileErrCalc(double* errMin, double* errMax, const double percent)
{
  if (percent < 0 || 1<percent)
  {
    std::cout <<"ERR percent should 0~1 percent = "<<percent <<std::endl;
  }
  if(!aveInit_) jackAveCalc();

  int threshold = round(0.5 * (double)((binnumber - 1.0) * (1.0 - percent))); //cut down
  double factor = sqrt((double)binnumber -1.0);

  for(int id =0; id<dataSize; id++)
  {
    std::vector<double> binArray;
    for(int b =0; b<binnumber; b++)
    {
      binArray.push_back(binData(id,b));
    }
    std::sort(binArray.begin(),binArray.end());
    //debug
    //if(id ==0){
    //  for (int b = 0 ; b < (int)binArray.size(); b++)
    //  {
    //    printf("%d %1.16e\n", b, binArray[b]);
    //  }
    //  printf("%d %1.16e\n",threshold, binArray[threshold]);
    //  printf("%d %1.16e\n", binnumber -1 -threshold, binArray[binnumber -1 -threshold]);

    //std::cout <<(threshold)<<"\t"<<binArray[threshold]<<std::endl;
    //std::cout <<(binnumber - 1 - threshold)<<"\t"<<binArray[binnumber - threshold]<<std::endl;
    //std::cout <<factor*(binArray[threshold] - ave_[id])<<std::endl;
    //std::cout <<factor*(binArray[binnumber - 1 - threshold] - ave_[id])<<std::endl;
    //}
    errMin[id] = factor*(binArray[threshold] - ave_[id]) + ave_[id];
    errMax[id] = factor*(binArray[binnumber - 1 - threshold] - ave_[id]) + ave_[id];
  }
}

#endif
