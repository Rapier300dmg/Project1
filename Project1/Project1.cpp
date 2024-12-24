#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

//1 user класс для хранения информации о пользователях
class User {
private:
    string login;
    string password;
    string fullName;
    string address;
    string phone;

public:
    User(const string& login, const string& password, const string& fullName, const string& address, const string& phone)
        : login(login), password(password), fullName(fullName), address(address), phone(phone) {
    }

    const string& getLogin() const { return login; }
    const string& getPassword() const { return password; }
    const string& getFullName() const { return fullName; }

    void printUserInfo() const {
        cout << "ФИО: " << fullName << "\nАдрес: " << address << "\nТелефон: " << phone << "\n";
    }
};

//2 usermanager класс было создана для управления пользователями
class UserManager {
private:
    vector<User> users;

public:
    bool registerUser(const string& login, const string& password, const string& fullName, const string& address, const string& phone) {
        for (const auto& user : users) {
            if (user.getLogin() == login) {
                cout << "Ошибка: Логин \"" << login << "\" уже используется.\n";
                return false;
            }
        }
        users.emplace_back(login, password, fullName, address, phone);
        cout << "Пользователь \"" << login << "\" успешно зарегистрирован.\n";
        return true;
    }

    const User* authenticate(const string& login, const string& password) const {
        for (const auto& user : users) {
            if (user.getLogin() == login && user.getPassword() == password) {
                return &user;
            }
        }
        return nullptr;
    }
};

//3 Класс Test для тестов и их управления
class Test {
private:
    string category;
    string name;
    vector<pair<string, bool>> questions; // вопрос + правильный ли ответ

public:
    Test(const string& category, const string& name) : category(category), name(name) {}

    void addQuestion(const string& question, bool isCorrect) {
        questions.emplace_back(question, isCorrect);
    }

    int runTest() const {
        int correctAnswers = 0;
        cout << "--- Тест \"" << name << "\" в категории \"" << category << "\" ---\n";
        for (size_t i = 0; i < questions.size(); ++i) {
            cout << i + 1 << ". " << questions[i].first << " (1 - Да, 0 - Нет): ";
            int answer;
            cin >> answer;
            if ((answer == 1 && questions[i].second) || (answer == 0 && !questions[i].second)) {
                correctAnswers++;
            }
        }
        return correctAnswers;
    }

    size_t getTotalQuestions() const { return questions.size(); }

    const string& getName() const { return name; }
    const string& getCategory() const { return category; }
};

//4 также как в user и usermanager классах, было создана класс управления тестами для класса test
class TestManager {
private:
    vector<Test> tests;

public:
    void addTest(const Test& test) {
        tests.push_back(test);
    }

    void listTests() const {
        cout << "--- Список доступных тестов ---\n";
        for (const auto& test : tests) {
            cout << "Категория: " << test.getCategory() << ", Тест: " << test.getName() << "\n";
        }
    }

    const Test* findTest(const string& name) const {
        for (const auto& test : tests) {
            if (test.getName() == name) {
                return &test;
            }
        }
        return nullptr;
    }
};

// основной код главные команды

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    UserManager userManager;
    TestManager testManager;

    // наши добавленные тесты
    Test mathTest("Математика", "Дискретная математика");
    mathTest.addQuestion("Четное ли число 4?", true);
    mathTest.addQuestion("Простое ли число 9?", false);

    Test physicsTest("Физика", "Механика");
    physicsTest.addQuestion("Закон Ньютона применим к телам в состоянии покоя?", true);
    physicsTest.addQuestion("Ускорение свободного падения равно 15 м/с^2?", false);
    testManager.addTest(mathTest);
    testManager.addTest(physicsTest);

    // регистрация наших пользователей
    userManager.registerUser("ivanov", "1234", "Иван Иванов", "г. Москва, ул. Ленина, д. 1", "+7(900)123-45-67");

    string login, password;
    cout << "Введите логин: ";
    cin >> login;
    cout << "Введите пароль: ";
    cin >> password;

    const User* user = userManager.authenticate(login, password);
    if (!user) {
        cout << "Ошибка: Неверный логин или пароль.\n";
        return 1;
    }

    cout << "Добро пожаловать, " << user->getFullName() << "!\n";

    // Запуск тестов
    testManager.listTests();
    cout << "Введите название теста для прохождения: ";
    string testName;
    cin.ignore();
    getline(cin, testName);

    const Test* test = testManager.findTest(testName);
    if (test) {
        int correctAnswers = test->runTest();
        int totalQuestions = test->getTotalQuestions();
        double percentage = (double(correctAnswers) / totalQuestions) * 100;

        cout << "Результаты теста:\n";
        cout << "Правильных ответов: " << correctAnswers << " из " << totalQuestions << "\n";
        cout << "Процент правильных ответов: " << fixed << setprecision(2) << percentage << "%\n";
    }
    else {
        cout << "Ошибка: Тест \"" << testName << "\" не найден.\n";
    }

    return 0;