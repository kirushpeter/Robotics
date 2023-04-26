// motor driver pins
#define ENA 13
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9
#define ENB 8

// IR PINS
#define LEFT_IR 22
#define IR2 24 // this is the left ir sensor
#define CENTER_IR 26 // this is the middle sensor
#define IR3 28 // this is the right sensor
#define RIGHT_IR 30

int delayTime = 1000;
int motorspeed = 100;

#define ECHO_PIN 5 // Pin for HC-SR04 ultrasonic sensor echo
#define TRIG_PIN 4 // Pin for HC-SR04 ultrasonic sensor trigger

#define STOP_DISTANCE 5 // Distance at which the robot should stop (in cm)

// Distance threshold for obstacle avoidance
#define OBSTACLE_DISTANCE 20

int state;
bool atJunction = false;
// States for the robot
#define LINE_FOLLOWING 0
#define TURNING_RIGHT 1
#define FOLLOWING_DESTINATION 2
#define OBSTACLE_AVOIDANCE 3

void setup() {
  pinMode(LEFT_IR, INPUT);
  pinMode(CENTER_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  analogWrite(ENA, motorspeed);
  analogWrite(ENB, motorspeed);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  int left = digitalRead(LEFT_IR);
  int center = digitalRead(CENTER_IR);
  int right = digitalRead(RIGHT_IR);
  
  if (state == 0) { // Following line
    if (center == 1 && left == 0 && right == 0) { // Move forward if center sensor detects line
      forward();
    } else if (center == 1 && left == 1 && right == 0) { // Turn left if left sensor detects line
      turnRight();
    } else if (center == 1 && left == 0 && right == 1) { // Turn right if right sensor detects line
      turnLeft();
    } else { // Stop if no sensors detect line
      stopTheCar();
    }
    
    // Check if the robot has reached a junction
    if (center == 1 && left == 1 && right == 1) {
      atJunction = true;
      state = 1; // Enter turn right state
    }
    
  } else if (state == 1) { // Turning right
    stopTheCar();
    turnRight();
    delay(delayTime); // Turn for 1 second
    state = 0; // Enter following line state
    
  } else if (state == 2) { // Following destination
    // TODO: Implement logic for following destination
  // Measure distance using HC-SR04 ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    int distance = duration * 0.034 / 2; // Convert duration to cm
    Serial.println(distance);
    if (distance > STOP_DISTANCE) { // Move forward if no obstacle is detected
      forward();
    } else { // Turn around if obstacle is detected
      turnRight();
      delay(delayTime); // Delay for turn
      state = 0; // Enter line following state
    }
  }
  
}

void forward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  }

void turnRight(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  }

void turnLeft(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  }

void stopTheCar(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  }
