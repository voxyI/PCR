#ifndef _PID
#define _PID

#include <stdint.h>

typedef struct {
	float kp;
	float ki;
	float kd;
	float prevError;
	float prevMeasure;
	float integral;
} PID_module;

/**
 * @brief Initialize the PID struct with start values
 * 
 * @param pid
 * 	The PID_module to be used
 * 
 * @param kp
 * 	The proportion constant
 * 
 * @param ki
 * 	The integral constant
 * 
 * @param kd
 * 	The derivative constant
 */
void PID_init(PID_mudule *pid, float kp, float ki, float kd) {
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->prevError = 0.0f;
	pid->prevMeasure = 0.0f;
	pid->integral = 0.0f;
}

/**
 * @brief Update the PID module and return a control value
 * 
 * @param pid
 * 	The PID_module to be used
 * 
 * @param target
 * 	The setpoint or target temperature
 * 
 * @param measurement
 * 	The temperature measured
 * 
 * @param time
 * 	The time between this and the previous measurement
 * 
 * @return
 * 	The value to adjust the temperature controller by
 */
float PID_update(PID_module *pid, float target, float measurement, float time) {
	//TODO
}


#endif
