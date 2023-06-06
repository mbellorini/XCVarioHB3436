/*
 * Polars.cpp
 *
 *  Created on: Mar 1, 2019
 *      Author: iltis
 *
 *      Class to deliver a library of default polars
 */

#include "Polars.h"
#include <logdef.h>
#include "SetupNG.h"

// Format per glider:  { Index, GliderType, Reference Wingload (kg/m2), speed1 (km/h), sink1 (m/s), speed2, sink2, speed3, sink3 , max ballast [liters or kg], wing area [m2] }, Empty Weight (Waegung)

// HB-3436: Addiere Empty Weight, was der Waegung entspricht. So muss das nicht mehr gesetzt werden nach dem Wähler der Polare. Die anderen Werte entsprechen genau
// der originalen Polaren "DG 800S/15" und "DG 800S/18", bzw. des Messfluges.
static const PROGMEM t_polar polars_default_arr[] = {
	{ 1370, "HB-3436/15", 34.93, 92.4983, -0.5834, 130.0, -0.9720, 170.0, -1.6852, 150, 10.68, 259.5},
	{ 1390, "HB-3436/18", 29.97, 77.9497, -0.4760, 130.0, -0.9192, 170.0, -1.7786, 150, 11.81, 267.5}
};


const t_polar Polars::getPolar( int x ) {
	return polars_default_arr[x];
}

int Polars::numPolars() {
	return(  sizeof( polars_default_arr ) / sizeof(t_polar) );
}

Polars::Polars(){ }


Polars::~Polars() {
	// TODO Auto-generated destructor stub
}

void Polars::begin(){
	ESP_LOGI( FNAME,"Polars::begin() configured glider type:%d, index:%d", glider_type.get(), glider_type_index.get() );
	if( glider_type_index.get() == 0 ){
		ESP_LOGI( FNAME,"Need first to initialize unique glider type index: %d", polars_default_arr[ glider_type.get() ].index );
		glider_type_index.set(  polars_default_arr[ glider_type.get() ].index );
	}
    int unique_glider_index = glider_type_index.get();
	if( polars_default_arr[ glider_type.get() ].index != unique_glider_index ){
		ESP_LOGI( FNAME,"Unique index missmatch, migrate index after polar DB change");
		for( int p=0; p<numPolars(); p++ ){
			if( polars_default_arr[p].index == unique_glider_index ){
				ESP_LOGI( FNAME,"Found Glider index %d at new position %d (old:%d)", unique_glider_index, p, glider_type.get() );
				if( glider_type.get() != p )
					glider_type.set( p );
				break;
			}
		}
	}
}


