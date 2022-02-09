/*LIC. ANA PATRICIA VALDES GONZALEZ*/
/*EJERCICIOS VISTOS EN LA CLASE DE PROGRAMACION AVANZADA*/

#include <iostream>
using namespace std;

#pragma region Declaracion de funciones

//Clase 1
void imprimirArreglo2D();
int obtenerCubo(int num);

//Clase 2

#pragma endregion

void main() {
	//Invocar aquí la función que deseas ejecutar

	system("pause");
}

#pragma region Definicion de funciones

#pragma region Clase 1

//Ejercicio 1
void imprimirArreglo2D() {
	const int size = 5;
	int miArreglo[size];

	for (int i = 0; i < size; i++) {
		printf("dato[%d] = ", i);
		cin >> miArreglo[i];
	}

	printf("\nGracias! Estos son tus valores:\n");

	for (int i = 0; i < size; i++) {
		printf("dato[%d] = %d\n", i, miArreglo[i]);
	}

	system("pause");
}

//Ejercicio 2
int obtenerCubo(int num) {
	return num * num * num;
}
#pragma endregion

#pragma region Clase 2

//Ejercicio 1
int encontrarFactorial(int num) {
	int factorial = 1;

	for (int i = 1; i <= num; i++) {
		factorial *= i;
	}

	return factorial;
}

//Ejercicio 2
int encontrarSumatoria(int sum) {
	int s = 0;
	for (int i = 1; i <= sum; i++) { s += i; }
	return s;
}
#pragma endregion

#pragma endregion