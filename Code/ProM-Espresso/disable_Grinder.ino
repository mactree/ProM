void grinderDisable() {

  // send time 0 to disable grinding while in Setup
  Serial.write(240);
  Serial.write(6);
  Serial.write(12);
  Serial.write((byte)0);
  Serial.write((byte)0);
  Serial.write((byte)0);
  Serial.write((byte)0);
  Serial.write((byte)0);
  Serial.write(254);
}
