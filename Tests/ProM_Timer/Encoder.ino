// Interrupt on A changing state
void doEncoderA() {
  // debounce
  if (rotating)
    delay(1);  // wait a little until the bouncing is done

  // Test transition, did things really change?
  if (digitalRead(ENCODER_A_PIN) != A_set) {  // debounce once more
    A_set = !A_set;

    // adjust counter +1 if A leads B
    if (A_set && !B_set)
      encoderPos++;

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB() {
  if (rotating)
    delay(1);

  if (digitalRead(ENCODER_B_PIN) != B_set) {
    B_set = !B_set;
    //  adjust counter -1 if B leads A
    if (B_set && !A_set)
      encoderPos--;

    rotating = false;
  }
}
