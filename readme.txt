# README

This is a C++ client application that interacts with a server to manage a
    library system. The application allows users to register, log in,
    enter the library, retrieve book information, add new books, delete
    books, and log out. The communication between the client and the server
    is handled using HTTP requests (GET, POST, DELETE).
    
JSON: I used the json library for parsing and creating Json data easier.

Features:
    Register: Create a new user account.
    Login: Authenticate and log in an existing user.
    Enter Library: Gain access to the library system.
    Get Books: Retrieve a list of all books in the user's library.
    Get Book: Retrieve information about a specific book by its ID.
    Add Book: Add a new book to the library.
    Delete Book: Remove a book from the library by its ID.
    Logout: Log out the current user.
    Exit: Exit the application.

Requirements
    C++ compiler
    JSON for Modern C++ library (nlohmann/json)
    Internet connection to communicate with the server

Usage:
    Clone the repository
    Compile using the Makefile
    Run the executable(./client)

Notes:
    Ensure the server is running and accessible at the specified IP and port.
