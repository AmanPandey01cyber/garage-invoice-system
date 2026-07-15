// ==================== STATE MANAGEMENT ====================
let state = {
    customers: [],
    vehicles: [],
    employees: [],
    services: [],
    parts: [],
    invoices: [],
    currentUser: null
};

// Pagination configurations (5 records per page matching C++ design)
const PAGE_SIZE = 5;
let currentPage = {
    customers: 1,
    vehicles: 1,
    employees: 1,
    services: 1,
    parts: 1,
    invoices: 1
};

// Seeding Default Service Catalogs (From C++ codebase)
const SEEDED_SERVICES = [
    { id: 1, name: "Oil Change", description: "Engine oil and filter replacement", labourCost: 500.00, isDeleted: false },
    { id: 2, name: "Brake Service", description: "Front/Rear brake pad check and replacement", labourCost: 800.00, isDeleted: false },
    { id: 3, name: "Wheel Alignment", description: "Laser guided 4-wheel calibration", labourCost: 600.00, isDeleted: false },
    { id: 4, name: "Battery Replacement", description: "12V lead-acid battery swap & testing", labourCost: 300.00, isDeleted: false },
    { id: 5, name: "Engine Repair", description: "Engine diagnostics and overhaul work", labourCost: 5000.00, isDeleted: false },
    { id: 6, name: "Car Wash", description: "Interior vacuum and exterior foam wash", labourCost: 400.00, isDeleted: false },
    { id: 7, name: "AC Repair", description: "AC refrigerant gas topup and leak repair", labourCost: 1500.00, isDeleted: false },
    { id: 8, name: "Denting", description: "Body panel dent removal services", labourCost: 2500.00, isDeleted: false },
    { id: 9, name: "Painting", description: "Premium paint coating per panel", labourCost: 4000.00, isDeleted: false },
    { id: 10, name: "Full Service", description: "Complete vehicle checkup, fluid replacement and filter cleans", labourCost: 3500.00, isDeleted: false }
];

// Seeding Mock Parts Inventory
const SEEDED_PARTS = [
    { id: 1, name: "Engine Oil Synthetic 4L", unitPrice: 2800.00, quantity: 20, isDeleted: false },
    { id: 2, name: "Front Brake Pads Set", unitPrice: 1200.00, quantity: 15, isDeleted: false },
    { id: 3, name: "Oil Filter", unitPrice: 350.00, quantity: 40, isDeleted: false },
    { id: 4, name: "12V Car Battery Amaron", unitPrice: 4500.00, quantity: 8, isDeleted: false },
    { id: 5, name: "Air Filter Element", unitPrice: 450.00, quantity: 25, isDeleted: false },
    { id: 6, name: "Spark Plug NGK (Set of 4)", unitPrice: 850.00, quantity: 30, isDeleted: false },
    { id: 7, name: "Wiper Blades Bosch (Pair)", unitPrice: 650.00, quantity: 12, isDeleted: false },
    { id: 8, name: "Coolant Liquid 1L", unitPrice: 280.00, quantity: 18, isDeleted: false },
    { id: 9, name: "Headlight Bulb H4 LED", unitPrice: 1500.00, quantity: 4, isDeleted: false },
    { id: 10, name: "Cabin AC Filter", unitPrice: 380.00, quantity: 14, isDeleted: false }
];

// Seeding Mock Customers
const SEEDED_CUSTOMERS = [
    { id: 1, name: "Rahul Sharma", phone: "9876543210", email: "rahul.sharma@gmail.com", address: "A-45, Phase 1, Ashok Vihar, New Delhi", isDeleted: false },
    { id: 2, name: "Priya Patel", phone: "9812345678", email: "priya.patel@yahoo.com", address: "Flat 402, Royal Residency, Ahmedabad", isDeleted: false },
    { id: 3, name: "Amit Verma", phone: "8800112233", email: "amit.verma@outlook.com", address: "Sector 15, H.No. 209, Gurugram", isDeleted: false }
];

// Seeding Mock Vehicles
const SEEDED_VEHICLES = [
    { vehicleNumber: "DL-3C-AB-1234", brand: "Maruti Suzuki", model: "Swift", year: 2021, color: "White", engineNumber: "K12M123456", customerId: 1, isDeleted: false },
    { vehicleNumber: "GJ-01-XY-5678", brand: "Hyundai", model: "i20", year: 2022, color: "Red", engineNumber: "G4LA987654", customerId: 2, isDeleted: false },
    { vehicleNumber: "HR-26-CZ-9012", brand: "Honda", model: "City", year: 2019, color: "Silver", engineNumber: "iVTEC9876", customerId: 3, isDeleted: false }
];

// Seeding Mock Employees
const SEEDED_EMPLOYEES = [
    { id: 1, name: "Suresh Kumar", designation: "Chief Mechanic", salary: 35000.00, phone: "9560123456", isDeleted: false },
    { id: 2, name: "Ramesh Singh", designation: "Junior Electrician", salary: 22000.00, phone: "9910456123", isDeleted: false },
    { id: 3, name: "Vikram Dev", designation: "Service Advisor", salary: 28000.00, phone: "8130987654", isDeleted: false }
];

// Seeding Mock Invoices
const SEEDED_INVOICES = [
    {
        invoiceNumber: 1001,
        customerId: 1,
        customerName: "Rahul Sharma",
        customerPhone: "9876543210",
        vehicleNumber: "DL-3C-AB-1234",
        vehicleBrand: "Maruti Suzuki",
        vehicleModel: "Swift",
        date: "2026-07-14",
        serviceIds: [1, 6], // Oil Change, Car Wash
        partsList: [{ partId: 1, qty: 1 }, { partId: 3, qty: 1 }], // Synthetic Oil, Oil Filter
        labourCharges: 900.00, // 500 + 400
        partsCharges: 3150.00, // 2800 + 350
        subtotal: 4050.00,
        discount: 250.00,
        gstAmount: 684.00, // (4050 - 250) * 0.18
        grandTotal: 4484.00,
        isPaid: true,
        paymentMode: 2, // UPI
        paymentRef: "UPI987654321023",
        isDeleted: false
    },
    {
        invoiceNumber: 1002,
        customerId: 2,
        customerName: "Priya Patel",
        customerPhone: "9812345678",
        vehicleNumber: "GJ-01-XY-5678",
        vehicleBrand: "Hyundai",
        vehicleModel: "i20",
        date: "2026-07-15",
        serviceIds: [2], // Brake Service
        partsList: [{ partId: 2, qty: 1 }], // Front Brake Pads Set
        labourCharges: 800.00,
        partsCharges: 1200.00,
        subtotal: 2000.00,
        discount: 0.00,
        gstAmount: 360.00, // 2000 * 0.18
        grandTotal: 2360.00,
        isPaid: false,
        paymentMode: 1,
        paymentRef: "",
        isDeleted: false
    }
];

// ==================== CORE INITIALIZER ====================
function initDatabase() {
    if (!localStorage.getItem("g_invoices_auth")) {
        state.currentUser = null;
    } else {
        state.currentUser = localStorage.getItem("g_invoices_auth");
    }

    if (!localStorage.getItem("g_customers")) {
        localStorage.setItem("g_customers", JSON.stringify(SEEDED_CUSTOMERS));
        localStorage.setItem("g_vehicles", JSON.stringify(SEEDED_VEHICLES));
        localStorage.setItem("g_employees", JSON.stringify(SEEDED_EMPLOYEES));
        localStorage.setItem("g_services", JSON.stringify(SEEDED_SERVICES));
        localStorage.setItem("g_parts", JSON.stringify(SEEDED_PARTS));
        localStorage.setItem("g_invoices", JSON.stringify(SEEDED_INVOICES));
    }

    loadStateFromStorage();
}

function loadStateFromStorage() {
    state.customers = JSON.parse(localStorage.getItem("g_customers")) || [];
    state.vehicles = JSON.parse(localStorage.getItem("g_vehicles")) || [];
    state.employees = JSON.parse(localStorage.getItem("g_employees")) || [];
    state.services = JSON.parse(localStorage.getItem("g_services")) || [];
    state.parts = JSON.parse(localStorage.getItem("g_parts")) || [];
    state.invoices = JSON.parse(localStorage.getItem("g_invoices")) || [];
}

function saveStateToStorage() {
    localStorage.setItem("g_customers", JSON.stringify(state.customers));
    localStorage.setItem("g_vehicles", JSON.stringify(state.vehicles));
    localStorage.setItem("g_employees", JSON.stringify(state.employees));
    localStorage.setItem("g_services", JSON.stringify(state.services));
    localStorage.setItem("g_parts", JSON.stringify(state.parts));
    localStorage.setItem("g_invoices", JSON.stringify(state.invoices));
}

// ==================== AUTHENTICATION WORKFLOW ====================
const loginScreen = document.getElementById("login-screen");
const mainApp = document.getElementById("main-app");
const loginForm = document.getElementById("login-form");
const loginError = document.getElementById("login-error");
const logoutBtn = document.getElementById("logout-btn");

loginForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const user = document.getElementById("username").value.trim();
    const pass = document.getElementById("password").value;

    if (user === "admin" && pass === "admin123") {
        state.currentUser = "admin";
        localStorage.setItem("g_invoices_auth", "admin");
        loginScreen.classList.add("hidden");
        mainApp.classList.remove("hidden");
        loginError.textContent = "";
        loginForm.reset();
        refreshAllViews();
    } else {
        loginError.textContent = "Invalid username or password. Try admin / admin123.";
    }
});

logoutBtn.addEventListener("click", () => {
    state.currentUser = null;
    localStorage.removeItem("g_invoices_auth");
    mainApp.classList.add("hidden");
    loginScreen.classList.remove("hidden");
});

function checkAuthStatus() {
    if (state.currentUser) {
        loginScreen.classList.add("hidden");
        mainApp.classList.remove("hidden");
        refreshAllViews();
    } else {
        loginScreen.classList.remove("hidden");
        mainApp.classList.add("hidden");
    }
}

// ==================== TAB ROUTING ====================
const menuItems = document.querySelectorAll(".menu-item");
const tabPanes = document.querySelectorAll(".tab-pane");
const pageTitle = document.getElementById("page-title");

menuItems.forEach(item => {
    item.addEventListener("click", (e) => {
        e.preventDefault();
        const tabId = item.getAttribute("data-tab");
        switchTab(tabId);
    });
});

function switchTab(tabId) {
    menuItems.forEach(mi => {
        if (mi.getAttribute("data-tab") === tabId) {
            mi.classList.add("active");
        } else {
            mi.classList.remove("active");
        }
    });

    tabPanes.forEach(pane => {
        if (pane.id === `tab-${tabId}`) {
            pane.classList.add("active");
        } else {
            pane.classList.remove("active");
        }
    });

    // Header updates
    const titleMap = {
        dashboard: "Dashboard Overview",
        customers: "Customer Directory",
        vehicles: "Registered Vehicles",
        employees: "Employee Payroll Registry",
        services: "Service Price List Catalog",
        parts: "Spare Parts Inventory",
        invoices: "Invoice Log Billing Ledger"
    };
    pageTitle.textContent = titleMap[tabId] || "Management Console";
    
    // Refresh the corresponding tab content
    renderTabContent(tabId);
}

function renderTabContent(tabId) {
    if (tabId === "dashboard") renderDashboardTab();
    if (tabId === "customers") renderCustomersTab();
    if (tabId === "vehicles") renderVehiclesTab();
    if (tabId === "employees") renderEmployeesTab();
    if (tabId === "services") renderServicesTab();
    if (tabId === "parts") renderPartsTab();
    if (tabId === "invoices") renderInvoicesTab();
}

function refreshAllViews() {
    loadStateFromStorage();
    const activeTab = document.querySelector(".menu-item.active").getAttribute("data-tab");
    renderTabContent(activeTab);
    updateDateDisplay();
}

function updateDateDisplay() {
    const options = { weekday: 'short', year: 'numeric', month: 'long', day: 'numeric' };
    document.getElementById("current-date").textContent = new Date().toLocaleDateString('en-US', options);
}

// ==================== HELPERS & PAGINATION TOOL ====================
function getPaginatedData(items, page, size) {
    const start = (page - 1) * size;
    const end = start + size;
    return {
        data: items.slice(start, end),
        total: items.length,
        totalPages: Math.ceil(items.length / size)
    };
}

function renderPaginationControls(containerId, totalPages, currentPageNum, prevCallback, nextCallback) {
    const container = document.getElementById(containerId);
    if (!container) return;

    if (totalPages <= 1) {
        container.innerHTML = "";
        return;
    }

    container.innerHTML = `
        <div class="pagination-info">Page ${currentPageNum} of ${totalPages}</div>
        <div class="pagination-controls">
            <button class="pagination-btn" id="${containerId}-prev" ${currentPageNum === 1 ? 'disabled' : ''}>&lt;</button>
            <button class="pagination-btn" id="${containerId}-next" ${currentPageNum === totalPages ? 'disabled' : ''}>&gt;</button>
        </div>
    `;

    document.getElementById(`${containerId}-prev`).addEventListener("click", prevCallback);
    document.getElementById(`${containerId}-next`).addEventListener("click", nextCallback);
}

// ==================== DASHBOARD ANALYTICS SYSTEM ====================
function renderDashboardTab() {
    // Total Revenue
    const activeInvoices = state.invoices.filter(inv => !inv.isDeleted);
    const paidInvoices = activeInvoices.filter(inv => inv.isPaid);
    const unpaidInvoices = activeInvoices.filter(inv => !inv.isPaid);
    
    const totalRev = paidInvoices.reduce((sum, inv) => sum + inv.grandTotal, 0);
    document.getElementById("metric-revenue").textContent = `Rs. ${totalRev.toLocaleString('en-IN', { minimumFractionDigits: 2, maximumFractionDigits: 2 })}`;
    
    // Invoices count
    document.getElementById("metric-invoices").textContent = activeInvoices.length;
    document.getElementById("metric-unpaid-invoices").textContent = `${unpaidInvoices.length} Unpaid / Pending`;

    // Customers / Vehicles
    const activeCustomers = state.customers.filter(c => !c.isDeleted);
    const activeVehicles = state.vehicles.filter(v => !v.isDeleted);
    document.getElementById("metric-customers").textContent = activeCustomers.length;
    document.getElementById("metric-vehicles").textContent = `${activeVehicles.length} Vehicles Registered`;

    // Active parts / Low stock
    const activeParts = state.parts.filter(p => !p.isDeleted);
    const lowStockParts = activeParts.filter(p => p.quantity < 5);
    document.getElementById("metric-parts").textContent = activeParts.length;
    const lowStockLabel = document.getElementById("metric-low-stock");
    lowStockLabel.textContent = `${lowStockParts.length} Low Stock Alert (<5 Qty)`;
    if (lowStockParts.length > 0) {
        lowStockLabel.className = "label alert-highlight";
    } else {
        lowStockLabel.className = "label";
    }

    // 1. Top Performing Services
    // Count occurrences of service IDs in invoices
    const serviceCounts = {};
    activeInvoices.forEach(inv => {
        if (inv.serviceIds) {
            inv.serviceIds.forEach(sid => {
                serviceCounts[sid] = (serviceCounts[sid] || 0) + 1;
            });
        }
    });

    const activeServices = state.services.filter(s => !s.isDeleted);
    const serviceStats = activeServices.map(srv => {
        return {
            name: srv.name,
            count: serviceCounts[srv.id] || 0
        };
    }).sort((a, b) => b.count - a.count);

    const topServicesList = document.getElementById("top-services-list");
    topServicesList.innerHTML = "";
    const maxServiceCount = Math.max(...serviceStats.map(s => s.count), 1);
    
    serviceStats.slice(0, 5).forEach(stat => {
        const pct = (stat.count / maxServiceCount) * 100;
        const item = document.createElement("div");
        item.className = "analytics-item";
        item.innerHTML = `
            <div class="item-info-row">
                <span class="item-name">${stat.name}</span>
                <span class="item-val">${stat.count} job(s)</span>
            </div>
            <div class="progress-track">
                <div class="progress-bar" style="width: ${pct}%"></div>
            </div>
        `;
        topServicesList.appendChild(item);
    });

    // 2. Most Utilized Spare Parts
    // Count total units sold of each part ID
    const partVolumes = {};
    activeInvoices.forEach(inv => {
        if (inv.partsList) {
            inv.partsList.forEach(item => {
                partVolumes[item.partId] = (partVolumes[item.partId] || 0) + item.qty;
            });
        }
    });

    const activePartsList = state.parts.filter(p => !p.isDeleted);
    const partStats = activePartsList.map(pt => {
        return {
            name: pt.name,
            qty: partVolumes[pt.id] || 0
        };
    }).sort((a, b) => b.qty - a.qty);

    const topPartsList = document.getElementById("top-parts-list");
    topPartsList.innerHTML = "";
    const maxPartQty = Math.max(...partStats.map(p => p.qty), 1);

    partStats.slice(0, 5).forEach(stat => {
        const pct = (stat.qty / maxPartQty) * 100;
        const item = document.createElement("div");
        item.className = "analytics-item";
        item.innerHTML = `
            <div class="item-info-row">
                <span class="item-name">${stat.name}</span>
                <span class="item-val">${stat.qty} unit(s) replacement</span>
            </div>
            <div class="progress-track">
                <div class="progress-bar" style="width: ${pct}%; background: var(--warning)"></div>
            </div>
        `;
        topPartsList.appendChild(item);
    });
}

// Database Actions: Export, Import, Reset
document.getElementById("btn-export-db").addEventListener("click", () => {
    const dataStr = JSON.stringify({
        customers: state.customers,
        vehicles: state.vehicles,
        employees: state.employees,
        services: state.services,
        parts: state.parts,
        invoices: state.invoices
    }, null, 2);
    
    const blob = new Blob([dataStr], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    const link = document.createElement("a");
    link.href = url;
    link.download = `garage_database_backup_${new Date().toISOString().slice(0,10)}.json`;
    link.click();
});

document.getElementById("import-db-file").addEventListener("change", (e) => {
    const file = e.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(evt) {
        try {
            const importedData = JSON.parse(evt.target.result);
            if (importedData.customers && importedData.vehicles && importedData.employees && 
                importedData.services && importedData.parts && importedData.invoices) {
                
                localStorage.setItem("g_customers", JSON.stringify(importedData.customers));
                localStorage.setItem("g_vehicles", JSON.stringify(importedData.vehicles));
                localStorage.setItem("g_employees", JSON.stringify(importedData.employees));
                localStorage.setItem("g_services", JSON.stringify(importedData.services));
                localStorage.setItem("g_parts", JSON.stringify(importedData.parts));
                localStorage.setItem("g_invoices", JSON.stringify(importedData.invoices));
                
                alert("Database successfully restored from backup file!");
                refreshAllViews();
            } else {
                alert("Invalid database schema. Please choose a valid G-Invoice backup file.");
            }
        } catch (err) {
            alert("Error parsing JSON backup file. Ensure it is a valid file.");
        }
    };
    reader.readAsText(file);
});

document.getElementById("btn-reset-db").addEventListener("click", () => {
    if (confirm("WARNING: This will permanently delete all records (Invoices, Customers, Employees, Vehicles) and restore the system to preseeded defaults. Do you want to continue?")) {
        localStorage.removeItem("g_customers");
        localStorage.removeItem("g_vehicles");
        localStorage.removeItem("g_employees");
        localStorage.removeItem("g_services");
        localStorage.removeItem("g_parts");
        localStorage.removeItem("g_invoices");
        
        initDatabase();
        alert("Database has been reset to default seeds.");
        refreshAllViews();
    }
});

// ==================== CUSTOMERS CRUD ====================
const customerModal = document.getElementById("modal-customer");
const customerForm = document.getElementById("customer-form");
const customersTableBody = document.getElementById("customers-table-body");
const searchCustomerInput = document.getElementById("search-customer");

document.getElementById("btn-add-customer").addEventListener("click", () => {
    customerForm.reset();
    document.getElementById("customer-id").value = "";
    document.getElementById("customer-modal-title").textContent = "Add Customer";
    customerModal.classList.remove("hidden");
});

searchCustomerInput.addEventListener("input", () => {
    currentPage.customers = 1;
    renderCustomersTab();
});

customerForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const idVal = document.getElementById("customer-id").value;
    const name = document.getElementById("c-name").value.trim();
    const phone = document.getElementById("c-phone").value.trim();
    const email = document.getElementById("c-email").value.trim();
    const address = document.getElementById("c-address").value.trim();

    if (idVal) {
        // Edit Mode
        const customer = state.customers.find(c => c.id === parseInt(idVal));
        if (customer) {
            customer.name = name;
            customer.phone = phone;
            customer.email = email;
            customer.address = address;
        }
    } else {
        // Create Mode
        const newId = state.customers.length > 0 ? Math.max(...state.customers.map(c => c.id)) + 1 : 1;
        state.customers.push({ id: newId, name, phone, email, address, isDeleted: false });
    }

    saveStateToStorage();
    customerModal.classList.add("hidden");
    refreshAllViews();
});

function renderCustomersTab() {
    let list = state.customers.filter(c => !c.isDeleted);
    const searchVal = searchCustomerInput.value.toLowerCase().trim();
    
    if (searchVal) {
        list = list.filter(c => 
            c.name.toLowerCase().includes(searchVal) || 
            c.phone.includes(searchVal) || 
            c.email.toLowerCase().includes(searchVal)
        );
    }

    const { data, totalPages } = getPaginatedData(list, currentPage.customers, PAGE_SIZE);
    
    customersTableBody.innerHTML = "";
    if (data.length === 0) {
        customersTableBody.innerHTML = `<tr><td colspan="7" class="text-center">No customers found.</td></tr>`;
    } else {
        data.forEach(c => {
            const customerVehicles = state.vehicles.filter(v => v.customerId === c.id && !v.isDeleted);
            const vehiclesStr = customerVehicles.map(v => v.vehicleNumber).join(", ") || "None";
            
            const tr = document.createElement("tr");
            tr.innerHTML = `
                <td>${c.id}</td>
                <td><strong>${c.name}</strong></td>
                <td>${c.phone}</td>
                <td>${c.email}</td>
                <td>${c.address}</td>
                <td><span class="badge badge-info">${vehiclesStr}</span></td>
                <td class="actions-col">
                    <div class="action-buttons">
                        <button class="btn-actions btn-edit" onclick="editCustomer(${c.id})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 20h9"/><path d="M16.5 3.5a2.12 2.12 0 0 1 3 3L7 19l-4 1 1-4Z"/></svg></button>
                        <button class="btn-actions btn-delete" onclick="deleteCustomer(${c.id})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg></button>
                    </div>
                </td>
            `;
            customersTableBody.appendChild(tr);
        });
    }

    renderPaginationControls(
        "customers-pagination", 
        totalPages, 
        currentPage.customers, 
        () => { currentPage.customers--; renderCustomersTab(); },
        () => { currentPage.customers++; renderCustomersTab(); }
    );
}

window.editCustomer = (id) => {
    const c = state.customers.find(cust => cust.id === id);
    if (!c) return;

    document.getElementById("customer-id").value = c.id;
    document.getElementById("c-name").value = c.name;
    document.getElementById("c-phone").value = c.phone;
    document.getElementById("c-email").value = c.email;
    document.getElementById("c-address").value = c.address;
    document.getElementById("customer-modal-title").textContent = "Update Customer Info";
    customerModal.classList.remove("hidden");
};

window.deleteCustomer = (id) => {
    if (confirm("Are you sure you want to delete this customer? This will also unregister their vehicles.")) {
        const idx = state.customers.findIndex(c => c.id === id);
        if (idx !== -1) {
            state.customers[idx].isDeleted = true;
            // cascade soft-delete vehicles
            state.vehicles.forEach(v => {
                if (v.customerId === id) v.isDeleted = true;
            });
            saveStateToStorage();
            refreshAllViews();
        }
    }
};

// ==================== VEHICLES CRUD ====================
const vehicleModal = document.getElementById("modal-vehicle");
const vehicleForm = document.getElementById("vehicle-form");
const vehiclesTableBody = document.getElementById("vehicles-table-body");
const searchVehicleInput = document.getElementById("search-vehicle");
const vehicleOwnerSelect = document.getElementById("v-owner");

document.getElementById("btn-add-vehicle").addEventListener("click", () => {
    vehicleForm.reset();
    document.getElementById("vehicle-old-number").value = "";
    document.getElementById("v-number").removeAttribute("disabled");
    document.getElementById("vehicle-modal-title").textContent = "Register Vehicle";
    
    // Fill customer select
    populateCustomerDropdown(vehicleOwnerSelect);
    vehicleModal.classList.remove("hidden");
});

searchVehicleInput.addEventListener("input", () => {
    currentPage.vehicles = 1;
    renderVehiclesTab();
});

vehicleForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const oldNumber = document.getElementById("vehicle-old-number").value;
    const vehicleNumber = document.getElementById("v-number").value.toUpperCase().trim();
    const brand = document.getElementById("v-brand").value.trim();
    const model = document.getElementById("v-model").value.trim();
    const year = parseInt(document.getElementById("v-year").value);
    const color = document.getElementById("v-color").value.trim();
    const engineNumber = document.getElementById("v-engine").value.trim();
    const customerId = parseInt(document.getElementById("v-owner").value);

    if (oldNumber) {
        // Edit Mode
        const veh = state.vehicles.find(v => v.vehicleNumber === oldNumber);
        if (veh) {
            veh.brand = brand;
            veh.model = model;
            veh.year = year;
            veh.color = color;
            veh.engineNumber = engineNumber;
            veh.customerId = customerId;
        }
    } else {
        // Verify unique registration
        const duplicate = state.vehicles.find(v => v.vehicleNumber === vehicleNumber && !v.isDeleted);
        if (duplicate) {
            alert("Vehicle Number is already registered in the system!");
            return;
        }
        
        // Create Mode
        state.vehicles.push({
            vehicleNumber, brand, model, year, color, engineNumber, customerId, isDeleted: false
        });
    }

    saveStateToStorage();
    vehicleModal.classList.add("hidden");
    refreshAllViews();
});

function populateCustomerDropdown(selectElement) {
    selectElement.innerHTML = `<option value="">-- Choose Owner --</option>`;
    state.customers.filter(c => !c.isDeleted).forEach(c => {
        const option = document.createElement("option");
        option.value = c.id;
        option.textContent = `${c.name} (${c.phone})`;
        selectElement.appendChild(option);
    });
}

function renderVehiclesTab() {
    let list = state.vehicles.filter(v => !v.isDeleted);
    const searchVal = searchVehicleInput.value.toLowerCase().trim();

    if (searchVal) {
        list = list.filter(v => 
            v.vehicleNumber.toLowerCase().includes(searchVal) || 
            v.brand.toLowerCase().includes(searchVal) || 
            v.model.toLowerCase().includes(searchVal)
        );
    }

    const { data, totalPages } = getPaginatedData(list, currentPage.vehicles, PAGE_SIZE);

    vehiclesTableBody.innerHTML = "";
    if (data.length === 0) {
        vehiclesTableBody.innerHTML = `<tr><td colspan="7" class="text-center">No vehicles registered.</td></tr>`;
    } else {
        data.forEach(v => {
            const owner = state.customers.find(c => c.id === v.customerId);
            const ownerStr = owner ? `${owner.name} (${owner.phone})` : "Unknown Owner";

            const tr = document.createElement("tr");
            tr.innerHTML = `
                <td><strong>${v.vehicleNumber}</strong></td>
                <td>${v.brand} ${v.model}</td>
                <td>${v.year}</td>
                <td>${v.color}</td>
                <td><code>${v.engineNumber}</code></td>
                <td>${ownerStr}</td>
                <td class="actions-col">
                    <div class="action-buttons">
                        <button class="btn-actions btn-edit" onclick="editVehicle('${v.vehicleNumber}')"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 20h9"/><path d="M16.5 3.5a2.12 2.12 0 0 1 3 3L7 19l-4 1 1-4Z"/></svg></button>
                        <button class="btn-actions btn-delete" onclick="deleteVehicle('${v.vehicleNumber}')"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg></button>
                    </div>
                </td>
            `;
            vehiclesTableBody.appendChild(tr);
        });
    }

    renderPaginationControls(
        "vehicles-pagination", 
        totalPages, 
        currentPage.vehicles, 
        () => { currentPage.vehicles--; renderVehiclesTab(); },
        () => { currentPage.vehicles++; renderVehiclesTab(); }
    );
}

window.editVehicle = (num) => {
    const v = state.vehicles.find(veh => veh.vehicleNumber === num);
    if (!v) return;

    populateCustomerDropdown(vehicleOwnerSelect);

    document.getElementById("vehicle-old-number").value = v.vehicleNumber;
    document.getElementById("v-number").value = v.vehicleNumber;
    document.getElementById("v-number").setAttribute("disabled", "true");
    document.getElementById("v-brand").value = v.brand;
    document.getElementById("v-model").value = v.model;
    document.getElementById("v-year").value = v.year;
    document.getElementById("v-color").value = v.color;
    document.getElementById("v-engine").value = v.engineNumber;
    document.getElementById("v-owner").value = v.customerId;
    document.getElementById("vehicle-modal-title").textContent = "Edit Vehicle Details";

    vehicleModal.classList.remove("hidden");
};

window.deleteVehicle = (num) => {
    if (confirm(`Unregister vehicle ${num}?`)) {
        const idx = state.vehicles.findIndex(v => v.vehicleNumber === num);
        if (idx !== -1) {
            state.vehicles[idx].isDeleted = true;
            saveStateToStorage();
            refreshAllViews();
        }
    }
};

// ==================== EMPLOYEES CRUD ====================
const employeeModal = document.getElementById("modal-employee");
const employeeForm = document.getElementById("employee-form");
const employeesTableBody = document.getElementById("employees-table-body");
const searchEmployeeInput = document.getElementById("search-employee");

document.getElementById("btn-add-employee").addEventListener("click", () => {
    employeeForm.reset();
    document.getElementById("employee-id").value = "";
    document.getElementById("employee-modal-title").textContent = "Add Employee";
    employeeModal.classList.remove("hidden");
});

searchEmployeeInput.addEventListener("input", () => {
    currentPage.employees = 1;
    renderEmployeesTab();
});

employeeForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const idVal = document.getElementById("employee-id").value;
    const name = document.getElementById("e-name").value.trim();
    const designation = document.getElementById("e-designation").value.trim();
    const salary = parseFloat(document.getElementById("e-salary").value);
    const phone = document.getElementById("e-phone").value.trim();

    if (idVal) {
        // Edit Mode
        const emp = state.employees.find(emp => emp.id === parseInt(idVal));
        if (emp) {
            emp.name = name;
            emp.designation = designation;
            emp.salary = salary;
            emp.phone = phone;
        }
    } else {
        // Create Mode
        const newId = state.employees.length > 0 ? Math.max(...state.employees.map(emp => emp.id)) + 1 : 1;
        state.employees.push({ id: newId, name, designation, salary, phone, isDeleted: false });
    }

    saveStateToStorage();
    employeeModal.classList.add("hidden");
    refreshAllViews();
});

function renderEmployeesTab() {
    let list = state.employees.filter(emp => !emp.isDeleted);
    const searchVal = searchEmployeeInput.value.toLowerCase().trim();

    if (searchVal) {
        list = list.filter(emp => 
            emp.name.toLowerCase().includes(searchVal) || 
            emp.designation.toLowerCase().includes(searchVal) || 
            emp.phone.includes(searchVal)
        );
    }

    const { data, totalPages } = getPaginatedData(list, currentPage.employees, PAGE_SIZE);

    employeesTableBody.innerHTML = "";
    if (data.length === 0) {
        employeesTableBody.innerHTML = `<tr><td colspan="6" class="text-center">No employees found.</td></tr>`;
    } else {
        data.forEach(emp => {
            const tr = document.createElement("tr");
            tr.innerHTML = `
                <td>${emp.id}</td>
                <td><strong>${emp.name}</strong></td>
                <td><span class="badge badge-info">${emp.designation}</span></td>
                <td>Rs. ${emp.salary.toFixed(2)}</td>
                <td>${emp.phone}</td>
                <td class="actions-col">
                    <div class="action-buttons">
                        <button class="btn-actions btn-edit" onclick="editEmployee(${emp.id})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 20h9"/><path d="M16.5 3.5a2.12 2.12 0 0 1 3 3L7 19l-4 1 1-4Z"/></svg></button>
                        <button class="btn-actions btn-delete" onclick="deleteEmployee(${emp.id})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg></button>
                    </div>
                </td>
            `;
            employeesTableBody.appendChild(tr);
        });
    }

    renderPaginationControls(
        "employees-pagination", 
        totalPages, 
        currentPage.employees, 
        () => { currentPage.employees--; renderEmployeesTab(); },
        () => { currentPage.employees++; renderEmployeesTab(); }
    );
}

window.editEmployee = (id) => {
    const emp = state.employees.find(emp => emp.id === id);
    if (!emp) return;

    document.getElementById("employee-id").value = emp.id;
    document.getElementById("e-name").value = emp.name;
    document.getElementById("e-designation").value = emp.designation;
    document.getElementById("e-salary").value = emp.salary;
    document.getElementById("e-phone").value = emp.phone;
    document.getElementById("employee-modal-title").textContent = "Update Staff File";

    employeeModal.classList.remove("hidden");
};

window.deleteEmployee = (id) => {
    if (confirm("Remove this employee from active payroll?")) {
        const idx = state.employees.findIndex(emp => emp.id === id);
        if (idx !== -1) {
            state.employees[idx].isDeleted = true;
            saveStateToStorage();
            refreshAllViews();
        }
    }
};

// ==================== SERVICES CRUD ====================
const serviceModal = document.getElementById("modal-service");
const serviceForm = document.getElementById("service-form");
const servicesTableBody = document.getElementById("services-table-body");
const searchServiceInput = document.getElementById("search-service");

document.getElementById("btn-add-service").addEventListener("click", () => {
    serviceForm.reset();
    document.getElementById("service-id").value = "";
    document.getElementById("service-modal-title").textContent = "Add Service Catalog";
    serviceModal.classList.remove("hidden");
});

searchServiceInput.addEventListener("input", () => {
    currentPage.services = 1;
    renderServicesTab();
});

serviceForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const idVal = document.getElementById("service-id").value;
    const name = document.getElementById("s-name").value.trim();
    const cost = parseFloat(document.getElementById("s-cost").value);
    const description = document.getElementById("s-desc").value.trim();

    if (idVal) {
        // Edit Mode
        const srv = state.services.find(s => s.id === parseInt(idVal));
        if (srv) {
            srv.name = name;
            srv.labourCost = cost;
            srv.description = description;
        }
    } else {
        // Create Mode
        const newId = state.services.length > 0 ? Math.max(...state.services.map(s => s.id)) + 1 : 1;
        state.services.push({ id: newId, name, labourCost: cost, description, isDeleted: false });
    }

    saveStateToStorage();
    serviceModal.classList.add("hidden");
    refreshAllViews();
});

function renderServicesTab() {
    let list = state.services.filter(s => !s.isDeleted);
    const searchVal = searchServiceInput.value.toLowerCase().trim();

    if (searchVal) {
        list = list.filter(s => s.name.toLowerCase().includes(searchVal));
    }

    const { data, totalPages } = getPaginatedData(list, currentPage.services, PAGE_SIZE);

    servicesTableBody.innerHTML = "";
    if (data.length === 0) {
        servicesTableBody.innerHTML = `<tr><td colspan="5" class="text-center">No services found.</td></tr>`;
    } else {
        data.forEach(s => {
            const tr = document.createElement("tr");
            tr.innerHTML = `
                <td>${s.id}</td>
                <td><strong>${s.name}</strong></td>
                <td>${s.description}</td>
                <td>Rs. ${s.labourCost.toFixed(2)}</td>
                <td class="actions-col">
                    <div class="action-buttons">
                        <button class="btn-actions btn-edit" onclick="editService(${s.id})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 20h9"/><path d="M16.5 3.5a2.12 2.12 0 0 1 3 3L7 19l-4 1 1-4Z"/></svg></button>
                        <button class="btn-actions btn-delete" onclick="deleteService(${s.id})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg></button>
                    </div>
                </td>
            `;
            servicesTableBody.appendChild(tr);
        });
    }

    renderPaginationControls(
        "services-pagination", 
        totalPages, 
        currentPage.services, 
        () => { currentPage.services--; renderServicesTab(); },
        () => { currentPage.services++; renderServicesTab(); }
    );
}

window.editService = (id) => {
    const s = state.services.find(srv => srv.id === id);
    if (!s) return;

    document.getElementById("service-id").value = s.id;
    document.getElementById("s-name").value = s.name;
    document.getElementById("s-cost").value = s.labourCost;
    document.getElementById("s-desc").value = s.description;
    document.getElementById("service-modal-title").textContent = "Modify Service Catalog";

    serviceModal.classList.remove("hidden");
};

window.deleteService = (id) => {
    if (confirm("Delete this service item from database?")) {
        const idx = state.services.findIndex(s => s.id === id);
        if (idx !== -1) {
            state.services[idx].isDeleted = true;
            saveStateToStorage();
            refreshAllViews();
        }
    }
};

// ==================== PARTS CRUD ====================
const partModal = document.getElementById("modal-part");
const partForm = document.getElementById("part-form");
const partsTableBody = document.getElementById("parts-table-body");
const searchPartInput = document.getElementById("search-part");

document.getElementById("btn-add-part").addEventListener("click", () => {
    partForm.reset();
    document.getElementById("part-id").value = "";
    document.getElementById("part-modal-title").textContent = "Add New Part";
    partModal.classList.remove("hidden");
});

searchPartInput.addEventListener("input", () => {
    currentPage.parts = 1;
    renderPartsTab();
});

partForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const idVal = document.getElementById("part-id").value;
    const name = document.getElementById("p-name").value.trim();
    const price = parseFloat(document.getElementById("p-price").value);
    const qty = parseInt(document.getElementById("p-qty").value);

    if (idVal) {
        // Edit Mode
        const part = state.parts.find(p => p.id === parseInt(idVal));
        if (part) {
            part.name = name;
            part.unitPrice = price;
            part.quantity = qty;
        }
    } else {
        // Create Mode
        const newId = state.parts.length > 0 ? Math.max(...state.parts.map(p => p.id)) + 1 : 1;
        state.parts.push({ id: newId, name, unitPrice: price, quantity: qty, isDeleted: false });
    }

    saveStateToStorage();
    partModal.classList.add("hidden");
    refreshAllViews();
});

function renderPartsTab() {
    let list = state.parts.filter(p => !p.isDeleted);
    const searchVal = searchPartInput.value.toLowerCase().trim();

    if (searchVal) {
        list = list.filter(p => p.name.toLowerCase().includes(searchVal));
    }

    const { data, totalPages } = getPaginatedData(list, currentPage.parts, PAGE_SIZE);

    partsTableBody.innerHTML = "";
    if (data.length === 0) {
        partsTableBody.innerHTML = `<tr><td colspan="6" class="text-center">No parts in inventory.</td></tr>`;
    } else {
        data.forEach(p => {
            const isLow = p.quantity < 5;
            const statusBadge = isLow ? `<span class="badge badge-danger">Low Stock (${p.quantity})</span>` : `<span class="badge badge-success">In Stock</span>`;
            
            const tr = document.createElement("tr");
            tr.innerHTML = `
                <td>${p.id}</td>
                <td><strong>${p.name}</strong></td>
                <td>Rs. ${p.unitPrice.toFixed(2)}</td>
                <td>${p.quantity} units</td>
                <td>${statusBadge}</td>
                <td class="actions-col">
                    <div class="action-buttons">
                        <button class="btn-actions btn-edit" onclick="editPart(${p.id})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 20h9"/><path d="M16.5 3.5a2.12 2.12 0 0 1 3 3L7 19l-4 1 1-4Z"/></svg></button>
                        <button class="btn-actions btn-delete" onclick="deletePart(${p.id})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg></button>
                    </div>
                </td>
            `;
            partsTableBody.appendChild(tr);
        });
    }

    renderPaginationControls(
        "parts-pagination", 
        totalPages, 
        currentPage.parts, 
        () => { currentPage.parts--; renderPartsTab(); },
        () => { currentPage.parts++; renderPartsTab(); }
    );
}

window.editPart = (id) => {
    const p = state.parts.find(pt => pt.id === id);
    if (!p) return;

    document.getElementById("part-id").value = p.id;
    document.getElementById("p-name").value = p.name;
    document.getElementById("p-price").value = p.unitPrice;
    document.getElementById("p-qty").value = p.quantity;
    document.getElementById("part-modal-title").textContent = "Adjust Stock/Inventory";

    partModal.classList.remove("hidden");
};

window.deletePart = (id) => {
    if (confirm("Delete this part item from inventory record?")) {
        const idx = state.parts.findIndex(p => p.id === id);
        if (idx !== -1) {
            state.parts[idx].isDeleted = true;
            saveStateToStorage();
            refreshAllViews();
        }
    }
};

// ==================== INVOICES BILLING SYSTEM ====================
const invoiceModal = document.getElementById("modal-invoice");
const invoiceForm = document.getElementById("invoice-form");
const invoicesTableBody = document.getElementById("invoices-table-body");
const searchInvoiceInput = document.getElementById("search-invoice");
const filterInvoiceStatus = document.getElementById("filter-invoice-status");
const invVehicleSelect = document.getElementById("inv-vehicle-select");

// Wizard wizard items
let currentInvoiceServices = []; // list of service ids
let currentInvoiceParts = []; // list of objects { partId, qty }

document.getElementById("btn-create-invoice").addEventListener("click", () => {
    invoiceForm.reset();
    currentInvoiceServices = [];
    currentInvoiceParts = [];
    
    document.getElementById("inv-owner-name").textContent = "-";
    document.getElementById("inv-owner-phone").textContent = "-";
    document.getElementById("inv-vehicle-details").textContent = "-";
    document.getElementById("part-stock-warning").classList.add("hidden");
    
    // Fill vehicles select dropdown
    invVehicleSelect.innerHTML = `<option value="">-- Choose Vehicle --</option>`;
    state.vehicles.filter(v => !v.isDeleted).forEach(v => {
        const option = document.createElement("option");
        option.value = v.vehicleNumber;
        option.textContent = `${v.vehicleNumber} (${v.brand} ${v.model})`;
        invVehicleSelect.appendChild(option);
    });

    // Populate service and part selectors
    populateInvoiceDropdowns();
    renderSelectedItemsInWizard();
    updateInvoiceCalculations();

    invoiceModal.classList.remove("hidden");
});

function populateInvoiceDropdowns() {
    const sSelect = document.getElementById("inv-service-select");
    sSelect.innerHTML = `<option value="">-- Add Service --</option>`;
    state.services.filter(s => !s.isDeleted).forEach(s => {
        const option = document.createElement("option");
        option.value = s.id;
        option.textContent = `${s.name} (Rs. ${s.labourCost})`;
        sSelect.appendChild(option);
    });

    const pSelect = document.getElementById("inv-part-select");
    pSelect.innerHTML = `<option value="">-- Add Spare Part --</option>`;
    state.parts.filter(p => !p.isDeleted).forEach(p => {
        const option = document.createElement("option");
        option.value = p.id;
        option.textContent = `${p.name} (Rs. ${p.unitPrice}) [Stock: ${p.quantity}]`;
        pSelect.appendChild(option);
    });
}

// Handle vehicle selection change to load customer owner info
invVehicleSelect.addEventListener("change", (e) => {
    const vNo = e.target.value;
    const v = state.vehicles.find(veh => veh.vehicleNumber === vNo && !veh.isDeleted);
    
    if (v) {
        const owner = state.customers.find(c => c.id === v.customerId && !c.isDeleted);
        if (owner) {
            document.getElementById("inv-owner-name").textContent = owner.name;
            document.getElementById("inv-owner-phone").textContent = owner.phone;
        } else {
            document.getElementById("inv-owner-name").textContent = "Unknown";
            document.getElementById("inv-owner-phone").textContent = "-";
        }
        document.getElementById("inv-vehicle-details").textContent = `${v.brand} ${v.model} (${v.year})`;
    } else {
        document.getElementById("inv-owner-name").textContent = "-";
        document.getElementById("inv-owner-phone").textContent = "-";
        document.getElementById("inv-vehicle-details").textContent = "-";
    }
});

// Switch checkout options based on status select
const invStatusSelect = document.getElementById("inv-status");
const paymentDetailsSec = document.getElementById("payment-details-section");

invStatusSelect.addEventListener("change", () => {
    if (invStatusSelect.value === "paid") {
        paymentDetailsSec.classList.remove("hidden");
        document.getElementById("inv-payment-ref").setAttribute("required", "true");
    } else {
        paymentDetailsSec.classList.add("hidden");
        document.getElementById("inv-payment-ref").removeAttribute("required");
    }
});

// Adding services to job sheet
document.getElementById("btn-add-service-to-invoice").addEventListener("click", () => {
    const select = document.getElementById("inv-service-select");
    const sId = parseInt(select.value);
    if (!sId) return;

    // Check C++ limit constraint
    if (currentInvoiceServices.length >= 5) {
        alert("Maximum 5 services can be added to a single invoice, as per system constraints!");
        return;
    }

    if (currentInvoiceServices.includes(sId)) {
        alert("This service is already added!");
        return;
    }

    currentInvoiceServices.push(sId);
    select.value = "";
    renderSelectedItemsInWizard();
    updateInvoiceCalculations();
});

// Adding parts to invoice sheet
document.getElementById("btn-add-part-to-invoice").addEventListener("click", () => {
    const select = document.getElementById("inv-part-select");
    const pId = parseInt(select.value);
    const qty = parseInt(document.getElementById("inv-part-qty").value) || 1;
    const warning = document.getElementById("part-stock-warning");
    
    if (!pId) return;

    // Check C++ limit constraint
    if (currentInvoiceParts.length >= 5) {
        alert("Maximum 5 parts replacements can be added to a single invoice, as per system constraints!");
        return;
    }

    const part = state.parts.find(p => p.id === pId && !p.isDeleted);
    if (!part) return;

    // Check duplicate
    const existing = currentInvoiceParts.find(item => item.partId === pId);
    const totalQty = existing ? existing.qty + qty : qty;

    if (totalQty > part.quantity) {
        warning.textContent = `Insufficient stock! Only ${part.quantity} unit(s) of ${part.name} available.`;
        warning.classList.remove("hidden");
        return;
    } else {
        warning.classList.add("hidden");
    }

    if (existing) {
        existing.qty = totalQty;
    } else {
        currentInvoiceParts.push({ partId: pId, qty: totalQty });
    }

    select.value = "";
    document.getElementById("inv-part-qty").value = 1;
    renderSelectedItemsInWizard();
    updateInvoiceCalculations();
});

function renderSelectedItemsInWizard() {
    const sContainer = document.getElementById("invoice-selected-services");
    sContainer.innerHTML = "";
    currentInvoiceServices.forEach(sid => {
        const s = state.services.find(srv => srv.id === sid);
        if (s) {
            const row = document.createElement("div");
            row.className = "selected-item-row";
            row.innerHTML = `
                <span>${s.name} (Rs. ${s.labourCost.toFixed(2)})</span>
                <button type="button" class="item-remove-btn" onclick="removeServiceFromWizard(${sid})">&times;</button>
            `;
            sContainer.appendChild(row);
        }
    });

    const pContainer = document.getElementById("invoice-selected-parts");
    pContainer.innerHTML = "";
    currentInvoiceParts.forEach(item => {
        const p = state.parts.find(pt => pt.id === item.partId);
        if (p) {
            const row = document.createElement("div");
            row.className = "selected-item-row";
            row.innerHTML = `
                <span>${p.name} x ${item.qty} (Rs. ${(p.unitPrice * item.qty).toFixed(2)})</span>
                <button type="button" class="item-remove-btn" onclick="removePartFromWizard(${item.partId})">&times;</button>
            `;
            pContainer.appendChild(row);
        }
    });
}

window.removeServiceFromWizard = (sid) => {
    currentInvoiceServices = currentInvoiceServices.filter(id => id !== sid);
    renderSelectedItemsInWizard();
    updateInvoiceCalculations();
};

window.removePartFromWizard = (pid) => {
    currentInvoiceParts = currentInvoiceParts.filter(item => item.partId !== pid);
    renderSelectedItemsInWizard();
    updateInvoiceCalculations();
};

// Real-time calculation computation
const discountInput = document.getElementById("inv-discount");
discountInput.addEventListener("input", updateInvoiceCalculations);

function updateInvoiceCalculations() {
    let labour = 0;
    currentInvoiceServices.forEach(sid => {
        const s = state.services.find(srv => srv.id === sid);
        if (s) labour += s.labourCost;
    });

    let partsCharges = 0;
    currentInvoiceParts.forEach(item => {
        const p = state.parts.find(pt => pt.id === item.partId);
        if (p) partsCharges += (p.unitPrice * item.qty);
    });

    const subtotal = labour + partsCharges;
    const discount = parseFloat(discountInput.value) || 0;
    
    // Subtotal subtraction discount cap
    const subtotalAfterDiscount = Math.max(0, subtotal - discount);
    const gst = subtotalAfterDiscount * 0.18; // GST 18% matching C++
    const grand = subtotalAfterDiscount + gst;

    document.getElementById("summary-labour").textContent = `Rs. ${labour.toFixed(2)}`;
    document.getElementById("summary-parts").textContent = `Rs. ${partsCharges.toFixed(2)}`;
    document.getElementById("summary-subtotal").textContent = `Rs. ${subtotal.toFixed(2)}`;
    document.getElementById("summary-discount").textContent = `-Rs. ${discount.toFixed(2)}`;
    document.getElementById("summary-gst").textContent = `Rs. ${gst.toFixed(2)}`;
    document.getElementById("summary-grand").textContent = `Rs. ${grand.toFixed(2)}`;
}

// Generate / Submit Invoice Flow
invoiceForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const vehicleNumber = invVehicleSelect.value;
    const discount = parseFloat(discountInput.value) || 0;
    const isPaid = invStatusSelect.value === "paid";
    
    if (!vehicleNumber) {
        alert("Please select a vehicle.");
        return;
    }

    if (currentInvoiceServices.length === 0 && currentInvoiceParts.length === 0) {
        alert("Please add at least one job service or spare part replacement item.");
        return;
    }

    const veh = state.vehicles.find(v => v.vehicleNumber === vehicleNumber && !v.isDeleted);
    const cust = state.customers.find(c => c.id === veh.customerId && !c.isDeleted);

    // Calculate final metrics
    let labourCharges = 0;
    currentInvoiceServices.forEach(sid => {
        const s = state.services.find(srv => srv.id === sid);
        if (s) labourCharges += s.labourCost;
    });

    let partsCharges = 0;
    currentInvoiceParts.forEach(item => {
        const p = state.parts.find(pt => pt.id === item.partId);
        if (p) partsCharges += (p.unitPrice * item.qty);
    });

    const subtotal = labourCharges + partsCharges;
    const subtotalAfterDiscount = Math.max(0, subtotal - discount);
    const gstAmount = subtotalAfterDiscount * 0.18;
    const grandTotal = subtotalAfterDiscount + gstAmount;

    // Deduct quantities from stock database
    for (const item of currentInvoiceParts) {
        const part = state.parts.find(p => p.id === item.partId);
        if (part) {
            part.quantity -= item.qty;
        }
    }

    const nextNumber = state.invoices.length > 0 ? Math.max(...state.invoices.map(inv => inv.invoiceNumber)) + 1 : 1001;

    const newInvoice = {
        invoiceNumber: nextNumber,
        customerId: cust.id,
        customerName: cust.name,
        customerPhone: cust.phone,
        vehicleNumber: veh.vehicleNumber,
        vehicleBrand: veh.brand,
        vehicleModel: veh.model,
        date: new Date().toISOString().slice(0, 10),
        serviceIds: [...currentInvoiceServices],
        partsList: [...currentInvoiceParts],
        labourCharges,
        partsCharges,
        subtotal,
        discount,
        gstAmount,
        grandTotal,
        isPaid,
        paymentMode: isPaid ? parseInt(document.getElementById("inv-payment-mode").value) : 1,
        paymentRef: isPaid ? document.getElementById("inv-payment-ref").value.trim() : "",
        isDeleted: false
    };

    state.invoices.push(newInvoice);
    saveStateToStorage();
    invoiceModal.classList.add("hidden");
    refreshAllViews();
    
    // Auto show receipt popup immediately
    showInvoiceReceipt(newInvoice.invoiceNumber);
});

searchInvoiceInput.addEventListener("input", () => {
    currentPage.invoices = 1;
    renderInvoicesTab();
});

filterInvoiceStatus.addEventListener("change", () => {
    currentPage.invoices = 1;
    renderInvoicesTab();
});

function renderInvoicesTab() {
    let list = state.invoices.filter(inv => !inv.isDeleted);
    const searchVal = searchInvoiceInput.value.toLowerCase().trim();
    const filterVal = filterInvoiceStatus.value;

    if (searchVal) {
        list = list.filter(inv => 
            inv.invoiceNumber.toString().includes(searchVal) || 
            inv.customerName.toLowerCase().includes(searchVal) || 
            inv.vehicleNumber.toLowerCase().includes(searchVal)
        );
    }

    if (filterVal !== "all") {
        const targetPaid = filterVal === "paid";
        list = list.filter(inv => inv.isPaid === targetPaid);
    }

    // Sort: newest invoices first
    list.sort((a, b) => b.invoiceNumber - a.invoiceNumber);

    const { data, totalPages } = getPaginatedData(list, currentPage.invoices, PAGE_SIZE);

    invoicesTableBody.innerHTML = "";
    if (data.length === 0) {
        invoicesTableBody.innerHTML = `<tr><td colspan="7" class="text-center">No invoices found.</td></tr>`;
    } else {
        data.forEach(inv => {
            const statusBadge = inv.isPaid ? `<span class="badge badge-success">Paid</span>` : `<span class="badge badge-warning">Unpaid</span>`;
            const tr = document.createElement("tr");
            tr.innerHTML = `
                <td>#${inv.invoiceNumber}</td>
                <td>${inv.date}</td>
                <td><strong>${inv.customerName}</strong><br><span style="font-size:12px;color:var(--color-text-muted)">${inv.customerPhone}</span></td>
                <td><span class="badge badge-info">${inv.vehicleNumber}</span></td>
                <td><strong>Rs. ${inv.grandTotal.toFixed(2)}</strong></td>
                <td>${statusBadge}</td>
                <td class="actions-col">
                    <div class="action-buttons">
                        <button class="btn-actions btn-edit" title="View Receipt" onclick="showInvoiceReceipt(${inv.invoiceNumber})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2 12s3-7 10-7 10 7 10 7-3 7-10 7-10-7-10-7Z"/><circle cx="12" cy="12" r="3"/></svg></button>
                        ${!inv.isPaid ? `<button class="btn-actions btn-edit" style="color:var(--success)" title="Collect Payment" onclick="collectPayment(${inv.invoiceNumber})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 2v20M17 5H9.5a3.5 3.5 0 0 0 0 7h5a3.5 3.5 0 0 1 0 7H6"/></svg></button>` : ''}
                        <button class="btn-actions btn-delete" title="Delete Invoice" onclick="deleteInvoice(${inv.invoiceNumber})"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg></button>
                    </div>
                </td>
            `;
            invoicesTableBody.appendChild(tr);
        });
    }

    renderPaginationControls(
        "invoices-pagination", 
        totalPages, 
        currentPage.invoices, 
        () => { currentPage.invoices--; renderInvoicesTab(); },
        () => { currentPage.invoices++; renderInvoicesTab(); }
    );
}

window.deleteInvoice = (num) => {
    if (confirm(`Delete invoice #${num} record? (This soft deletes the record)`)) {
        const idx = state.invoices.findIndex(inv => inv.invoiceNumber === num);
        if (idx !== -1) {
            // Restore stock before deleting
            const inv = state.invoices[idx];
            if (inv.partsList) {
                inv.partsList.forEach(item => {
                    const pt = state.parts.find(p => p.id === item.partId);
                    if (pt) pt.quantity += item.qty;
                });
            }
            inv.isDeleted = true;
            saveStateToStorage();
            refreshAllViews();
        }
    }
};

window.collectPayment = (num) => {
    const inv = state.invoices.find(i => i.invoiceNumber === num);
    if (!inv) return;

    const mode = prompt(`Select Payment Mode for Invoice #${num}:\n1. Cash\n2. UPI\n3. Credit Card\n4. Debit Card\n5. Net Banking\n\nEnter number (1-5):`, "2");
    if (!mode) return;
    
    const modeInt = parseInt(mode);
    if (isNaN(modeInt) || modeInt < 1 || modeInt > 5) {
        alert("Invalid selection!");
        return;
    }

    const ref = prompt("Enter payment transaction reference (optional):", `TXN${Date.now().toString().slice(-6)}`);
    
    inv.isPaid = true;
    inv.paymentMode = modeInt;
    inv.paymentRef = ref || "";
    
    saveStateToStorage();
    alert("Payment collected successfully!");
    refreshAllViews();
};

// ==================== RECEIPT RENDERING & PRINTING ====================
const receiptModal = document.getElementById("modal-invoice-receipt");
const receiptArea = document.getElementById("receipt-printable-area");

const modeStringMap = {
    1: "CASH",
    2: "UPI / DIGITAL",
    3: "CREDIT CARD",
    4: "DEBIT CARD",
    5: "NET BANKING"
};

window.showInvoiceReceipt = (num) => {
    const inv = state.invoices.find(i => i.invoiceNumber === num && !i.isDeleted);
    if (!inv) return;

    let itemsHtml = "";
    
    // Services
    if (inv.serviceIds && inv.serviceIds.length > 0) {
        itemsHtml += `<div class="receipt-section-title">Jobs / Labour Services</div>`;
        inv.serviceIds.forEach(sid => {
            const s = state.services.find(srv => srv.id === sid);
            if (s) {
                itemsHtml += `
                    <div class="receipt-grid-line">
                        <span>${s.name}</span>
                        <span>Rs. ${s.labourCost.toFixed(2)}</span>
                    </div>
                `;
            }
        });
    }

    // Spare parts
    if (inv.partsList && inv.partsList.length > 0) {
        itemsHtml += `<div class="receipt-section-title">Replaced Spare Parts</div>`;
        inv.partsList.forEach(item => {
            const p = state.parts.find(pt => pt.id === item.partId);
            if (p) {
                itemsHtml += `
                    <div class="receipt-grid-line">
                        <span>${p.name} (x${item.qty})</span>
                        <span>Rs. ${(p.unitPrice * item.qty).toFixed(2)}</span>
                    </div>
                `;
            }
        });
    }

    const paymentStatusText = inv.isPaid ? `PAID VIA ${modeStringMap[inv.paymentMode] || "CASH"}` : "PENDING DUE";
    const refLine = inv.isPaid && inv.paymentRef ? `<div class="receipt-grid-line"><span>Ref Number:</span><span>${inv.paymentRef}</span></div>` : "";

    receiptArea.innerHTML = `
        <div class="receipt-header">
            <h2>G-INVOICE AUTOMOBILES</h2>
            <p>123 Garage Lane, Industrial Area, New Delhi</p>
            <p>Phone: +91 9999988888 | Email: contact@ginvoice.com</p>
        </div>
        
        <div class="receipt-meta">
            <div>
                <strong>Invoice:</strong> #${inv.invoiceNumber}<br>
                <strong>Date:</strong> ${inv.date}<br>
                <strong>Status:</strong> ${paymentStatusText}
            </div>
            <div style="text-align: right">
                <strong>Customer:</strong> ${inv.customerName}<br>
                <strong>Phone:</strong> ${inv.customerPhone}<br>
                <strong>Vehicle:</strong> ${inv.vehicleNumber} (${inv.vehicleBrand} ${inv.vehicleModel})
            </div>
        </div>
        
        <div class="receipt-divider"></div>
        
        ${itemsHtml}
        
        <div class="receipt-divider"></div>
        
        <div class="receipt-total-block">
            <div class="receipt-total-line">
                <span>Labour Charges:</span>
                <span>Rs. ${inv.labourCharges.toFixed(2)}</span>
            </div>
            <div class="receipt-total-line">
                <span>Parts Charges:</span>
                <span>Rs. ${inv.partsCharges.toFixed(2)}</span>
            </div>
            <div class="receipt-total-line">
                <span>Subtotal:</span>
                <span>Rs. ${inv.subtotal.toFixed(2)}</span>
            </div>
            <div class="receipt-total-line" style="color: #059669">
                <span>Discount:</span>
                <span>-Rs. ${inv.discount.toFixed(2)}</span>
            </div>
            <div class="receipt-total-line">
                <span>GST (18%):</span>
                <span>Rs. ${inv.gstAmount.toFixed(2)}</span>
            </div>
            <div class="receipt-total-line receipt-grand-total">
                <span>GRAND TOTAL:</span>
                <span>Rs. ${inv.grandTotal.toFixed(2)}</span>
            </div>
        </div>
        
        <div class="receipt-divider"></div>
        
        <div style="font-size:12px; margin-top:10px;">
            ${refLine}
        </div>

        <div class="receipt-footer">
            <p>THANK YOU FOR YOUR BUSINESS!</p>
            <p>Please drive safely. Visit again for your next scheduled service.</p>
        </div>
    `;

    receiptModal.classList.remove("hidden");
};

document.getElementById("btn-print-receipt").addEventListener("click", () => {
    window.print();
});

// ==================== MODAL OVERLAYS CONTROLLERS ====================
const closeButtons = document.querySelectorAll(".modal-close, .modal-close-btn");
closeButtons.forEach(btn => {
    btn.addEventListener("click", () => {
        document.querySelectorAll(".modal-overlay").forEach(modal => {
            modal.classList.add("hidden");
        });
    });
});

window.addEventListener("click", (e) => {
    if (e.target.classList.contains("modal-overlay")) {
        e.target.classList.add("hidden");
    }
});


// ==================== APPLICATION RUNTIME START ====================
document.addEventListener("DOMContentLoaded", () => {
    initDatabase();
    checkAuthStatus();
});
