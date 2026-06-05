#include <IcsHardSerialClass.h>
#include <Adafruit_BNO055.h>

float getYaw(Adafruit_BNO055 bno);
void setMotor(IcsHardSerialClass krs,float fl,float fr,float rl,float rr);
void moveMecanum(IcsHardSerialClass krs,float speed,float directionDeg);