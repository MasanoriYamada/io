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


using namespace std;

static const int T_in=9;
static const int T_fi=9;
static const int XnodeSites =16;
static const int YnodeSites =16;
static const int ZnodeSites =16;
static const int TnodeSites =32/2;
static const int binsize=1;
static const int localConf=700;
static const int Tshiftsize=1;
static const double ascale = 0.1209;
static const double hbarc = 197.327;
static const double ToMev = hbarc/ascale;

//set in out info

std::string inPath = "/Users/SINYAMADA/lab/data/results/bin1/Potential/binPotential/xyz";
std::string outPath = "../out";
std::string physInfo = "RC16x32_B1830Kud013760Ks013710C17610.kap_013710";
std::string inStaticsInfo = "Potential";
std::string outStaticsInfo = "param";
bool inBinary = true;
bool outBinary = false;


inline void reScale(COMPLEX* pot){
    for(int id = 0; id < XYZnodeSites; id++){
        pot[id] = ToMev*pot[id];
    }
}


main(){


    
  IODATA inPot;
  inPot.setReadBinaryMode(inBinary);
  inPot.setWriteBinaryMode(outBinary);
  inPot.setConfSize(binnumber);


  for(int iT=T_in  ; iT< T_fi +1  ; iT++ ){
    JACK jackPot(Confsize,binsize,XYZnodeSites);
    COMPLEX* binPot = new COMPLEX[XYZnodeSites*binnumber];
    for (int iconf=0; iconf< binnumber; iconf++) {
      COMPLEX* pot = new COMPLEX[XYZnodeSites];
      inPot.callData(pot,inPath,inStaticsInfo,physInfo,iconf,iT);
            reScale(pot);
	    memcpy(binPot + iconf*XYZnodeSites, pot, sizeof(pot)*XYZnodeSites*2);    
	               if(iconf == 0){      for(int ixyz = 0;ixyz<XYZnodeSites;ixyz++){ cout<<iconf<<" "<<ixyz<<" "<<pot[ixyz]<<endl;}}
      jackPot.setBinData(pot,iconf);
      delete [] pot;
    }
    
    double* err= new double[XYZnodeSites]; 
    double* ave= new double[XYZnodeSites]; 
        err= jackPot.calcErr();
    ave= jackPot.calcAve();
    //    jackPot.aveErr(ave,err);
        for(int ixyz = 0;ixyz<XYZnodeSites;ixyz++){cout<<"ave "<<ixyz<<" "<<ave[ixyz]<<" "<<err[ixyz]<<endl;}
     inPot.outErr(ave,err,outPath,outStaticsInfo,physInfo,700,iT,XYZnodeSites);
    }//iconf  
    
  }//It
  cout <<"@End all jobs"<<endl; 
}
