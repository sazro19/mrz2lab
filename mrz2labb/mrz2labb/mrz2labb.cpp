//// Лабораторная работа 2 по дисциплине МРЗвИС
//// Выполнена студентами группы 821701
//// БГУИР Залесским Александром Андреевичем и Киселёвым Никитой Владимировичем
//// Реализация модели решения задачи на ОКМД архитектуре


#include <iostream>
#include<iomanip>
#include <ctime>
#include <math.h>
#include <windows.h>

using namespace std;

int m;
int p;
int q;
int n;
int r;

double** A;
double** B;
double* E;
double** G;
double*** F;
double*** D;
double** C;

int timeOfSumm = 1;
int timeOfDifference = 1;
int timeOfMultiplying = 1;
int timeOfCom = 1;

int callsOfSumm;
int callsOfDifference;
int callsOfMultiplying;
int callsOfCom;


int T1;
int Tn;
double Ky;
double Eff;
double Diff;
int Lavg;

double a_and_b(double** A, double** B, int k, int i, int j)
{
	callsOfCom++;
	callsOfSumm++;
	callsOfDifference++;
	return max(A[i][k] + B[k][j] - 1., 0.);
}

double a_to_b(double** A, double** B, int k, int j, int i)
{
	callsOfCom++;
	return min(1. - A[i][k] + B[k][j], 0.);
}

double b_to_a(double** A, double** B, int k, int i, int j)
{
	callsOfCom++;
	return min(1. - B[k][j] + A[i][k], 0.);
}

void Fijk(double** A, double** B, double* E, int i, int j, int k)
{
	callsOfMultiplying += 7;
	callsOfDifference += 3;
	callsOfSumm += 2;
	F[i][j][k] = (a_to_b(A, B, k, j, i) * (2. * E[k] - 1.) * E[k] + b_to_a(A, B, k, i, j) * (1. + (4. * a_to_b(A, B, k, j, i) - 2.) * E[k]) * (1. - E[k]));
}

void Dijk(double** A, double** B, int i, int j, int k)
{
	D[i][j][k] = a_and_b(A, B, k, i, j);
}

double f_func(int i, int j) {
	double result = 1;
	for (int k = 0; k < m; k++) {
		result *= F[i][j][k];
	}
	callsOfMultiplying += m - 1; return result;
}

double d_func(int i, int j) {
	double result = 1;
	for (int k = 0; k < m; k++) {
		result *= 1 - D[i][j][k];
	}
	callsOfDifference += m + 1;
	callsOfMultiplying += m - 1;
	callsOfDifference++;
	return 1 - result;
}


double f_and_d(int i, int j)
{
	return max(f_func(i, j) + d_func(i, j) - 1., 0.);
}

double cij(int i, int j) {
	double c;
	callsOfMultiplying += 8;
	callsOfDifference += 3;
	callsOfSumm += 2;
	c = f_func(i, j) * (3. * G[i][j] - 2.) * G[i][j] + (d_func(i, j) + (4. * f_and_d(i, j) - 3. * d_func(i, j)) * G[i][j]) * (1. - G[i][j]);

	return c;
}

int main() {
	srand(time(nullptr));
	int i, j, k;

	cout << "Input m,p,q,n" << endl;
	cin >> m >> p >> q >> n;

	r = p * m * q;
	Tn = 0;
	callsOfSumm = 0;
	callsOfDifference = 0;
	callsOfMultiplying = 0;
	callsOfCom = 0;

	A = new double* [p];
	B = new double* [m];
	E = new double[m];
	G = new double* [p];
	C = new double* [p];
	for (i = 0; i < p; i++) {
		A[i] = new double[m];
		G[i] = new double[q];
		C[i] = new double[q];
	}
	for (i = 0; i < m; i++) {
		B[i] = new double[q];
	}


	for (i = 0; i < p; i++) {
		for (j = 0; j < m; j++) {
			A[i][j] = (double)(rand() % 20001) / 10000 - 1;
		}
	}
	for (i = 0; i < m; i++) {
		for (j = 0; j < q; j++) {
			B[i][j] = (double)(rand() % 20001) / 10000 - 1;
		}
	}
	for (i = 0; i < m; i++) {
		E[i] = (double)(rand() % 20001) / 10000 - 1;
	}
	for (i = 0; i < p; i++) {
		for (j = 0; j < q; j++) {
			G[i][j] = (double)(rand() % 20001) / 10000 - 1;
		}
	}


	F = new double** [p];
	for (i = 0; i < p; i++)
		F[i] = new double* [q];
	for (i = 0; i < p; i++)
		for (j = 0; j < q; j++)
			F[i][j] = new double[m];

	D = new double** [p];
	for (i = 0; i < p; i++)
		D[i] = new double* [q];
	for (i = 0; i < p; i++)
		for (j = 0; j < q; j++)
			D[i][j] = new double[m];

	cout << "A:";
	for (i = 0; i < p; i++) {
		cout << endl;
		for (j = 0; j < m; j++) {
			cout << setw(7) << A[i][j] << " ";
		}
	}
	cout << endl << endl;
	cout << "B:";
	for (i = 0; i < m; i++) {
		cout << endl;
		for (j = 0; j < q; j++) {
			cout << setw(7) << B[i][j] << " ";
		}
	}
	cout << endl << endl;
	cout << "E:";
	cout << endl;
	for (i = 0; i < m; i++) {
		cout << setw(7) << E[i] << " ";
	}
	cout << endl << endl;
	cout << "G:";
	for (i = 0; i < p; i++) {
		cout << endl;
		for (j = 0; j < q; j++) {
			cout << setw(7) << G[i][j] << " ";
		}
	}
	for (i = 0; i < p; i++)
		for (j = 0; j < q; j++)
			for (k = 0; k < m; k++)
				Fijk(A, B, E, i, j, k);
	int reductionTime = 3 * (timeOfMultiplying + timeOfDifference + timeOfSumm);
	int operationTime = 7 * timeOfMultiplying + 3 * timeOfDifference + 2 * timeOfSumm;
	Tn += (int)(reductionTime + operationTime) * (int)ceil((double)r / n);

	for (i = 0; i < p; i++)
		for (j = 0; j < q; j++)
			for (k = 0; k < m; k++)
				Dijk(A, B, i, j, k);

	Tn += timeOfMultiplying * (int)ceil((double)r / n);

	for (i = 0; i < p; i++) {
		for (j = 0; j < q; j++) {
			C[i][j] = cij(i, j);
		}
	}
	int FTime = 2 * timeOfMultiplying * (m - 1);
	int DTime = 3 * (timeOfDifference * (m + 1) + timeOfMultiplying * (m - 1));
	operationTime = 8 * timeOfMultiplying + 4 * timeOfDifference + 2 * timeOfSumm;
	Tn += (FTime + DTime + operationTime) * (int)ceil((double)(p * q) / (double)n);

	cout << endl << endl;
	cout << "C:";

	for (i = 0; i < p; i++) {
		cout << endl;
		for (j = 0; j < q; j++) {
			cout << setw(12) << C[i][j] << " ";
		}
	}
	cout << endl << endl;

	cout << "Paramerts:" << endl;
	T1 = timeOfSumm * callsOfSumm + timeOfDifference * callsOfDifference + timeOfMultiplying * callsOfMultiplying + timeOfCom * callsOfCom;
	if (Tn > T1) Tn = T1;
	Ky = (double)T1 / Tn;
	Eff = (double)Ky / n;
	//D
	Lavg = timeOfMultiplying * r;
	//F
	Lavg += (7 * timeOfMultiplying + 3 * timeOfDifference + 2 * timeOfSumm) * r;
	//C
	Lavg += (8 * timeOfMultiplying + 3 * timeOfDifference + 2 * timeOfSumm) * p * q;
	//a_or_b
	Lavg += (timeOfCom + timeOfSumm + timeOfDifference) * (m - 1) * 2 * p * q;
	//D_func
	Lavg += (timeOfMultiplying * (m - 1) + timeOfDifference * (m + 1)) * 3 * p * q;
	//a_to_b
	Lavg += (timeOfCom)* r * 3;
	Lavg = ceil(Lavg / r);
	Diff = (double)Tn / Lavg;

	cout << "T1= " << T1 << "\nTn= " << Tn << "\nKy= " << Ky << "\ne= " << Eff << "\nLsum= " << Tn << "\nLavg= " << Lavg << "\nD= " << Diff << endl;

	for (i = 0; i < p; i++)
	{
		delete[]A[i];
		delete[]G[i];
		delete[]C[i];
	}
	delete[]A;
	delete[]B;
	delete[]C;
	delete[]E;
	delete[]G;
	return 0;
}