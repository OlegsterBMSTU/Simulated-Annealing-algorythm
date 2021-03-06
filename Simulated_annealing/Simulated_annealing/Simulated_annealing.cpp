// Simulated_annealing.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
/*This is array size*/
#define MAX_LENGTH 30

typedef int solutionType[MAX_LENGTH];

typedef struct {
	solutionType solution;
	float energy;
} membertype;

/*Algotythm parameters*/
#define INITIAL_TEMPERATURE 30.0
#define FINAL_TEMP 0.5
#define ALPHA 0.98
#define STEPS_PER_CHANGE 100

/* Initializating and search functions */
void tweakSolution(membertype *member) {
	int temp, x, y;
	srand(time(0));
	x = rand() % MAX_LENGTH +1;
	do {
		y = rand() % MAX_LENGTH + 1;
	} while (x == y);
	temp = member->solution[x];
	member->solution[x] = member->solution[y];
	member->solution[y] = temp;
}
void initializateSolution(membertype *member) {
	int i;
	/*Begin initializate of solution*/
	for (i = 0; i < MAX_LENGTH; i++)
		member->solution[i] = i;
	/*Random changing solution*/
	for (i = 0; i < MAX_LENGTH; i++)
		tweakSolution(member);
}

/* Rating of solution (Оценка решения) */
void computeEnergy(membertype *member) {
	int i, j, x, y, tempx, tempy;
	char board[MAX_LENGTH][MAX_LENGTH];
	int conflicts;
	const int dx[4] = { -1,1,-1,1 };
	const int dy[4] = { -1,1,1,-1 };
	/*Standart function's clean memory */
	memset((void *)board, 0, MAX_LENGTH*MAX_LENGTH); // there was bzero - function instead memset. Look in the book page 33;

	for (i = 0; i < MAX_LENGTH; i++)
		board[i][member->solution[i]] = 'Q';
	/* Count a quantity conflicts  for every queen */
	conflicts = 0;
	for (i = 0; i < MAX_LENGTH; i++) {
		x = i;
		y = member->solution[i];
		/*по условию кодировки конфликты по вертикали и горизонтали исключены*/
		/*проверяем диагонали*/
		for (j = 0; j < 4; j++) {
			tempx = x;
			tempy = y;
			while (1) {
				tempx += dx[j]; tempy += dy[j];
				if ((tempx < 0) || (tempx >= MAX_LENGTH) || (tempy < 0) || (tempy >= MAX_LENGTH)) break;
				if (board[tempx][tempy] == 'Q') conflicts++;
			}
		}
	}
	member->energy = (float)conflicts;
}

/*Copying solution to other solution*/
/*Копирование одного рещения в другое*/
void copySolution(membertype *dest, membertype *src) {
	for (int i = 0; i < MAX_LENGTH; i++) {
		dest->solution[i] = src->solution[i];
	}
	dest->energy = src->energy;
}

/* отображение решения в виде шахматной доски */
void emitSolution(membertype *member) {
	char board[MAX_LENGTH][MAX_LENGTH];
	int x, y;
	memset((void *)board, 0, MAX_LENGTH*MAX_LENGTH);
	for (x = 0; x < MAX_LENGTH; x++) {
		board[x][member->solution[x]] = 'Q';
	}
	printf("board:\n");
	for (y = 0; y < MAX_LENGTH; y++) {
		for (x = 0; x < MAX_LENGTH; x++) {
			if (board[x][y] == 'Q') printf("Q");
			else printf(". ");
		}
		printf("\n");
	}
	printf("\n\n");
}

void main()
{
    std::cout << "Hello World!\n"; 
	int timer = 0, step, solution = 0, useNew, accepted;
	float temperature = INITIAL_TEMPERATURE;
	membertype current, working, best;
	FILE *fp,*ftest;

	//fp = fopen("stats.txt", "w");
	fopen_s(&fp, "stats.txt", "w");
	fopen_s(&ftest, "ftest.txt", "w");
	
	srand(time(NULL));
	initializateSolution(&current);
	computeEnergy(&current);
	best.energy = 100;
	copySolution(&working, &current);
	printf("OK!!!\n");
	while (temperature > FINAL_TEMP) {
		printf("Temperature: %f\n", temperature);
		accepted = 0;
		/*Solutions were changed random */
		for (step = 0; step < STEPS_PER_CHANGE; step++) {
			srand(time(NULL));
			useNew = 0;
			tweakSolution(&working);
			computeEnergy(&working);
			if (working.energy <= current.energy) {
				useNew = 1;
			}
			else {
				srand(time(0));
				//float test1 = (rand() % 100);
				float test = (rand() % 1000) / (10000 * 0.1);
				float delta = working.energy - current.energy;
				float calc = exp(-delta / temperature);
				if (calc > test) {
					accepted++;
					useNew = 1;
				}
				fprintf_s(ftest, "%i\nDelta= %f, Temperature= %f\n working.energy= %f, current.energy= %f\ncalc= %f  test= %f\n\n", useNew,delta,temperature, working.energy, current.energy ,calc, test);
			}

			if (useNew) {
				useNew = 0;
				copySolution(&current, &working);
				if (current.energy < best.energy) {
					copySolution(&best, &current);
					solution = 1;
				}
			}
			else {
				copySolution(&working, &current);
			}
		}
		fprintf_s(fp, "timer temper energy accept\n%i %f %f %i -- \n", timer++, temperature,best.energy, accepted);
		printf("Best energy= %f\n", best.energy);
		temperature *= ALPHA;

		//fclose(fp);
		//solution = 1;
		if (solution) {
			emitSolution(&best);
		}
	}
	fclose(fp);
	fclose(ftest);
}
 void ex_main() {
	 srand(time(NULL));
	 float x = (rand() % 100) / (1000 * 0.1);
	int z = 111;
	
	printf("Value= %f\n", z);
	printf("Result= %f", x);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
