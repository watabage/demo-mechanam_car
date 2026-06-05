#include "mecanum.h"

    float yaw = 0;
    float targetYaw = 0;

void setMotorSpd(IcsHardSerialClass krs,Uint8_array servoIDs,float flSpd,float frSpd,float rlSpd,float rrSpd){
    krs.setPos(servoIDs[0],7500+flSpd);
    krs.setPos(servoIDs[1],7500+frSpd);
    krs.setPos(servoIDs[2],7500+rlSpd);
    krs.setPos(servoIDs[3],7500+rrSpd);
}

void moveMecanum(IcsHardSerialClass krs,Uint8_array servoIDs,float speed,float directionDeg,float duration = 0,Adafruit_BNO055 bno){
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    float yaw = euler.x();   // heading = yaw
    float roll = euler.y();
    float pitch = euler.z();

    unsigned long Kp = 10.0;
    float error = targetYaw - yaw;
    // -180〜180へ正規化
    while(error > 180) error -= 360;
    while(error < -180) error += 360;

    float rot = Kp * error;
    
    float rad = directionDeg * DEG_TO_RAD;

    float vx = cos(rad) * speed;
    float vy = sin(rad) * speed;

    float flSpd = vy + vx + rot;
    float frSpd = vy - vx - rot;
    float rlSpd = vy - vx + rot;
    float rrSpd = vy + vx - rot;

    float maxv = max(max(abs(flSpd), abs(frSpd)),max(abs(rlSpd), abs(rrSpd)));
    if(maxv > 255){
        flSpd = flSpd / maxv * 255;
        frSpd = frSpd / maxv * 255;
        rlSpd = rlSpd / maxv * 255;
        rrSpd = rrSpd / maxv * 255;
    }

    setMotorSpd(krs,servoIDs,flSpd, frSpd, rlSpd, rrSpd);
    if(duration > 0){
        delay(duration);
        stopMecanum(krs,servoIDs,duration);
        return;
    }
    

}
void stopMecanum(IcsHardSerialClass krs,Uint8_array servoIDs,float duration){
    for(int i=0;i<4;i++){
        krs.setPos(servoIDs[i],7500);
    }
}
