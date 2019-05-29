#include <SoftwareSerial.h>
#include "sender.h"
#include "Arduino.h"

#define BUFFER_MAX_SIZE 50

SoftwareSerial mySerial(RF_RX, RF_TX); //RX, TX
SoftwareSerial BTSerial(BT_RX, BT_TX); // RX | TX
long previousMillis = 0;
long pingBackMillis = 0;
long prevListenMillis = 0;
GasValues *mGasValues;
byte pingBack[2];
int bufferIndex;
DATASTATE state;
char inputBuffer[BUFFER_MAX_SIZE] = "";
char pingPackage[2] = {'f', 'u'};
char confirmCon[3] = {'c', 'o', 'n'};
int connectionFlag = 0;
int listenFlag = 0;

void initSender(GasValues *gas_main)
{
  Serial.begin(9600);
  mySerial.begin(9600);
  BTSerial.begin(9600);

  pinMode(RF_RX, INPUT);
  pinMode(RF_TX, OUTPUT);

  /*mGasValues = gas_main;

  mGasValues->CH4 = CH4_init_val;
  mGasValues->IBUT = IBUT_init_val;
  mGasValues->O2 = O2_init_val;
  mGasValues->CO = CO_init_val;

  bufferIndex = 0;
  state = CLEARED;*/
}
void readInput()
{
  //BTSerial.listen();
  if (BTSerial.available())
  {
    state = AVAILABLE;
    BTSerial.readBytes(inputBuffer, 32);
    if (headerCheck())
    {
      mySerial.listen();
      listenFlag = 1;
    }
    for (int i = 0; i < 32; i++)
    {
      Serial.println(inputBuffer[i]);
    }

    // read the incoming byte:

    //Serial.print(" I received:");

    /*Serial.println(inputBuffer[bufferIndex]);
    if (bufferIndex < BUFFER_MAX_SIZE)
    {
      bufferIndex++;
    }*/

    /*else
    {
      sendGasPackage(inputBuffer); //send the package if it is full
      clearBuffer();               //clear the buffer
      bufferIndex = 0;
    }*/
  }
  else
  {
    if (state == AVAILABLE)
    {
      bufferIndex = 0;
      sendGasPackage(inputBuffer);
      clearBuffer();
      state = CLEARED;

      Serial.println("updated");
    }
    //mySerial.listen();
  }
}

void sendGasPackage(char *package)
{

  mySerial.write((byte *)package, BUFFER_MAX_SIZE);
  mySerial.flush();
}

void clearBuffer()
{
  for (int i = 0; i < BUFFER_MAX_SIZE; ++i)
  {
    inputBuffer[i] = '0';
  }
}

void sendPing(long interval)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval && listenFlag == 1)
  {
    sendGasPackage(pingPackage);
    Serial.println("ping sent");
    previousMillis = currentMillis;
  }
}

void connectionCheck(long interval)
{
  unsigned long currentMillis = millis();
  if (currentMillis - pingBackMillis > interval && connectionFlag == 0)
  {
    Serial.println("connection lost");
    connectionFlag = 1;
  }
}

void listen()
{
  //sendPing(1000);
  //mySerial.listen();
  //Serial.println(mySerial.available()); //Serial.println("");
  if (mySerial.available() > 0)
  {
    Serial.println("incoming message");
    mySerial.readBytes(pingBack, 2);

    if (pingCheck())
    {
      Serial.println("ping recieved");
      pingBackMillis = millis();
      BTSerial.listen();
      BTSerial.write(confirmCon);
      BTSerial.flush();
      listenFlag = 0;
    }
  }
}

int pingCheck()
{
  for (int i = 0; i < 2; i++)
  {
    if ((char)pingBack[i] != pingPackage[i])
    {
      return 0;
    }
  }
  return 1;
}

int headerCheck()
{
  for (int i = 0; i < 5; i++)
  {

    if (header[i] == (char)inputBuffer[i])
    {
      continue;
    }
    else
    {
      return 0;
    }
  }
  return 1;
  /*if (strncmp(header, (char *)gasValues, 8) == 0)
    return 1;
  else
    return 0;*/
}

/*void toggleListen(long interval)
{
  unsigned long currentMillis = millis();
  if (currentMillis - prevListenMillis > interval)
  {
    if (listenFlag == 0)
    {
      listenFlag = 1;
      BTSerial.listen();
    }
    else if (listenFlag == 1)
    {
      listenFlag = 0;
      mySerial.listen();
    }
  }
}*/