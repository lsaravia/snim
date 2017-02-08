/**
  \file   snim.h
  \brief  Stochastic network interaction model class definition
 */
#ifndef SNIM_CLASS_HH_
#define SNIM_CLASS_HH_

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <random>
#include <initializer_list>
#include <fstream>
#include <string>
#include <cerrno>
#include "matrix.h"

namespace snim {

struct SimulationParameters {
    size_t rndSeed=0;               // Seed of the random generator
    size_t nEvals=0;                // number of evaluations steps
    float tau=0;                   // Tau method steps  
    size_t nSteps = nEvals*tau;     // total number of steps

};

class SnimModel {

    // Model parameters 
    //
    
    matrix <float> omega;       // Interaction matrix
    std::vector<float> e;       // extinction vector
    std::vector<float> u;       // inmigration parameter 
    std::vector<float> nIni;       // Initial Population values
    size_t totSize=0;           // Total size of the community

    // Simulation Parameters 
    SimulationParameters simPar;

    
public:

  SnimModel(size_t nsp) : omega(nsp+1,nsp+1), e(nsp),u(nsp), nIni(nsp+1) {}
  
  SnimModel(const SnimModel& s) : omega(s.omega), e(s.e),u(s.u), nIni(s.nIni), simPar(s.simPar) {}

  SnimModel& operator=(const SnimModel& s){
    if(this == &s )
        return *this;
    assert(omega.size()== s.omega.size());
    omega = s.omega;
    e = s.e;
    u = s.u;
    nIni = s.nIni;
    simPar = s.simPar;
    return *this;
  }
  
  /**
  \brief Set Interaction matrix in colum-wise order
  */
  void SetOmega( std::initializer_list<float> const& om) {
//      matrix<float> temp(omega.rows(),omega.cols(),om);  
//      omega = temp;
      omega = om;
  };
  
  /**
  \brief Set Extinction vector 
  */
    void SetExtinction(std::initializer_list<float> const& om){
        assert(e.size()==om.size());
      e = om;
  };
  
  /**
  \brief Set Inmigration vector 
  */
  void SetInmigration(std::initializer_list<float> const& om){
      assert(u.size()==om.size());
      u = om;
  };
  
  /**
  \brief Set Initial value of species' populations  
  */
  void SetInitialN(std::initializer_list<float> const& om ){
    using namespace std;
    assert((nIni.size()-1)==om.size());
    copy(begin(om),end(om),begin(nIni)+1);
  };
  
  void SetTotalSize( const float & t) { totSize=t;}
  
  std::string ReadSimulParams(const char *filename);
  std::string ReadModelParams(const char *filename);

  void SimulTauLeap(const SimulationParameters & sp, matrix<size_t> & N );
  
  friend std::ostream& operator<<(std::ostream& os,  const SnimModel& s);
};

template<typename T>
std::ostream &operator <<(std::ostream &os, const std::vector<T> &v) {
    
    os << "[" << v[0];
    for (auto r=1u; r<v.size(); ++r)
    {
        os << ", " << v[r];
    }
    os << "]\n";

   return os;
}

std::ostream& operator<<(std::ostream& os, const SnimModel& s) {
  os << "[Omega]\n" << s.omega << std::endl;
  
  os << "[Extinction]\n";
  os << s.e << std::endl;
  
  os << "[Immigration]\n";
  os << s.u << std::endl;

  os << "[Initial population]\n";
  os << s.nIni << std::endl;
  
  os << "[Total Size]\n[";
  os << s.totSize << "]\n";
  
  return os;
};

void SnimModel::SimulTauLeap(const SimulationParameters& sp, matrix<size_t>& N){
    
};

std::string SnimModel::ReadSimulParams(const char *filename) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
};

std::string SnimModel::ReadModelParams(const char *filename) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
};


} /* end namespace */

#endif