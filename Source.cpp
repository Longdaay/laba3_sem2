#include <iostream>
#include <sstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include <fstream>

using namespace std;
string input_str;
string check_str;
map <string, string> variables;

int m_count = 0;

void SetColor(int text, int bg) { //Функция смены цвета, взятая из Интернета

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

////////////////// PROTOTYPES /////////////////////

void menu();
void menu_choice();

////////////////// PROTOTYPES /////////////////////

////////////////// STACK /////////////////////

typedef struct Node {
	string value;
	struct Node* next;
} Node;

void push(Node** head, string data) {
	void *ex_raw = malloc(sizeof(Node));
	Node* tmp = new(ex_raw) Node;
	tmp->value = data;
	tmp->next = (*head);
	(*head) = tmp;
}


string pop(Node** head) {
	Node* prev = NULL;
	string val;
	if (head == NULL) {
		exit(-1);
	}
	prev = (*head);
	val = prev->value;
	(*head) = (*head)->next;
	free(prev);
	return val;
}

Node* getLast(Node* head) {
	if (head == NULL) {
		return NULL;
	}
	while (head->next) {
		head = head->next;
	}
	return head;
}

void pushBack(Node* head, string value) {
	Node* last = getLast(head);
	void* ex_raw = malloc(sizeof(Node));
	Node* tmp = new(ex_raw) Node;
	tmp->value = value;
	tmp->next = NULL;
	last->next = tmp;
}

Node* getLastButOne(Node* head) {
	if (head == NULL) {
		exit(-2);
	}
	if (head->next == NULL) {
		return NULL;
	}
	while (head->next->next) {
		head = head->next;
	}
	return head;
}

string popBack(Node** head) {
	string value;
	Node* lastbn = NULL;
	//Получили NULL
	if (!head) {
		exit(-1);
	}
	//Список пуст
	if (!(*head)) {
		exit(-1);
	}
	lastbn = getLastButOne(*head);
	value = lastbn->next->value;

	//Если в списке один элемент
	if (lastbn == NULL) {
		free(*head);
		*head = NULL;
	}
	else {
		free(lastbn->next);
		lastbn->next = NULL;
	}
	return value;
}

void deleteList(Node** head) {
	Node* prev = NULL;
	while ((*head)->next) {
		prev = (*head);
		(*head) = (*head)->next;
		free(prev);
	}
	free(*head);
}


void printStack(const Node* head) {
	while (head) {
		cout << head->value << " ";
		head = head->next;
	}
	cout << endl;
}

int sizeStack(Node* head) {
	int count = 0;
	while (head) {
		count++;
		head = head->next;
	}
	return count;
}

Node* reverseStack(Node* outStack)
{
	Node* tmp = NULL;
	int size_stack = sizeStack(outStack);
	if (outStack) {
		push(&tmp, popBack(&outStack));
		size_stack--;
	}
	else
		exit(-1);
	while (outStack)
	{
		if (size_stack != 1)
			pushBack(tmp, popBack(&outStack));
		else
			pushBack(tmp, pop(&outStack));
		size_stack--;
	}
	return tmp;
}

void addStackElem(Node* &Stack , const string& token) {
	if (!Stack)
		push(&Stack, token);
	else
		pushBack(Stack, token);
}

////////////////// STACK /////////////////////

////////////////// QUEUE /////////////////////

////////////////// QUEUE /////////////////////

///////////////////////////// С видоса ///////////////////////////////

vector<string> Tokenizer(const string& text) {
	// <- 1 2 3 4 + 3 -2
	// -> [1,2,3,4,+,3,-2]

	vector<string> tokens{};
	istringstream input(text);

	for (string token; getline(input, token, ' '); /*nothing to do*/) {
		tokens.push_back(token);
	}

	return tokens;
}

bool IsNumber(const string& str) {
	if (str.length() > 1) {
		vector <string> tmp;
		string temp;
		for (int i = 0; i <= str.size() - 1; i++)
		{
			temp = temp + str[i];
			tmp.push_back(temp);
			temp.clear();
		}
		for (const auto& token : tmp)
		{
			if (token == "+" || token == "-") {}
			else {
				try {
					auto res = std::stod(token);  // we need only exception or no exception
				}
				catch (const std::invalid_argument& ex) {
					return FALSE;
				}
			}
		}
		return TRUE;
	}
	try {
		auto res = std::stod(str);  // we need only exception or no exception
	}
	catch (const std::invalid_argument& ex) {
		return FALSE;
	}
	return TRUE;
}


///////////////////////////// С видоса ///////////////////////////////

bool checkWeight(const string& str, Node* head, const map <string, int> & map_) {
	if (map_.find(str)->second > map_.find((getLast(head)->value))->second)
		return TRUE;
	else
		return FALSE;
}

bool IsOperation(const string& str, map <string, int> map_) {
	auto res = map_.find(str);
	if (res == map_.end())
		return FALSE; 
	else
		return TRUE;
}

bool IsLiteral(const string& str) {
	auto res = variables.find(str);
	if (res == variables.end())
		return FALSE;
	else
		return TRUE;
}

double Action(double & value1, double &value2, string& operation) {
	switch (operation[0]) {
	case '+':
		return value1 + value2;
		break;
	case '-':
		return value1 - value2;
		break;
	case '*':
		return value1 * value2;
		break;
	case '/':
		return value1 / value2;
		break;
	default:
		break;
	}
}

/////////////////// OPZ ///////////////////

void reverseTokens(vector <string>& str_tokens) {
	vector<string> temp = str_tokens;
	for (int i = 0; i <= str_tokens.size() - 1; i++)
	{
		if (temp[str_tokens.size() - 1 - i] == "(") {
			str_tokens[i] = ")";
			continue;
		}
		if (temp[str_tokens.size() - 1 - i] == ")") {
			str_tokens[i] = "(";
			continue;
		}
		str_tokens[i] = temp[str_tokens.size() - 1 - i];
	}
}

void polishNotation(string& inpt_str, vector<string>& str_tokens, Node*&outStack, Node* &opStack, map<string, int> & weight_op) {
	string current_str;

	for (const auto& token : str_tokens) {
		if (IsNumber(token)) {
			addStackElem(outStack, token);
			continue;
		}
		if (IsOperation(token, weight_op)) {
			if (token == "(") {
				addStackElem(opStack, token);
				continue;
			}
			if (token == ")") {
				while (opStack && current_str != "(") {
					if (sizeStack(opStack) >= 2)
						current_str = popBack(&opStack);
					else
						current_str = pop(&opStack);

					addStackElem(outStack, current_str);
					current_str = getLast(opStack)->value;
				}
				current_str = "";
				if (!opStack) {
					cerr << "[ERROR] character ( is not found. Try another one" << endl;
					exit(-1);  // TODO: throw exception
				}
				if (sizeStack(opStack) >= 2)
					popBack(&opStack);
				else
					pop(&opStack);
				continue;
			}
			if (!opStack) {
				push(&opStack, token);
				continue;
			}
			if (checkWeight(token, opStack, weight_op))
				// Если вес текущего больше или равен предыдущему
				pushBack(opStack, token);
			else {
				// Если вес текущего меньше предыдущего
				if (sizeStack(opStack) >= 2)// если стэк операций больше двух
					pushBack(outStack, popBack(&opStack));
				else
					pushBack(outStack, pop(&opStack));

				addStackElem(opStack, token);
			}
			continue;
		}

		if (IsLiteral(token)) {
			addStackElem(outStack, variables.find(token)->second);
			continue;
		}

		cerr << "[ERROR] unexpected token " << token << endl;
		exit(-1);  // TODO: throw exception
	}

	while (opStack) {
		if (sizeStack(opStack) >= 2)// если стэк операций больше двух
			pushBack(outStack, popBack(&opStack));
		else
			pushBack(outStack, pop(&opStack));
	}
}

void normalPolishNotation(string& inpt_str, Node* &outStack, Node* &opStack, map<string, int>& weight_op) {
	vector<string> str_tokens;
	
	cout << "Строка - \"" << inpt_str << "\"" << endl;
	str_tokens = Tokenizer(inpt_str);
	reverseTokens(str_tokens);

	polishNotation(inpt_str, str_tokens, outStack, opStack, weight_op);
	outStack = reverseStack(outStack);

	cout << endl << "Normal Стек выхода = ";
	printStack(outStack);

}

void reversePolishNotation(string& inpt_str, Node*& outStack, Node*& opStack, map <string, int>& weight_op) {
	vector<string> str_tokens;

	cout << "Строка - \"" << inpt_str << "\"" << endl;
	str_tokens = Tokenizer(inpt_str);

	polishNotation(inpt_str, str_tokens, outStack, opStack, weight_op);

	cout << endl << "Reverse Стек выхода = ";
	printStack(outStack);
}

/////////////////// OPZ ///////////////////

double Calc_PN(Node* &outStack, Node* &opStack, map<string, int> weight_op, bool fl_notation) { // fl_notation : FALSE - Reverse notation; TRUE - normal notation
	vector<double> res;
	double value1 = 0;
	double value2 = 0;
	string operation;

	if (fl_notation) // if normal polish notation
		outStack = reverseStack(outStack);
	while (outStack) {
		if (IsNumber(outStack->value)) {
			res.push_back(stod(pop(&outStack)));
			continue;
		}
		if (IsOperation(outStack->value, weight_op)) {
			if (res.size() >= 2) {

				operation = pop(&outStack);
				if (fl_notation) { // if normal polish notation
					value1 = res[res.size() - 1];
					res.pop_back();
					value2 = res[res.size() - 1];
					res.pop_back();
					res.push_back(Action(value1, value2, operation));
				}
				else {
					value2 = res[res.size() - 1];
					res.pop_back();
					value1 = res[res.size() - 1];
					res.pop_back();
					res.push_back(Action(value1, value2, operation));
				}
				continue;
			}
			cerr << "[ERROR] Выражение записано некорректно" << endl;
			exit(-1);  // TODO: throw exception
		}
	}
	return res[0];
}

void check_PN(string& str) {
	Node* outStack = NULL;
}

/////////////////// INPUT EXPRESSION /////////////////////

void fill_str_self(string& str) { 

	cout << "Введите строку выражения (Каждый символ отделяйте пробелом): ";
	getline(cin, str);
	cout << "Полученная строка - " << str << endl;
	cout << "Выражение записано. Нажмите любую кнопку для продолжения.";

	_getch();
}

void fill_str_via_file(string& str, bool fl_file) { // TODO : Double values 10.5
	str.clear();
	string temp;
	ifstream fin;
	if (fl_file)
		fin.open("inputStr.txt");
	else
		fin.open("checkPN.txt");
	if (fin.is_open()) {
		while (!fin.eof()) {
			getline(fin, temp);
			str += temp;
		}
	}
	else {
		cout << "Файл не удалось открыть. Закройте программу и создайте файл inputStr.txt в корневой папке программы и попробуйте снова." << endl;
		cout << "Нажмите любую клавишу, чтобы закрыть программу" << endl;
		_getch();
		exit(3);
	}

	cout << endl << "Полученная строка - " << str << endl;
	cout << "Выражение записано. Нажмите любую кнопку для продолжения.";
	fin.close();
	_getch();
}

void check_literals(string& str) {
	int i = 0;
	string value;
	string temp_str;
	for (int i = 0; i <= str.size() - 1; i++) {
		if (isalpha(str[i])) {
			if (str[i - 1] == ' ' and str[i + 1] == ' ') {
				cout << endl << "Введите значение переменной " << str[i] << " : ";
				getline(cin, value);
				temp_str += str[i]; // map не принимает один символ за стринг, поэтому добавляю к временному string
				variables.insert(pair<string, string>(temp_str, value));
				temp_str.clear();
				continue;
			}
			cerr << "[ERROR] unexpected token " << str[i] << endl;
			exit(-1);  // TODO: throw exception
		}
	}
}

/////////////////// INPUT EXPRESSION /////////////////////

////////////////// MENU ////////////////////////

void conf_val() {
	switch (m_count)
	{
	case 0:
		
		break;
	case 1:

		break;
	case 2:

		break;
	case 3:

		break;
	case 4:
		SetColor(0, 0);
		exit(0);
		break;
	default:
		break;
	}
}

void menu_choice() { // в зависимости от стрелок изменяем меню 
	int k1;
	k1 = _getch(); // получаем символ стрелки без вывода знака
	if (k1 == 0xE0) { // если стрелки
		switch (k1) {
		case 0x48: // стрелка вверх
			m_count--;
			if (m_count < 0) m_count = 0;
			menu();
			break;

		case 0x50: // стрелка вниз
			m_count++;
			if (m_count > 4) m_count = 4;
			menu();
			break;
		case 0xD: // подтвердить
			conf_val();
			break;
		default:
			menu_choice();
		}
	}
	switch (k1) {
	case 0x48: // стрелка вверх
		m_count--;
		if (m_count < 0) m_count = 0;
		menu();
		break;

	case 0x50: // стрелка вниз
		m_count++;
		if (m_count > 4) m_count = 4;
		menu();
		break;
	case 0xD: // подтвердить
		conf_val();
		break;
	default:
		menu_choice();
	}
}

void menu() { // меню
	system("cls");

	if (m_count == 0) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		SetColor(5, 0);
		cout << "Вычислить выражение с помощью ПН" << endl;
		SetColor(7, 0);
		cout << "Проверить выражение с помощью ПН" << endl;
		cout << "Создание теста" << endl;
		cout << "Задание по варианту" << endl;
		cout << "Выход" << endl;
		menu_choice();
	}
	if (m_count == 1) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Вычислить выражение с помощью ПН" << endl;
		SetColor(5, 0);
		cout << "Проверить выражение с помощью ПН" << endl;
		SetColor(7, 0);
		cout << "Создание теста" << endl;
		cout << "Задание по варианту" << endl;
		cout << "Выход" << endl;
		menu_choice();
	}
	if (m_count == 2) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Вычислить выражение с помощью ПН" << endl;
		cout << "Проверить выражение с помощью ПН" << endl;
		SetColor(5, 0);
		cout << "Создание теста" << endl;
		SetColor(7, 0);
		cout << "Задание по варианту" << endl;
		cout << "Выход" << endl;
		menu_choice();
	}
	if (m_count == 3) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Вычислить выражение с помощью ПН" << endl;
		cout << "Проверить выражение с помощью ПН" << endl;
		cout << "Создание теста" << endl;
		SetColor(5, 0);
		cout << "Задание по варианту" << endl;
		SetColor(7, 0);
		cout << "Выход" << endl;
		menu_choice();
	}
	if (m_count == 4) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Вычислить выражение с помощью ПН" << endl;
		cout << "Проверить выражение с помощью ПН" << endl;
		cout << "Создание теста" << endl;
		cout << "Задание по варианту" << endl;
		SetColor(5, 0);
		cout << "Выход" << endl;
		SetColor(7, 0);
		menu_choice();
	}
}

////////////////// MENU ////////////////////////

int main() {
	setlocale(0, "");

	string inpt_str;
	Node* outStack = NULL;
	Node* opStack = NULL;
	Node* outStack2 = NULL;
	Node* opStack2 = NULL;

	map <string, int> weight_op = { {"+", 1 },
									{"-", 1 },
									{"*", 2 },
									{"/", 2 },
									{"(", 0 },
									{")", 0 } };
	//menu();
	fill_str_via_file(input_str, TRUE);
	check_literals(input_str);
	system("pause");
	//cout << "Введите выражение: ";
	inpt_str = input_str;
	//getline(cin, inpt_str);
	string inpt_str2 = inpt_str;
	normalPolishNotation(inpt_str, outStack, opStack, weight_op);
	
	reversePolishNotation(inpt_str2, outStack2, opStack2, weight_op);
	system("pause");
	cout << "Результат normal: " << Calc_PN(outStack, opStack, weight_op, TRUE) << endl;
	cout << "Результат reverse: " << Calc_PN(outStack2, opStack2, weight_op, FALSE) << endl;
	cout << endl << "done!" << endl;
	system("pause");


	return 0;
}
