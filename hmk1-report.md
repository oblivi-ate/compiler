# compiler

## Group member
YU QIANSHUO : 
WANG KUN : 1210025783
ZHENG AO : 1220005554 

#### Scanner Features Implemented

1. **File Reading**:
   - The program can accept a command-line argument as the input file path and attempts to open the file.

2. **Scanner Initialization**:
   - The `Scanner` class is initialized with an input stream (in this case, a file stream) and stored as a member variable.

3. **Scanning Process**:
   - The `scan` method implements a loop that continuously reads characters from the input stream and calls the `Transition` function (implementation not provided).

4. **Error Handling**:
   - If an error state (`ERROR`) is encountered during scanning, the program prints an error message and terminates scanning early.

5. **End of Scanning**:
   - The scanning loop terminates when the end of the input stream (EOF) is reached.

6. **Token List Creation and Initialization**:
   - In the `main` function, a `Token` object and a `Token_List` object are created to store the tokens generated during the scanning process.

#### Issues
1. **Error Messages and Program Termination**:
   - When the file fails to open, the program prints an error message and returns 1. While this is a good practice, more detailed error handling may be needed, such as cleaning up allocated resources.

2. **Code Style and Consistency**:
   - Comments and formatting in the code should be consistent to improve code readability.
3. **File Closure**:
   - Although the file is closed at the end of the `main` function, it may not reach this line in cases of exceptions (such as file open failure). It is recommended to use the constructor to automatically manage file closure.


#### Compile
g++ token.cpp dfa.cpp scanner.cpp -o scanner -std=c++17

#### Run
./scanner while_etc.pc
