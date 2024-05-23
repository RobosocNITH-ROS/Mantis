#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>

#define PWM_MIN 20
#define PWM_MAX 255 // Adjust this according to the maximum PWM range supported by your motor driver
#define L 0.35
#define R 0.0504

bool _connected = false;
bool rosConnected();
void onTwist(const geometry_msgs::Twist &msg);
float mapPwm(float x, float in_min, float in_max, float out_min, float out_max);

#define IN1_M1 5   // Arduino pin 4 is connected to L298N IN1 for motor 1.
#define IN2_M1 6   // Arduino pin 5 is connected to L298N IN2 for motor 1.
// #define ENA_M1 9   // Arduino pin 9 is connected to L298N ENA for motor 1 speed control.
#define IN1_M2 9   // Arduino pin 6 is connected to L298N IN1 for motor 2.
#define IN2_M2 10   // Arduino pin 7 is connected to L298N IN2 for motor 2.
// #define ENB_M2 10  // Arduino pin 10 is connected to L298N ENB for motor 2 speed control.

ros::NodeHandle node;
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &onTwist);
std_msgs::String str_msg;
ros::Publisher chatter("debug", &str_msg);
float l= 0, r= 0;

void setup() {
  Serial.begin(9600);
  node.initNode();
  node.subscribe(sub);
  node.advertise(chatter);
  pinMode(IN1_M1, OUTPUT);
  pinMode(IN2_M1, OUTPUT);
  // pinMode(ENA_M1, OUTPUT);
  pinMode(IN1_M2, OUTPUT);
  pinMode(IN2_M2, OUTPUT);
  // pinMode(ENB_M2, OUTPUT);
}

void onTwist(const geometry_msgs::Twist &msg) {
  if (!_connected) {
    Serial.println("Stopped");
    return;
  }

  // Debugging: Print received twist message
  Serial.print("Received Twist - Linear: ");
  Serial.print(msg.linear.x);
  Serial.print(", Angular: ");
  Serial.println(msg.angular.z);

  float l = (1 / R) * (msg.linear.x - msg.angular.z * L / 2);
  float r = (1 / R) * (msg.linear.x + msg.angular.z * L / 2);
  float lPwm, rPwm;

  // Calculate PWM values with adjusted mapping
  lPwm = mapPwm(l, -0.22, 0.22, -PWM_MAX, PWM_MAX);
  rPwm = mapPwm(r, -0.22, 0.22, -PWM_MAX, PWM_MAX);

  // Debugging: Print calculated PWM values
  Serial.print("Calculated PWM - Left: ");
  Serial.print(lPwm);
  Serial.print(", Right: ");
  Serial.println(rPwm);

  // Apply PWM values to motors
  // if (l>= 0 ) {
  //   analogWrite(IN1_M1,lPwm);
  //   analogWrite(IN2_M1, 0);
  // } else {
  //   analogWrite(IN1_M1, 0);
  //   analogWrite(IN2_M1, lPwm);
  //}
    // if (l>= 0) {
    //     digitalWrite(IN1_M1,HIGH);
    //     digitalWrite(IN2_M1, LOW);
    //   } else {
    //     digitalWrite(IN1_M1, LOW);
    //     digitalWrite(IN2_M1, HIGH);
    //   }

  // if (r >= 0) {
  //   analogWrite(IN1_M2, rPwm);
  //   analogWrite(IN2_M2, 0);
  // } else {
  //   analogWrite(IN1_M2, 0);
  //   analogWrite(IN2_M2, rPwm);

  // }
  if (lPwm>=0 && rPwm>=0){
    analogWrite(IN1_M1, lPwm);
    analogWrite(IN2_M1, 0);
    analogWrite(IN1_M2, rPwm);
    analogWrite(IN2_M2, 0);
  }
  else if (lPwm<=0 && rPwm<=0){
    analogWrite(IN1_M1, 0);
    analogWrite(IN2_M1, lPwm);
    analogWrite(IN1_M2, 0);
    analogWrite(IN2_M2, rPwm); 
  }
  else if (lPwm>=0 && rPwm<=0){
    analogWrite(IN1_M1, 0);
    analogWrite(IN2_M1, lPwm);
    analogWrite(IN1_M2, rPwm);
    analogWrite(IN2_M2, 0); 
  }
  else {
    analogWrite(IN1_M1, lPwm);
    analogWrite(IN2_M1, 0);
    analogWrite(IN1_M2, 0);
    analogWrite(IN2_M2, rPwm); 
  }

  // if (R>= 0) {
  //   digitalWrite(IN1_M2,HIGH);
  //   digitalWrite(IN2_M2, LOW);
  // } else {
  //   digitalWrite(IN1_M2, LOW);
  //   digitalWrite(IN2_M2, HIGH);
  // }

 delay(1);
}


void loop() {
  if (!rosConnected()) {
    Serial.println("Stopped");
  }
  node.spinOnce();
}

bool rosConnected() {
  // If value changes, notify via LED and console.
  bool connected = node.connected();
  if (_connected != connected) {
    _connected = connected;
    digitalWrite(LED_BUILTIN, !connected); // false -> on, true -> off
    Serial.println(connected ? "ROS connected" : "ROS disconnected");
  }
  return connected;
}

float mapPwm(float x, float in_min, float in_max, float out_min, float out_max) {
  float scaling_factor = (out_max - out_min) / (in_max - in_min);
  return (x - in_min) * scaling_factor + out_min;
}
