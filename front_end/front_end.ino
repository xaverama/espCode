

 

 

#if ! (ESP8266 || ESP32 )

  #error This code is intended to run on the ESP8266/ESP32 platform! Please check your Tools->Board setting

#endif

 

#include "Credentials.h"

 

#define MYSQL_DEBUG_PORT      Serial

 

// Debug Level from 0 to 4

#define _MYSQL_LOGLEVEL_      1

 

#include <MySQL_Generic_WiFi.h>

 

#include <SPI.h>

#include <MFRC522.h>

#define SS_PIN D4

#define RST_PIN D3

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

 

IPAddress server_addr(192, 168, 0, 32);  //  IMMER DIE AKTUELLE!!!! //

uint16_t server_port = 3306;    //3306;

 

char default_database[] = "pointbreakproject";           //"test_arduino";

char default_table[]    = "cardid";          //"test_arduino";

 

    

 

MySQL_Connection conn((Client *)&client);

 

MySQL_Query *query_mem;

 

void setup()

{

  Serial.begin(115200);

  while (!Serial);

 

SPI.begin();      // Initiate  SPI bus

  mfrc522.PCD_Init();   // Initiate MFRC522

  Serial.println("Approximate your card to the reader...");

  Serial.println();

  Serial.println("\nStarting Basic_Insert_ESP on " + String(ARDUINO_BOARD));

 

  // Begin WiFi section

  Serial.println(String("Connecting to ") + ssid);

 

  WiFi.begin(ssid, pass);

 

  while (WiFi.status() != WL_CONNECTED)

  {

    delay(500);

    Serial.print(".");

  }

 

  // print out info about the connection:

  Serial.print("Connected to network. My IP address is: ");

  Serial.println(WiFi.localIP());

 

  Serial.print("Connecting to SQL Server @ ");

  Serial.print(server_addr);

  Serial.println(String(", Port = ") + server_port);

  Serial.println(String("User = ") + user + String(", PW = ") + password + String(", DB = ") + default_database);

}

 

void runInsert(String default_value)

{

 

 

//String default_value    = "Hello, Arduino!";

 

 

String INSERT_SQL = String("INSERT INTO ") + default_database + "." + default_table

                + " (id) VALUES ('" + default_value + "')";

 

 

  MySQL_Query query_mem = MySQL_Query(&conn);

 

  if (conn.connected())

  {

    Serial.println(INSERT_SQL);

   

    // Execute the query

    // KH, check if valid before fetching

    if ( !query_mem.execute(INSERT_SQL.c_str()) )

      Serial.println("Insert error");

    else   

      Serial.println("Data Inserted.");

  }

  else

  {

    Serial.println("Disconnected from Server. Can't insert.");

  }

}

 

void loop()

{

  // Look for new cards

  if ( ! mfrc522.PICC_IsNewCardPresent())

  {

       return;

  }

 

  // Select one of the cards

  if ( ! mfrc522.PICC_ReadCardSerial())

  {

    return;

  }

  //Show UID on serial monitor

  Serial.print("UID tag :");

  sql();

  String content= "";

  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++)

  {

     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");

     Serial.print(mfrc522.uid.uidByte[i], DEC);

     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));

     content.concat(String(mfrc522.uid.uidByte[i], DEC));

  }

 

  runInsert(content);

  delay(3000);

  String test = content;

  Serial.println();

  Serial.print(content);

 

  }

 

  void sql(){

 

  Serial.println("Connecting...");

 

  if (conn.connectNonBlocking(server_addr, server_port, user, password) != RESULT_FAIL)

  {

    delay(500);

  

  }

  else

  {

    Serial.println("\nConnect failed. Trying again on next iteration.");

  }

 

  delay(1000);

  }

 

 

 
