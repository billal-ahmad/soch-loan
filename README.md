# Soch Loan
A CLI-based loan management system with two separate interfaces: one for customers applying for loans and one for administrators/lenders to manage applications.

## Description
Soch Loan is a dual-interface system that facilitates loan applications and management. It reads predefined user inputs and responses from data files to interact with users, providing a chatbot-like experience for loan applicants and an administrative dashboard for lenders to review, approve, or reject applications.

## Features
- **Customer Interface (soch-bot)**: Allows users to browse and apply for different types of loans
- **Admin Interface (lender)**: Enables lenders/admins to view, update, and manage loan applications
- Supports multiple loan types: Home Loan, Car Loan, Scooter Loan, and Personal Loan
- File-driven design with data files for loans, utterances, and applications
- Reads user inputs and chatbot responses from 'utterances.txt'
- Provides general response for unknown inputs using a '*' fallback
- Easily extensible to add new loan types or features

## Project Structure
```
soch-loan/
│
├── build.sh                          # Build script for customer interface (soch-bot)
├── build-lender.sh                   # Build script for admin interface (lender)
├── soch-bot.cpp                      # Main file for customer application interface
├── lender.cpp                        # Main file for admin/lender interface
│
├── include/                          # Header files
│   ├── application.h
│   ├── application-handler.h
│   ├── car-loan.h
│   ├── car-loan-selection.h
│   ├── display.h
│   ├── general-chat-handler.h
│   ├── home-loan.h
│   ├── loan-selection.h
│   ├── personal-loan.h
│   ├── personal-loan-selection.h
│   ├── scooter-loan.h
│   ├── scooter-loan-selection.h
│   └── utterance-handler.h
│
├── src/                              # Implementation files
│   ├── application.cpp
│   ├── application-handler.cpp
│   ├── car-loan.cpp
│   ├── car-loan-selection.cpp
│   ├── display.cpp
│   ├── general-chat-handler.cpp
│   ├── home-loan.cpp
│   ├── loan-selection.cpp
│   ├── personal-loan.cpp
│   ├── personal-loan-selection.cpp
│   ├── scooter-loan.cpp
│   ├── scooter-loan-selection.cpp
│   └── utterance-handler.cpp
│
├── data/                             # Data files
│   ├── applications.txt              # Stores submitted loan applications
│   ├── car.txt                       # Car loan specifications
│   ├── home.txt                      # Home loan specifications
│   ├── personal.txt                  # Personal loan specifications
│   ├── scooter.txt                   # Scooter loan specifications
│   ├── utterances.txt                # Chatbot utterances and responses
│   └── human-chat-corpus.txt         # General chat responses
│
└── bin/                              # Compiled executables
    ├── soch-bot.exe                  # Customer interface executable
    └── lender.exe                    # Admin interface executable
```

## How to Build and Run

### Customer Interface (soch-bot)
For customers applying for loans:
```bash
./build.sh
```
or manually:
```bash
g++ soch-bot.cpp src/*.cpp -Iinclude -o bin/soch-bot
./bin/soch-bot
```

### Admin Interface (lender)
For lenders/admins managing applications:
```bash
./build-lender.sh
```
or manually:
```bash
g++ lender.cpp src/*.cpp -Iinclude -o bin/lender
./bin/lender
```

## Data Files Description
- **utterances.txt** - Contains predefined user inputs and chatbot responses for conversation flow
- **applications.txt** - Stores all submitted loan applications with their current status
- **home.txt** - Contains home loan options with area, size, price, and installment details
- **car.txt** - Contains car loan specifications by make and model
- **scooter.txt** - Contains scooter loan specifications
- **personal.txt** - Contains personal loan categories and terms
- **human-chat-corpus.txt** - General chat responses for fallback conversations

## Core Modules Description
- **utterance-handler.h/cpp** - Manages user input matching and response generation
- **application.h/cpp** - Defines the Application data structure and serialization
- **application-handler.h/cpp** - Manages loading, saving, and updating applications
- **display.h/cpp** - Handles all console output formatting and UI display
- **loan-selection.h/cpp** - Base loan selection and filtering logic
- **[loan-type]-loan.h/cpp** - Specific loan types (Home, Car, Scooter, Personal)
- **[loan-type]-loan-selection.h/cpp** - Selection and filtering for each loan type
- **general-chat-handler.h/cpp** - Handles general conversation outside loan context

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
