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

	string queryC, queryN;
	
	if (!gSettings.LoadFromFile("/usr/lib/qgis/plugins/settings.xml"))
		return 0;

	if(!gDb.Connect ("postgres", "postqrap"))
	{
		cout << "Database connect failed:" << endl;
		return 0;
	}

	cout << "Connected " << endl;

  	queryC = "update coefficients set coefficient=0.0;";
	if (!gDb.PerformRawSql(queryC))
	{
		cout << "Error clearing coefficients" << endl;
	}

  	queryN = "update qrap_config set value='false' where name = 'UseAntANN';";
	if (!gDb.PerformRawSql(queryN))
	{
		cout << "Error updating qrap_config" << endl;
	}

	cGPpropModel GP;
	GP.mainTuning();
    	return 0;
}


