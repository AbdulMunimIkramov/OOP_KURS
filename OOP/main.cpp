#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>

using namespace std;

class Date {
private:
    int year;
    int month;
    int day;

public:
    Date() : year(0), month(0), day(0) {}

    Date(int year, int month, int day) : year(year), month(month), day(day) {}

    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }
};

class Category {
private:
    string name;

public:
    Category(const string& name) : name(name) {}

    string getName() const { return name; }
};

class Expense {
private:
    double amount;
    Date date;
    Category category;
    string description;

public:
    Expense(double amount, const Date& date, const Category& category, const string& description)
        : amount(amount), date(date), category(category), description(description) {}

    double getAmount() const { return amount; }
    Date getDate() const { return date; }
    Category getCategory() const { return category; }
    string getDescription() const { return description; }
};

class Transaction {
private:
    double amount;
    Date date;
    Category category;

public:
    Transaction(double amount, const Date& date, const Category& category)
        : amount(amount), date(date), category(category) {}

    double getAmount() const { return amount; }
    Date getDate() const { return date; }
    Category getCategory() const { return category; }
};

class Budget {
private:
    double balance;
    vector<Transaction> transactions;

    static Category foodCategory;
    static Category transportCategory;
    static Category housingCategory;
    static Category entertainmentCategory;

public:
    Budget(double initialBalance) : balance(initialBalance) {}

    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
        balance += transaction.getAmount();
    }

    double getBalance() const { return balance; }

    void setTransactions(const vector<Transaction>& transactions) {
        this->transactions = transactions;
    }

    bool addExpense(const Expense& expense) {
        if (balance < expense.getAmount()) {
            cout << "Ошибка: Недостаточно средств на счете!" << endl;
            return false;
        }

        Transaction expenseTransaction(-expense.getAmount(), expense.getDate(), expense.getCategory());
        addTransaction(expenseTransaction);
        return true;
    }

    void addIncome(double amount, const Date& date) {
        Transaction incomeTransaction(amount, date, Category("Доход"));
        addTransaction(incomeTransaction);
    }

    vector<Transaction> getTransactionHistory() const {
        return transactions;
    }
};

Category Budget::foodCategory("Продукты");
Category Budget::transportCategory("Транспорт");
Category Budget::housingCategory("Жилье");
Category Budget::entertainmentCategory("Развлечения");

void displayMenu() {
    cout << "===== Система Финансового Управления =====" << endl;
    cout << "1. Добавить Расход" << endl;
    cout << "2. Добавить Доход" << endl;
    cout << "3. Просмотреть Текущий Баланс" << endl;
    cout << "4. Просмотреть Историю Транзакций" << endl;
    cout << "5. Добавить Долг" << endl;
    cout << "6. Просмотреть Информацию о Долгах" << endl;
    cout << "7. Сохранить отчет в файл" << endl;
    cout << "8. Выйти из программы" << endl;
    cout << "===========================================" << endl;
}

void displayTransactionHistory(const vector<Transaction>& history) {
    cout << "===== История Транзакций =====" << endl;
    for (const auto& transaction : history) {
        cout << "Дата: " << transaction.getDate().getYear() << "-" << transaction.getDate().getMonth() << "-" << transaction.getDate().getDay();
        cout << ", Сумма: " << transaction.getAmount();
        cout << ", Категория: " << transaction.getCategory().getName() << endl;
    }
    cout << "=============================" << endl;
}

Date getCurrentDate() {
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    int year = currentTime->tm_year + 1900;
    int month = currentTime->tm_mon + 1;
    int day = currentTime->tm_mday;
    return Date(year, month, day);
}

Date inputDueDate() {
    Date currentDate = getCurrentDate();
    Date dueDate;
    do {
        int year, month, day;
        cout << "Введите дату погашения долга (год, месяц, день): ";
        cin >> year >> month >> day;
        dueDate = Date(year, month, day);
        if (dueDate.getYear() < currentDate.getYear() ||
            (dueDate.getYear() == currentDate.getYear() && dueDate.getMonth() < currentDate.getMonth()) ||
            (dueDate.getYear() == currentDate.getYear() && dueDate.getMonth() == currentDate.getMonth() && dueDate.getDay() < currentDate.getDay())) {
            cout << "Ошибка: Вы ввели дату, которая уже прошла. Пожалуйста, введите корректную дату." << endl;
        }
    } while (dueDate.getYear() < currentDate.getYear() ||
             (dueDate.getYear() == currentDate.getYear() && dueDate.getMonth() < currentDate.getMonth()) ||
             (dueDate.getYear() == currentDate.getYear() && dueDate.getMonth() == currentDate.getMonth() && dueDate.getDay() < currentDate.getDay()));
    return dueDate;
}

template<typename T>
T inputNumber(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            break;
        } else {
            cout << "Ошибка: Некорректный ввод. Пожалуйста, введите число." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return value;
}

class DebtManager {
private:
    struct Debt {
        double amount;
        Date dueDate;
        Debt(double amount, const Date& dueDate) : amount(amount), dueDate(dueDate) {}
    };

    vector<Debt> debts;

public:
    void addDebt(double amount, const Date& dueDate) {
        Debt newDebt(amount, dueDate);
        debts.push_back(newDebt);
    }

    vector<Debt> getDebts() const {
        return debts;
    }
};

class ReportManager {
private:
    string filename;

public:
    ReportManager(const string& filename) : filename(filename) {}

    void saveData(const vector<Transaction>& transactions) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& transaction : transactions) {
                file << transaction.getAmount() << " "
                     << transaction.getDate().getYear() << " "
                     << transaction.getDate().getMonth() << " "
                     << transaction.getDate().getDay() << " "
                     << transaction.getCategory().getName() << endl;
            }
            file.close();
            cout << "Данные успешно сохранены в файле " << filename << endl;
        } else {
            cout << "Ошибка при открытии файла для сохранения данных." << endl;
        }
    }

    vector<Transaction> loadData() {
        vector<Transaction> transactions;
        ifstream file(filename);
        if (file.is_open()) {
            double amount;
            int year, month, day;
            string categoryName;
            while (file >> amount >> year >> month >> day >> categoryName) {
                Date date(year, month, day);
                Category category(categoryName);
                Transaction transaction(amount, date, category);
                transactions.push_back(transaction);
            }
            file.close();
            cout << "Данные успешно загружены из файла " << filename << endl;
        } else {
            cout << "Ошибка при открытии файла для загрузки данных." << endl;
        }
        return transactions;
    }
};

int main() {
    Date currentDate(2024, 5, 24);
    Budget budget(1000.0);
    DebtManager debtManager;
    ReportManager reportManager("transactions.txt");

    vector<Transaction> loadedTransactions = reportManager.loadData();
    for (const auto& transaction : loadedTransactions) {
        budget.addTransaction(transaction);
    }

    int choice;
    do {
        displayMenu();
        cout << "Введите ваш выбор: ";
        choice = inputNumber<int>("");

        switch (choice) {
            case 1: {
                double amount = inputNumber<double>("Введите сумму расхода: ");
                cout << "Выберите категорию расхода:" << endl;
                cout << "1. Продукты" << endl;
                cout << "2. Транспорт" << endl;
                cout << "3. Жилье" << endl;
                cout << "4. Развлечения" << endl;
                int categoryChoice = inputNumber<int>("Введите номер категории: ");
                Category selectedCategory("None");
                switch (categoryChoice) {
                    case 1:
                        selectedCategory = Category("Продукты");
                        break;
                    case 2:
                        selectedCategory = Category("Транспорт");
                        break;
                    case 3:
                        selectedCategory = Category("Жилье");
                        break;
                    case 4:
                        selectedCategory = Category("Развлечения");
                        break;
                    default:
                        cout << "Некорректный выбор категории. Расход не добавлен." << endl;
                        continue;
                }
                if (!budget.addExpense(Expense(amount, currentDate, selectedCategory, "Расход")))
                    cout << "Операция отменена." << endl;
                break;
            }
            case 2: {
                double amount = inputNumber<double>("Введите сумму дохода: ");
                budget.addIncome(amount, currentDate);
                break;
            }
            case 3:
                cout << "Текущий баланс: " << budget.getBalance() << endl;
                break;
            case 4:
                budget.setTransactions(reportManager.loadData());
                displayTransactionHistory(budget.getTransactionHistory());
                break;
            case 5: {
                double amount = inputNumber<double>("Введите сумму долга: ");
                Date dueDate = inputDueDate();
                debtManager.addDebt(amount, dueDate);
                cout << "Долг успешно добавлен." << endl;
                break;
            }
            case 6: {
                auto debts = debtManager.getDebts();
                cout << "===== Информация о Долгах =====" << endl;
                for (const auto& debt : debts) {
                    cout << "Сумма долга: " << debt.amount << ", Дата погашения: "
                         << debt.dueDate.getYear() << "-" << debt.dueDate.getMonth() << "-" << debt.dueDate.getDay() << endl;
                }
                cout << "=============================" << endl;
                break;
            }
            case 7:
                reportManager.saveData(budget.getTransactionHistory());
                cout << "Данные сохранены. Завершение программы. До свидания!" << endl;
                break;
            default:
                cout << "Неверный выбор. Пожалуйста, введите правильную опцию." << endl;
        }
    } while (choice != 8);

    return 0;
}


