/*
 * Copyright 2017 Leonardo A. Saravia <lsaravia@ungs.edu.ar>.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iterator>
#include "snim.h"
#include "configfile.h"

namespace snim{ 

/// Simulation of the model using the TauLeap method
///
/// \param sp = Parameters of the simulations
/// \param N  = Output of the model
    
void SnimModel::SimulTauLeap(const SimulationParameters& sp, matrix<size_t>& N){
    using namespace std;
    // if output matrix undefined define it with the correct dimensions
    // 
    if (omega.rows() != N.rows() || (sp.nEvals+1) != N.cols()){
        
        N.resize(omega.rows(),sp.nEvals+1);
    }

    // Initialize output matrix with initial populations
    //
    N(0,0)=communitySize;
    if(sp.iniCond.size()==1)
        for( size_t i=1; i<N.rows(); ++i){
            N(i,0)= sp.iniCond[0];
            N(0,0)-=sp.iniCond[0];
        }
    else {
        if ((sp.iniCond.size()+1) != omega.rows()) {
            std::ostringstream message;
            message << "Different no. of species in Initial conditions: "
                    << "expected " << omega.rows()-1 << ", "
                    << "got " << sp.iniCond.size() << ".";

            throw std::invalid_argument(message.str());
        }

        for( size_t i=1; i<N.rows(); ++i){
             N(i,0)= sp.iniCond[i-1];
             N(0,0)-=sp.iniCond[i-1];
        }
    }
    // Setup random number generator with random seed
    // 
    auto rng = std::mt19937_64(sp.rndSeed);
    if(sp.rndSeed==0) {
        std::random_device rd{};
        rng.seed(rd());
    }
   
    // Number of steps for each model evaluation 
    auto nSteps = 1.0 / sp.tau;
    
    // Number of species
    auto nSpecies = omega.rows();
    
    // Change in number of individuals for each event
    //
    matrix<size_t> intDelta(nSpecies,nSpecies,0ul);
    
    // Vector with only the non-zero elements of the matrix
    //
    vector< pair<size_t,size_t>> actualInteractions;
            
    for(auto s=1; s<nSpecies; ++s)
            for( auto r=0; r<nSpecies; ++r)
                if(omega(s,r)>0) {
                    actualInteractions.emplace_back(s,r);//  
                } 

    // Vectors for extinction and immigration 
    //
   

    // Simulate the model - Calculate the transitions with poison random numbers
    //
    for (auto y = 0; y < (sp.nEvals) ; ++y){

        // Initialize the internal matrix with N
        matrix <long long int> S(nSpecies,1);
        for(auto i=0u; i<S.rows(); ++i)
            S(i)=N(i,y);
        
        for(auto n=0; n < nSteps; ++n) {
            
//            cout << endl << "Inner Steps: " << n << endl;    
 
            // Calculate interactions between species 
            //
            // Species 0 is the empty space
            //
            for(auto t:actualInteractions){
                auto s=t.first;
                auto r=t.second;
                double evRate =(omega(s,r)-omega(r,s))*S(s)*S(r)/communitySize;

//                cout << s << " - " << r << " - " << omega(s,r) << " - " << omega(r,s) << " - " << S(s) << " - " << S(r) << endl;
//                cout <<  evRate << " - "<< intDelta(s,r) << endl;

                if( evRate > 0.0 ){  
                    auto pois = std::poisson_distribution<size_t>(evRate*sp.tau);
                    intDelta(s,r) = pois(rng);
                }
                else
                    intDelta(s,r) = 0.0;
                    
                
            }
//            cout << "intDelta :" << intDelta << endl;
            
            long long int sumDelta = 0;
            // Calculate inmigration extinction and sum interactions
            //            
            for(auto s=1; s<nSpecies; ++s){
                
                // Calculate extinction
                double evRate = 0;
                evRate = S(s)*e[s-1];
                size_t exDelta = 0;
                if(evRate > 0.0) {
                    auto pois = std::poisson_distribution<size_t>(evRate*sp.tau);
                    exDelta= pois(rng);
                }
                
                // Calculate immigration 
                evRate = S(0)*u[s-1];
                size_t imDelta=0;
                if( evRate > 0.0) {
                    auto pois = std::poisson_distribution<size_t>(evRate*sp.tau);
                    imDelta= pois(rng);
                }
                // Calculate new population values
                long long int totDelta = imDelta - exDelta + intDelta.row_sum(s) - intDelta.col_sum(s);
                if( S(s)+totDelta >0){
                    S(s) += totDelta;
                }
                else
                    S(s) =0;
                
                sumDelta += totDelta;
//                cout <<"S: "<< S << endl;        
//                cout << s << " - " << imDelta << " - " << exDelta << " - " << intDelta.row_sum(s) << " - " << intDelta.col_sum(s) << " - " 
//                                << totDelta << " - " << endl;
            
                
            }
            
            if(S(0) - sumDelta < 0 )
                S(0) = 0;
            else
                S(0)-=sumDelta;

//            cout <<"S: "<< S << endl;        
        }

//        cout << "Outer Step: " << y+1 << endl; 
        for(auto i=0u; i<S.rows(); ++i){
            N(i,y+1)=S(i);
//            cout << N(i,y+1) << endl;    
        }
    }
            
}
       
std::ostream& operator<<(std::ostream& os,  const SnimModel &  s) {
  os << "[Omega]\n" << s.omega << std::endl;
  
  os << "[Extinction]\n";
  os << s.e << std::endl;
  
  os << "[Immigration]\n";
  os << s.u << std::endl;

  os << "[Number of Species, Total Size]\n[";
  
  os << s.nSpecies << ", ";
  os << s.communitySize << "]\n";
  
  return os;
}


std::ostream& operator<<(std::ostream& os,  const SimulationParameters&  s) {
    
  os << "[Number of Evaluations, Random Seed, Tau]\n[" << s.nEvals << ", " << s.rndSeed << ", " 
          << s.tau << "]\n" << std::endl;
  
  os << "[Initial populations]\n" << s.iniCond << std::endl;
  
  return os;
}

/// Read simulation parameters from configuration file 
///
SimulationParameters::SimulationParameters(const std::string &fName){
    
    ConfigFile cfg(fName);
    rndSeed = cfg.getValueOfKey<size_t>("rndSeed");
    
    nEvals  = cfg.getValueOfKey<size_t>("nEvals"); // number of evaluations steps
    
    tau     = cfg.getValueOfKey<double>("tau");
    
    if( cfg.keyExists("iniCond")){
        auto iniCondStr = cfg.getValueOfKey<std::string>("iniCond");
        std::istringstream strline(iniCondStr);
        size_t tempd=0;
        while (strline >> tempd)    // Will read up to eof()
                iniCond.push_back(tempd);
     
      
    }

} 

/// Read model parameters from file 
///
/// File structure by line:
///
/// NumberOfSpecies CommunitySize
/// <inmigration rates> Vector of NumberOfSpecies 
/// <Extinction rates> Vector of NumberOfSpecies 
/// <Interaction matrix> Matrix of NumberOfSpecies+1 by NumberOfSpecies+1  
///
void SnimModel::ReadModelParams(const std::string &fName) {
    ConfigFile cfg;
    
    std::ifstream file;
    file.open(fName.c_str());
    if (!file)
            exitWithError("Model Parameters File [" + fName + "] couldn't be found!\n");

    std::string line;
    size_t lineNo = 0;
    while (std::getline(file, line)) {
        std::string temp = line;

        if (temp.empty())
                continue;

        cfg.removeComment(temp);
        if (cfg.onlyWhitespace(temp))
                continue;
        lineNo++;

        ReadModelParamsLine(temp, lineNo);
    }

}

/// Auxiliary function of ReadModelParams extract values from lines
///
/// \param line String with the line to be extracted
/// \param lineNo Number of the line with information comments lines are skipped
///
void SnimModel::ReadModelParamsLine(const std::string& line, const size_t lineNo){
    using namespace std;
    istringstream strline(line);
    double tempd=0;
    switch(lineNo){
        case 1:
            strline >> nSpecies;
            strline >> communitySize;
            break;
            
        case 2:                         // Read immigration vector
            u.reserve(nSpecies);
            while (strline >> tempd) // Will read up to eof()
                u.push_back(tempd);
            if (u.size() != nSpecies) {
                std::ostringstream message;
                message << "Wrong length of Immigration vector: "
                        << "expected " << nSpecies << ", "
                        << "got " << u.size() << ".";

                throw std::invalid_argument(message.str());
            }
            break;
            
        case 3:                         // Read extinction vector
            e.reserve(nSpecies);
            while (strline >> tempd)    // Will read up to eof()
                e.push_back(tempd);
            
            if (e.size() != nSpecies) {
                std::ostringstream message;
                message << "Wrong length of Extinction vector: "
                        << "expected " << nSpecies << ", "
                        << "got " << e.size() << ".";

                throw std::invalid_argument(message.str());
            }

            break;
            
       default:                        // Read a line of the interaction matrix omega 
           if(omega.size()!= (nSpecies+1)*(nSpecies+1) )
           {
               omega.resize(nSpecies+1,nSpecies+1);
           }

           auto row=lineNo-4; 
           if(row < omega.rows())
              for( auto i=0u; i<omega.cols(); ++i ){
                strline >> tempd;
                omega(row,i)= tempd;
                if(strline.eof()) break; 
              } 
    }
}

} // end namespace