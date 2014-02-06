//--------------------------------------------------------------------------
/**
 * @Filex main_fit_pot3g.cpp
 * @brief fiting potential 3 gauss
 * @ingroup YAMADA
 * @author  M.YAMADA * @date    Sat Jun 13 22:09:45 2013
 */
//--------------------------------------------------------------------------

#include "../include/io.h"
#include "../include/jack.h"
#include <complex>
#include <string>
#include <iostream>

using namespace std;
typedef complex<double> COMPLEX;




static const int T_in=7;
static const int T_fi=7;
static const int XnodeSites =16;
static const int YnodeSites =16;
static const int ZnodeSites =16;
static const int TnodeSites =32/2;
static const int binsize=1;
static const int Confsize=700;
static const int binnumber = Confsize / binsize;
static const int DataSize = XnodeSites*YnodeSites*ZnodeSites;
//set in out info

std::string inPath = "../debug/in";
std::string outPath = "../debug/out";
std::string physInfo = "RC16x32_B1830Kud013760Ks013710C17610";
std::string inStaticsInfo = "OmgOmgwave";
std::string outStaticsInfo = "jack";

bool inBinary = true;
bool outBinary = false;

#define radius(x,y,z) sqrt(((x)*(x) + (y)*(y) + (z)*(z)))
#define min(a,b) (((a) < (b)) ? (a) : (b))


int main(){


    
  IODATA inPot;
  inPot.setReadBinaryMode(inBinary);
  inPot.setWriteBinaryMode(outBinary);
  inPot.setConfSize(Confsize);

  double* xdata = new double[DataSize]();
    double* err= new double[DataSize]; 
    double* ave= new double[DataSize]; 

  for(int ix =0; ix<XnodeSites; ix++){
    for(int iy =0; iy<YnodeSites; iy++){
      for(int iz =0; iz<ZnodeSites; iz++){
	xdata[ix+XnodeSites*(iy+YnodeSites*iz)] = radius( min(ix,XnodeSites-ix), min(iy,YnodeSites-iy), min(iz,ZnodeSites-iz) );;
      }
    }
  }

  for(int iT=T_in  ; iT< T_fi +1  ; iT++ ){
    {
      JACK jackPot;
    jackPot.set(Confsize, binsize, DataSize);
    for (int iconf=0; iconf< Confsize; iconf++) {
      COMPLEX* ydata = new COMPLEX[DataSize];
      inPot.callData(ydata,1,inPath,inStaticsInfo,physInfo,iconf,iT);
	               if(iconf == 0){      for(int ixyz = 0;ixyz<DataSize;ixyz++){ cout<<iconf<<" "<<ixyz<<" "<<ydata[ixyz]<<endl;}}
      jackPot.setData(ydata,iconf);
      delete [] ydata;
    }
    

        jackPot.calcErr(err);
	jackPot.calcAve(ave);
    }
	//    jackPot.aveErr(ave,err);
        for(int ixyz = 0;ixyz<DataSize;ixyz++){cout<<"ave "<<ixyz<<" "<<ave[ixyz]<<" err "<<err[ixyz]<<endl;}
	inPot.outErr(xdata,ave,err,outPath,outStaticsInfo,physInfo,binnumber,iT,DataSize);

  }//It
    delete [] err;
    delete [] ave;
    delete [] xdata;
cout <<"@End all jobs"<<endl; 
return 0;
}
