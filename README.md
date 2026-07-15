# Garage Invoice Management System

A robust, fully modular, Object-Oriented console-based Garage Invoice Management System implemented in **C++17**. The system features platform-native terminal styling (colors), binary file record handling, detailed invoicing logs, customer/vehicle registration matching, inventory tracking, employee payroll, and analytical reports.

---

## Admin Credentials

- **Username:** `admin`
- **Password:** `admin123` *(masked with `*` or hidden during input)*

---

## Architecture and Design

1. **Object-Oriented Programming (OOP):** Decoupled entities representing `Customer`, `Vehicle`, `Employee`, `Service`, `Part`, and `Invoice`.
2. **Binary File Serialization:** To ensure data integrity, memory-stable fixed-size records (`*Record` structures) are used for safe, direct binary reads/writes to avoid memory leaks/pointer issues associated with dynamic elements like `std::string` or `std::vector` inside binary streams.
3. **Database Files:**
   - `customers.dat`: Customer directories.
   - `vehicles.dat`: Vehicle registrations, linked to Customer Owner IDs.
   - `employees.dat`: Staff payroll database.
   - `services.dat`: Service catalog containing prepopulated items.
   - `parts.dat`: Spare parts inventory.
   - `invoices.dat`: Invoice billing registry logs.

---

## Preseeded Services

Upon launching for the first time, the application seeds 10 standard service catalogs:
1. Oil Change (Rs. 500.00)
2. Brake Service (Rs. 800.00)
3. Wheel Alignment (Rs. 600.00)
4. Battery Replacement (Rs. 300.00)
5. Engine Repair (Rs. 5000.00)
6. Car Wash (Rs. 400.00)
7. AC Repair (Rs. 1500.00)
8. Denting (Rs. 2500.00)
9. Painting (Rs. 4000.00)
10. Full Service (Rs. 3500.00)

---

## Compilation Instructions

The project structure is modular, separating declarations (`.h` files) from implementation (`.cpp` files).

### 1. Using GCC (g++)
Compile all source files using standard compilation flags:
```bash
g++ -std=c++17 -Wall -Wextra *.cpp -o GarageInvoiceSystem.exe
```
Run the compiled executable:
```bash
./GarageInvoiceSystem.exe
```

### 2. Using Code::Blocks
1. Open Code::Blocks.
2. Select **File -> New -> Project -> Console Application**.
3. Choose **C++** and set the project name to `GarageInvoiceSystem`.
4. Right-click on the project in the workspace panel and select **Add files...**.
5. Add all `.h` and `.cpp` files in the directory.
6. Go to **Project -> Build options -> Compiler Settings -> Compiler Flags**, and verify that **[-std=c++17]** (or higher) is selected.
7. Click **Build and Run** (F9).

### 3. Using Visual Studio
1. Open Visual Studio and create a **New empty C++ Project**.
2. Copy all files into the project directory.
3. Right-click on **Header Files** in Solution Explorer -> **Add -> Existing Item**, select all `.h` files.
4. Right-click on **Source Files** -> **Add -> Existing Item**, select all `.cpp` files.
5. In properties (**Project -> Properties -> Configuration Properties -> General -> C++ Language Standard**), select **ISO C++17 Standard (/std:c++17)**.
6. Press **Ctrl + F5** to Build and Run.

---

## Features Summary

- **Visual Interface:** Dynamic progress bars, loading screen, customized color coding based on status (e.g., green for paid status/success, red for warnings/errors).
- **Pagination & Sorting:** Customer list, parts list, services catalog, and invoice history are fully paginated (5 records per page) with easy direction buttons (`N`/`P`/`Q`).
- **Billing Calculations:**
  - Automated ID/Invoice code generation.
  - Interactive multi-item parts selection and labor additions.
  - Active stock control deduction (warning when stock is insufficient).
  - Discount values subtraction and **18% GST** calculation.
- **Reports Suite:**
  - Daily & Monthly Revenue aggregators.
  - Top Performing Services (sorts services by job usage frequency).
  - Most Utilized Spare Parts (sorts parts by unit volume sales).
  - Detailed Customer history profile & vehicle job sheets log.
- **Administrative Tools:** Full maintenance dashboard displaying database sizes, counts of active/deleted records, file paths, dynamic database reset utilities, and snapshot backup tools.
