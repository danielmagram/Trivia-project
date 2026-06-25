# Trivia Project

A multiplayer trivia platform built with a **multi-threaded C++ server**, **SQLite database**, and a **C# WPF client**. The system allows users to register, create and join game rooms, participate in trivia matches, and track their statistics.

## Features

### User Management
- User registration and login
- Authentication system
- Player statistics tracking

### Room System
- Create trivia rooms
- Join existing rooms
- Manage room settings
- Room administrator controls
- Real-time room updates

### Game System
- Multiplayer trivia matches
- Timed questions
- Score calculation
- Automatic game state management
- Results and ranking display

### Statistics
- Games played
- Correct answers
- Average answer time
- User performance tracking

---

## Architecture

The project follows a modular server architecture based on the **Request Handler Pattern**.

### Server Components

#### Communication Layer
Responsible for:
- TCP socket communication
- Client connection handling
- Message transmission
- Concurrent client management

#### Request Handlers

The server routes requests using specialized handlers:

- `LoginRequestHandler`
- `MenuRequestHandler`
- `RoomAdminRequestHandler`
- `RoomMemberRequestHandler`
- `GameRequestHandler`

This design allows each user state to have its own dedicated logic and responsibilities.

#### Managers

Business logic is separated into dedicated manager classes:

- `LoginManager`
- `RoomManager`
- `GameManager`
- `StatisticsManager`

#### Database Layer

The persistence layer is abstracted through:

```cpp
IDatabase
```

with a SQLite implementation:

```cpp
SqliteDatabase
```

This design allows the database backend to be replaced with minimal changes to the rest of the system.

---

## Technologies Used

### Server
- C++17
- WinSock2
- Multithreading (`std::thread`)
- SQLite3
- Object-Oriented Design

### Client
- C#
- WPF
- XAML

### Design Patterns
- Factory Pattern
- State Pattern
- Interface-Based Abstraction
- Request Handler Pattern

---

## Project Structure

```text
Trivia Project
│
├── Server
│   ├── Communicator
│   ├── Request Handlers
│   ├── Managers
│   ├── Database
│   ├── Game Logic
│   └── Serialization
│
├── Client
│   ├── WPF UI
│   ├── Networking
│   └── Game Screens
│
└── SQLite Database
```

---

## Network Flow

```text
Client
   │
   ▼
Communicator
   │
   ▼
RequestHandlerFactory
   │
   ▼
Active Request Handler
   │
   ▼
Managers
   │
   ▼
Database
```

---

## Skills Demonstrated

This project demonstrates:

- Network programming
- Multi-threaded server development
- TCP socket communication
- Client-server architecture
- Object-oriented design
- Design patterns
- Database integration
- Software architecture and separation of concerns
- C++ memory management
- Desktop application development

---

## Screenshots

Add screenshots of:

- Login screen
- Main menu
- Room creation
- Game screen
- Statistics screen

---

## Future Improvements

- TLS encrypted communication
- Password hashing and salting
- Thread pool implementation
- Asynchronous networking
- Improved matchmaking system
- REST API integration
- Web client support

---

## Authors

**Daniel Magram**  
C++ Server Development • Networking • Cybersecurity

**Oshri Grubman**
C++ Server Development • Networking • Cybersecurity
---

## Educational Purpose

This project was developed as part of a software engineering and networking learning process, focusing on building a complete client-server application from the ground up.
