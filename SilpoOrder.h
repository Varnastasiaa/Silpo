#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

// Структура Product зберігає один товар у замовленні.
struct Product {
    string name;   // Назва товару
    int quantity;       // Кількість одиниць
    double price;       // Ціна за 1 одиницю
};

// Статус замовлення.
// За умовою є 4 можливі стани.
enum class OrderStatus { //enum class це перелік фіксованих значень (набір констант), але у “безпечному” сучасному форматі.
    Completed,  // Виконано
    Canceled,   // Скасовано
    Paid,       // Оплачено
    Unpaid      // Неоплачено
};

// Клас SilpoOrder описує одне онлайн-замовлення "Сільпо".
class SilpoOrder {
private:
    vector<Product> products; // STL контейнер: список товарів
    OrderStatus status;       // Поточний статус замовлення
    string deliveryDate;      // Дата доставки, наприклад 2026-04-20

public:
    // Конструктор за замовчуванням:
    // стартовий статус - Неоплачено, дата - "не вибрана".
    SilpoOrder();

    // Операції з товарами.
    void addProduct(const string& name, int quantity, double price);
    void removeProduct(const string& name);
    void changeProductQuantity(const string& name, int newQuantity);

    // Робота зі статусом і датою.
    void setStatus(OrderStatus newStatus);
    void setDeliveryDate(const string& date);

    // Додаткові корисні методи.
    double getTotalPrice() const;
    void printOrder() const;

    // Робота з файлами.
    // saveToFile: зберігає замовлення у файл.
    // loadFromFile: читає замовлення з файлу.
    bool saveToFile(const string& fileName) const;
    bool loadFromFile(const string& fileName);
};

// Допоміжні функції: Ці допоміжні функції перетворюють статус замовлення між `enum class` і текстом.
// Це потрібно, щоб зручно показувати статус на екрані/записувати у файл і правильно читати його назад з файлу в програму.
// перетворення статусу в текст і назад.
string statusToString(OrderStatus status);
OrderStatus stringToStatus(const string& value);


