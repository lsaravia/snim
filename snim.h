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
#include <sstream>
#include <cerrno>
#include <utility>

#include "matrix.h"

namespace snim {

struct SimulationParameters {
    size_t rndSeed=0;               // Seed of the random generator
    size_t nEvals=0;                // number of evaluations steps
    double tau=0;                   // Tau method steps  
    // size_t nSteps = nEvals*tau;     // total number of steps

};

class SnimModel {

    // Model parameters 
    //
    
    matrix <float> omega;       // Interaction matrix
    std::vector<float> e;       // extinction vector
    std::vector<float> u;       // inmigration parameter 
    std::vector<float> nIni;       // Initial Population values
    size_t communitySize=0;           // Total size of the community

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
  \brief Set Interaction matrix in row-wise order
  */
  void SetOmega( std::initializer_list<float> const& om) {
//      matrix<float> temp(omega.rows(),omega.cols(),om);  
//      omega = temp;
      if (om.size() != omega.size()) {
            std::ostringstream message;
            message << "Different no. of elements: "
                    << "expected " << omega.size() << ", "
                    << "got " << om.size() << ".";

            throw std::invalid_argument(message.str());
        }

   
      // Standard assign omega = om is column-wise so I have to do it manually
      // in row-wise order
      //
      auto it=begin(om); 
      for (auto i = 0; i<omega.rows(); ++i)
        for (auto j = 0; j<omega.cols(); j++)
            omega(i,j) = *it++;
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
  
  /**
  \brief Set the size of the community species populations i.e. the maximun value of the sum of species pop  
  */
  void SetCommunitySize( const float & t) { communitySize=t;}
  
  std::string ReadSimulParams(const char *filename);
  std::string ReadModelParams(const char *filename);

  /**
  \brief Simulate the model using the Tau-leap method   
  */
  void SimulTauLeap(const SimulationParameters & sp, matrix<size_t> & N );
  
  friend std::ostream& operator<<(std::ostream&,  const SnimModel&);
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


} /* end namespace */

#endif