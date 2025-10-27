#ifndef _PID
#define _PID

typedef struct {
	float kp;
	float ki;
	float kd;
	float prevError;
	float prevMeasure;
	float integral;
} PID_module;

void PID_init(PID_mudule *pid, float kp, float ki, float kd);
float PID_update(PID_module *pid, float target, float measurement, float time);

#endif