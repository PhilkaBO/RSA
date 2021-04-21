#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>

namespace mul = boost::multiprecision;
using namespace std;

//Публичный ключ
struct PublicKey
{
	int N;
	int e;
};

//Личный ключ
struct PrivateKey
{
	int N;
	int d;
};

//Проверка на простоту
bool Primary(int);

//Генерирование чисел
int RandNum(int, int);

//Функции генерации публичного ключа
int GenPublicKey(PublicKey&);
int GenE(int);

//Функция генерации личного ключа
void GenPrivateKey(int, int, PrivateKey&);

//Шифровщик
string Decoder(string, PrivateKey&);

//Дешифровщик
string Encoder(string, PublicKey&);


int main()
{
	//Леденцов Владислав Павлович
	//Лабораторная работа 2 "Алгоритм RSA"

	srand(time(0));

	PublicKey rsa_public;
	PrivateKey rsa_private;

	//Генерация ключей
	int euler = GenPublicKey(rsa_public);
	rsa_private.N = rsa_public.N;
	GenPrivateKey(rsa_public.e, euler, rsa_private);
	
	//Пользовательский ввод и вывод
	string str, resultEncode, resultDecode;
	cout << "\nEnter message: ";
	_flushall();
	getline(cin, str);

	//Зашифровка
	resultEncode = Encoder(str, rsa_public);
	//Расшифровка
	resultDecode = Decoder(resultEncode, rsa_private);

	//Вывод результатов
	cout << "Original message: " << str << endl;
	cout << "Encrypted message: " << resultEncode << endl;
	cout << "Decrypted message: " << resultDecode << endl;

	cout << "PublicKey: (" << rsa_public.e << ", " << rsa_public.N << ")" << endl;
	cout << "PrivateKey: (" << rsa_private.d << ", " << rsa_private.N << ")" << endl;
	cout << "Euler: " << euler << endl;

	return 0;
}

int RandNum(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

int GenPublicKey(PublicKey & rsa_public)
{
	int randnumber = RandNum(1, 30);
	int P = 0;
	int Q = 0;

	while (!Primary(randnumber)) randnumber++;

	P = randnumber;

	randnumber = RandNum(130/P, 60);

	while (!Primary(randnumber)) randnumber++;

	Q = randnumber;

	rsa_public.N = Q * P;
	rsa_public.e = GenE((P - 1) * (Q - 1));

	return (P - 1) * (Q - 1);
}

void GenPrivateKey(int e, int euler, PrivateKey & rsa_private)
{
	int d = 1;
	while ((d * e) % euler != 1) d++;
	rsa_private.d = d;
}

int GenE(int euler)
{
	for (int i = 2; i < euler; i++)
	{
		if (Primary(i) && euler % i != 0) return i;
	}
	return -1;
}

string Encoder(string str, PublicKey& rsa_public)
{
	string resultstr;
	mul::cpp_int result;

	for (int i = 0; i < str.size(); i++)
	{
		result = (mul::pow(mul::cpp_int(str[i]), rsa_public.e)) % rsa_public.N;
		resultstr = resultstr + to_string(int(result));
		if (str.size() - 1 != i) resultstr.push_back('.');
	}
	return resultstr;
}

string Decoder(string str, PrivateKey & rsa_private)
{
	mul::cpp_int result;
	string temp;
	string resultstr;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] != '.') temp.push_back(str[i]);
		if (str[i] == '.' || i == (str.size() - 1))
		{
			istringstream(temp) >> result;
			result = (mul::pow(result, rsa_private.d)) % rsa_private.N;
			temp = int(result);
			resultstr = resultstr + temp;

			temp.clear();
		}
	}
	return resultstr;
}

bool Primary(int number)
{
	if (number > 1)
	{
		for (int i = 2; i < number; i++)
		{
			if (number % i == 0) return false;
		}
		return true;
	}
	else return false;
}