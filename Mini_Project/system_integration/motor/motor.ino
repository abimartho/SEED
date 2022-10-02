#include <Encoder.h>
#include <DualMC33926MotorShield.h>

DualMC33926MotorShield md;
#define ENC_A 3
#define ENC_B 5
Encoder myEnc(ENC_A, ENC_B);

long angleV[401]={0};
double velocity=0;

/*
void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while(1);
  }
}
*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Dual MC33926 Motor Shield");
  md.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 1; i <= 400; i++)
  {
    md.setM1Speed(i);
    //stopIfFault();
    if (abs(i)%200 == 100)
    {
      angleV[i]=myEnc.read();
      velocity=860*(angleV[i]-angleV[i-1])/6;
     
      Serial.print(millis());
      Serial.print("\t");
      Serial.println(angleV[i]);
      //Serial.println(velocity);
     
    }
    delay(.005);
  }
 /*  
  for (int i = 400; i >= -400; i--)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("Encoder: ");
      Serial.println(myEnc.read());
      //Serial.print("M1 current: ");
      //Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }
   
  for (int i = -400; i <= 0; i++)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("Encoder: ");
      Serial.println(myEnc.read());
      //Serial.print("M1 current: ");
      //Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }
  */
 
}
