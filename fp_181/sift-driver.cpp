#include "sift.hpp"
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "alt_types.h"
#include "sys/alt_timestamp.h"
#include "altera_avalon_performance_counter.h"
#include  "system.h"

extern "C" {
#include<getopt.h>
#if defined (VL_MAC)
#include<libgen.h>
#else
#include<string.h>
#endif
#include<assert.h>
}
#include<memory>

using namespace std ;

size_t const not_found = numeric_limits<size_t>::max() - 1 ;

/** @brief Case insensitive character comparison
 ** This predicate returns @c true if @a a and @a b are equal up to
 ** case.
 ** @return predicate value.
 **/
inline bool ciIsEqual(char a, char b) {
  return
    tolower((char unsigned)a) ==
    tolower((char unsigned)b) ;
}

/* keypoint list */
typedef vector<pair<VL::Sift::Keypoint,VL::float_t> > Keypoints ;

/* predicate used to order keypoints by increasing scale */
bool cmpKeypoints (Keypoints::value_type const&a,
		   Keypoints::value_type const&b) {
  return a.first.sigma < b.first.sigma ;
}

//main
int main(int argc, char** argv) {
    int    first          = -1 ;
    int    octaves        = -1 ;
    int    levels         = 3 ;
    float  threshold      = 0.04f / levels / 2.0f ;
    float  edgeThreshold  = 10.0f;
    float  magnif         = 3.0 ;
    int    noorient       = 0 ;
  	int    haveKeypoints  = 0 ;
  	int    unnormalized   = 0 ;

    VL::PgmBuffer buffer ;

    //Load PGM image
    cout << endl << "Extracting ...";
    extractPgm(buffer) ;
    cout << endl << "Extraction Done" << endl;

    //Gaussian scale space
    int         O      = octaves ;
    int const   S      = levels ;
    int const   omin   = first ;
    float const sigman = .5 ;
    float const sigma0 = 1.6 * powf(2.0f, 1.0f / S) ;

    // optionally autoselect the number number of octaves
    // we downsample up to 8x8 patches
    if(O < 1) {
    	O = std::max
				(int
    			(std::floor
    			(log2
    			(std::min(buffer.width,buffer.height))) - omin -3), 1) ;
    }


    // initialize scalespace
    cout << endl << "Running Sift ... ";
    VL::Sift sift(buffer.data, buffer.width, buffer.height,
		    	  sigman, sigma0,
		  	  O, S, omin, -1, S+1) ;
    cout << endl << "Sift Complete" << endl;
    //Run SIFT detector
    if( ! haveKeypoints ) {
    	cout << endl << "Detecting Keypoints ...";
    	sift.detectKeypoints(threshold, edgeThreshold) ;
    	cout << endl << "Keypoints Found" << endl;
    }

    //Run SIFT orientation detector and descriptor
    /* set descriptor options */
    sift.setNormalizeDescriptor( ! unnormalized ) ;
    sift.setMagnification( magnif ) ;

    cout << "Detecting Orientations and Descriptors";
	//Run detector, compute orientations and descriptors
    for( VL::Sift::KeypointsConstIter iter = sift.keypointsBegin() ;
	    iter != sift.keypointsEnd(); ++iter ) {

		// detect orientations
		VL::float_t angles [4] ;
		int nangles ;

		if( ! noorient ) {
		    nangles = sift.computeKeypointOrientations(angles, *iter) ;
		}
		else {
		    nangles = 1;
			angles[0] = VL::float_t(0) ;
		}

		// compute descriptors
		for(int a = 0 ; a < nangles ; ++a) {
			cout << setprecision(2) << iter->x << ' '
			     << setprecision(2) << iter->y << ' '
			     << setprecision(2) << iter->sigma << ' '
			     << setprecision(3) << angles[a] ;

            // compute descriptor
            VL::float_t descr_pt [128] ;
            sift.computeKeypointDescriptor(descr_pt, *iter, angles[a]) ;
	            cout << endl ;
	        }
    } // detector for loop

	cout << endl << "End Reached" << endl;
	usleep(2000000);
	cout << endl << "Sleep Over ... exiting" << endl;
	exit(0);
    return 0 ;
}
