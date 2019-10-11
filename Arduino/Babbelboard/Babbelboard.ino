//row1:White-cable: Gaan,familie, liefde, Het is je gelukdsag, tienertijd
//row2:           :Kindertijd,Hobbies,Familie,Liefde,Ga terug naar je vorige kleurvak
//row3:           :Tienertijd,Kindertijd,Hobbies,Familie,Liefde
//row4:           :Verwijder een verdiende kleurkaart,Tienertijd,Kindertijd,Hobbies,Familie
//row5:           :Liefde,Geef de laast verdiende kaart aan de vorige speler,Tienertijd,Kindertijd,Hobbies

//row 1:
//------/  ----/ ----/ ------/  ---/  -----
//row2  |       |     |       |     |
//------/  ----/ ----/ ------/  ---/  -----
//row3  |       |     |       |     |
//------/  ----/ ----/ ------/  ---/  -----
//row 4 |       |     |       |     |
//------/  ----/ ----/ ------/  ---/  -----
//row5  |       |     |       |     |
//------/  ----/ ----/ ------/  ---/  -----
//      |       |     |       |     |
//      col1    col2   col 3   col4  col 5

#include <WiFi.h>
#include <HTTPClient.h>
#include "arduino_secrets.h"

//Wifi connection settings
const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
//String url = "http://localhost:8081/api/category/";
String url = "http://babbelbord.herokuapp.com/api/category/";

WiFiClient sender;
HTTPClient http;

// Babbelboard
// Global variables
#define DELAY 50000
#define CHECKRIGHT 80000

// Timer things:
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds

//Memory things:
int row1 = 0;
int col1 = 0;
int row2 = 0;
int col2 = 0;
int pawn = 1;
int locations;
int previousLocations = 0;

//Buffer for reading sensory input
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total;
double average;

// Multiple demensional array representing positions on the babbel board

// pins for inputs (column) and outputs (row)
int row[6] = {0, 12, 21, 25, 4, 26}; //0, white, purple, brown, orange, grey
int column[6] = {0, 14, 33, 27, 32, 13}; // 0, pink, green, blue, yellow, red
int categories[5][5] = {{0,1,2,3,4},{5,6,1,2,7},{4,5,6,1,2},{8,4,5,6,1},{2,9,4,5,6}};


//OLD (Arduino Mega configuration):
// pins for inputs and outputs
//int row[6] = {0, 25, 23, 22, 52, 53};
//int column[6] = {0, 50, 48, 46, 44, 42};

//Counters for the buttons and time
int count = 0;// counter for how many time has passed
int count2 = 0; // counter for the button
int Interval; // Timer for interval
int start = 0; // Counting when pawn is at start

//Used for analysing the data, all the arrays start at 1
int DataNew[6][6] = { 0 };//  Used to save the most updated changes on the board
int DataOld[6][6] = { 0 }; // Used to save the position if the pawn is placed
int CheckData[6][6] = { 0 }; // To check former position witht he new position
int FinalCheck[6][6] = { 0 };// Save the final position and map to category
String category;// Category sent to server
int statusCode = 0;// a variable to read incoming serial data into

//Categories
String OldCategory = {};

void setup() {
    Serial.begin(115200);
    delay(10);

  for(int i = 1; i < 6; i++){
    pinMode(column[i],INPUT);
    pinMode(row[i], OUTPUT);
  } 
  connectWIFI();
  startMillis = millis();
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  Serial.println("Started"); 
}

void loop() {
  currentMillis = millis();
  if(currentMillis - startMillis >= period){
    readPosition();
    startMillis = currentMillis;  
  }
}

void connectWIFI()
{
  Serial.print("Connecting to WIFI network: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("My IP address is ");
  Serial.println(WiFi.localIP());
}


void sendPostRequest(int categoryID){
//  Serial.println("making POST request");
  String body;
  String message;
  Serial.print("Received category:");
  Serial.print(categoryID, DEC);
  Serial.println("");
  switch(categoryID){
    case 0:
      //Start
      message = "S";
      body = "{\"special\": \"" + message + "\"}";         
      break;
    case 3:
      //Het is je geluksdag
      message = "H";
      body = "{\"special\": \"" + message + "\"}";         
      break;
    case 7:
      //Ga terug
      message = "G";
      body = "{\"special\": \"" + message + "\"}";         
      break;
    case 8:
      //Verwijder kaart
      message = "V";
      body = "{\"special\": \"" + message + "\"}";         
      break;
    case 9:
      // Geef laatstverdiende kaart
      message = "GE";
      body = "{\"special\": \"" + message + "\"}";         
      break;    
    case 1:
      message = "Familie";
      body = "{\"name\": \"" + message + "\"}";         
      break;
    case 2:
      message = "Liefde";
      body = "{\"name\": \"" + message + "\"}";         
      break;    
    case 4:
      message = "Tienertijd";
      body = "{\"name\": \"" + message + "\"}";         
      break;
    case 5:
      message = "Kindertijd";
      body = "{\"name\": \"" + message + "\"}";         
      break;
    case 6:
      message = "Hobbies";
      body = "{\"name\": \"" + message + "\"}";         
      break;
    case 10:
      // Movepawn
      message = "MP";
      body = "{\"special\": \"" + message + "\"}";   
      break;      
    case 11:
      // MovepawnToo Fast
      message = "MPF";
      body = "{\"special\": \"" + message + "\"}";         
      break;
    default:
      body = "";
      Serial.println("Unknown message: " + message);          
  }
  if(body != ""){
    Serial.print("Posting to server: ");
    Serial.print(message);
    Serial.println("");
    http.begin(url); //HTTP
    http.addHeader("Content-Type", "application/json");  
    int response = http.POST(body); 
    Serial.print("Response: ");
    Serial.print(response, DEC);
    Serial.println("");
  }  
}
