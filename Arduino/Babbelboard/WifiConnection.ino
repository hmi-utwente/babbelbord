//Here is the the position to the fitting category and sent to the server

//ERROR MESSAGES
String error1 = "Both pawns are at gaan";
String error2 = "Please move the pawn a little around in the square";
String error3 = "Please remove last placed pawn, and place it back after X seconds";

//Uses global variable CHECK
void PositionToNewCategory() {
  for (int i = 1; i < 6; i++) {// Outputs(rows)
    for (int j = 1; j < 6; j++) { // Inputs(column)
      if (FinalCheck[i][j] == HIGH) { 
//        Serial.print("i: ");
//        Serial.print(i,DEC);
//        Serial.print(", j: ");
//        Serial.print(j,DEC);
//        Serial.println("");      
        saveCategoryInt(categories[i-1][j-1]);       
      }
    }
  }
}
