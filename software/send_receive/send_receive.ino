// demo: CAN-BUS Shield, receive data
#include <mcp_can.h>
#include <SPI.h>

#define MYID 0x02

unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];
INT32U sender_id;
unsigned char stmp[8] = {11, 12,  13, 3, 4, 5, 6, 7};

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting Script");
  Serial.print("My ID is:");
  Serial.println(MYID);
  Serial.print("Starting CAN: ");
  
  CAN.begin(CAN_500KBPS);                       // init can bus : baudrate = 500k
  // init can bus, baudrate: 500k
  if(CAN.begin(CAN_500KBPS) == CAN_OK) 
	Serial.println("CAN init ok!!");
  else Serial.println("CAN init fail!!");

  attachInterrupt(0, MCP2515_ISR, FALLING);     // start interrupt
}

void MCP2515_ISR()
{
    Flag_Recv = 1;
}

void loop()
{
    if(Flag_Recv)                           // check if get data
    {
      Flag_Recv = 0;                        // clear flag
	      
		// iterate over all pending messages
		// If either the bus is saturated or the MCU is busy,
		// both RX buffers may be in use and reading a single
		// message does not clear the IRQ conditon.
		while (CAN_MSGAVAIL == CAN.checkReceive()) {
			sender_id = CAN.getCanId();          // get senders id
			CAN.readMsgBuf(&len, buf);            // read data,  len: data length, buf: data buf
			Serial.println("RECEIVED: ");
			Serial.print("sender id = ");
			Serial.print(sender_id);
			Serial.print(", data len = ");
			Serial.print(len);
      
			Serial.print(", data: ");
			for(int i = 0; i<len; i++)            // print the data
			{
				Serial.print(buf[i]);Serial.print("\t");
			}
			Serial.println();
		}
    }
	// send data:  id = MYID, standrad flame, data len = 8, stmp: data buf
    CAN.sendMsgBuf(MYID, 0, 8, stmp);
    Serial.println("sent message");
    
    delay(random(100,500));
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
