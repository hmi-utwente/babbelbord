//Here is the the position to the fitting category and sent to the server

//ERROR MESSAGES
String error1 = "Both pawns are at gaan";
String error2 = "Please move the pawn a little around in the square";
String error3 = "Please remove last placed pawn, and place it back after X seconds";


void saveCategory(String category) {
  String category_ = category;
  Serial.println(category_);
  OldCategory = category_;



}




//Uses global variable CHECK

void PositionToNewCategory() {

  for (int i = 1; i < 6; i++) {// Outputs(rows)
    for (int j = 1; j < 6; j++) { // Inputs(column)
      if (FinalCheck[i][j] == HIGH) {
        switch (i) {
          case 1:
            switch (j) {
              case (1):
                break;
              case (2):
                saveCategory("Familie");
                break;
              case (3):
                saveCategory("Liefde");
                break;
              case (4):
                saveCategory("Het");
                break;
              case (5):
                saveCategory("Tienertijd");
                break;

            }
            break;
          case 2:
            switch (j) {
              case (1):
                saveCategory("Kindertijd");
                break;
              case (2):
                saveCategory("Hobbies");
                break;
              case (3):
                saveCategory("Familie");
                break;
              case (4):
                saveCategory("Liefde");
                break;
              case (5):
                saveCategory("Ga");
                break;

            }
            break;



          case 3:
            switch (j) {
              case (1):
                saveCategory("Tienertijd");
                break;
              case (2):
                saveCategory("Kindertijd");
                break;
              case (3):
                saveCategory("Hobbies");
                break;
              case (4):
                saveCategory("Familie");
                break;
              case (5):
                saveCategory("Liefde");
                break;

            }
            break;

          case 4:
            switch (j) {
              case (1):
                saveCategory("Verwijder");
                break;
              case (2):
                saveCategory("Tienertijd");
                break;
              case (3):
                saveCategory("Kindertijd");
                break;
              case (4):
                saveCategory("Hobbies");
                break;
              case (5):
                saveCategory("Familie");
                break;

            }
            break;
          case 5:
            switch (j) {
              case (1):
                saveCategory("Liefde");
                break;
              case (2):
                saveCategory("Geef");
                break;
              case (3):
                saveCategory("Tienertijd");

                break;
              case (4):
                saveCategory("Kindertijd");
                break;
              case (5):

                //EXCEPTION
                saveCategory("Hobbies");
                break;

            }
            break;
        }

      }
    }
  }
}












