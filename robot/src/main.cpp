
#include <AccelStepper.h>
//Right motor
#define R_IN1 6
#define R_IN2 7
#define R_IN3 8
#define R_IN4 9

//Left motor
#define L_IN1 2
#define L_IN2 3
#define L_IN3 4
#define L_IN4 5

AccelStepper l_motor(8, R_IN4, R_IN2, R_IN3, R_IN1);

AccelStepper r_motor(8, L_IN4, L_IN2, L_IN3, L_IN1);

//AccelStepper s28BYJ_48(8, L_IN1, L_IN2, L_IN3, L_IN4);

//Sensors pins
#define LEFT_SENSOR_PIN  A2
#define RIGHT_SENSOR_PIN A1
 
// Скорость, с которой мы движемся вперёд (0-255)
#define SPEED            150
 
// Скорость прохождения сложных участков
#define SLOW_SPEED       35
 
#define BACK_SLOW_SPEED  30
#define BACK_FAST_SPEED  50
 
// Коэффициент, задающий во сколько раз нужно затормозить
// одно из колёс для поворота
#define BRAKE_K          8
 
#define STATE_FORWARD    0
#define STATE_RIGHT      1
#define STATE_LEFT       2
 
#define SPEED_STEP       2
 
#define FAST_TIME_THRESHOLD     500
 
int state = STATE_FORWARD;
int currentSpeed = SPEED;
int fastTime = 0;
 
void runForward() 
{
    state = STATE_FORWARD;
 
    fastTime += 1;
    if (fastTime < FAST_TIME_THRESHOLD) {
        currentSpeed = SLOW_SPEED;
    } else {
        currentSpeed = min(currentSpeed + SPEED_STEP, SPEED);
    }

    
    Serial.print("FORWARD");
    Serial.println(currentSpeed);
    r_motor.setSpeed(currentSpeed);
    l_motor.setSpeed(-currentSpeed);

    r_motor.runSpeed();
    l_motor.runSpeed();
}
 
void steerRight() 
{
    state = STATE_RIGHT;
    fastTime = 0;
 
    // Замедляем правое колесо относительно левого,
    // чтобы начать поворот
    Serial.println("RIGHT");

    r_motor.setSpeed(SPEED);
    l_motor.setSpeed(0);

    r_motor.runSpeed();
    l_motor.runSpeed();
}
 
void steerLeft() 
{
    state = STATE_LEFT;
    fastTime = 0;

    Serial.println("LEFT");


    r_motor.setSpeed(0);
    l_motor.setSpeed(-SPEED);

    r_motor.runSpeed();
    l_motor.runSpeed();
 

}
 
 
void stepBack(int duration, int state) {
    if (!duration)
        return;

    Serial.println("BACK");
 
    // В зависимости от направления поворота при движении назад будем
    // делать небольшой разворот 
    int leftSpeed = (state == STATE_RIGHT) ? BACK_SLOW_SPEED : BACK_FAST_SPEED;
    int rightSpeed = (state == STATE_LEFT) ? BACK_SLOW_SPEED : BACK_FAST_SPEED;

    r_motor.setSpeed(-rightSpeed);
    l_motor.setSpeed(leftSpeed);

    r_motor.runSpeed();
    l_motor.runSpeed();
 
    delay(duration);
}
 
 
void setup() 
{
    Serial.begin(9600);
    // Настраивает выводы платы 4,5,6,7 на вывод сигналов 
    for(int i = 2; i <= 9; i++)
        pinMode(i, OUTPUT);

    r_motor.setMaxSpeed(1000);
    l_motor.setMaxSpeed(1000);

    r_motor.setAcceleration(100);
    l_motor.setAcceleration(100);

    runForward();
} 
 
void loop() 
{ 
    // Наш робот ездит по белому полю с чёрным треком. В обратном случае не нужно
    // инвертировать значения с датчиков
    boolean left = !digitalRead(LEFT_SENSOR_PIN);
    boolean right = !digitalRead(RIGHT_SENSOR_PIN);

 
    // В какое состояние нужно перейти
    int targetState;
 
    if (left == right) {
        // под сенсорами всё белое или всё чёрное
        // едем вперёд
        targetState = STATE_FORWARD;
    } else if (right) {
        // левый сенсор упёрся в трек
        // поворачиваем налево
        targetState = STATE_LEFT;
    } else {
        targetState = STATE_RIGHT;
    }
 
//    if (state == STATE_FORWARD && targetState != STATE_FORWARD) {
//        int brakeTime = (currentSpeed > SLOW_SPEED) ?
//            currentSpeed : 0;
//        stepBack(brakeTime, targetState);
//    }
 
    switch (targetState) {
        case STATE_FORWARD:
            runForward();
            break;
 
        case STATE_RIGHT:
            steerRight();
            break;
 
        case STATE_LEFT:
            steerLeft();
            break;
    }
 
}