/***********************************************************/
/* 
 *  Open Source MP3 Player designed for kids
 *  Envoirement: DEV
 */

#define LED 9

#include <SoftwareSerial.h>

SoftwareSerial myfeature(11, 12); // RX, TX

void specialfeature(int SWITCHKey){

    Serial.println("Switch=" + String(SWITCHKey));
    Serial.println(String(digitalRead(LED)));
    if(digitalRead(LED)){
      digitalWrite(LED, 0);
    }else{
      digitalWrite(LED, 1);
    }
    while (!digitalRead(SWITCHKey)) {}
}
