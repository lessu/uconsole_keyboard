/*
 * clockworkpi devterm trackball 
 */
#include "keys_io_map.h"

#include <stdint.h>
#include <stdbool.h>
#include <USBComposite.h>
#include "trackball.h"

#define REPORT_RATE 60
#define REPORT_INTERVAL_MS (1000/REPORT_RATE)
 
enum Axis: uint8_t {
	AXIS_X,
	AXIS_Y,
	AXIS_NUM,
};

static int8_t s_distances[AXIS_NUM];

static int8_t responsive_curve_pipe(int8_t input){
	int32_t v = 2 * input * input * (input >= 0 ? 1 : -1);
	if(v > 128) return 128;
	if(v < -127) return -127;
	return (int8_t)v;
}

template<Axis AXIS, int8_t Direction >
static void interrupt( ) {
	s_distances[AXIS] += Direction;
}
 
void trackball_task(DEVTERM*dv) {
	static uint32_t s_last_report = 0;
	if( millis() - s_last_report < REPORT_INTERVAL_MS ){
		return;
	}
	s_last_report = millis();

	noInterrupts();
	//copy distance
	int8_t cp_s_distances[AXIS_NUM];
	cp_s_distances[AXIS_X] = s_distances[AXIS_X];
	cp_s_distances[AXIS_Y] = s_distances[AXIS_Y];
	s_distances[AXIS_X] = 0;
	s_distances[AXIS_Y] = 0;
	interrupts();

	const auto mode = dv->state->moveTrackball();
	static TrackballMode lastMode = TrackballMode::Mouse;

	if (lastMode != mode) {
	} else {
	}

	lastMode = mode; 
	
	switch(mode){
		case TrackballMode::Mouse: {
			int8_t x = cp_s_distances[AXIS_X];
			int8_t y = cp_s_distances[AXIS_Y];
			if(x !=0 || y != 0) {
				dv->Mouse->move(
					responsive_curve_pipe(x),
					responsive_curve_pipe(y), 0
				);
			}
			break;
		}
		case TrackballMode::Wheel: {
			int8_t x = cp_s_distances[AXIS_X];
			int8_t y = cp_s_distances[AXIS_Y];
			if(x !=0 || y != 0) {
				// https://github.com/nikita-tomilov/HMouse/tree/master/src
				dv->Mouse->move(0,0,x);
				dv->state->setScrolled();
			}
			break;
		}
	}
}


void trackball_init(DEVTERM*dv){

	pinMode(LEFT_PIN, INPUT);
	pinMode(UP_PIN, INPUT);
	pinMode(RIGHT_PIN, INPUT);
	pinMode(DOWN_PIN, INPUT);

	attachInterrupt(LEFT_PIN, &interrupt<AXIS_X,-1> , ExtIntTriggerMode::CHANGE);
	attachInterrupt(RIGHT_PIN, &interrupt<AXIS_X, 1>, ExtIntTriggerMode::CHANGE);
	attachInterrupt(UP_PIN, &interrupt<AXIS_Y, -1>, ExtIntTriggerMode::CHANGE);
	attachInterrupt(DOWN_PIN, &interrupt<AXIS_Y, 1>, ExtIntTriggerMode::CHANGE);
	
}
