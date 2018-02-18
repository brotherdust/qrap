/********************************************************
 *
 * Sample GP program
 *
 *  Edited by Magdaleen Ballot 2018-02-13
 *
*********************************************************/
			
#include "cGPpropModel.h"

using namespace Qrap;
using namespace std;

int main (int argc, char * const argv[]) 
{
	cout << "Before constructing GP in main" << endl;
	cGPpropModel GP;
	GP.mainTuning();
    	return 0;
}


