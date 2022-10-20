#include <math.h>
#include <stdbool.h>

#define WIDTH 15
#define MAGICNUMBER 1
#define SPEEDMULT 10 // To scale from target vector magnitude to relevant # range for PWM

void MoveLeftWheel(bool dir, int speed) {
  	HAL_GPIO_WritePin(L_MOTOR_DIR_GPIO_Port, L_MOTOR_DIR_Pin, dir);
  	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed);
  }

void MoveRightWheel(bool dir, int speed) {
	// NB! RIGHT MOTOR IS MOUNTED BACKWARDS RELATIVE TO MOTOR DIR
  	HAL_GPIO_WritePin(R_MOTOR_DIR_GPIO_Port, R_MOTOR_DIR_Pin, !dir);
  	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed);
  }

double determinant(int *vec1, int *vec2) {
  	return vec1[0] * vec2[0] + vec1[1] * vec2[1];
  }

double dotprod(int *vec1, int *vec2) {
  	return vec1[0] * vec2[1] - vec1[1] * vec2[0];
  }

double angleDiff(int *destVec, int currSpeed) {
	  double y_coord = (destVec[1] > currSpeed ? currSpeed : destVec[1]);
	  return atan2(destVec[0], y_coord);
  }

/// moveVector = int[2] -- [x_coord, y_coord]
void Go(int *moveVector, int currSpeed) {
	  bool hardL = (moveVector[1] == 0 && moveVector[0] < 0); // check if turn hard right
	  bool hardR = (moveVector[1] == 0 && moveVector[0] > 0); // check if turn hard left
	  bool dirL = (moveVector[1] > 0 + hardR); // check if go forward, add hardR check
	  bool dirR = (moveVector[1] > 0 + hardL); // check if go fwd, add hardL check

	  double turnAngle = angleDiff(moveVector, currSpeed);

	  int steerVect[2] = { moveVector[0], moveVector[1] - currSpeed }; // target vector

	  float speedRatio = fabs(turnAngle) * 2 * WIDTH * fabs(sin(turnAngle/2)) * MAGICNUMBER; // this is maybe very wrong

	  int speed = sqrt(steerVect[0]*steerVect[0] + steerVect[1]*steerVect[1]) * SPEEDMULT;

	  // hardR/hardL are 0 unless true, if turn angle is > 0, L wheel should go faster by N, and the reverse for R

	  int driveSpeedL = hardR * speed + hardL * speed + (!hardR && !hardL) * (speed + (turnAngle > 0) * speed * speedRatio);
	  int driveSpeedR = hardR * speed + hardL * speed + (!hardR && !hardL) * (speed + (turnAngle < 0) * speed * speedRatio);

	  MoveLeftWheel(dirL, (int) driveSpeedL);
	  MoveRightWheel(dirR, (int) driveSpeedR);

}
