#include "SilpoOrder.h" 
using namespace std;

SilpoOrder::SilpoOrder() : status(OrderStatus::Unpaid), deliveryDate("не вибрана") {}

void SilpoOrder::addProduct(const string& name, int quantity, double price) {
    if (quantity <= 0 || price < 0) {
        cout << "Помилка: некоректна кількість або ціна.\n";
        return;
    }

    // Шукаємо товар по назві простим циклом.
    size_t i;
    for (i = 0; i < products.size(); ++i) {
        if (products[i].name == name) {
            // Якщо товар вже є, збільшуємо кількість.
            products[i].quantity += quantity;
            products[i].price = price; // оновлюємо ціну
            return;
        }
    }

    // Якщо товар не знайдений, додаємо новий.
    Product newProduct;
    newProduct.name = name;
    newProduct.quantity = quantity;
    newProduct.price = price;
    products.push_back(newProduct);
}

void SilpoOrder::removeProduct(const string& name) {
    bool removed = false;

    // Проходимо по вектору і видаляємо всі товари з потрібною назвою.
    vector<Product>::iterator it = products.begin();
    while (it != products.end()) {
        if (it->name == name) {
            it = products.erase(it); // erase повертає ітератор на наступний елемент
            removed = true;
        } else {
            ++it;
        }
    }

    if (!removed) {
        cout << "Товар не знайдено: " << name << "\n";
        return;
    }
}

void SilpoOrder::changeProductQuantity(const string& name, int newQuantity) {
    size_t i;
    bool found = false;

    for (i = 0; i < products.size(); ++i) {
        if (products[i].name == name) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Товар не знайдено: " << name << "\n";
        return;
    }

    // found це змінна з сенсом: “знайдено?”
    // !found це логічне заперечення: “не знайдено?”
    // found = true → товар знайдено
    // found = false → товар не знайдено
    // !found = true означає саме “не знайдено” (бо заперечили false)
    // Тому if (!found) читається як: “якщо НЕ знайдено”.

    if (newQuantity <= 0) {
        // Якщо кількість <= 0, логічно видалити товар.
        removeProduct(name);
        return;
    }

    products[i].quantity = newQuantity;
}

void SilpoOrder::setStatus(OrderStatus newStatus) {
    status = newStatus;
}

void SilpoOrder::setDeliveryDate(const string& date) {
    // Для простоти просто зберігаємо текст.
    // Формат бажано: YYYY-MM-DD
    deliveryDate = date;
}

double SilpoOrder::getTotalPrice() const {
    double total = 0.0;
    size_t i;
    for (i = 0; i < products.size(); ++i) {
        total += products[i].price * products[i].quantity;
    }
    return total;
}

void SilpoOrder::printOrder() const {
    cout << "\n====== Замовлення Сільпо ======\n";
    cout << "Дата доставки: " << deliveryDate << "\n";
    cout << "Статус: " << statusToString(status) << "\n";
    cout << "Товари:\n";

    if (products.empty()) {
        cout << "  (список порожній)\n";
    } else {
        size_t i;
        for (i = 0; i < products.size(); ++i) {
            cout << "  - " << products[i].name
                 << " | к-сть: " << products[i].quantity
                 << " | ціна: " << fixed << setprecision(2) << products[i].price
                 << " | сума: " << products[i].price * products[i].quantity
                 << "\n";
        }
    }

    cout << "Загальна сума: " << fixed << setprecision(2) << getTotalPrice() << "\n";
    cout << "===============================\n";
}

bool SilpoOrder::saveToFile(const string& fileName) const {
    ofstream out(fileName);
    if (!out.is_open()) {
        cout << "Не вдалося відкрити файл для запису: " << fileName << "\n";
        return false;
    }

    // Проста структура файлу:
    // 1 рядок - статус
    // 2 рядок - дата доставки
    // 3 рядок - кількість товарів
    // далі кожен товар: name|quantity|price
    out << statusToString(status) << "\n";
    out << deliveryDate << "\n";
    out << products.size() << "\n";

    size_t i;
    for (i = 0; i < products.size(); ++i) {
        out << products[i].name << "|" << products[i].quantity << "|" << products[i].price << "\n";
    }

    return true;
}

bool SilpoOrder::loadFromFile(const string& fileName) {
    ifstream in(fileName);
    if (!in.is_open()) {
        cout << "Не вдалося відкрити файл для читання: " << fileName << "\n";
        return false;
    }

    string statusLine;
    string dateLine;
    string countLine;

    if (!getline(in, statusLine)) return false;
    if (!getline(in, dateLine)) return false;
    if (!getline(in, countLine)) return false;

    status = stringToStatus(statusLine);
    deliveryDate = dateLine;

    size_t count = 0;
    {
        istringstream iss(countLine); // istringstream читає з рядка (string) так, ніби це потік вводу.
        iss >> count;
    }

    products.clear();

    for (size_t i = 0; i < count; ++i) {
        string line;
        if (!getline(in, line)) return false;

        istringstream iss(line);
        string name;
        string quantityStr;
        string priceStr;

        if (!getline(iss, name, '|')) return false;
        if (!getline(iss, quantityStr, '|')) return false;
        if (!getline(iss, priceStr, '|')) return false;

        Product p;
        p.name = name;
        p.quantity = stoi(quantityStr); //stoi = string to int ("3" -> 3)
        p.price = stod(priceStr); //stod = string to double ("19.99" -> 19.99)
        products.push_back(p);
    }

    return true;
}

string statusToString(OrderStatus status) { 
    //Це “перекладач” з коду в людський текст.
    // Потрібно для друку і запису в файл.
    switch (status) {
        case OrderStatus::Completed: return "виконано";
        case OrderStatus::Canceled: return "скасовано";
        case OrderStatus::Paid: return "оплачено";
        case OrderStatus::Unpaid: return "неоплачено";
        //case ... це варіанти: “якщо статус такий-то”.
        //При збігу одразу робимо return ... і виходимо з функції.
    }
    return "неоплачено";
    // Останній return "неоплачено"; це запасний варіант “на всякий випадок”.
}

OrderStatus stringToStatus(const string& value) {
    //Це зворотний “перекладач”: з тексту в enum class.
    //Потрібно саме при читанні файлу.
    //Якщо текст дивний/невідомий — ставимо Unpaid як запасний варіант.
    if (value == "виконано") return OrderStatus::Completed;
    if (value == "скасовано") return OrderStatus::Canceled;
    if (value == "оплачено") return OrderStatus::Paid;
    return OrderStatus::Unpaid;
}
