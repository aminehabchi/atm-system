# 💳 ATM Management System (C Language)

## 📌 Objective
The objective of this project is to demonstrate your ability to apply programming logic in the C language and adapt to an existing codebase by implementing new features and fixing existing issues. You are given a partially implemented ATM management system. Your task is to **add the missing features** and ensure proper file-based persistence and functionality.

## 🛠 Implemented Features
1. 🧾 **Registration**
   - Users can register with a **unique username** (duplicates are not allowed).
   - User data is stored in `database.db`.

2. 🔐 **Login**
   - Users can log in to access account operations securely.

3. 🔄 **Update Account Information**
   - Users can update the **country** or **phone number** of an account.
   - Prompts for account `id` and field to update.
   - All changes are written back to `database.db`.

4. 🔍 **Check Account Details**
   - Users can retrieve information for a single account using its `id`.
   - Interest is displayed based on account type:
     - `savings`: 7%
     - `fixed01`: 4%
     - `fixed02`: 5%
     - `fixed03`: 8%
     - `current`: No interest

5. 💸 **Make Transactions**
   - Supports **withdrawals** and **deposits** for eligible accounts.
   - ❌ Transactions are **not allowed** for fixed-term accounts.

6. 🗑 **Remove Account**
   - Allows users to delete their own accounts.
   - Changes are persisted in `database.db`.

7. 🔁 **Transfer Ownership**
   - Users can transfer ownership of an account to another registered user.
   - Validates both accounts and updates ownership in the database.

## 📁 Project Structure
```
.
├── data
│   └── database.db           # Stores all user and account data
└── src
    ├── auth.c                # Handles login and registration
    ├── funcs.c               # Core functionality for account operations
    ├── header.h              # Shared declarations
    ├── header.h.gch          # Precompiled header (can be ignored or regenerated)
    ├── main.c                # Program entry point
    ├── manageDB.c            # File/database handling
    └── system.c              # System UI and navigation logic
```

## 🧰 Compilation & Execution
To compile the project, run:
```bash
gcc src/*.c -l sqlite3 -o atm
```
To execute:
```bash
./atm
```

## 📌 Notes
- All data is stored in a single file: `data/database.db`.


Feel free to enhance this project further by adding encryption, a GUI, or additional validations.
