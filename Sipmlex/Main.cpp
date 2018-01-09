/*
*
* @user Yesim SAKIZ
* @date 05.11.2016
* @description Simpleks Metodunun Maksimizasyon algoritmasinin implementasyonu
*
*/

#include <iostream>													//kutuphaneler
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>


using namespace std;

string debug = "0";													//debug modu icin global degisken

/*
* Kullanicidan verileri alir
* @return vector<vector<double> > girilen verilere gore olusturulan tabloyu iki boyutlu vektor halinde dondurur
*/
vector<vector<double> > input();
/*
* Alinan veriler ile tablo olusturur ve bunu iki boyutlu vektor olarak dondurur
* @param maxFunction amac fonksiyonunu icerir
* @param equations denklemleri icerir
* @param rightHandSides denklemlerin <= den sonraki degerini icerir
* @return vector<vector<double> > tabloyu dondurur
*/
vector<vector<double> > createTable(vector<double> maxFunction, vector<vector<double> > equations, vector<double> rightHandSides);
/*
* Tablo uzerinde pivot sayiyi ve bu sayinin koordinatlarini bulur
* @param table iki boyutlu vektordur.Tum veriler bu degiskende tutulur
* @param pivotColumn pivot sayinin sutun numarasi
* @param pivotRow pivot sayinin satir numarasi
* @return bool true ya da false dondurur
*/
bool pivotFinder(bool & hasPivot, int & pivotColumn, int & pivotRow, vector<vector<double> > table);
/*
* Bu fonksiyonda asil islemler gerceklestirilir. Bitiminde ise amac fonksiyonu ve
* denklemlerin degiskenleri elde edilir
* @param table iki boyutlu vektordur.Tum veriler bu degiskende tutulur
* @param max amac fonksiyonunu referans olarak donduru
* @return vector<double> tek boyutlu vektor seklinde denklem desiskenlerini dondurur
*/
vector<double> makeProcess(vector<vector<double> > table, double & max);
/*
* Tabloyu ekrana bastirir
* @param table iki boyutlu vektordur.Tum veriler bu degiskende tutulur
* @param pivotRow pivot sayinin satir numarasi
* @param pivotColumn pivot sayinin sutun numarasi
*/
void printTable(vector<vector<double> > table, int & pivotRow, int & pivotColumn);
/*
* Girilen verileri matematiksel olarak yani denklem seklinde ekrana bastirir
* @param maxFunction amac fonksiyonunu icerir
* @param equations denklemleri icerir
* @param rightHandSides denklemlerin <= den sonraki degerini icerir
*/
void printMathematically(vector<double> maxFunction, vector<vector<double> > equations, vector<double> rightHandSides);
/*
* Simplex metodu bu fonksiyon icinde test edilir
*/
void tester();
//Start Of Main
int main(void) {
	cout << "########################### WELCOME #########################" << endl;
	cout << "We can show processings step by step" << endl;
	cout << "If you want enter '1' or not enter something =>:";
	cin >> debug;
	tester();
	cout << "########################### THE END #########################" << endl;

}//End Of Main
/*------------------------------------------------------------------------------------------*/
vector<vector<double> > input() {
	string maxFuncStr;
	vector<double> maxFunction;
	vector<double> equationCoef;
	vector<double> rightHandSides;
	vector<vector<double> > equations;

	int economicFuncVarNumber = 0;
	int numberOfEquations = 0;
	double nums;
	vector <double> economicFuncNumber;

	cout << "____________________________________________" << endl;
	cout << "Please enter objective function's coefficients :";
	cin >> economicFuncVarNumber;
	cout << "\nPlease number of equations :";
	cin >> numberOfEquations;
	cout << "\nEnter the objective function numbers :" << endl;

	for (int i = 0; i < economicFuncVarNumber; i++) {
		cout << "    X" << i + 1 << " :";
		cin >> nums;
		maxFunction.push_back(nums);
	}

	cout << "\nEnter equations's coefficients :" << endl;
	int j;

	for (int i = 0; i < numberOfEquations; i++) {
		cout << "\nEquaion " << i + 1 << " :" << endl;
		for (j = 0; j < economicFuncVarNumber + 1; j++) {
			if (j != economicFuncVarNumber) {
				cout << "    X" << j + 1 << " :";
				cin >> nums;
				equationCoef.push_back(nums);
			}
			else {
				cout << "Right hand side :";
				cin >> nums;
				rightHandSides.push_back(nums);
			}
		}
		if (j != economicFuncVarNumber)
			equations.push_back(equationCoef);
		equationCoef.clear();
	}
	vector<vector<double> > simplex = createTable(maxFunction, equations, rightHandSides);

	return simplex;
}
/*------------------------------------------------------------------------------------------*/
vector<vector<double> > createTable(vector<double> maxFunction,
	vector<vector<double> > equations, vector<double> rightHandSides) {
	int junkValue = -1;
	vector<vector<double> > table;
	int coefficientSize = maxFunction.size();
	int rowSize = equations.size();
	int columnSize = coefficientSize + rowSize + 1 + 1;

	printMathematically(maxFunction, equations, rightHandSides);

	for (int rowIndex = 0; rowIndex < rowSize; rowIndex++) {
		vector<double> equation(columnSize - 1, 0);
		for (int colIndex = 0; colIndex < coefficientSize; colIndex++) {
			equation[colIndex] = equations[rowIndex][colIndex];
		}
		equation[coefficientSize + rowIndex] = 1;
		equation[columnSize - 2] = rightHandSides[rowIndex];
		table.push_back(equation);
	}

	vector<double> maxFuncRow(columnSize - 1, 0);

	for (int i = 0; i < coefficientSize; i++)
		maxFuncRow[i] = 0 - maxFunction[i];
	table.push_back(maxFuncRow);
	cout << "''''''''''''''''''''First Simplex Table'''''''''''''''" << endl;
	printTable(table, junkValue, junkValue);
	cout << "''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
	cout << endl << "--------------------- Processings ---------------------" << endl;
	return table;
}
/*------------------------------------------------------------------------------------------*/
bool pivotFinder(bool & hasPivot, int & pivotColumn, int & pivotRow, vector<vector<double> > table) {
	int i = 0, j = 0;
	int RowSize = table.size();
	int ColumnSize = table[0].size();
	double minimumNumber = 0;
	double minimumRatio = 0.0;
	bool checkForPivot = true;
	hasPivot = false;

	while (i < ColumnSize - 2) {
		double value = table[RowSize - 1][i];
		if (value < minimumNumber) {
			pivotColumn = i;
			minimumNumber = value;
		}
		i++;
	}
	if (minimumNumber == 0)
		return false;
	while (j < RowSize - 1) {
		double value = table[j][pivotColumn];
		if (value > 0.0) {
			double colValue = table[j][ColumnSize - 1];
			double ratio = colValue / value;
			if ((checkForPivot || ratio < minimumRatio) && ratio >= 0.0) {
				minimumRatio = ratio;
				pivotRow = j;
				checkForPivot = false;
			}
		}
		j++;
	}
	hasPivot = checkForPivot;
	return !hasPivot;
}
/*------------------------------------------------------------------------------------------*/
vector<double> makeProcess(vector<vector<double> > table, double & max) {
	int pivotColumn, pivotRow;
	int rowSize = table.size();
	int columnSize = table[0].size();
	bool hasPivot = false;
	vector<double> junkResult;
	int numVariables = columnSize - rowSize;

	while (pivotFinder(hasPivot, pivotColumn, pivotRow, table)) {
		double pivotNumber = table[pivotRow][pivotColumn];
		for (int iCol = 0; iCol < columnSize; iCol++)
			table[pivotRow][iCol] /= pivotNumber;
		for (int iRow = 0; iRow < rowSize; iRow++) {

			if (iRow != pivotRow) {
				double ratio = -1 * table[iRow][pivotColumn];
				for (int iCol = 0; iCol < columnSize; iCol++)
					table[iRow][iCol] = table[pivotRow][iCol] * ratio + table[iRow][iCol];

			}
		}
		if (debug == "1")
			printTable(table, pivotRow, pivotColumn);
	}
	if (hasPivot)
		return junkResult;
	max = table[rowSize - 1][columnSize - 1];
	vector<double> result(numVariables, 0);

	for (int i = 0; i < numVariables; i++) {
		bool notZero = true;
		bool first = true;
		double value;
		for (int j = 0; j < rowSize; j++) {
			if (table[j][i] == 1.0 && first) {
				first = false;
				value = table[j][columnSize - 1];
			}
			else if (table[j][i] != 0.0)
				notZero = false;
		}
		if (notZero && !first)
			result[i] = value;
		else
			result[i] = 0.0;
	}
	return result;
}
/*------------------------------------------------------------------------------------------*/
void printTable(vector<vector<double> > table, int & pivotRow, int & pivotColumn) {
	double tempValue;

	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].size(); j++) {
			tempValue = table[i][j];
			if (j == (table[i].size() - 1))
				cout << "|";
			if (tempValue < 0) {
				if (tempValue > -1.0 && tempValue < 0.0)
					cout << "  " << tempValue;
				if (tempValue <= -9)
					cout << "  " << tempValue;
				else if (tempValue <= -99)
					cout << " " << tempValue;
				else
					cout << "   " << tempValue;
			}
			else {
				if (tempValue >= 9)
					cout << "   " << tempValue;
				else if (tempValue >= 99)
					cout << "  " << tempValue;
				else
					cout << "    " << tempValue;
			}
		}
		if (pivotRow != -1 && pivotColumn != -1 && i == table.size() / 2)
			cout << "     Pivot : [" << pivotRow << "][" << pivotColumn << "] = " << table[pivotRow + 1][pivotColumn + 1];
		cout << endl;
		if (i == (table.size() - 2)) {
			cout << "   ";
			for (int l = 0; l< table[i].size() * 6; l++)
				cout << "-";
			cout << endl;
		}
	}
	cout << endl;
}
/*------------------------------------------------------------------------------------------*/
void printMathematically(vector<double> maxFunction, vector<vector<double> > equations, vector<double> rightHandSides) {
	cout << "''''''''''''''''''''' Your Equations '''''''''''''''''" << endl;
	cout << "    ";
	for (int i = 0; i < maxFunction.size(); i++) {

		if (i == 0)
			cout << "(" << maxFunction[i] << "X" << i + 1 << ")  ";
		else
			cout << "+  (" << maxFunction[i] << "X" << i + 1 << ")  ";
	}
	cout << "<--- Maximum Function" << endl;
	for (int i = 0; i<equations.size(); i++) {
		cout << "    ";
		for (int j = 0; j<equations[0].size(); j++) {
			if (j == 0)
				cout << "(" << equations[i][j] << "X" << j + 1 << ")  ";
			else
				cout << "+  (" << equations[i][j] << "X" << j + 1 << ")  ";
		}
		cout << " <=  " << rightHandSides[i] << endl;
	}
	cout << endl << "''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
}
/*------------------------------------------------------------------------------------------*/
void tester() {
	string status;
	int errorCheck = 0;
	vector<vector<double> > table;
	vector<double> result;
	double resultMax = 0;

	do{
		cout << "Are you want to begin with user input?(Y/N) :";
		cin >> status;
		if (status == "Y" || status == "y") {
			vector<double> maxFunction;
			vector<vector<double> > equations;
			vector<double> rightHandSides;

			maxFunction = { 2.0, -1.0, 2.0 };
			cout << endl;
			//               equation1        equation2
			//             x1   x2   x3     x1   x2   x3
			equations = { { 2.0, 1.0, 0.0 }, { 1.0, 2.0, -2.0 }, { 0.0, 1.0, 2.0 } };
			//                b1    b2
			rightHandSides = { 10.0, 20.0, 5.0 };

			table = createTable(maxFunction, equations, rightHandSides);


			result = makeProcess(table, resultMax);

			cout << "_____________________ Results _____________________" << endl;
			cout << "Maximize  = " << resultMax << endl;
			for (int i = 0; i<result.size(); i++)
				cout << "x" << i + 1 << " = " << result[i] << endl;
			cout << "___________________________________________________" << endl;
			break;

		}
		else if (status == "N" || status == "n"){
			table = input();

			result = makeProcess(table, resultMax);

			cout << "_____________________ Results _____________________" << endl;
			cout << "Maximize  = " << resultMax << endl;
			for (int i = 0; i < result.size(); i++)
				cout << "x" << i + 1 << " = " << result[i] << endl;
			cout << "___________________________________________________" << endl;
			break;
		}
		else {
			errorCheck = 1;
			cout << "You entered wrong input!" << endl;
		}
	} while (errorCheck == 1);
}
