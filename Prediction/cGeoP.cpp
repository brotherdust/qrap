 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cGeoP.cpp
 *    Copyright   : (c) University of Pretoria & Meraka Institute
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Represents basic point location
 *
 **************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; See the GNU General Public License for      *
 *   more details                                                          *
 *                                                                         *
 ************************************************************************* */

#include "cGeoP.h"

using namespace Qrap;
using namespace std;

const double rd = PI/180.0; //converts degrees to radials
const double a = 6378137.0; // WGS84 ellipsiod
const double b = 6356752.31424518; // WGS-84 ellipsiod
const double f = 1.0/298.257223563;  // computed from WGS84 earth flattening coefficient definition
//	const long double a = 6378249.145;           /*Clarke 1880 spheroid */
//	const long double b = 6356514.96639;
const double c= a*a/b;              /* section 3.2-23 */
const double e2 = 1.0-b*b/(a*a);
const double eprime2 = a*a/(b*b)-1.0;
const double e4= e2*e2;
const double e6= e4*e2;
const double m1 = a*(1.-0.25*e2-3./64.*e4-5./256.*e6);
const double m2 = a*(-3./8.*(e2+0.25*e4+15./128.*e6));
const double m3 = a*(15./256.*(e4+0.75*e6));
const double m4 = a*(-35./3072.*(e6));
const double n  = (a-b)/(a+b);  /* section 3.2-24 *///e1
const double n2 = n*n;
const double n3 = n2*n;
const double n4 = n2*n2;
const double n5 = n3*n2;
const double G  = a*(1.-n)*(1.-n2)*(1.+9./4.*n2+225./64.*n4)*PI/180.; /* 3.61-37 */
const double phi1  = 3./2.*n-27./32.*n3;
const double phi2  = 21./16.*n2-55./32.*n4;
const double phi3  = 151./96.*n3;
const double phi4  = 1097./512.*n4;
const double sinpp = sinl(PI/180.0/60.0/60.0);


//*********************************************************************
cGeoP::cGeoP()
{
	mLat = -26.00;
	mLon = 28.00;
	mType = DEG;
	DefaultCentMer(WGS84GC);
	mSouth = true;
}

//*********************************************************************
cGeoP::cGeoP(const cGeoP &right)
{
	mLat = right.mLat;
	mLon = right.mLon;
	mType = right.mType;
	mCentMer = right.mCentMer;
	mSouth = right.mSouth;
}

//*********************************************************************
cGeoP::cGeoP(double lat, double lon, GeoType type, int centMer, bool South)
{
	mLat = lat;
	mLon = lon;
	mType = type;
	mSouth = South;
	
	if ((mType==DEG)||(mType==NDEF))
	{
		mSouth = (mLat<=0.0);
		DefaultCentMer(WGS84GC);
	}
	else 	mCentMer = centMer;
}

//*********************************************************************
cGeoP::~cGeoP()
{
}

//*********************************************************************
bool cGeoP::Get(double &lat, double &lon)
{
	lat = mLat;
	lon = mLon;
	if (!(mType==DEG))
	{
		cout << "This overloaded Get assumes that the coordinate is in Degrees" << endl;
		cout << " it would seem that this is not the case ... ";
		cout << " hope you know what you are doing ..." << endl;
		return false;
	}
	return true; 
}

//***********************************************************************
bool cGeoP::Get(double &lat, double &lon, GeoType &type, int &centMer, bool &Hemisphere)
{
	lat = mLat;
	lon = mLon;
	type = mType;
	centMer = mCentMer;
	Hemisphere = mSouth;
	return true;
}

//***********************************************************************
void cGeoP::Set(double lat, double lon, GeoType type, int centMer, bool South)
{
	mLat = lat;
	mLon = lon;
	mType = type;
	mSouth = South;
	if (mType==DEG)
	{
		mSouth = (mLat<=0.0);
		DefaultCentMer(DEG);
	}
	else mCentMer = centMer;

} 

//************************************************************************
bool cGeoP::Between(cGeoP NW,cGeoP SE)
{
	bool ReturnValue;
	cGeoP tempT(*this);
	tempT.SetGeoType(DEG);
	NW.SetGeoType(DEG);
	SE.SetGeoType(DEG);
	ReturnValue = (tempT.mLat<=NW.mLat)&&(tempT.mLat>=SE.mLat)
					&&(tempT.mLon>=NW.mLon)&&(tempT.mLon<=SE.mLon);
	return ReturnValue;
}

//***********************************************************************
const cGeoP & cGeoP::operator=(const cGeoP &right)
{
//	cout << " In cGeoP overloaded = " << endl;	
	if (this==&right)
		return(*this);
	mLat = right.mLat;
	mLon = right.mLon;
	mType = right.mType;
	mCentMer = right.mCentMer;
	mSouth = right.mSouth;
	return(*this);
}

//***********************************************************************
bool cGeoP::operator!=(const cGeoP &right) const
{
	bool ReturnValue;
	double tol;
	if ((mType==DEG)||(mType==NDEF))
		tol = 0.000001;
	else tol = 1.0;
	cGeoP temp(right);
	if (mType!=temp.mType)
	{
		cout << "In overloaded cGeoP != " << endl;
		cout << "Different Types ... the program is coverting ... " << endl;
		cout << " To save time you might want to sent them in the same type " << endl;
		temp.SetGeoType(mType);
	}
	if (mType==DEG)
		ReturnValue = ((fabs(mLat-temp.mLat) > tol)||(fabs(mLon-temp.mLon) > tol));
	else 
		ReturnValue = (fabs(mLat-temp.mLat) > tol)||(fabs(mLon-temp.mLon) >tol)||
						(mSouth!=temp.mSouth)||(mCentMer!=temp.mCentMer);
	return ReturnValue;
}

//**********************************************************************
bool cGeoP::operator==(const cGeoP &right) const
{
	bool ReturnValue;
	cGeoP temp(right);
	double tol;
	if ((mType==DEG)||(mType==NDEF))
		tol = 0.000001;
	else tol = 1.0;
	if (mType!=temp.mType)
	{
		cout << "In overloaded cGeoP == " << endl;
		cout << "Different Types ... the program is coverting ... " << endl;
		cout << " To save time you might want to sent them in the same type " << endl;
		if (mType==DEG)
		{
			cout << mLat;
			if (mLat<0.0) cout << " S, ";
			else cout << " N, ";
			cout << mLon;
			if (mLon<0) cout << " W, ";
			else cout << " E, ";
		}
		else
		{
			cout << mLat <<", ";
			cout << mLon <<", ";
			cout << ",T:," << mType;
			cout << ",Cent, " << mCentMer;
			if (mSouth)	cout << " S, ";
			else cout << " N, ";
		}
		cout << endl;
		if (mType==DEG)
		{
			cout << right.mLat;
			if (right.mLat<0.0) cout << " S, ";
			else cout << " N, ";
			cout << right.mLon;
			if (right.mLon<0) cout << " W, ";
			else cout << " E, ";
		}
		else
		{
			cout << right.mLat <<", ";
			cout << right.mLon <<", ";
			cout << ",T:," << mType;
			cout << ",Cent, " << right.mCentMer;
			if (right.mSouth)	cout << " S, ";
			else cout << " N, ";
		}
		cout << endl << endl;

		temp.SetGeoType(mType);
	}
	if (mType==DEG)
		ReturnValue = ((fabs(mLat-temp.mLat) < tol)&&(fabs(mLon-temp.mLon) < tol));
	else 
		ReturnValue = (fabs(mLat-temp.mLat) < tol)&&(fabs(mLon-temp.mLon) < tol)&&
						(mSouth==temp.mSouth)&&(mCentMer==temp.mCentMer);
	return ReturnValue;
}

//***************************************************************************************************
inline int cGeoP::DefaultCentMer(GeoType type)
{
	if ((WGS84GC==mType)&&(UTM==type)&&(-1!=mCentMer))
	{
		double Zone = int((mCentMer + 180)/6) + 1;
		mCentMer = Zone;
		return Zone;
	}
	if (DEG!=mType) SetGeoType(DEG);
	if (UTM==type) // return Zone
	{
		mCentMer = int((mLon + 180)/6) + 1;
		if( mLat>=56.0&&mLat<64.0&& mLon>=3.0&&mLon<12.0)
			mCentMer = 32;
		// Special zones for Svalbard
		if( mLat >= 72.0 && mLat < 84.0 ) 
		{
			if(      mLon>=0.0&&mLon<9.0 ) mCentMer = 31;
			else if( mLon>=9.0&&mLon<21.0) mCentMer = 33;
			else if( mLon>=21.0&&mLon<33.0) mCentMer = 35;
			else if( mLon>=33.0&&mLon<42.0) mCentMer = 37;
		}
	}
	else mCentMer = (int)floor((mLon-1)/2+0.5)*2+1; //WGS84 Gauss Conform
	return mCentMer;
};


//**************************************************************************
bool cGeoP::SetGeoType(GeoType type, int central)
{
	if (mType==type)
	{
		if (mType==DEG) return true;
		else if (mCentMer==central) return true;
	}
	switch (mType)
	{
		case DEG:
			mSouth = (mLat<=0.0); 
			switch (type)
			{
				case DEG: 		return true;
				case WGS84GC: 		return DEGtoWGS84GC(central);
				case UTM:		return DEGtoUTM(central);
				case NDEF:
				{
					cout << "Assuming Not Defined GeoType is Degrees." << endl;
					mType = DEG;
					return false;
				}
			}
		case WGS84GC:
			switch (type)
			{
				case DEG:		return WGS84GCtoDEG();
				case WGS84GC:		return WGS84GCtoWGS84GC(central);
				case UTM:		return WGS84GCtoUTM(central);
				case NDEF:
				{
					cout << "Assuming Not Defined GeoType is Degrees." << endl;
					WGS84GCtoDEG();
					return false;
				}
			}
		case UTM:
			switch (type)
			{
				case DEG:			return UTMtoDEG();
				case WGS84GC:			return UTMtoWGS84GC(central);
				case UTM:			return UTMtoUTM(central);
				case NDEF:
				{
					cout << "Assuming Not Defined GeoType is Degrees." << endl;
					UTMtoDEG();
					return false;
				}
			}
		case NDEF:
			cout << "In cGeoP::SetGeoType: GeoType not defined: Make Degrees" << endl;
			mType=DEG;
			switch (type)
			{
				case DEG: 			return true;
				case WGS84GC: 			return DEGtoWGS84GC(central);
				case UTM:			return DEGtoUTM(central);
				case NDEF:
				{
					cout << "Assuming Not Defined GeoType is Degrees." << endl;
					mType = DEG;
					return false;
				}
			}
		default:
			cout << "cGeoP::SetGeoType: Coversion requested is not surported." << endl;		
			return false;
	}
	return false;
}

//************************************************************************
void cGeoP::Display()
{
	if (mType==DEG)
	{
		cout << mLat;
		if (mLat<0.0) cout << " S, ";
		else cout << " N, ";
		cout << mLon;
		if (mLon<0) cout << " W";
		else cout << " E";
	}
	else
	{
		cout << mLat <<", ";
		cout << mLon <<", ";
		cout << ",T:" << mType;
		cout << ", Cent: " << mCentMer;
		if (mSouth)	cout << " S";
		else cout << " N";
	}
	cout << endl;
}

//************************************************************************
double cGeoP::Distance(const cGeoP right)
{
//	cout << " In cGeoP:: Distance " << endl;
	double s=0.0;
	cGeoP tempR(right);
	cGeoP tempT(*this);
	if (tempT.mType!=tempR.mType)
	{
		if (tempT.mType==DEG) 
			tempT.SetGeoType(tempR.mType); 
		else tempR.SetGeoType(tempT.mType);
	}
	if (tempT==tempR) return 0.0;
	if (tempT.mType!=DEG)
	{
		if (tempT.mCentMer==tempR.mCentMer)
			s = (sqrt((tempT.mLat-tempR.mLat)*(tempT.mLat-tempR.mLat) 
					+ (tempT.mLon - tempR.mLon)*(tempT.mLon - tempR.mLon)));
		if (tempT.mCentMer!=tempR.mCentMer)
		{
			tempR.SetGeoType(DEG);
			tempT.SetGeoType(DEG);
		}
		else return s;
	}
		
	double lat1 = tempR.mLat * rd; //convert inputs in degrees to radians:
	double lon1 = tempR.mLon * rd;
	double lat2 = tempT.mLat * rd;
	double lon2 = tempT.mLon * rd;
	// correct for errors at exact poles by adjusting 0.6 millimeters:
	if (fabs(PI/2.0-fabs(lat1)) < 1e-10)
 	   lat1 = lat1/fabs(lat1)* (PI/2.0-(1e-10));
	if (fabs(PI/2.0-fabs(lat2)) < 1e-10)
    	lat2 = lat2/fabs(lat2)* (PI/2.0-(1e-10));
	double U1 = atan((1.0-f)*tan(lat1));
	double U2 = atan((1.0-f)*tan(lat2));
	double sinU1 = sin(U1);
	double sinU2 = sin(U2);
	double cosU1 = cos(U1);
	double cosU2 = cos(U2);
	double lambda = fabs(lon2-lon1);
	if (lambda > PI)	lambda = 2.0*PI - lambda;
	double L=lambda;
	double lambdaold = 0.0;
	unsigned i = 0;
	double sinsigma, cossigma, sigma, alpha, cos2sigmam, C;
	double cosAlpha, sinLambda, cosLambda, sinSigma, cosSigma;
	while (((!i) || (fabs(lambda-lambdaold) > 1e-9))&&(lambda<PI))// force at least one execution
    	{
	    	i++;
    		if (i>50)
	    	{
    			cout << "Points are essentially antipodal. Precision may be reduced slightly." << endl;
  		      	lambda = PI;
  	  	}
        	else
 		{
    			lambdaold 	= lambda;
    			sinLambda 	= sin(lambda);
	    		cosLambda	= cos(lambda);
    			sinsigma 	= sqrt(cosU2*cosU2*sinLambda*sinLambda 
    					+ (cosU1*sinU2-sinU1*cosU2*cosLambda)
    					* (cosU1*sinU2-sinU1*cosU2*cosLambda));
    			cossigma 	= sinU1*sinU2+cosU1*cosU2*cosLambda;
    			sigma 		= atan2(sinsigma,cossigma);
    			sinSigma	= sin(sigma);
    			cosSigma	= cos(sigma);
    			alpha 		= asin(cosU1*cosU2*sinLambda/sinSigma);
    			cosAlpha	= cos(alpha);
    			cos2sigmam 	= cosSigma-2.0*sinU1*sinU2/(cosAlpha*cosAlpha);
    			C 		= f/16.0*cosAlpha*cosAlpha*(4.0+f*(4.0-3.0*cosAlpha*cosAlpha));
    			lambda 		= L+(1.0-C)*f*sin(alpha)*(sigma+C*sinSigma*
        					(cos2sigmam+C*cosSigma*(-1.0+2.0*cos2sigmam*cos2sigmam)));
    		//correct for convergence failure in the case of essentially antipodal points
	    		if (lambda > PI)
    			{
        			cout << "Points are essentially antipodal. Precision may be reduced slightly." << endl;
        			lambda = PI;
    			}
 		}
    	}
	double u2 = cosAlpha*cosAlpha*eprime2;
	double A = 1.0+u2/16384.0*(4096.0+u2*(-768.0+u2*(320.0-175.0*u2)));
	double B = u2/1024.0*(256.0+u2*(-128.0+u2*(74.0-47.0*u2)));
	double deltasigma = B*sinSigma*(cos2sigmam+B/4.0*(cosSigma
						*(-1.0+2.0*cos2sigmam*cos2sigmam)-B/6.0*cos2sigmam
						*(-3.0+4.0*sin(sigma)*sinSigma)*(-3.0+4.0*cos2sigmam*cos2sigmam)));
	s = b*A*(sigma-deltasigma);
	if (s < 0.01) s = 0.0; // well what is 1cm between friends
   	return(s); 
}

//*****************************************************************************
// to calculate bearing of right as seen from this.
// Formula: θ = atan2(sin(Δlong).cos(lat2), cos(lat1).sin(lat2) − sin(lat1).cos(lat2).cos(Δlong))
// Answer in degrees
//
double cGeoP::Bearing(const cGeoP right)
{
//	cout << " In cGeoP::Bearing " << endl;
	double bearing;
	cGeoP tempR(right);
	cGeoP tempT(*this);
	if (mType!=tempR.mType)
		tempR.SetGeoType(mType);
	if (tempT==tempR) return 0.0;
	if (mType!=DEG)
	{
		if (tempR.mCentMer==mCentMer)
		{	
			long double dX, dY;
			if (mType==UTM)
			{
				dY = tempR.mLat-mLat;
				dX = tempR.mLon-mLon;
			}
			else //Gauss Conform
			{
				dY = tempR.mLat-mLat;
				dX = mLon-tempR.mLon;
			}
			bearing = atan2(dX,dY);
			if (bearing<0.0) bearing+=2*PI;
			return bearing/rd;
		}
		else
		{
			tempR.SetGeoType(DEG);
			tempT.SetGeoType(DEG);
		}
	}
	
//	long double lat1 = tempT.mLat*rd;
//	long double lon1 = tempT.mLon*rd;
//	long double lat2 = tempR.mLat*rd;
//	long double lon2 = tempR.mLon*rd;
//	bearing = atan2l(sinl(lon2-lon1)*cosl(lat2), 
//					cosl(lat1)*sinl(lat2)-sinl(lat1)*cosl(lat2)*cosl(lon2-lon1));
	double lat1 = tempT.mLat*rd;
	double lon1 = tempT.mLon*rd;
	double lat2 = tempR.mLat*rd;
	double lon2 = tempR.mLon*rd;
	bearing = atan2(sin(lon2-lon1)*cos(lat2), 
					cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon2-lon1));
	if (bearing<0.0) bearing+=2*PI;
	return bearing/rd;
}

/***********************************************************************************
 * Calculate destination point given start point lat/long (numeric degrees), 
 * bearing (numeric degrees) & distance (in m).
 *
 * from: Vincenty direct formula - T Vincenty, 
 *      "Direct and Inverse Solutions of Geodesics on the 
 *       Ellipsoid with application of nested equations", 
 * 					Survey Review, vol XXII no 176, 1975
 *       http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
 */
//***********************************************************************************
void cGeoP::FromHere(cGeoP here, double distance, double direction)
{
	// distance in meter 
	// direction in degrees
//	cout << " In cGeoP FromHere " << endl;
	cGeoP tempT(*this);

 	double s = distance;
	if (direction<-180) direction = 360 + direction;
	else if  (direction>180) direction = direction - 360;
 	double alpha1 = direction * rd; 
	double sinAlpha1 = sin(alpha1);
	double cosAlpha1 = cos(alpha1);

	if (s<50)
	{
		cGeoP HereT(here);
		int centMer = HereT.DefaultCentMer(WGS84GC);
		HereT.SetGeoType(WGS84GC,centMer);
		mCentMer = HereT.mCentMer;
		mSouth = HereT.mSouth;
		mLat = HereT.mLat + s*cosAlpha1;
		mLon = HereT.mLon - s*sinAlpha1;
		mType = WGS84GC;
//		cout << "In 	cGeoP::FromHere short distance:  s=" << s << "	dir=" << direction<< endl;
//		Display();
	}
	else
	{
		mType = here.mType;
		if (mType==UTM)
		{
			mCentMer = here.mCentMer;
			mSouth = here.mSouth;
			mLat = here.mLat + s*cosAlpha1;
			mLon = here.mLon + s*sinAlpha1;
		}
		else if (mType==WGS84GC)
		{
			mCentMer = here.mCentMer;
			mSouth = here.mSouth;
			mLat = here.mLat + s*cosAlpha1;
			mLon = here.mLon - s*sinAlpha1;			
		}
		else // hence DEG
		{
			double tanU1 = (1.0-f) * tan(here.mLat*rd);
			double cosU1 = 1.0 / sqrt((1.0 + tanU1*tanU1));
			double sinU1 = tanU1*cosU1;
			double sigma1 = atan2(tanU1, cosAlpha1);
			double sinAlpha = cosU1 * sinAlpha1;
			double cosSqAlpha = 1.0 - sinAlpha*sinAlpha;
			double uSq = cosSqAlpha * eprime2;
			double A = 1 + (uSq/16384)*(4096+uSq*(-768+uSq*(320-175*uSq)));
			double B = (uSq/1024) * (256+uSq*(-128+uSq*(74-47*uSq)));
			double sigma = s / (b*A);
			double sigmaP = 1.99*PI;
			double cos2SigmaM, sinSigma, cosSigma, deltaSigma;
			// TODO change tolerance from 1e-12 to 1e-9
			while (fabs(sigma-sigmaP) > 1e-10) 
			{
				cos2SigmaM = cos(2.0*sigma1 + sigma);
				sinSigma = sin(sigma);
				cosSigma = cos(sigma);
				deltaSigma = B*sinSigma*(cos2SigmaM+B/4.0*(cosSigma*(-1.0+2.0*cos2SigmaM*cos2SigmaM)-
   	   			B/6.0*cos2SigmaM*(-3.0+4.0*sinSigma*sinSigma)*(-3.0+4.0*cos2SigmaM*cos2SigmaM)));
				sigmaP = sigma;
				sigma = s / (b*A) + deltaSigma;
			}
	
			double tmp = sinU1*sinSigma - cosU1*cosSigma*cosAlpha1;
			double lat2 = atan2((sinU1*cosSigma + cosU1*sinSigma*cosAlpha1),(
   	   					(1-f)*sqrt(sinAlpha*sinAlpha + tmp*tmp)));
			double lambda = atan2(sinSigma*sinAlpha1, cosU1*cosSigma - sinU1*sinSigma*cosAlpha1);
			double C = (f/16.0)*cosSqAlpha*(4.0+f*(4.0-3.0*cosSqAlpha));
			double L = lambda - (1-C) * f * sinAlpha *
						(sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1.0+2.0*cos2SigmaM*cos2SigmaM)));
//			double revAz = atan2(sinAlpha, -tmp);  // final bearing
			mLat=lat2/rd;
			mLon=here.mLon +L/rd;
// 	 	cout << "R: "<< distance << "   B: "<< revAz/rd 
//		  		<< "  Here.lat: "<< here.mLat<< "  Here.lon: "<< here.mLon
// 	 		<< "  Lat: " << mLat << "  Long: " << mLon << endl;
			DefaultCentMer(WGS84GC);
			mType = DEG;
			mSouth = (mLat<0);
		}
	}
	SetGeoType(tempT.mType,tempT.mCentMer);

}


//*************************************************************************
inline bool cGeoP::DEGtoWGS84GC(int central)
{
    double phi,p;                       // For coordinates
    double p2,p3,p4,p5,p6;                  // Powers of w
    double t,t2,t4,sp,cp,cp2,cp3,cp5;       // sin,cos,tan of params
    double psi,psi2,psi3,psi4;
    double md,v;

    if (mType != DEG)
    {
    	cout << "DEGtoWGS84CG: You should not be using this conversion" << endl;
    	return false;
    }

    if (central == -1.0)  // no central meridian to convert to specified
    	DefaultCentMer(WGS84GC);
    else mCentMer = (int)floor(central+0.5);
    mSouth = (mLat<0);
    p = -(mLon - (double)mCentMer)*rd;              // Distance west of centmer in degrees
    p2 = p*p;
    p3 = p2*p;
    p4 = p3*p;
    p5 = p4*p;
    p6 = p5*p;

    phi = mLat*rd;                       // Radians n-s
    t = tan(phi);                       //section 3.2-23 
    t2 = t*t;
    t4 = t2*t2;
    sp = sin(phi);
    cp = cos(phi);
    cp2 = cp*cp;
    cp3 = cp2*cp;
    cp5 = cp3*cp2;
    psi = 1.0+eprime2*cp2;                // 7.19-100, section 7.12-112 
    psi2 = psi*psi;
    psi3 = psi2*psi;
    psi4 = psi3*psi;
	 // Calculate the meridian arc length from phi
    md   = m1*phi+m2*sin(2.*phi)+m3*sin(4.*phi)+m4*sin(6.*phi); // 3.55-36 
    v    = c/sqrt(psi);                     // 7.1-98 

    mLat = (md + 0.5*p2*v*sp*cp + 1.0/24.0*p4*v*sp*cp3*(4.0*psi2+psi-t2)) +
		       (1.0/720.0*p6*v*sin(phi)*cp5*(8.0*psi4*(11.0-24.0*t2) -
		    	28.0*psi3*(1.0 - 6.0*t2) +	psi2*(1.0-32.0*t2) -
		    	psi*2.0*t2 + t4));              // 7.23-112 
    mLon = p*v*cp + 1.0/6.0*p3*v*cp3*(psi-t2) +
		      1.0/120.0*p5*v*cp5*(4.0*psi3*(1.0-6.0*t2) +
		       psi2*(1.0+8.0*t2) - psi*2.0*t2+t4);      // 7.23-112 

    mType = WGS84GC;
    return true;	
}

//*************************************************************************
inline bool cGeoP::WGS84GCtoDEG()
{

    double phi,                         // foot point latitude 
	   			cp,                          // cosine of phi 
	   			secp,                        //   
	   			p,                           // radius of curvature of the meridian ellipse 
	   			t,t2,t4,                     // tangent of phi 
	   			psi,psi2,psi3,psi4,
	   			v,v3,v5,                     // radius of curvature of a normal section
					   						//	perpendicular to the meridian 
	   			V2,
	   			x,                           // gauss x coordinate 
	   			y,y2,y3,y4,y5,y6,            // gauss y coordinate 
	   			sigma;

	double lat,w,radlong;

    if (mType != WGS84GC)
    {
    	cout << "WGS84CGtoDEG: You should not be using this conversion routine" << endl;
    	return false;
    }
       
    y = mLon;    y2 = y*y;    y3 = y2*y;    y4 = y2*y2; 	y5 = y4*y;    y6 = y5*y;
    x = mLat;
    sigma = x*PI/(G*180.);                                   // 3.72-38

    phi = sigma+phi1*sin(2.*sigma)+phi2*sin(4.*sigma)+
	      phi3*sin(6.*sigma)+phi4*sin(8.*sigma);   // 3.72-38 

    cp  = cos(phi);
    secp = 1./cp;
    t   = tan(phi);                  // section 3.2-23 
    t2  = t*t;    t4  = t2*t2;
    V2  = 1.+eprime2*cp*cp;          // section 3.2-23 
    psi = V2;                        // section 3.2-23, section 7.12-112 
    psi2 = psi*psi;    psi3 = psi2*psi;    psi4 = psi3*psi;
	p   = c/(psi*sqrt(V2));          // 3.4-31 
    v   = c/sqrt(V2);               // 7.1-98 
    v3 = v*v*v;    v5 = v3*v*v;
    lat = ((phi-0.5*y2*t/(p*v)+1./24.*y4*t/(p*v3)*
    		(-4.*psi2+9.*psi*(1.-t2)+12.*t2)) -
    		(1/720*y6*t/(p*v5)*(8*psi4*(11.-24.*t2)-12*psi3*(21.-71.*t2)
    		+(15*psi2*(15.-98.*t2+15.*t4)+180*psi*t2*(5.-3.*t2)+360*t4))));
    w   = (y*secp/v-1./6.*y3*secp/v3*(psi+2.*t2)+
	       1/120*y5*secp/v5*(-4.*psi3*(1.-6.*t2)+psi2*(9.-68.*t2)+
	       72.*psi*t2+24*t4));                // 7.37-112 

   radlong = mCentMer*rd - w;
   mLon = radlong/rd;
   mLat = lat/rd;
   mType = DEG;
   mCentMer =-1;
   return true;	
}


//*************************************************************************
/*
inline bool cGeoP::WGS84GCtoDEG()
{
	mLon*=-1.0;
	long double k0 = 1.0;
	if (mType!=WGS84GC)
	{
		cout << "WGS84GCtoDEG: you should not be calling this conversion!!" << endl;
		return false;	
	}
	long double md = mLat/k0;
	long double mu = md/m1;
    long double fp = mu+phi1*sinl(2.*mu)+phi2*sinl(4.*mu)
    				+phi3*sinl(6.*mu)+phi4*sinl(8.*mu);
    long double cp = cosl(fp);
    long double tp = tanl(fp);
    long double T1= fp*fp;
    long double sinL2 = sinl(fp)*sinl(fp);
    long double C1 = eprime2*cp*cp;
    long double R1 = a*(1.-e2)/(powl((1.-e2*sinL2),1.5));
    long double N1 = a/sqrtl(1.0-e2*sinL2);
    long double D = mLon/(N1*k0);
    long double D2 = D*D;
    long double D4 = D2*D2;
    long double Q1 = N1*tp/R1;
    long double Q2 = D*D/2.;
    long double Q3 = (5.+3.*T1+10.*C1-4.*C1*C1-9.*eprime2)*D4/24.0;
    long double Q4 = (61.+90.*T1+298.*C1+45.*T1*T1-3.*C1*C1-252.*eprime2)*D4*D2/720.0; 
    long double Q6 = (1. + 2.*T1 + C1)*D2*D/6.0;
    long double Q7 = (5.0-2.0*C1+28.*T1-3.*C1*C1+8.*eprime2+24.0*T1*T1)*D4*D/120.0;
    mLat = (fp - Q1*(Q2 - Q3 + Q4))/rd;
    mLon = mCentMer + (D - Q6 + Q7)/cp/rd;
    mType=DEG;
    return true;
}

// *************************************************************************
inline bool cGeoP::DEGtoWGS84GC(int central)
{
	long double k0 = 1.0;
	if (mType!=DEG)
	{
		cout << "DEGtoWGS84GC: you should not be calling this conversion!!" << endl;
		return false;	
	}
	mSouth = (mLat<0);
	if (central==-1) DefaultCentMer(WGS84GC);
	else mCentMer = central;
	
	long double p = (mLon-(double)mCentMer)*rd;
	long double p2=p*p; 
	long double p3=p2*p;
    long double lat = mLat*rd;
	long double cp = cosl(lat);
	long double sp = sinl(lat);
	long double sp2 = sp*sp;
	long double cp2 = cp*cp;
    long double cp3 = cp2*cp;
    long double tp = tanl(lat); 
    long double tp2 = tp*tp;
//	long double rho = a*(1.-e2)/(pow((1.-e2*sp2),(3./2.)));

	long double nu = a/sqrtl(1.0-e2*sp2);
	long double eecc = eprime2*cp2;
	long double md = m1*lat+m2*sinl(2.*lat)+m3*sinl(4.*lat)+m4*sinl(6.*lat);
	long double K1 = md*k0;
	long double K2 = k0*nu*sp*cp/2.0;
	long double K3 = (k0*nu*sp*cp3/24.) *
					(5.-tp2 + 9.0*eecc + 4.0*eecc*eecc);
	long double K7 = k0*nu*sp*(61.0-58.0*tp2+tp2*tp2+600*eecc-330*eprime2)*cp2*cp3/720.0;
	long double K4 = k0*nu*cp;
	long double K5 = (k0*nu*cp3/6.0)*(1.0-tp2 + eecc);
	long double K6 = k0*nu*(5. - 18.*tp2 + tp2*tp2 + 72.0*eecc-58.0*eprime2)*cp3*cp2/120;
	mLat = K1 + K2*p2 + K3*p2*p2 + K7*p3*p3;
	mLon = K4*p + K5*p3 + K6*p2*p3;
	mLon *=-1.0;
	mType=WGS84GC;
	return true;
}
*/

//*************************************************************************
inline bool cGeoP::UTMtoDEG()
{
	double k0 = 0.9996;
	if (mType!=UTM)
	{
		cout << "UTMtoDEG: you should not be calling this conversion!!" << endl;
		return false;	
	}

	double x = mLon - 500000.0; //remove 500,000 meter offset for longitude
	double y = mLat;
	if (mSouth) y-=10000000.0;
	
	double Cent = (mCentMer - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone

	double md = y/k0;
	double mu = md/m1;
	double fp = mu+phi1*sin(2.*mu)+phi2*sin(4.*mu)
    			+phi3*sin(6.*mu)+phi4*sin(8.*mu); //phi1Rad

    	double cp = cos(fp);
    	double tp = tan(fp);
    	//double T1= fp*fp;
	double T1 = tp*tp;
    	double sinL2 = sin(fp)*sin(fp);
    	double C1 = eprime2*cp*cp;
    	double R1 = a*(1.-e2)/(pow((1.-e2*sinL2),1.5));
    	double N1 = a/sqrt(1.0-e2*sinL2);
    	double D = x/(N1*k0);
    	double D2 = D*D;
    	double D4 = D2*D2;
    	double Q1 = N1*tp/R1;
    	double Q2 = D*D/2.;
    	double Q3 = (5.+3.*T1+10.*C1-4.*C1*C1-9.*eprime2)*D4/24.0;
    	double Q4 = (61.+90.*T1+298.*C1+45.*T1*T1-3.*C1*C1-252.*eprime2)*D4*D2/720.0; 
    	double Q6 = (1. + 2.*T1 + C1)*D2*D/6.0;
    	double Q7 = (5.0-2.0*C1+28.*T1-3.*C1*C1+8.*eprime2+24.0*T1*T1)*D4*D/120.0;
    	mLat = (fp - Q1*(Q2 - Q3 + Q4))/rd;
    	mLon = Cent + (D - Q6 + Q7)/cp/rd;
    	mType=DEG;
	mCentMer =-1;
    	return true;
}

//*************************************************************************
inline bool cGeoP::DEGtoUTM(int zone)
{
	double k0 = 0.9996;
	if (mType!=DEG)
	{
		cout << "DEGtoUTM: you should not be calling this conversion!!" << endl;
		return false;	
	}
	mSouth = (mLat<0);
	if (zone==-1) DefaultCentMer(UTM);
	else mCentMer = zone;
	double CentMer = (mCentMer - 1)*6 - 180 + 3;
	
	double p = (mLon-(double)CentMer)*rd;
	double p2=p*p; 
	double p3=p2*p;
    	double lat = mLat*rd;
	double cp = cos(lat);
	double sp = sin(lat);
	double sp2 = sp*sp;
	double cp2 = cp*cp;
    	double cp3 = cp2*cp;
    	double tp = tan(lat); 
    	double tp2 = tp*tp; //tp2
//	double rho = a*(1.-e2)/(pow((1.-e2*sp2),(3./2.)));

	double nu = a/sqrt(1.0-e2*sp2); //N
	double eecc = eprime2*cp2; //C
//	double A = cp*p;
	double md = m1*lat+m2*sin(2.*lat)+m3*sin(4.*lat)+m4*sin(6.*lat); //M
	double K1 = md*k0;
	double K2 = k0*nu*sp*cp/2.0;
	double K3 = (k0*nu*sp*cp3/24.) *
					(5.-tp2 + 9.0*eecc + 4.0*eecc*eecc);
	double K7 = k0*nu*sp*(61.0-58.0*tp2+tp2*tp2+600*eecc-330*eprime2)*cp2*cp3/720.0;
	double K4 = k0*nu*cp;
	double K5 = (k0*nu*cp3/6.0)*(1.0-tp2 + eecc);
	double K6 = k0*nu*(5. - 18.*tp2 + tp2*tp2 + 72.0*eecc-58.0*eprime2)*cp3*cp2/120;

	mLon = K4*p + K5*p3 + K6*p2*p3 + 500000.0;

	mSouth = (mLat < 0);
	mLat = (double)(K1 + K2*p2 + K3*p2*p2 + K7*p3*p3);
	if (mSouth) mLat += 10000000.0; //10000000 meter offset for southern hemisphere

	mType=UTM;
	return true;
}

//*************************************************************************
inline bool cGeoP::UTMtoWGS84GC(int CentMer)
{
	if (mType!=UTM)
	{
		cout << "UTMtoWGS84GC: you should not be calling this conversion!!" << endl;
		return false;	
	}
	bool ReturnValue;
	double tLon;
	int TempCent = (mCentMer - 1)*6 - 180 + 3;
	tLon = -(mLon - 500000)/0.9996;
	if ((CentMer==TempCent)||
		((-1==CentMer)&&(fabs(tLon)<55800)/*OK for |Lat|<60*/))
	{
		if (mSouth) mLat -= 10000000.0;		
		mLat = mLat/0.9996;
		mLon = -(mLon - 500000)/0.9996;
		mType = WGS84GC;	
		mCentMer=TempCent;
		return true;
	}
	
//	cout << "In cGeoP::UTMtoWGS84GC NOT Same" << endl;
	ReturnValue = UTMtoDEG();
	if (CentMer==-1) DefaultCentMer(WGS84GC);
	else mCentMer=CentMer;
	ReturnValue = (ReturnValue)&&(DEGtoWGS84GC(mCentMer));
	return ReturnValue;
}

//*************************************************************************
inline bool cGeoP::WGS84GCtoUTM(int Zone)
{
	if (mType!=WGS84GC)
	{
		cout << "WGS84GCtoUTM: you should not be calling this conversion!!" << endl;
		return false;	
	}
	bool ReturnValue;
	bool SameCentMer = false; 
	int CentMer;
	if (-1==Zone) Zone = int((mCentMer -3 + 180)/6) + 1;
	CentMer = (Zone - 1)*6 - 180 + 3;
	if ((CentMer==mCentMer))
	{
		mLat = mLat*0.9996;
		if (mSouth) mLat += 10000000.0;
		mLon = -(mLon*0.9996 - 500000);
		mType=UTM;
		mCentMer = Zone;
//		cout << "In cGeoP::WGS84GCtoUTM SameCentmer" << endl;
		return true;
	}
	else
	{
		ReturnValue = WGS84GCtoDEG();
//		cout << "In cGeoP::WGS84GCtoUTM NOT SameCentmer" << endl;
//		Display();
		if (CentMer==-1) DefaultCentMer(UTM);
		else mCentMer= Zone;
		ReturnValue = (ReturnValue)&&(DEGtoUTM(mCentMer));
		return ReturnValue;
	}
}

//*************************************************************************
inline bool cGeoP::WGS84GCtoWGS84GC(int central)
{
	if (mType!=WGS84GC)
	{
		cout << "WGS84GCtoWGS84GC: you should not be calling this conversion!!" << endl;
		return false;	
	} 
	else if (central==mCentMer) return true;
	bool ReturnValue;
	ReturnValue = WGS84GCtoDEG();
	ReturnValue = ReturnValue && DEGtoWGS84GC(central);
	return ReturnValue;
}

//*************************************************************************
inline bool cGeoP::UTMtoUTM(int zone)
{
	if (mType!=UTM)
	{
		cout << "UTMtoUTM: you should not be calling this conversion!!" << endl;
		return false;	
	} 
	else if (zone==mCentMer) return true;
	bool ReturnValue;
	ReturnValue = UTMtoDEG();
	ReturnValue = ReturnValue && DEGtoUTM(zone);
	return ReturnValue;
}
