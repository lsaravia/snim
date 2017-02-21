// 
// Sochastic Network Interaction Model
//
#include "snim.h"

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " SimulationParameterFile ModelParameterFile\n"
              << std::endl;
}


int main(int argc, char* argv[]){
  using namespace std;
  using namespace snim;

//	if (argc < 3) {
//	    show_usage(argv[0]);
//	    return 1;
//	}


std::cout << "3 species 1 Predator 2 prey" << std::endl;
std::cout << "Initial densities [0,0,0]" << std::endl;
SnimModel mdl(3,10000);
SimulationParameters sp = {1234,1,0.01};
matrix <size_t> out;

//mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
//               0.0, 0.0, 1.0, 1.0,
//               1.0, 0.0, 0.0, 0.3,
//               1.0, 0.0, 0.2, 0.0} );
//mdl.SetExtinction({0.5,0.5,0.5});
//mdl.SetInmigration({0.0,0.0,0.0});
//mdl.SetInitialN({0,0,0});        
//mdl.SimulTauLeap(sp,out);
//
//cout << mdl;
//cout << "All Species must have 0 individuals" ;
//cout << out.column(1) << endl;
//
//
//
//mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
//               0.0, 0.0, 2.0, 0.0,
//               4.0, 0.0, 0.0, 0.0,
//               1.7, 0.0, 0.0, 0.0} );
//mdl.SetExtinction({0.0,0,0.0});
//mdl.SetInmigration({0.0,0.0,0.0});
//mdl.SetInitialN({2000,4000,4000});        
//
//sp = {1234,100,0.01};
//mdl.SimulTauLeap(sp,out);
//cout << mdl;
//cout << "Species 2 must have 0 individuals";
//cout << out.column(100) << endl;

// Predator eats all prey - never dies
//
//mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
//               0.0, 0.0, 2.0, 2.0,
//               4.0, 0.0, 0.0, 0.0,
//               1.7, 0.0, 0.0, 0.0} );
//mdl.SetExtinction({0.0,0,0.0});
//mdl.SetInmigration({0.0,0.0,0.0});
//mdl.SetInitialN({2000,4000,4000});     

mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 3.0, 2.0,
                4.0, 0.0, 0.0, 0.0,
                2.0, 0.0, 0.0, 0.0} );
mdl.SetExtinction({1.0,2.0,1.0});
mdl.SetInmigration({0.0,0.0,0.0});
mdl.SetInitialN({200,400,400});        


cout << mdl;

sp = {1234,100,0.01};
mdl.SimulTauLeap(sp,out);
cout << mdl;
cout << out;

cout << "Species 3 must have 0 individuals";
cout << out.column(100) << endl;

    // Read model parameter file 
  // Reaf simulations parameter file 

  // Run the model
  return 0;
}