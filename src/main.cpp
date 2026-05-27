#include <Arduino.h>
#include <ICSHardSerialClass.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>

#define BNO055_SAMPLERELAYTIME_MS (100) ///< BNO055のセンサーデータのサンプリング間隔を100msに設定

IcsHardSerialClass krs(&Serial1, 2, 1250000, 100); ///< ICS通信クラスのインスタンスを作成。HardwareSerialはSerial1、イネーブルピンは2、通信速度は1250000bps、タイムアウトは100ms
Adafruit_BNO055 bno = Adafruit_BNO055(55,0x28,&Wire); ///< BNO055のインスタンスを作成

const int IDs[4] = {0,1,2,3};

float lastAccX = 0;
float lastAccY = 0;
float lastAccZ = 0;
float alpha = 0.8; // フィルタ係数
float yaw0 = 0; //初期yaw


const float THRESHOLD = 15.0; ///< 衝撃を検知するための加速度の閾値 (単位: m/s^2)

void setup() {
  Serial.begin(115200);
  Wire.begin();
  krs.begin();
  bno.begin();
  bno.setExtCrystalUse(true); // 外部クリスタルを使用する設定
  delay(1000); // BNO055の初期化が安定するまで待機
  Serial.println("BNO055 initialized.");
  krs.setPos(0, 7500); //サーボID1に目標値7500を設定
  krs.setPos(1, 7500); //サーボID2に目標値7500を設定
  krs.setPos(2, 7500); //サーボID3に目標値7500を設定
  krs.setPos(3, 7500); //サーボID4に目標値7500を設定
  Serial.println("All servos are ready.");

  yaw0 = euler.x();

}
void loop() {
  //linear accelleration
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  float currentAccX = acc.x();
  float currentAccY = acc.y();
  float currentAccZ = acc.z();
  float jerkX = abs(currentAccX - lastAccX);
  float jerkY = abs(currentAccY - lastAccY);
  float jerkZ = abs(currentAccZ - lastAccZ);
  // yaw
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  float yaw = euler.x() - yaw0;

  // -180～180へ正規化
  if (yaw > 180) yaw -= 360;
  if (yaw < -180) yaw += 360;

  Serial.println(yaw);
  if (jerkX > 10.0 || jerkY > 10.0 || jerkZ > 10.0) { // 急激な変化のみを検知
     // すぐに停止させる関数など
    Serial.println("衝突により緊急停止！");
  }
  
  lastAccX = currentAccX;
  lastAccY = currentAccY;
  lastAccZ = currentAccZ;
  delay(BNO055_SAMPLERELAYTIME_MS);
  Serial.println("Current Acc Y: "+String(currentAccY)+" Jerk Y: "+String(jerkY));
}