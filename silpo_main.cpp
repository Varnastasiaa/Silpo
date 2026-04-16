#include "SilpoOrder.h"
using namespace std;

// Функція друкує меню команд.
void printMenu() {
    cout << "\n===== МЕНЮ =====\n";
    cout << "1 - Додати товар\n";
    cout << "2 - Видалити товар\n";
    cout << "3 - Змінити кількість товару\n";
    cout << "4 - Змінити статус\n";
    cout << "5 - Вибрати дату доставки\n";
    cout << "6 - Показати замовлення\n";
    cout << "7 - Зберегти у файл\n";
    cout << "8 - Зчитати з файлу\n";
    cout << "0 - Вихід\n";
    cout << "Ваш вибір: ";
}

// Функція читає статус від користувача.
OrderStatus readStatusFromUser() {
    int s = 0;
    cout << "\nОберіть статус:\n";
    cout << "1 - виконано\n";
    cout << "2 - скасовано\n";
    cout << "3 - оплачено\n";
    cout << "4 - неоплачено\n";
    cout << "Ваш вибір: ";
    cin >> s;

    switch (s) {
        case 1: return OrderStatus::Completed;
        case 2: return OrderStatus::Canceled;
        case 3: return OrderStatus::Paid;
        default: return OrderStatus::Unpaid; // default - за замовчуванням (неоплачено)
    }
}

int main() {
    SilpoOrder order;
    int choice = -1;

    while (choice != 0) {
        printMenu();
        cin >> choice;

        if (choice == 1) {
            string name;
            int quantity;
            double price;

            cout << "Назва товару: ";
            cin >> name;
            cout << "Кількість: ";
            cin >> quantity;
            cout << "Ціна за 1 шт: ";
            cin >> price;

            order.addProduct(name, quantity, price);
        } else if (choice == 2) {
            string name;
            cout << "Назва товару для видалення: ";
            cin >> name;
            order.removeProduct(name);
        } else if (choice == 3) {
            string name;
            int newQuantity;

            cout << "Назва товару: ";
            cin >> name;
            cout << "Нова кількість: ";
            cin >> newQuantity;

            order.changeProductQuantity(name, newQuantity);
        } else if (choice == 4) {
            order.setStatus(readStatusFromUser());
        } else if (choice == 5) {
            string date;
            cout << "Введіть дату доставки (формат YYYY-MM-DD): ";
            cin >> date;
            order.setDeliveryDate(date);
        } else if (choice == 6) {
            order.printOrder();
        } else if (choice == 7) {
            string fileName;
            cout << "Назва файлу для збереження: ";
            cin >> fileName;

            if (order.saveToFile(fileName)) {
                cout << "Успішно збережено у файл.\n";
            }
        } else if (choice == 8) {
            string fileName;
            cout << "Назва файлу для зчитування: ";
            cin >> fileName;

            if (order.loadFromFile(fileName)) {
                cout << "Успішно зчитано з файлу.\n";
            }
        } else if (choice == 0) {
            cout << "Програму завершено.\n";
        } else {
            cout << "Невірний пункт меню.\n";
        }
    }

    return 0;
}
