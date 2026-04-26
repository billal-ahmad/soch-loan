# Soch Loan
A CLI-based chat bot to provide the loan.

## Description
It reads predefined user inputs from file to interact with users to give the feel of chat bot and guide the users applying for different types of loans. 

## Features
- Reads user inputs and Soch Loan responses from 'utterances.txt'.
- Provides general response for unknown inputs using a '*' fallback.
- Supports Home Loan module with area, size, price, installments, and down payment options.
- File-driven design. No need to modify the code to add new utterances or loan data.
- Easily extendible to handle car, scooter or personal loans in future phases.

## File Structure
```
soch-loan/
|
├ data/
|   ├utterances.txt
|   └home.txt
|
├ include/
|   ├utterance-handler.h
|   └loan-selection.h
|
├ src/
|   ├utterance-handler.cpp
|   └loan-selection.cpp
|
└ main.cpp
```

## How to Run
1. Open the terminal in project folder.
2. Compile using:
     g++ src/*.cpp main.cpp -I include -o soch-loan
3. Run using:
    .\soch-loan.exe

## File Description
- **utterances.txt** - Contains predefined user inputs and chatbot responses.
- **home.txt** - Contains home-loan details (area, size, price, etc.).
- **utterance-handler.h/cpp** - Module to read and match utterances.
- **loan-selection.h/cpp** - Module to read and display loan data.
- **main.cpp** - Controls the chatbot flow.

## Naming Conventions

This project follows consistent naming conventions to ensure clarity, readability, and maintainability across all source files.

| **Type** | **Convention** | **Example** |
|-----------|----------------|--------------|
| File | lowercase-hyphen | user-manager.cpp |
| Class / Struct | PascalCase | EmployeeManager |
| Function | camelCase | calculateSalary |
| Variable | snake_case | employee_count |
| Constant | UPPERCASE_WITH_UNDERSCORES | MAX_USERS |
| Global | UPPERCASE_WITH_UNDERSCORES | GLOBAL_TOTAL_USERS |
| Class Attributes | _snake_case | _input |
