//// Лабораторная работа №3 - Польская нотация
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;
string input_str;
string check_str;
string calc_notation_file = "notation.txt";
string input_file = "inputStr.txt";
string check_file_pn = "checkPN.txt";
string check_file_exp = "checkexp.txt";
bool fl_test = FALSE;
bool back = FALSE;

map <string, string> variables;
map <string, int> weight_op = { {"+", 1 },
								{"-", 1 },
								{"*", 2 },
								{"/", 2 },
								{"(", 0 },
								{")", 0 } };

map <string, int> check_op =  { {"+", 1 },
								{"|", 1 },
								{"*", 2 },
								{"/", 2 } };

int m_count = 0;
int subm_conv_count = 0;
int subm_conv_pn_count = 0;
int subm_calc_pn_count = 0;
int subm_check_pn_count = 0;

void SetColor(int text, int bg) { //Функция смены цвета, взятая из Интернета

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

////////////////// PROTOTYPES /////////////////////
typedef struct Node;
bool getCalcNotation();
void menu(string& str, Node*& outStack, Node*& opStack);
void submenu_convert_exp(string& str, Node*& outStack, Node*& opStack);
void submenu_calc_pn(string& str, Node*& outStack, Node*& opStack);
void submenu_check_pn(string& str, Node*& outStack, Node*& opStack);
void menu_choice(string& str, Node*& outStack, Node*& opStack);
void submenu_convert_exp_choice(string& str, Node*& outStack, Node*& opStack);
void submenu_calc_pn_choice(string& str, Node*& outStack, Node*& opStack);
void submenu_check_pn_choice(string& str, Node*& outStack, Node*& opStack);
void check_literals(string& str, Node*& outStack, Node*& opStack);
void fill_str_self(string& str, Node*& outStack, Node*& opStack);
void fill_str_via_file(string& str, Node*& outStack, Node*& opStack, string& filename);
void logFile(ofstream& fout, string str, const string& token, Node* Stack, string object, string Action);

////////////////// PROTOTYPES /////////////////////

double str2doub(std::string str)
{
	double d;
	std::stringstream iss(str);
	iss >> d;
	return d;
}

int checkdigit() {
	while (true) {
		int value;
		cin >> value;
		if (cin.fail()) {
			cin.clear(); 
			cin.ignore(32767, '\n');
			cout << "Недопустимое заданное число. Введите число правильно" << '\n';
		}
		else {
			if (value >= 0) {
				cin.ignore(32767, '\n');
				return value;
			}
			else {
				cin.clear();
				cin.ignore(32767, '\n');
				cout << "Недопустимое заданное число. Введите число правильно" << '\n';
			}
		}
	}
}

typedef struct exercise {
	string task;
	string notation_answer;
	double answer;
};

typedef struct Test {
	vector<exercise> Task;
};
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


void printStack(const Node* head, ofstream& fout, bool console) {
	while (head) {
		if(!console)
			fout << head->value << " ";
		else {
			cout << head->value << " ";
			fout << head->value << " ";
		}
		head = head->next;
	}
	if(!console)
		fout << endl;
	else {
		cout << endl;
		fout << endl;
	}
	return;
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
		if (size_stack >= 2)
			push(&tmp, popBack(&outStack));
		else
			push(&tmp, pop(&outStack));
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

void printQueue(const vector<int>& Queue, int my_index) {
	system("cls");
	cout << "ОЧЕРЕДЬ" << endl;
	cout << "Ваше текущее место в очереди: " << Queue.size() - my_index << endl;
	cout << "Ваш номерок: " << Queue[my_index] << endl;
	for (int i = 0; i <= Queue.size() - 1; i++) {
		if (my_index == i) {
			
			SetColor(3, 2); cout << "| " << setw(3) << right << Queue[i] << setw(3) << right << " |" << endl;
			SetColor(7, 0);
		}
		else 
			cout << "| " << setw(3) << right << Queue[i] << setw(3) << right << " |" << endl;
	}
	
}

void fill_queue(vector<int>& Queue, int& value) {
	value = rand() % 50;
	for (int i = Queue.size() - 1; i >= 0; i--) {
		Queue[i] = value++;
	}
}

void step_forward(vector<int>& Queue, int& value) {
	Queue.pop_back();
	Queue.insert(Queue.begin(), value++);
}

void queue() {
	system("cls");
	int count = rand() % 20;
	int value;
	vector<int> Queue;
	while (count == 0) {
		count = rand() % 20;
	}
	Queue.resize(count);
	fill_queue(Queue, value);

	cout << setw(80) << right << "Индивидуальное задание - электронная очередь в магазине." << endl;
	cout << setw(110) << "Пользователь после нажатия любой клавиши получает номерок и визульно постепенно спускается вниз по очереди" << endl;
	cout << setw(73) << "Магазин большой и очередь постоянно пополняется" << endl;
	SetColor(2, 0);
	cout << setw(70) << "Все держат социальную дистанцию и в масках" << endl;
	SetColor(7, 0);
	SetColor(6, 0);
	cout << setw(72) << "Нажмите любую клавишу, чтобы получить номерок" << endl;
	SetColor(7, 0);
	_getch();

	for (int i = 0; i <= Queue.size() - 1; i++) {
		step_forward(Queue, value);
		printQueue(Queue, i);
		Sleep(3000);
	}
	cout << "Теперь вы можете оплатить покупку!" << endl;
	system("pause");
}

////////////////// QUEUE /////////////////////

/////////////////// OPZ ///////////////////

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
	try {
		auto res = std::stod(str);  // we need only exception or no exception
	}
	catch (const std::invalid_argument& ex) {
		return FALSE;
	}
	return TRUE;
}

bool checkWeight(const string& str, Node* head, const map <string, int> & map_) {
	if (map_.find(str)->second > map_.find((getLast(head)->value))->second)
		return TRUE;
	else
		return FALSE;
}

bool checkWeight_vec(const string& token, vector<string>& opArr, const map <string, int>& map_) {
	if (map_.find(token)->second > map_.find(opArr[opArr.size() - 1])->second)
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

void polishNotation(string& inpt_str, vector<string>& str_tokens, Node*&outStack, Node* &opStack, ofstream& fout) {
	string current_str;
	back = FALSE;

	for (const auto& token : str_tokens) {
		logFile(fout, inpt_str, token, outStack, "token", "curr");
		if (IsNumber(token)) {
			addStackElem(outStack, token);
			logFile(fout, inpt_str, token, outStack, "token", "add");
			logFile(fout, inpt_str, token, outStack, "stack", "curr");
			continue;
		}
		if (IsOperation(token, weight_op)) {
			if (token == "(") {
				addStackElem(opStack, token);
				logFile(fout, inpt_str, token, opStack, "token", "add");
				logFile(fout, inpt_str, token, opStack, "stack", "curr");
				continue;
			}
			if (token == ")") {
				while (opStack && current_str != "(") {
					if (sizeStack(opStack) >= 2)
						current_str = popBack(&opStack);
					else
						current_str = pop(&opStack);
					logFile(fout, inpt_str, current_str, opStack, "stack", "del");
					addStackElem(outStack, current_str);
					logFile(fout, inpt_str, current_str, outStack, "token", "add");
					logFile(fout, inpt_str, current_str, outStack, "stack", "curr");
					current_str = getLast(opStack)->value;
				}
				current_str.clear();
				if (!opStack) {
					cerr << "[ERROR] character ( is not found. Try another one" << endl;
					fout << "[ERROR] character ( is not found. Try another one" << endl;
					back = TRUE;
					_getch();
					return;
				}
				if (sizeStack(opStack) >= 2)
					popBack(&opStack);
				else
					pop(&opStack);
				logFile(fout, inpt_str, "(", opStack, "stack", "del");
				continue;
			}
			if (!opStack) {
				push(&opStack, token);
				logFile(fout, inpt_str, token, opStack, "token", "add");
				logFile(fout, inpt_str, token, opStack, "stack", "curr");
				continue;
			}
			if (checkWeight(token, opStack, weight_op)) {
				// Если вес текущего больше или равен предыдущему
				pushBack(opStack, token);
				logFile(fout, inpt_str, token, opStack, "token", "add");
				logFile(fout, inpt_str, token, opStack, "stack", "curr");
			}
			else {
				// Если вес текущего меньше предыдущего
				logFile(fout, inpt_str, getLast(opStack)->value, opStack, "stack", "del");
				if (sizeStack(opStack) >= 2)// если стэк операций больше двух
					pushBack(outStack, popBack(&opStack));
				else
					pushBack(outStack, pop(&opStack));

				addStackElem(opStack, token);
				logFile(fout, inpt_str, token, opStack, "token", "add");
				logFile(fout, inpt_str, token, opStack, "stack", "curr");
			}
			continue;
		}

		if (IsLiteral(token)) {
			addStackElem(outStack, variables.find(token)->second);
			logFile(fout, inpt_str, variables.find(token)->second, outStack, "token", "add");
			logFile(fout, inpt_str, variables.find(token)->second, outStack, "stack", "curr");
			continue;
		}

		cerr << "[ERROR] unexpected token " << token << endl;
		fout << "[ERROR] unexpected token " << token << endl;
		back = TRUE;
		_getch();
		return;
	}
	fout << "Строка пройдена." << endl;
	fout << "Стек выхода = "; printStack(outStack, fout, FALSE); // FALSE - file output
	fout << "Стек переменных = "; printStack(opStack, fout, FALSE); // FALSE - file output
	fout << "Очищаем стек переменных, заполняя стек выхода" << endl;
	while (opStack) {
		if (sizeStack(opStack) >= 2)// если стэк операций больше двух
			pushBack(outStack, popBack(&opStack));
		else
			pushBack(outStack, pop(&opStack));
	}
	fout << "Стек выхода = "; printStack(outStack, fout, FALSE); // FALSE - file output
	fout << "Стек переменных = "; printStack(opStack, fout, FALSE); // FALSE - file output
	fout << endl;
}

void polishNotation_vec(string& inpt_str, vector<string>& str_tokens, vector<string>& outArr, vector<string>& opArr) {
	string current_str;
	back = FALSE;

	for (const auto& token : str_tokens) {
		if (IsNumber(token)) {
			outArr.push_back(token);
			continue;
		}
		if (IsOperation(token, weight_op)) {
			if (token == "(") {
				opArr.push_back(token);
				continue;
			}
			if (token == ")") {
				while (opArr.size() == 0 && current_str != "(") {
					current_str = opArr[opArr.size() - 1];
					opArr.pop_back();
					outArr.push_back(current_str);
					current_str = opArr[opArr.size() - 1];
				}
				current_str.clear();
				if (opArr.size() == 0) {
					cerr << "[ERROR] character ( is not found. Try another one" << endl;
					_getch();
					back = TRUE;
					return;
				}
				opArr.pop_back();
				continue;
			}
			if (opArr.size() == 0) {
				opArr.push_back(token);
				continue;
			}
			if (checkWeight_vec(token, opArr, weight_op)) {
				// Если вес текущего больше или равен предыдущему
				opArr.push_back(token);
			}
			else {
				// Если вес текущего меньше предыдущего
				outArr.push_back(opArr[opArr.size() - 1]);
				opArr.pop_back();
				opArr.push_back(token);
			}
			continue;
		}

		if (IsLiteral(token)) {
			outArr.push_back(variables.find(token)->second);
			continue;
		}
		cerr << "[ERROR] unexpected token " << token << endl;
		_getch();
		back = TRUE;
		return;
	}
	while (opArr.size() != 0) {
		outArr.push_back(opArr[opArr.size() - 1]);
		opArr.pop_back();
	}
}

void normalPolishNotation(string& inpt_str, Node* &outStack, Node* &opStack) {
	outStack = NULL;
	opStack = NULL;
	vector<string> str_tokens;
	ofstream fout;
	if (fl_test)
		fout.open("test_solution.txt", ios::app);
	else
		fout.open("notation_solution.txt", ios::app);
	fout << endl << endl << "Преобразование выражения в нормальную польскую нотацию" << endl;
	fout << "Строка, над которой происходит преобразование - " << input_str << endl;
	
	if (!fl_test)
		cout << "Строка - \"" << inpt_str << "\"" << endl;
	str_tokens = Tokenizer(inpt_str);
	reverseTokens(str_tokens);

	polishNotation(inpt_str, str_tokens, outStack, opStack, fout);
	if (back)
		menu(inpt_str, outStack, opStack);
	outStack = reverseStack(outStack);
	if (!fl_test) {
		cout << "Normal Стек выхода = ";
		SetColor(2, 0);
		printStack(outStack, fout, TRUE); // TRUE - console output
		SetColor(7, 0);
	}
	fout.close();
}

void normalPolishNotation_vec(string& inpt_str, vector<string>& outArr, vector<string>& opArr) {
	vector<string> str_tokens;

	str_tokens = Tokenizer(inpt_str);
	reverseTokens(str_tokens);

	polishNotation_vec(inpt_str, str_tokens, outArr, opArr);
	if (back)
		return;
	reverseTokens(outArr);
}

void reversePolishNotation(string& inpt_str, Node*& outStack, Node*& opStack) { 
	outStack = NULL;
	opStack = NULL;
	vector<string> str_tokens;
	ofstream fout;
	if (fl_test)
		fout.open("test_solution.txt", ios::app);
	else
		fout.open("notation_solution.txt", ios::app);
	fout << endl << endl << "Преобразование выражения в обратную польскую нотацию" << endl;
	fout << "Строка, над которой происходит преобразование - " << input_str << endl;

	if (!fl_test)
		cout << "Строка - \"" << inpt_str << "\"" << endl;
	str_tokens = Tokenizer(inpt_str);

	polishNotation(inpt_str, str_tokens, outStack, opStack, fout);
	if (back)
		menu(inpt_str, outStack, opStack);
	if (!fl_test) {
		cout << "Reverse Стек выхода = ";
		SetColor(2, 0);
		printStack(outStack, fout, TRUE); // TRUE - console output
		SetColor(7, 0);
	}
	fout.close();
}

void reversePolishNotation_vec(string& inpt_str, vector<string>& outArr, vector<string>& opArr) {
	vector<string> str_tokens;
	ofstream fout;

	str_tokens = Tokenizer(inpt_str);

	polishNotation_vec(inpt_str, str_tokens, outArr, opArr);
}

/////////////////// OPZ ///////////////////

void printOut(vector<double>& res, ofstream& fout) {
	for (const auto& token : res) {
		fout << token << " ";
	}
	fout << endl;
}

double Calc_PN(Node* &outStack, bool fl_notation) { // fl_notation : FALSE - Reverse notation; TRUE - normal notation
	back = FALSE;
	vector<double> res;
	double value1 = 0;
	double value2 = 0;
	string operation;
	ofstream fout;
	if (fl_test)
		fout.open("test_solution.txt", ios::app);
	else
		fout.open("notation_solution.txt", ios::app);
	fout << endl << "Вычисление выражения" << endl; printStack(outStack, fout, FALSE); // FALSE - file output
	if (fl_notation) { // if normal polish notation
		outStack = reverseStack(outStack);
		fout << "Перевернем стек для удобства вычисления" << endl; printStack(outStack, fout, FALSE); // FALSE - file output
	}
	while (outStack) {
		logFile(fout,"",outStack->value,outStack,"token", "curr");
		if (IsNumber(outStack->value)) {
			logFile(fout, "", outStack->value, outStack, "stack", "del");
			res.push_back(str2doub(pop(&outStack)));
			logFile(fout, "", outStack->value, outStack, "stack", "curr");
			fout << "Массив выхода: "; printOut(res, fout);
			continue;
		}
		if (IsOperation(outStack->value, weight_op)) {
			if (res.size() >= 2) {
				logFile(fout, "", outStack->value, outStack, "stack", "del");
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
				fout << "Выполняем действие" << operation << " получаем результат: "; printOut(res, fout);
				continue;
			}
			cerr << endl << "[ERROR] Выражение записано некорректно" << endl;
			fout << endl << "[ERROR] Выражение записано некорректно" << endl;
			cout << "Попробуйте снова." << endl;
			fout << "Попробуйте снова." << endl;
			back = TRUE;
			_getch();
			return 0;
		}
		if (IsLiteral(outStack->value)) {
			res.push_back(str2doub(variables.find(outStack->value)->second));
			logFile(fout, "", variables.find(outStack->value)->second, outStack, "token", "add");
			logFile(fout, "", variables.find(outStack->value)->second, outStack, "stack", "curr");
			pop(&outStack);
			continue;
		}
		cerr << endl << "[ERROR] unexpected token " << outStack->value << endl;
		fout << endl << "[ERROR] unexpected token " << outStack->value << endl;
		cout << "Попробуйте снова." << endl;
		fout << "Попробуйте снова." << endl;
		back = TRUE;
		_getch();
		return 0;
	}
	fout << "Вычисление закончено. Результат вычисления - " << res[0] << endl;
	fout.close();
	return res[0];
}

double Calc_PN_vec(vector <string>& outArr, bool fl_notation) {
	back = FALSE;
	vector<double> res;
	double value1 = 0;
	double value2 = 0;
	string operation;


	if (fl_notation) { // if normal polish notation
		reverseTokens(outArr);
	}
	while (outArr.size() != 0) {
		if (IsNumber(outArr[0])) {
			res.push_back(str2doub(outArr[outArr.size() - 1]));
			outArr.erase(outArr.begin());
			continue;
		}
		if (IsOperation(outArr[0], weight_op)) {
			if (res.size() >= 2) {
				operation = outArr[0];
				outArr.erase(outArr.begin());
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
			cerr << endl << "[ERROR] Выражение записано некорректно" << endl;
			cout << "Попробуйте снова." << endl;
			back = TRUE;
			_getch();
			return 0;
		}
		if (IsLiteral(outArr[0])) {
			res.push_back(str2doub(variables.find(outArr[0])->second));
			outArr.erase(outArr.begin());
			continue;
		}
		cerr << endl << "[ERROR] unexpected token " << outArr[0] << endl;
		cout << "Попробуйте снова." << endl;
		back = TRUE;
		_getch();
		return 0;
	}
	if (res.size() == 1)
		return res[0];
	else {
		cerr << endl << "[ERROR] Выражение записано некорректно" << endl;
		cout << "Попробуйте снова." << endl;
		back = TRUE;
		_getch();
		return 0;
	}
}

void convert_exp(string& str, Node* &outStack, Node* &opStack) {
	back = FALSE;
	system("cls");
	chrono::steady_clock sc;
	vector<string> outArr;
	vector<string> opArr;

	cout << "Выберите вид преобразования польской нотации:" << endl;
	cout << "1. Прямая" << endl;
	cout << "2. Обратная" << endl;
	cout << "Ваш выбор: ";
	char value[256]; // переменная, которая хранит выбранное значение
	cin >> value; // считываем выбранное значение
	if (strlen(value) == 1) { // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
		switch (value[0]) {  // проверям, взяв первый символ переменной value
		case '1': {
			auto start = sc.now();     // устанавливаем начало отсчета времени события
			cout << "Прямая польская нотация выражения" << endl;
			normalPolishNotation(str, outStack, opStack);
			auto end = sc.now();       // устанавливаем конец отсчета времени события
			auto time_span = static_cast<chrono::duration<double>>(end - start);   // высчитываем время, затраченное на событие
			cout << "Скорость проверки стеком равна: " << time_span.count() << " секунд" << endl;
			auto start2 = sc.now();     // устанавливаем начало отсчета времени события
			normalPolishNotation_vec(str, outArr, opArr);
			auto end2 = sc.now();       // устанавливаем конец отсчета времени события
			auto time_span2 = static_cast<chrono::duration<double>>(end2 - start2);   // высчитываем время, затраченное на событие
			cout << "Скорость проверки массивом равна: " << time_span2.count() << " секунд" << endl;
			break;
		}
		case '2': {
			auto start1 = sc.now();     // устанавливаем начало отсчета времени события
			cout << "Обратная польская нотация выражения" << endl;
			reversePolishNotation(str, outStack, opStack);
			auto end1 = sc.now();       // устанавливаем конец отсчета времени события
			auto time_span1 = static_cast<chrono::duration<double>>(end1 - start1);   // высчитываем время, затраченное на событие
			cout << "Скорость проверки стеком равна: " << time_span1.count() << " секунд" << endl;
			auto start3 = sc.now();     // устанавливаем начало отсчета времени события
			reversePolishNotation_vec(str, outArr, opArr);
			auto end3 = sc.now();       // устанавливаем конец отсчета времени события
			auto time_span3 = static_cast<chrono::duration<double>>(end3 - start3);   // высчитываем время, затраченное на событие
			cout << "Скорость проверки массивом равна: " << time_span3.count() << " секунд" << endl;
			break;
		}
		default: // если число не подходит ни к одному из
			cout << "Число введено неверно. Введите заново" << endl;
			convert_exp(str, outStack, opStack);
		}
	}
	else { // если введено символов больше необходимого
		cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
		convert_exp(str, outStack, opStack);
	}

	cout << "Выражение записано. Нажмите любую кнопку для продолжения.";
	_getch();
	variables.clear();
	menu(str, outStack, opStack);
}

void check_exp(string& str, Node*& outStack, Node*& opStack) {
	chrono::steady_clock sc;
	vector<string> outArr;
	vector<string> opArr;
	vector<string> str_tokens;
	string temp1, temp2;
	system("cls");
	bool notation;
	outStack = NULL;
	opStack = NULL;

	cout << "Выберите способ ввода выражения:" << endl;
	cout << "1. Через консоль" << endl;
	cout << "2. С помощью файла" << endl;
	cout << "Ваш выбор: ";
	char value[256]; // переменная, которая хранит выбранное значение
	cin >> value; // считываем выбранное значение
	if (strlen(value) == 1) { // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
		switch (value[0]) {  // проверям, взяв первый символ переменной value
		case '1':
			fill_str_self(str, outStack, opStack);
			break;
		case '2':
			fill_str_via_file(str, outStack, opStack, check_file_exp);
			break;
		default: // если число не подходит ни к одному из
			cout << "Число введено неверно. Введите заново" << endl;
			check_exp(str, outStack, opStack);
		}
	}
	else { // если введено символов больше необходимого
		cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
		check_exp(str, outStack, opStack);
	}

	
	notation = getCalcNotation();
	auto start = sc.now();     // устанавливаем начало отсчета времени события
	if (str.size() > 1) {
		temp1 += str[0];
		temp2 += str[str.size() - 1];
		if (IsOperation(temp1, check_op) || IsOperation(temp2, check_op)) {
			cerr << endl << "[ERROR] Выражение записано некорректно" << endl;
			cout << "Попробуйте снова." << endl;
			_getch();
			menu(str, outStack, opStack);
		}
	}
	if (notation)
		normalPolishNotation_vec(str, outArr, opArr);
	else
		reversePolishNotation_vec(str, outArr, opArr);
	if (back)
		menu(str, outStack, opStack);
	Calc_PN_vec(outArr, notation);
	if (back)
		menu(str, outStack, opStack);
	auto end = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // высчитываем время, затраченное на событие
	auto start1 = sc.now();     // устанавливаем начало отсчета времени события
	if (notation)
		normalPolishNotation(str, outStack, opStack);
	else
		reversePolishNotation(str, outStack, opStack);
	Calc_PN(outStack, notation);
	if (back)
		menu(str, outStack, opStack);
	auto end1 = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span1 = static_cast<chrono::duration<double>>(end1 - start1);   // высчитываем время, затраченное на событие
	SetColor(2, 0);
	cout << endl << "Выражение корректно." << endl;
	SetColor(7, 0);

	cout << "Скорость проверки стеком равна: " << time_span1.count() << " секунд" << endl;
	cout << "Скорость проверки массивом равна: " << time_span.count() << " секунд" << endl;
	cout << "Нажмите любую кнопку для продолжения.";
	_getch();
	variables.clear();
	menu(str, outStack, opStack);
}



void check_PN(string& str, Node*& outStack, Node*& opStack) {
	chrono::steady_clock sc;
	vector<string> outArr;
	vector<string> opArr;
	system("cls");
	bool notation = getCalcNotation();
	system("cls");
	outStack = NULL;
	opStack = NULL;
	vector <string> str_tokens;

	cout << "Выберите способ ввода выражения:" << endl;
	cout << "1. Через консоль" << endl;
	cout << "2. С помощью файла" << endl;
	cout << "Ваш выбор: ";
	char value[256]; // переменная, которая хранит выбранное значение
	cin >> value; // считываем выбранное значение
	if (strlen(value) == 1) { // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
		switch (value[0]) {  // проверям, взяв первый символ переменной value
		case '1':
			fill_str_self(str, outStack, opStack);
			break;
		case '2':
			fill_str_via_file(str, outStack, opStack, check_file_pn);
			break;
		default: // если число не подходит ни к одному из
			cout << "Число введено неверно. Введите заново" << endl;
			check_PN(str, outStack, opStack);
		}
	}
	else { // если введено символов больше необходимого
		cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
		check_PN(str, outStack, opStack);
	}
	auto start1 = sc.now();     // устанавливаем начало отсчета времени события
	str_tokens = Tokenizer(str);
	Calc_PN_vec(str_tokens, notation);
	if (back)
		menu(str, outStack, opStack);
	auto end1 = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span1 = static_cast<chrono::duration<double>>(end1 - start1);   // высчитываем время, затраченное на событие

	auto start = sc.now();     // устанавливаем начало отсчета времени события
	str_tokens = Tokenizer(str);
	for (const auto& token : str_tokens) {
		addStackElem(outStack, token);
	}
	Calc_PN(outStack, notation);
	if (back)
		menu(str, outStack, opStack);
	SetColor(2, 0);
	cout << "Выражение корректно." << endl;
	SetColor(7, 0);
	auto end = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // высчитываем время, затраченное на событие

	cout << "Скорость проверки стеком равна: " << time_span.count() << " секунд" << endl;
	cout << "Скорость проверки массивом равна: " << time_span1.count() << " секунд" << endl;
	cout << "Нажмите любую кнопку для продолжения.";
	_getch();
	variables.clear();
	menu(str, outStack, opStack);
}

bool getCalcNotation() {
	system("cls");

	cout << "Выберите нотацию, по которой производить вычисление:" << endl;
	cout << "1. Прямая " << endl;
	cout << "2. Обратная" << endl;
	cout << "Ваш выбор: ";
	char value[256]; // переменная, которая хранит выбранное значение
	cin >> value; // считываем выбранное значение
	if (strlen(value) == 1) { // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
		switch (value[0]) {  // проверям, взяв первый символ переменной value
		case '1':
			cout << "Прямая польская нотация выражения" << endl;
			return TRUE;
		case '2':
			cout << "Обратная польская нотация выражения" << endl;
			return FALSE;
		default: // если число не подходит ни к одному из
			cout << "Число введено неверно. Введите заново" << endl;
			getCalcNotation();
		}
	}
	else { // если введено символов больше необходимого
		cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
		getCalcNotation();
	}
}

void Calculate_exp(string& str, Node*& outStack, Node*& opStack) {

	chrono::steady_clock sc;
	variables.clear();
	vector <string> outArr;
	vector <string> opArr;
	string temp1, temp2;
	system("cls");
	bool notation;
	outStack = NULL;
	opStack = NULL;

	cout << "Выберите способ ввода выражения:" << endl;
	cout << "1. Через консоль" << endl;
	cout << "2. С помощью файла" << endl;
	cout << "Ваш выбор: ";
	char value[256]; // переменная, которая хранит выбранное значение
	cin >> value; // считываем выбранное значение
	if (strlen(value) == 1) { // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
		switch (value[0]) {  // проверям, взяв первый символ переменной value
		case '1':
			fill_str_self(str, outStack, opStack);
			break;
		case '2':
			fill_str_via_file(str, outStack, opStack, input_file);
			break;
		default: // если число не подходит ни к одному из
			cout << "Число введено неверно. Введите заново" << endl;
			Calculate_exp(str, outStack, opStack);
		}
	}
	else { // если введено символов больше необходимого
		cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
		Calculate_exp(str, outStack, opStack);
	}
	notation = getCalcNotation();
	auto start1 = sc.now();     // устанавливаем начало отсчета времени события
	if (str.size() > 1) {
		temp1 += str[0];
		temp2 += str[str.size() - 1];
		if (IsOperation(temp1, check_op) || IsOperation(temp2, check_op)) {
			cerr << endl << "[ERROR] Выражение записано некорректно" << endl;
			cout << "Попробуйте снова." << endl;
			_getch();
			menu(str, outStack, opStack);
		}
	}
	if (notation)
		normalPolishNotation_vec(str, outArr, opArr);
	else
		reversePolishNotation_vec(str, outArr, opArr);
	if (back)
		menu(str, outStack, opStack);
	Calc_PN_vec(outArr, notation);
	if (back)
		menu(str, outStack, opStack);
	auto end1 = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span1 = static_cast<chrono::duration<double>>(end1 - start1);   // высчитываем время, затраченное на событие
	auto start = sc.now();     // устанавливаем начало отсчета времени события

	if (notation)
		normalPolishNotation(str, outStack, opStack);
	else
		reversePolishNotation(str, outStack, opStack);
	double res = Calc_PN(outStack, notation);
	if (back)
		menu(str, outStack, opStack);
	cout << endl << "Результат = " << res << endl;
	auto end = sc.now();       // устанавливаем конец отсчета времени события

	auto time_span = static_cast<chrono::duration<double>>(end - start);   // высчитываем время, затраченное на событие
	cout << "Скорость вычисления стеком равна: " << time_span.count() << " секунд" << endl;
	cout << "Скорость вычисления массивом равна: " << time_span1.count() << " секунд" << endl;
	cout << "DONE! Нажмите любую кнопку для продолжения.";
	_getch();
	variables.clear();
	menu(str, outStack, opStack);
}

void Calculate_pn(string& str, Node*& outStack, Node*& opStack) {
	chrono::steady_clock sc;
	variables.clear();
	system("cls");
	bool notation = getCalcNotation();
	system("cls");
	outStack = NULL;
	opStack = NULL;
	vector <string> str_tokens;

	cout << "Выберите способ ввода выражения:" << endl;
	cout << "1. Через консоль" << endl;
	cout << "2. С помощью файла" << endl;
	cout << "Ваш выбор: ";
	char value[256]; // переменная, которая хранит выбранное значение
	cin >> value; // считываем выбранное значение
	if (strlen(value) == 1) { // проверяем количество введенных символов. Если много, то просим ввести еще раз, иначе проверям дальше
		switch (value[0]) {  // проверям, взяв первый символ переменной value
		case '1':
			fill_str_self(str, outStack, opStack);
			break;
		case '2':
			fill_str_via_file(str, outStack, opStack, calc_notation_file);
			break;
		default: // если число не подходит ни к одному из
			cout << "Число введено неверно. Введите заново" << endl;
			Calculate_pn(str, outStack, opStack);
		}
	}
	else { // если введено символов больше необходимого
		cout << "Необходимо ввести один символ. Попробуйте ввести заново" << endl;
		Calculate_pn(str, outStack, opStack);
	}
	auto start1 = sc.now();     // устанавливаем начало отсчета времени события
	str_tokens = Tokenizer(str);
	Calc_PN_vec(str_tokens, notation);
	if (back)
		menu(str, outStack, opStack);
	auto end1 = sc.now();       // устанавливаем конец отсчета времени события
	auto time_span1 = static_cast<chrono::duration<double>>(end1 - start1);   // высчитываем время, затраченное на событие
	auto start = sc.now();     // устанавливаем начало отсчета времени события
	str_tokens = Tokenizer(str);
	for (const auto& token : str_tokens) {
		addStackElem(outStack, token);
	}
	double res = Calc_PN(outStack, notation);
	if (back)
		menu(str, outStack, opStack);
	cout << endl << "Результат = " << res << endl;
	auto end = sc.now();       // устанавливаем конец отсчета времени события

	auto time_span = static_cast<chrono::duration<double>>(end - start);   // высчитываем время, затраченное на событие
	cout << "Скорость вычисления стеком равна: " << time_span.count() << " секунд" << endl;
	cout << "Скорость вычисления массивом равна: " << time_span1.count() << " секунд" << endl;
	cout << "DONE! Нажмите любую кнопку для продолжения.";
	_getch();
	variables.clear();
	menu(str, outStack, opStack);
}

/////////////////// INPUT /////////////////////

void fill_str_self(string& str, Node*& outStack, Node*& opStack) {
	
	system("cls");
	cout.width(15);
	cout << right << "__Запись через консоль__" << endl << endl;
	str.clear();
	cin.clear();
	//cin.ignore();
	cout << "Введите строку выражения (Каждый символ отделяйте пробелом): ";
	getline(cin, str);
	if (str.size() == 0) {
		cout << "Выражение записано некорректно. Попробуйте снова" << endl;
		fill_str_self(str, outStack, opStack);
		return;
	}
	cout << "Полученная строка - " << str << endl;
	check_literals(str, outStack, opStack);
	cout << "Выражение записано. Нажмите любую кнопку для продолжения." << endl;

	_getch();
}

void fill_str_via_file(string& str, Node*& outStack, Node*& opStack, string& filename) {
	system("cls");
	cout.width(15);
	cout << right << "__Запись через файл__" << endl;
	str.clear();
	string temp;
	ifstream fin;

	fin.open(filename);
	if (fin.is_open()) {
		while (!fin.eof()) {
			getline(fin, temp);
			str += temp;
		}
	}
	else {
		cout << "Файл не удалось открыть. Закройте программу и создайте файл \"" << filename <<"\" в корневой папке программы и попробуйте снова." << endl;
		cout << "Нажмите любую клавишу, чтобы закрыть программу" << endl;
		_getch();
		exit(3);
	}
	if (str.size() == 0) {
		cout << "Файл пуст. Закройте программу и заполните файл \"" << filename << "\" в корневой папке программы данными и попробуйте снова." << endl;
		cout << "Нажмите любую клавишу, чтобы закрыть программу" << endl;
		_getch();
		exit(3);
	}
	cout << endl << "Полученная строка - " << str << endl;
	check_literals(str, outStack, opStack);
	cout << "Выражение записано. Нажмите любую кнопку для продолжения." << endl;

	fin.close();
	_getch();
}

void add_variable(char str) {
	string value;
	string temp_str;
	bool fl_stop = FALSE;

	temp_str += str;
	if (variables.find(temp_str) != variables.end())
		return;
	temp_str.clear();

	while (!fl_stop) {
		fl_stop = TRUE;
		cout << "Введите значение переменной " << str << " : ";
		cin >> value;
		for (int i = 0; i <= value.size() - 1; i++) {
			if (isalpha(value[i])) {
				fl_stop = FALSE;
				cout << "Значение введено некорректно, попробуйте снова" << endl;
				break;
			}
		}
	}
	temp_str += str; // map не принимает один символ за стринг, поэтому добавляю к временному string
	variables.insert(pair<string, string>(temp_str, value));
	temp_str.clear();
}

void check_literals(string& str, Node*& outStack, Node*& opStack) {
	string temp;

	for (int i = 0; i <= str.size() - 1; i++) {
		if (isalpha(str[i])) {
			if (i == 0) {
				if (str[i + 1] == ' ') {
					add_variable(str[i]);
					continue;
				}
			}
			if (i >= 1 and i != str.size() - 1) {
				if (str[i - 1] == ' ' and str[i + 1]) {
					add_variable(str[i]);
					continue;
				}
			}
			if (i == str.size() - 1) {
				if (str[i - 1] == ' ') {
					add_variable(str[i]);
					continue;
				}
			}
			cerr << "[ERROR] unexpected token " << str[i] << endl;
			exit(-1);
		}
	}
}
/////////////////// EXPRESSION /////////////////////

////////////////// TEST //////////////////////

string getOperation() {
	vector<string> operators{ "+","*","/","-" };
	return operators[rand() % (operators.size())];
}

string getNumeric(char op) {
	string value;
	string temp;
	vector<string> numbers{ "1","2","3","4","5","6","7","8","9","0" };
	bool rank = rand() % 2;
	bool stop = TRUE;
	int count = 0;
	int rang;

	rank ? rang = 1 : rang = 2;
	do {
		if (op == '/' && stop) {
			temp = numbers[rand() % numbers.size()];
			while (temp == "0") {
				temp = numbers[rand() % numbers.size()];
			}
			value += temp;
			stop = FALSE;
		}
		else {
			value += numbers[rand() % numbers.size()];
		}
		count++;
	} while (count != rang);
	return value;
}

void createTask(string& task) {
	bool variant = FALSE;
	task += getNumeric(' ');
	for (int i = 1; i <= 13; i++) {
		if (i % 2 == 0) {
			variant ? task += getNumeric(task[task.size() - 1 - 1]): task +=getOperation();
			variant = !variant;
		}
		else
			task += " ";
	}
}

void createTest(string& str, Node*& outStack, Node*& opStack) {
	system("cls");
	fl_test = TRUE;
	int count_of_ex;
	int index = 1;
	Test test;
	bool notation = FALSE;
	ofstream fout;

	fout.open("test_tasks.txt", ios::app);
	cout << "Введите количество заданий: ";
	count_of_ex = checkdigit();
	test.Task.resize(count_of_ex);
	fout << "ПРОВЕРОЧНАЯ РАБОТА НА ПРЕОБРАЗОВАНИЕ ВЫРАЖЕНИЮ В ПОЛЬСКУЮ НОТАЦИЮ" << endl;
	fout << "Количество заданий в работе: " << count_of_ex << endl;
	for (auto& ex : test.Task) {
		createTask(ex.task);
		fout << index << ". Перевести выражение " << ex.task;
		if (notation) {
			fout << " в нормальную польскую нотацию." << endl;
			normalPolishNotation(ex.task, outStack, opStack);
		}
		else {
			fout << " в обратную польскую нотацию." << endl;
			reversePolishNotation(ex.task, outStack, opStack);
		}
		ex.answer = Calc_PN(outStack, notation);
		index++;
		notation = !notation;
	}
	cout << "Тестовые задания созданы. Задания находятся в файле test_task.txt. Побдробое решение находится в файле test_solution.txt" << endl;
	cout << "Нажмите любую кнопку для продолжения.";
	fl_test = FALSE;
	
	fout.close();
	_getch();
	menu(str, outStack, opStack);
}

////////////////// TEST //////////////////////

////////////////// MENU ////////////////////////

void submenu_check_pn_conf_val(string& str, Node*& outStack, Node*& opStack) {
	switch (subm_check_pn_count)
	{
	case 0:
		check_exp(str, outStack, opStack);
		break;
	case 1:
		check_PN(str, outStack, opStack);
		break;
	case 2:
		menu(str, outStack, opStack);
		break;
	default:
		break;
	}
}

void submenu_calc_pn_conf_val(string& str, Node*& outStack, Node*& opStack) {
	switch (subm_calc_pn_count)
	{
	case 0:
		Calculate_exp(str, outStack, opStack);
		break;
	case 1:
		Calculate_pn(str, outStack, opStack);
		break;
	case 2:
		menu(str, outStack, opStack);
		break;
	default:
		break;
	}
}

void submenu_convert_exp_conf_val(string& str, Node*& outStack, Node*& opStack) {
	switch (subm_conv_count) {
	case 0:
		fill_str_self(input_str, outStack, opStack);
		break;
	case 1:
		fill_str_via_file(input_str, outStack, opStack, input_file);
		break;
	case 2:
		menu(str, outStack, opStack);
		break;
	default:
		break;
	}
	convert_exp(str, outStack, opStack);
}

void conf_val(string& str, Node*& outStack, Node*& opStack) {
	switch (m_count) {
	case 0:
		submenu_convert_exp(str, outStack, opStack);
		break;
	case 1:
		submenu_check_pn(str, outStack, opStack);
		break;
	case 2:
		submenu_calc_pn(str, outStack, opStack);
		break;
	case 3:
		createTest(str, outStack, opStack);
		break;
	case 4:
		queue();
		break;
	case 5:
		SetColor(0, 0);
		exit(0);
		break;
	default:
		break;
	}
	menu(str, outStack, opStack);
}

void submenu_check_pn_choice(string& str, Node*& outStack, Node*& opStack) {
	int k1;
	k1 = _getch(); // получаем символ стрелки без вывода знака
	if (k1 == 0xE0) { // если стрелки
		switch (k1) {
		case 0x48: // стрелка вверх
			subm_check_pn_count--;
			if (subm_check_pn_count < 0) subm_check_pn_count = 0;
			submenu_check_pn(str, outStack, opStack);
			break;

		case 0x50: // стрелка вниз
			subm_check_pn_count++;
			if (subm_check_pn_count > 2) subm_check_pn_count = 2;
			submenu_check_pn(str, outStack, opStack);
			break;
		case 0xD: // подтвердить
			submenu_check_pn_conf_val(str, outStack, opStack);
			break;
		default:
			submenu_check_pn_choice(str, outStack, opStack);
		}
	}
	switch (k1) {
	case 0x48: // стрелка вверх
		subm_check_pn_count--;
		if (subm_check_pn_count < 0) subm_check_pn_count = 0;
		submenu_check_pn(str, outStack, opStack);
		break;

	case 0x50: // стрелка вниз
		subm_check_pn_count++;
		if (subm_check_pn_count > 2) subm_check_pn_count = 2;
		submenu_check_pn(str, outStack, opStack);
		break;
	case 0xD: // подтвердить
		submenu_check_pn_conf_val(str, outStack, opStack);
		break;
	default:
		submenu_check_pn_choice(str, outStack, opStack);
	}
}

void submenu_calc_pn_choice(string& str, Node*& outStack, Node*& opStack) {
	int k1;
	k1 = _getch(); // получаем символ стрелки без вывода знака
	if (k1 == 0xE0) { // если стрелки
		switch (k1) {
		case 0x48: // стрелка вверх
			subm_calc_pn_count--;
			if (subm_calc_pn_count < 0) subm_calc_pn_count = 0;
			submenu_calc_pn(str, outStack, opStack);
			break;

		case 0x50: // стрелка вниз
			subm_calc_pn_count++;
			if (subm_calc_pn_count > 2) subm_calc_pn_count = 2;
			submenu_calc_pn(str, outStack, opStack);
			break;
		case 0xD: // подтвердить
			submenu_calc_pn_conf_val(str, outStack, opStack);
			break;
		default:
			submenu_calc_pn_choice(str, outStack, opStack);
		}
	}
	switch (k1) {
	case 0x48: // стрелка вверх
		subm_calc_pn_count--;
		if (subm_calc_pn_count < 0) subm_calc_pn_count = 0;
		submenu_calc_pn(str, outStack, opStack);
		break;

	case 0x50: // стрелка вниз
		subm_calc_pn_count++;
		if (subm_calc_pn_count > 2) subm_calc_pn_count = 2;
		submenu_calc_pn(str, outStack, opStack);
		break;
	case 0xD: // подтвердить
		submenu_calc_pn_conf_val(str, outStack, opStack);
		break;
	default:
		submenu_calc_pn_choice(str, outStack, opStack);
	}
}

void submenu_convert_exp_choice(string& str, Node*& outStack, Node*& opStack) {
	int k1;
	k1 = _getch(); // получаем символ стрелки без вывода знака
	if (k1 == 0xE0) { // если стрелки
		switch (k1) {
		case 0x48: // стрелка вверх
			subm_conv_count--;
			if (subm_conv_count < 0) subm_conv_count = 0;
			submenu_convert_exp(str, outStack, opStack);
			break;

		case 0x50: // стрелка вниз
			subm_conv_count++;
			if (subm_conv_count > 2) subm_conv_count = 2;
			submenu_convert_exp(str, outStack, opStack);
			break;
		case 0xD: // подтвердить
			submenu_convert_exp_conf_val(str, outStack, opStack);
			break;
		default:
			submenu_convert_exp_choice(str, outStack, opStack);
		}
	}
	switch (k1) {
	case 0x48: // стрелка вверх
		subm_conv_count--;
		if (subm_conv_count < 0) subm_conv_count = 0;
		submenu_convert_exp(str, outStack, opStack);
		break;

	case 0x50: // стрелка вниз
		subm_conv_count++;
		if (subm_conv_count > 2) subm_conv_count = 2;
		submenu_convert_exp(str, outStack, opStack);
		break;
	case 0xD: // подтвердить
		submenu_convert_exp_conf_val(str, outStack, opStack);
		break;
	default:
		submenu_convert_exp_choice(str, outStack, opStack);
	}
}

void menu_choice(string& str, Node*& outStack, Node*& opStack) { // в зависимости от стрелок изменяем меню 
	int k1;
	k1 = _getch(); // получаем символ стрелки без вывода знака
	if (k1 == 0xE0) { // если стрелки
		switch (k1) {
		case 0x48: // стрелка вверх
			m_count--;
			if (m_count < 0) m_count = 0;
			menu(str, outStack, opStack);
			break;

		case 0x50: // стрелка вниз
			m_count++;
			if (m_count > 5) m_count = 5;
			menu(str, outStack, opStack);
			break;
		case 0xD: // подтвердить
			conf_val(str, outStack, opStack);
			break;
		default:
			menu_choice(str, outStack, opStack);
		}
	}
	switch (k1) {
	case 0x48: // стрелка вверх
		m_count--;
		if (m_count < 0) m_count = 0;
		menu(str, outStack, opStack);
		break;

	case 0x50: // стрелка вниз
		m_count++;
		if (m_count > 5) m_count = 5;
		menu(str, outStack, opStack);
		break;
	case 0xD: // подтвердить
		conf_val(str, outStack, opStack);
		break;
	default:
		menu_choice(str, outStack, opStack);
	}
}

void submenu_check_pn(string& str, Node*& outStack, Node*& opStack) {
	system("cls");

	if (subm_check_pn_count == 0) {
		cout.width(15);
		cout << right << "__Вычисление выражения__" << endl << endl;
		SetColor(2, 0);
		cout << "Что будет в качестве выражения?" << endl << endl;
		SetColor(7, 0);
		SetColor(5, 0);
		cout << "Числовое выражение" << endl;
		SetColor(7, 0);
		cout << "Польская нотация" << endl;
		cout << "Назад" << endl;
		submenu_check_pn_choice(str, outStack, opStack);
	}
	if (subm_check_pn_count == 1) {
		cout.width(15);
		cout << right << "__Вычисление выражения__" << endl << endl;
		SetColor(2, 0);
		cout << "Что будет в качестве выражения?" << endl << endl;
		SetColor(7, 0);
		cout << "Числовое выражение" << endl;
		SetColor(5, 0);
		cout << "Польская нотация" << endl;
		SetColor(7, 0);
		cout << "Назад" << endl;
		submenu_check_pn_choice(str, outStack, opStack);
	}
	if (subm_check_pn_count == 2) {
		cout.width(15);
		cout << right << "__Вычисление выражения__" << endl << endl;
		SetColor(2, 0);
		cout << "Что будет в качестве выражения?" << endl << endl;
		SetColor(7, 0);
		cout << "Числовое выражение" << endl;
		cout << "Польская нотация" << endl;
		SetColor(5, 0);
		cout << "Назад" << endl;
		SetColor(7, 0);
		submenu_check_pn_choice(str, outStack, opStack);
	}
}

void submenu_calc_pn(string& str, Node*& outStack, Node*& opStack) {
	system("cls");

	if (subm_calc_pn_count == 0) {
		cout.width(15);
		cout << right << "__Вычисление выражения__" << endl << endl;
		SetColor(2, 0);
		cout << "Что будет в качестве выражения?" << endl << endl;
		SetColor(7, 0);
		SetColor(5, 0);
		cout << "Числовое выражение" << endl;
		SetColor(7, 0);
		cout << "Польская нотация" << endl;
		cout << "Назад" << endl;
		submenu_calc_pn_choice(str, outStack, opStack);
	}
	if (subm_calc_pn_count == 1) {
		cout.width(15);
		cout << right << "__Вычисление выражения__" << endl << endl;
		SetColor(2, 0);
		cout << "Что будет в качестве выражения?" << endl << endl;
		SetColor(7, 0);
		cout << "Числовое выражение" << endl;
		SetColor(5, 0);
		cout << "Польская нотация" << endl;
		SetColor(7, 0);
		cout << "Назад" << endl;
		submenu_calc_pn_choice(str, outStack, opStack);
	}
	if (subm_calc_pn_count == 2) {
		cout.width(15);
		cout << right << "__Вычисление выражения__" << endl << endl;
		SetColor(2, 0);
		cout << "Что будет в качестве выражения?" << endl << endl;
		SetColor(7, 0);
		cout << "Числовое выражение" << endl;
		cout << "Польская нотация" << endl;
		SetColor(5, 0);
		cout << "Назад" << endl;
		SetColor(7, 0);
		submenu_calc_pn_choice(str, outStack, opStack);
	}
}

void submenu_convert_exp(string& str, Node*& outStack, Node*& opStack) {
	system("cls");

	if (subm_conv_count == 0) {
		cout.width(15);
		cout << right << "__Преобразование выражение в ПН__" << endl << endl;
		SetColor(2, 0);
		cout << "Каким образом будет записано выражение?" << endl << endl;
		SetColor(7, 0);
		SetColor(5, 0);
		cout << "С помощью консоли" << endl;
		SetColor(7, 0);
		cout << "С помощью файла" << endl;
		cout << "Назад" << endl;
		submenu_convert_exp_choice(str, outStack, opStack);
	}
	if (subm_conv_count == 1) {
		cout.width(15);
		cout << right << "__Преобразование выражение в ПН__" << endl << endl;
		SetColor(2, 0);
		cout << "Каким образом будет записано выражение?" << endl << endl;
		SetColor(7, 0);
		cout << "С помощью консоли" << endl;
		SetColor(5, 0);
		cout << "С помощью файла" << endl;
		SetColor(7, 0);
		cout << "Назад" << endl;
		submenu_convert_exp_choice(str, outStack, opStack);
	}
	if (subm_conv_count == 2) {
		cout.width(15);
		cout << right << "__Преобразование выражение в ПН__" << endl << endl;
		SetColor(2, 0);
		cout << "Каким образом будет записано выражение?" << endl << endl;
		SetColor(7, 0);
		cout << "С помощью консоли" << endl;
		cout << "С помощью файла" << endl;
		SetColor(5, 0);
		cout << "Назад" << endl;
		SetColor(7, 0);
		submenu_convert_exp_choice(str, outStack, opStack);
	}
}

void menu(string& str, Node*& outStack, Node*& opStack) { // меню
	system("cls");

	if (m_count == 0) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		SetColor(5, 0);
		cout << "Преобразовать выражение в ПН" << endl;
		SetColor(7, 0);
		cout << "Проверить ПН" << endl;
		cout << "Вычислить выражение с помощью ПН" << endl;
		cout << "Создание теста" << endl;
		cout << "Задание по варианту" << endl;
		cout << "Выход" << endl;
		menu_choice(str, outStack, opStack);
	}
	if (m_count == 1) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Преобразовать выражение в ПН" << endl;
		SetColor(5, 0);
		cout << "Проверить ПН" << endl;
		SetColor(7, 0);
		cout << "Вычислить выражение с помощью ПН" << endl;
		cout << "Создание теста" << endl;
		cout << "Задание по варианту" << endl;
		cout << "Выход" << endl;
		menu_choice(str, outStack, opStack);
	}
	if (m_count == 2) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Преобразовать выражение в ПН" << endl;
		cout << "Проверить ПН" << endl;
		SetColor(5, 0);
		cout << "Вычислить выражение с помощью ПН" << endl;
		SetColor(7, 0);
		cout << "Создание теста" << endl;
		cout << "Задание по варианту" << endl;
		cout << "Выход" << endl;
		menu_choice(str, outStack, opStack);
	}
	if (m_count == 3) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Преобразовать выражение в ПН" << endl;
		cout << "Проверить ПН" << endl;
		cout << "Вычислить выражение с помощью ПН" << endl;
		SetColor(5, 0);
		cout << "Создание теста" << endl;
		SetColor(7, 0);
		cout << "Задание по варианту" << endl;
		cout << "Выход" << endl;
		menu_choice(str, outStack, opStack);
	}
	if (m_count == 4) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Преобразовать выражение в ПН" << endl;
		cout << "Проверить ПН" << endl;
		cout << "Вычислить выражение с помощью ПН" << endl;
		cout << "Создание теста" << endl;
		SetColor(5, 0);
		cout << "Задание по варианту" << endl;
		SetColor(7, 0);
		cout << "Выход" << endl;
		menu_choice(str, outStack, opStack);
	}
	if (m_count == 5) {
		cout.width(15);
		cout << right << "__МЕНЮ__" << endl;
		cout << "Преобразовать выражение в ПН" << endl;
		cout << "Проверить ПН" << endl;
		cout << "Вычислить выражение с помощью ПН" << endl;
		cout << "Создание теста" << endl;
		cout << "Задание по варианту" << endl;
		SetColor(5, 0);
		cout << "Выход" << endl;
		SetColor(7, 0);
		menu_choice(str, outStack, opStack);
	}
}

////////////////// MENU ////////////////////////

void logFile(ofstream& fout, string str, const string& token, Node* Stack, string object, string Action) {
	if (object == "str") {
		if (Action == "add") {
			fout << "Строка, над которой происходит преобразование - " << str << endl;
		}
	}
	if (object == "token") {
		if (Action == "curr") {
			fout << "Текущий элемент - " << token << endl;
			return;
		}
		if (Action == "add") {
			fout << "Добавление элемента " << token << endl;
			return;
		}
	}
	if (object == "stack") {
		if (Action == "curr") {
			fout << "Текущий стек = ";
			printStack(Stack, fout, FALSE); // FALSE - file output
			return;
		}
		if (Action == "del") {
			fout << "Удаление текущего элемента " << token << " из стека " << endl;
			return;
		}
	}
}

int main() {
	setlocale(0, "");

	Node* outStack = NULL;
	Node* opStack = NULL;
	srand(time(NULL));
	menu(input_str, outStack, opStack);

	system("pause");


	return 0;
}
