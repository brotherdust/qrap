#include "cGeoP.h"

const double a = 6378137.0; // WGS84 ellipsiod
const double b = 6356752.31424518; // computed from WGS84 earth flattening coefficient definition
const double rd = PI/180.0; //converts degrees to radials
const double f = 1.0/298.25722210088;  // WGS-84 ellipsiod
const double esq = f*(2.0-f);

//*********************************************************************
cGeoP::cGeoP()
{
	m_lat = -26.00;
	m_lon = 29.00;
	m_type = NDEF;
	m_centMer = 29;
}

//*********************************************************************
cGeoP::cGeoP(double lat, double lon, GeoType type, int centMer)
{
	m_lat = lat;
	m_lon = lon;
	m_type = type;
	m_centMer = centMer;
}

//*********************************************************************
cGeoP::~cGeoP()
{
}

//*********************************************************************
int cGeoP::Get(double &lat, double &lon)
{
	lat = m_lat;
	lon = m_lon;
	if (m_type)
	{
		cout << "This overloaded Get assumes that the coordinate is in Degrees" << endl;
		cout << " it would seem that this is not the case ... ";
		cout << " hope you know what you are doing " << endl;
		return 0;
	}
	return 1; 
}

//***********************************************************************
int cGeoP::Get(double &lat, double &lon, GeoType &type, int &centMer)
{
	lat = m_lat;
	lon = m_lon;
	type = m_type;
	centMer = m_centMer;
	return 1;
}

//***********************************************************************
void cGeoP::Set(double lat, double lon, GeoType type, int centMer)
{
	m_lat = lat;
	m_lon = lon;
	m_type = type;
	m_centMer = centMer;
} 

//***********************************************************************
const cGeoP & cGeoP::operator=(const cGeoP &right)
{
	if (this==&right)
		return(*this);
	m_lat = right.m_lat;
	m_lon = right.m_lon;
	m_type = right.m_type;
	m_centMer = right.m_centMer;
	return(*this);
}

//***********************************************************************
int cGeoP::operator!=(const cGeoP &right) const
{
	if (m_type!=right.m_type)
	{
		cout << "Different Types ... results might not be valid" << endl;
		return 1;
	}
	if ((fabs(m_lat-right.m_lat) > TOL)||(fabs(m_lon-right.m_lon) > TOL))
		return 1;
	else return 0;
}

//**********************************************************************
int cGeoP::operator==(const cGeoP &right) const
{
	if (m_type!=right.m_type)
	{
		cout << "Different Types ... results might not be valid" << endl;
		return 0;
	}
	if ((fabs(m_lat-right.m_lat) > TOL)||(fabs(m_lon-right.m_lon) > TOL))
		return 0;
	else return 1;	
}

//************************************************************************
void cGeoP::Display()
{
	cout << m_lat;
	if (m_lat<0.0) cout << ",S, ";
	else cout << ",N, ";
	cout << m_lon;
	if (m_lon<0) cout << ",W, ";
	else cout << ",E, ";
	if (m_type!=DEG)
		cout << ",T:," << m_type << ", ";
}

//************************************************************************
double cGeoP::Distance(const cGeoP &right)
{
	if ((fabs(right.m_lat-m_lat)+fabs(right.m_lon-m_lon))<1e-7)
		return 0.0;
	if (m_type)
	{
		cout << "The Distance function assumes that the coordinate is in Degrees" << endl;
		cout << " it would seem that this is not the case ... ";
		cout << " hope you know what you are doing " << endl;
	}
	double lat1 = right.m_lat * rd; //convert inputs in degrees to radians:
	double lon1 = right.m_lon * rd;
	double lat2 = m_lat * rd;
	double lon2 = m_lon * rd;
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
	while (((!i) || (fabs(lambda-lambdaold) > 1e-12))&&(lambda<PI))// force at least one execution
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
    		C 			= f/16.0*cosAlpha*cosAlpha*(4.0+f*(4.0-3.0*cosAlpha*cosAlpha));
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
	double u2 = cosAlpha*cosAlpha*(a*a-b*b)/(b*b);
	double A = 1.0+u2/16384.0*(4096.0+u2*(-768.0+u2*(320.0-175.0*u2)));
	double B = u2/1024.0*(256.0+u2*(-128.0+u2*(74.0-47.0*u2)));
	double deltasigma = B*sinSigma*(cos2sigmam+B/4.0*(cosSigma
						*(-1.0+2.0*cos2sigmam*cos2sigmam)-B/6.0*cos2sigmam
						*(-3.0+4.0*sin(sigma)*sinSigma)*(-3.0+4.0*cos2sigmam*cos2sigmam)));
	double s = b*A*(sigma-deltasigma);
	if (s < 0.01) s = 0.0; // well what is 1cm between friends
   	return(s); 
}

//*****************************************************************************
// to calculate bearing
// Formula: θ = atan2(sin(Δlong).cos(lat2), cos(lat1).sin(lat2) − sin(lat1).cos(lat2).cos(Δlong))
double cGeoP::Bearing(const cGeoP &right)
{
	double lat1 = m_lat*rd;
	double lon1 = m_lon*rd;
	double lat2 = right.m_lat*rd;
	double lon2 = right.m_lon*rd;

	double bearing = atan2(sin(lon2-lon1)*cos(lat2), 
							cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon2-lon1));

/*	double d = acos(sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(lon2-lon1));
	double t1 = sin(lat2) - sin(lat1) * cos(d);
	double t2 = cos(lat1) * sin(d);
	double t3 = t1 / t2;
	double bearing;
	if (t3 * t3 < 1.0)
		bearing = atan(-t3/ sqrt(-t3 * t3 + 1.0)) + 2.0 * atan(1);
	else bearing = PI;
	if (sin (lon2 - lon1) > 0.0)
		bearing = 2.0*PI-bearing;
*/
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
	if (here.m_type)
	{
		cout << "The FromHere function assumes that the coordinate is in Degrees" << endl;
		cout << " it would seem that this is not the case ... ";
		cout << " hope you know what you are doing " << endl;
	}

 	double s = distance;
 	if (direction<-180) direction = 360 + direction; 
  	double alpha1 = direction * rd; 
  	double sinAlpha1 = sin(alpha1);
  	double cosAlpha1 = cos(alpha1);
  
 	double tanU1 = (1.0-f) * tan(here.m_lat*rd);
  	double cosU1 = 1.0 / sqrt((1.0 + tanU1*tanU1));
  	double sinU1 = tanU1*cosU1;
  	double sigma1 = atan2(tanU1, cosAlpha1);
  	double sinAlpha = cosU1 * sinAlpha1;
  	double cosSqAlpha = 1.0 - sinAlpha*sinAlpha;
  	double uSq = cosSqAlpha * (a*a - b*b) / (b*b);
  	double A = 1 + (uSq/16384)*(4096+uSq*(-768+uSq*(320-175*uSq)));
  	double B = (uSq/1024) * (256+uSq*(-128+uSq*(74-47*uSq)));
   	double sigma = s / (b*A);
  	double sigmaP = 1.99*PI;
  	double cos2SigmaM, sinSigma, cosSigma, deltaSigma;
  	while (fabs(sigma-sigmaP) > 1e-12) 
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
  
//	double revAz = atan2(sinAlpha, -tmp);  // final bearing

  	m_lat=lat2/rd;
  	m_lon=here.m_lon +L/rd;
//  cout << "R: "<< distance << "   B: "<< revAz/rd 
//	  	<< "  Here.lat: "<< here.m_lat<< "  Here.lon: "<< here.m_lon
//  	<< "  Lat: " << m_lat << "  Long: " << m_lon << endl;
  	m_type = DEG;
  	m_centMer = 29;
}
