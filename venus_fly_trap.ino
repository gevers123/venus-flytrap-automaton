#include <Automaton.h>
#include <State.h>
#include <Transition.h>

#include <Servo.h>

Servo servo;
Automaton* automaton;
State* currentState;


void setup() {
  // serial monitor for debugging:
  Serial.begin(9600);
  Serial.print("setting up...\n");

// sensor pins:
#define SENSOR1 8
  pinMode(SENSOR1, INPUT);
#define SENSOR2 7
  pinMode(SENSOR2, INPUT);

  servo.attach(9);

// automaton init with start state at q0:
  automaton    = initAutomaton();
  currentState = automaton->q0;
}



void loop() {
  // if the sensor is pressed once, move to q1 and start the timer
  if (digitalRead(SENSOR1) == HIGH || digitalRead(SENSOR2) == HIGH) {
    Serial.print("sensor pressed once, moving to q1...\n");
    currentState = transition(currentState, '1', automaton->T);
    start_timer();
  // else, move to q0 and open
  } else {
    Serial.print("open, moving to q0\n");
    currentState = transition(currentState, '0', automaton->T);
  }
  // if in the start state, open
  if (currentState->isStart) {
        servo.write(0);
  }

  delay(100);
}

/** start_timer
 * if the sensor is pressed within 20 seconds, move to state q2
 * if the current state is a final state, close and stop timer
*/
void start_timer() {
  delay(1000);
  int ms = 0;
  Serial.print("starting timer...\n");
  while (ms < 20000) {
    if (digitalRead(SENSOR1) == HIGH || digitalRead(SENSOR2) == HIGH) {
      Serial.print("moving to q2...\n");
      currentState = transition(currentState, '1', automaton->T);
    }
    if (currentState->isFinal) {
      Serial.print("Final state detected, closing...\n");
      servo.write(90);
      ms = 20000;
    } else {
      delay(1);
      ms++;
    }
  }
  delay(1000);
}
