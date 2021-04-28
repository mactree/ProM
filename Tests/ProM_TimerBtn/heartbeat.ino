// heartbeat, response
void sendHeartbeat() {
  mySerial.write(240);
  mySerial.write(1);
  mySerial.write(6);
  mySerial.write(9);
}
