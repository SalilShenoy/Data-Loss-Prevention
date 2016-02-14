# TextvsCode
This project train's a Hidden Markov Model using Java keywords to distinguish between java code and text files.
The project has a java code which parses Java code files extracts java keywords and maps them to english alphabets. It creates a output file in which we can see the sequence of how the keywords were encountered in that particular code file.
The parsed files are then used to train the Hidden markov Model .
And the model is tested using text and code files. 

For our project, we trained the model using 200 Java code files and tested the model against 50 files (25 text and 25 java). We got good results as we were able to seperate text and code files. We got an Area Under Curve of 0.997 which meant high accuracy for the model.
