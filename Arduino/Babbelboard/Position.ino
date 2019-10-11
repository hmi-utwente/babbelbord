//Positions get read from the board in DataNew and saved later DataOld.
// This data is compared, if the old positions are still the same as before. 
// If the positions are not changed after a certain time the new positions are detected
// and sent to the wifi connection. After this process, both the DateNew and DataOld are reset



// Reads dataNew
int CompareFormerPositionWithNewPosition() {
  //Check if data equal
  int locations = 0;// number of locations
  count2 = 0;
  for (int i = 1; i < 6; i++) {
    for (int j = 1; j < 6; j++) {
      if ( DataOld[i][j] == CheckData[i][j]) {
        count2++;
      }
      CheckData[i][j] = DataOld[i][j];
      if ( DataOld[i][j] == HIGH) {
        locations++;
      }
    }
  }
  return locations;
}

// Checkes if only 1 category is detected
int checkFinalLocations() {
  //Check if data equal
  int detectedLocations = 0;// number of locations
  for (int i = 1; i < 6; i++) {
    for (int j = 1; j < 6; j++) {
      if ( FinalCheck[i][j] == HIGH) {
        detectedLocations++;
      }
    }
  }
  return detectedLocations;
}

void IntervalCheck( int locations) {
  if (count2 == 25)// Because of 25 positions
    //0,1,2
  {
    // Timer for intervals
    if (Interval < DELAY && locations > 0)// Milisecond for checking
    {
      Interval++;
    }
    if (Interval == DELAY) {
      //If pawns are on the same position
      if (locations == 1) {
        memcpy(FinalCheck, CheckData, sizeof(CheckData));
        if (FinalCheck [1][1] == HIGH && start <= 1) { // Both pawns are at start and not wrongly sensed on an other position.

        }
        else {
          
        }
        start = 0;
      }
      if (locations == 2) {
        for (int i = 1; i < 6; i++) {
          for (int j = 1; j < 6; j++) {
            if ( FinalCheck[i][j] != CheckData[i][j]) {
              FinalCheck[i][j] = HIGH;
            }
            else
            {
              if (CheckData[i][j] == HIGH)
              {
                FinalCheck[i][j] = LOW;
              }
            }
          }
        }
        int detectedLocations = checkFinalLocations();
        if (detectedLocations > 1)
        {
          resetFinalCheck();
        }
        PositionToNewCategory();
      }
      Interval = DELAY + 1; // Random number larger than DELAY to only fire once
    }
  }
  else
  {
    Interval = 0;
  }
}

void resetData() {
  int EmptyArray[6][6] = {0};
  memcpy(DataOld, EmptyArray, sizeof(EmptyArray));
}

void resetFinalCheck() {
  int EmptyArray[6][6] = {0};
  memcpy(FinalCheck, EmptyArray, sizeof(EmptyArray));
}

// Compare position before saving
void compareOldandNewPos(int i, int j) {
  DataOld[i][j] = DataOld[i][j] | DataNew[i][j];// COMPARE OLD AND NEW POSITIONS
}

void TimeCheck() {
  //How many locations do I have?
  locations = CompareFormerPositionWithNewPosition();   
  Serial.print("Locations: ");
  Serial.print(locations, DEC);
  Serial.println("");
  IntervalCheck( locations);
}

void saveCategoryInt(int category) {
  sendPostRequest(category);
  int category_ = category;
  Serial.println(category_);
  OldCategory = category_;  
}

// Reset all pawns location
void removeAll(){
  row1 = 0;
  row2 = 0;
  col1 = 0;
  col2 = 0;
}

// Removes the pawn that has not the given location if in memory.
void removeLocationOtherThan(int i, int j){  
  if(row1 == i && col1 == j){
    Serial.print("Remove location:");
    Serial.print(row2, DEC);
    Serial.print(" ,");
    Serial.print(col2, DEC);
    Serial.println("");
    row2 = 0;
    col2 = 0;
  }
  else if(row2 == i && col2 == j){
    Serial.print("Remove location:");
    Serial.print(row1, DEC);
    Serial.print(" ,");
    Serial.print(col1, DEC);
    Serial.println("");
    row1 = 0;
    col1 = 0;
  }
  else{
    Serial.println("Location does not exist");
  }
}

// Check if active location is already in memory
bool inLocations(int i,int j){
  Serial.print("Location 1:");
  Serial.print(row1, DEC);
  Serial.print(" ,");
  Serial.print(col1, DEC);
  Serial.println("");

  Serial.print("Location 2:");
  Serial.print(row2, DEC);
  Serial.print(" ,");
  Serial.print(col2, DEC);
  Serial.println("");
  if(row1 == i && col1 == j){
     Serial.println("Location in first");    
     return true;
  }
  else if(row2 == i && col2 == j){
     Serial.println("Location in second");
     return true;
  }
  else{
    return false;  
  }  
}

// Adds the pawn with the given location and returns which pawn was added
int addLocation(int i, int j){
  Serial.print("Adding location:");
  Serial.print(i, DEC);
  Serial.print(" ,");
  Serial.print(j, DEC);
  Serial.println("");
  if(row1 == i && col1 == j || row2 == i && col2 == j){    
     Serial.println("Locations already in memory");      
     return 0;
  }
  else{    
    if(row1 == 0 && col1 == 0){
      row1 = i;
      col1 = j;
      return 1;   
    }
    else if(row2 == 0 && col2 == 0){
      row2 = i;
      col2 = j;
      return 2;
    }
    else{
      Serial.println("Locations already in memory");
      return 0;    
    }
  }
}

double addValues(int readValue){
  total = total - readings[readIndex];
  readings[readIndex] = readValue;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if(readIndex >= numReadings){
    readIndex = 0;
  }
  average = total / numReadings;
  Serial.print("Total: ");
  Serial.print(total,DEC);
  Serial.println("");
  Serial.print("Average: ");
  Serial.print(average,DEC);
  Serial.println("");
  delay(1);
  return average;
}

void readPosition() {  
  for (int i = 1; i < 6; i++) {// Outputs(rows)
    digitalWrite(row[i], HIGH);
    for (int j = 1; j < 6; j++) { // Inputs(column)
      DataNew[i][j] = digitalRead(column[j]);
      compareOldandNewPos(i, j);
        if(DataNew[i][j] == HIGH){    
            //addValues(1);
          //Wait for 1/5th of a second to see if it's still high. -> Replace with array average loop to make it robust.          
            delay(200);
          if(DataNew[i][j] == HIGH){
//            if(average > 0.9){
            Serial.print("i: ");
            Serial.print(i,DEC);
            Serial.print(", j: ");
            Serial.print(j,DEC);
            Serial.println("");
            if(previousLocations != locations){              
              if(previousLocations == 2 && locations == 1){ // Remove the old location
                Serial.println("Removing pawn (from 2)");
                removeLocationOtherThan(i,j);
                sendPostRequest(10);
              }
              else if(previousLocations == 1 && locations == 2){ // Check what is the new location and send message with that particular coordinates.                
                  if(!inLocations(i,j)){
                    Serial.println("Adding pawn");
                    pawn = addLocation(i,j);                             
                    sendPostRequest(categories[i-1][j-1]);                    
                  }                  
              }
              else if(previousLocations == 1 && locations == 0){ // The pawns may have been moved too fast, so send an MPF message
                Serial.println("Removing pawn (from 1)");
                removeAll;
                //sendPostRequest(11);                
              }
              else if(previousLocations == 0 && locations == 1){ // This might be a start message if i==j==1. Could also happen when both are placed on same tile.                
                Serial.println("Adding first pawn");
                if(i == 1 && j == 1){
                  sendPostRequest(0);
                }
                pawn = addLocation(i,j);
              }
              else if(previousLocations == 0 && locations == 2){
                // This can hardly happen, unless you move the pawns on the BOARD at EXACTLY the same time.                
                pawn = addLocation(i,j);
              }
              else if(previousLocations == 2 && locations == 0){
                // This can hardly happen, unless you move the pawns AWAY at EXACTLY the same time.
                removeLocationOtherThan(i,j);
              }
              else{//Locations is < 0 and > 2 in this case. That is weird. Play only with two pawns!
              }
            }             
          }
        }
        else{
          //addValues(0);          
        }
      }      
      digitalWrite(row[i], LOW);  
    }
  previousLocations = locations;
  count = count + 1;
  if (count = CHECKRIGHT) {// If he checked N times of the board, let him check if the interval is the same
    count = 0;// checks how many times the algorythm is excecuted
    TimeCheck();
    resetData();
  }
}
