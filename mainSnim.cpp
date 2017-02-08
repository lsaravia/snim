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
  
SnimModel mdl(3);

cout << mdl;

mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
               0.0, 0.0, 0.2, 0.3,
               0.1, 1.0, 0.0, 0.3,
               0.1, 1.0, 0.2, 0.0} );
mdl.SetExtinction({0.1,0.1,0.1});
mdl.SetInmigration({0.1,0.1,0.1});
mdl.SetTotalSize(10000);
mdl.SetInitialN({1000,1000,1000});        

cout << mdl;

SimulationParameters sp = {1234,100,0.01};

matrix <size_t> out(3,100);

mdl.SimulTauLeap(sp,out);

  // Read model parameter file 
  // Reaf simulations parameter file 

  // Run the model

  return 0;
}