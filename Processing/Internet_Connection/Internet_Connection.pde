import processing.serial.*; // use serial port libraries
import processing.net.*; 
import http.requests.*;

//Configuration
int arduinoPort = 0;
String url = "http://localhost:8081/api/category/";
//String url = "https://babbelbord.herokuapp.com/api/category/";

Serial myPort; // make a fresh serial port
String val;

//For setup we want to connect to the port the Arduino is sending commands (format: {"type":"value"})
void setup()
{   
    myPort = new Serial(this, Serial.list()[arduinoPort], 9600); // open port 0 in the list
}

//Continuously check the output of the port for new data if the Arduino is connected.
void draw()
{
    if(myPort.available() > 0){
        val = myPort.readStringUntil('\n');
        if(val != null){
            postRequest(val.trim());
        }
    }
}

void postRequest(String message) {
      PostRequest post = new PostRequest(url);
      post.addHeader("Content-Type", "application/json");      
      System.out.println("Received message: " + message);
      if (message.equals("Familie") ||
          message.equals("Liefde") || 
          message.equals("Tienertijd") || 
          message.equals("Kindertijd") || 
          message.equals("Hobbies")){
          post.addJson("{\"name\": \"" + message + "\"}");
      } else {
        switch(message){
          case "START":
            message = "S";
            break;
          case "Gaan":
            message = "S";
            break;
          case "Geef":
            message = "GE";
            break;
          case "Ga":
            message = "G";
            break;
          case "Het":
            message = "H";
            break;
          case "Verwijder":
            message = "V";
            break;
          case "MOVEPAWN":
            message = "MP";
            break;
          case "MOVEDPAWNTOOFAST":
            message = "MPF";
            break;
          default:
            System.out.println("Unknown message: " + message);          
        }
        post.addJson("{\"special\": \"" + message + "\"}");
      }
      System.out.println("Posting to server: " + message);
      post.send();
    }
