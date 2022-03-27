/*LIC. ANA PATRICIA VALDES GONZALEZ*/
/*EJERCICIOS VISTOS EN LA CLASE DE PROGRAMACION AVANZADA*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#pragma region Declaracion de funciones y variables globales

#pragma region Clase1

void imprimirArreglo2D();
int obtenerCubo(int num);

#pragma endregion

#pragma region Clase2

int encontrarFactorial(int num);
int encontrarSumatoria(int sum);

#pragma endregion

#pragma region Listas Ligadas

struct nodo {
	string nombre;
	int edad;
	nodo* anterior;
	nodo* siguiente;
};

nodo* buscarPorDato(nodo** nodo_inicial, nodo busqueda);
nodo* buscarPorPosicion(nodo** nodo_inicial, int posicion);
bool validarCampos(nodo elemento);
void insertarAlFinal(nodo** nodo_inicial, nodo elementoCreado);
void insertarEnPosicion(nodo** nodo_inicial, int posicion, nodo elementoCreado);
void insertarDespuesDe(nodo* anterior, nodo elementoCreado);
void eliminarPorPosicion(nodo** nodo_inicial, int posicion);
void eliminarPorDato(nodo** nodo_inicial, nodo eliminarElemento);
void editarPorPosicion(nodo** nodo_inicial, int posicion, nodo nuevoDato);
void editarPorDato(nodo** nodo_inicial, nodo busqueda, nodo nuevoDato);
void imprimeNodo(nodo* imprimir);

#pragma endregion

#pragma region Archivos Binarios

string nombreArchivo = "nodos.bin";

void guardaNodos(nodo** nodo_inicial);
void leeNodos(nodo** nodo_inicial);

#pragma endregion


#pragma endregion

void main() {
	//Invocar aquí la función que deseas ejecutar

	nodo* NODO_INICIAL = NULL;

	leeNodos(&NODO_INICIAL);

	insertarEnPosicion(&NODO_INICIAL, 1, { "Nombre 1", 27, NULL });
	insertarAlFinal(&NODO_INICIAL, { "Nombre 2", 27, NULL });

	guardaNodos(&NODO_INICIAL);

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

#pragma region Listas Ligadas
nodo* buscarPorDato(nodo** nodo_inicial, nodo busqueda) {
	//Creamos un puntero para usar como índice para recorrer la lista
	nodo* indice = *nodo_inicial;

	//Creamos un puntero en donde almacenaremos el elemento encontrado
	//Si no encuentra nada, seguirá siendo NULL
	nodo* encontrado = NULL;

	//Ciclo para recorrer la lista
	while (indice != NULL) {
		//Si se especificaron ambos datos del nodo, buscar que ambos coincidan
		//Si solo se especificó uno de ellos, hace la comparación con el que se haya mandado
		//Una vez encontrado el nodo, rompe el ciclo
		if ((busqueda.edad != NULL || busqueda.nombre != "") &&
			((indice->edad == busqueda.edad || busqueda.edad == NULL) &&
			(indice->nombre == busqueda.nombre || busqueda.nombre == ""))) {
			encontrado = indice;
			break;
		}
		indice = indice->siguiente;
	}

	return encontrado;
}
nodo* buscarPorPosicion(nodo** nodo_inicial, int posicion) {
	//Creamos un puntero para usar como índice para recorrer la lista
	nodo* indice = *nodo_inicial;

	//Revisar si la lista está vacía
	if (*nodo_inicial == NULL) {
		cout << "La lista esta vacia" << endl;
		return NULL;
	}

	for (int i = 1; i < posicion; i++) {
		if (indice->siguiente == NULL)
			break;

		indice = indice->siguiente;
	}

	return indice;
}
bool validarCampos(nodo elemento) {
	if (elemento.nombre == "")
		return false;

	if (elemento.edad == NULL || elemento.edad < 0)
		return false;

	return true;
}

void insertarAlFinal(nodo** nodo_inicial, nodo elementoCreado) {
	//Validar campos
	if (!validarCampos(elementoCreado)) {
		cout << "Nodo invalido" << endl;
		return;
	}

	/* ========== Esto solo va a pasar si los datos fueron validados  ========== */

	//Crear nuevo nodo
	nodo* nuevo = new nodo;
	nuevo->edad = elementoCreado.edad;
	nuevo->nombre = elementoCreado.nombre;
	nuevo->siguiente = NULL;

	//Revisar si la lista está vacía
	if (*nodo_inicial == NULL) {
		//Si la lista está vacía, hace que el puntero NODO_INICIAL
		//apunte al nuevo nodo, ya que será el primero
		*nodo_inicial = nuevo;
		return;
	}

	/* ========== Esto solo va a pasar si la lista no está vacía  ========== */

	//Creamos un puntero para usar como índice para recorrer la lista
	nodo* indice = *nodo_inicial;

	//Recorremos la lista hasta encontrar el último elemento
	while (indice->siguiente != NULL) {
		indice = indice->siguiente;
	}

	//Conectamos el último elemento al nodo nuevo
	indice->siguiente = nuevo;
	nuevo->anterior = indice;


}
void insertarEnPosicion(nodo** nodo_inicial, int posicion, nodo elementoCreado) {
	//Validar campos
	if (!validarCampos(elementoCreado)) {
		cout << "Nodo invalido" << endl;
		return;
	}

	/* ========== Esto solo va a pasar si los datos fueron validados  ========== */

	//Crear nuevo nodo
	nodo* nuevo = new nodo{
		elementoCreado.nombre,
		elementoCreado.edad,
		NULL,
		NULL
	};

	//Revisar si la lista está vacía
	if (*nodo_inicial == NULL) {
		//Si la lista está vacía, hace que el puntero NODO_INICIAL
		//apunte al nuevo nodo, ya que será el primero
		*nodo_inicial = nuevo;
		return;
	}

	/* ========== Esto solo va a pasar si la lista no está vacía  ========== */

	//Revisa si se quiere insertar en la primera posición
	if (posicion == 1) {
		nuevo->siguiente = *nodo_inicial;
		*nodo_inicial = nuevo;
		return;
	}

	/* ========== Esto solo va a pasar si la no se quiere insertar ========== */
	/* ======================= en la primera posición ======================= */

	nodo* indice = *nodo_inicial;

	//Ciclo para llegar a la posición deseada
	//¿Por qué crees que el for se detiene en posicion-2?
	//Utiliza breakpoints para confirmar tu respuesta
	for (int i = 0; i < posicion - 2; i++) {
		//Romper el ciclo si llegamos al último elemento para no insertar
		//en una posición que no existe en la lista
		if (indice->siguiente == NULL)
			break;
		
		indice = indice->siguiente;
	}

	//Conectamos el nuevo nodo a los nodos existentes previamente
	nuevo->siguiente = indice->siguiente;
	nuevo->anterior = indice;
	indice->siguiente = nuevo;
}
void insertarDespuesDe(nodo* anterior, nodo elementoCreado) {
	//Validar campos
	if (!validarCampos(elementoCreado)) {
		cout << "Nodo invalido" << endl;
		return;
	}

	/* ========== Esto solo va a pasar si los datos fueron validados  ========== */

	//Revisar si el nodo anterior es NULL
	if (anterior == NULL) {
		cout << "Nodo anterior no puede ser NULL";
		return;
	}

	/* ========== Esto solo va a pasar si el nodo anterior no es NULL  ========== */
	
	//Crear nuevo nodo
	nodo* nuevo = new nodo{
		elementoCreado.nombre,
		elementoCreado.edad,
		NULL
	};

	//Insertar nuevo nodo después del anterior que recibimos como parámetro
	nuevo->siguiente = anterior->siguiente;
	nuevo->anterior = anterior;
	anterior->siguiente = nuevo;
}

/*Función vista en clase - SOLO FUNCIONA CON LISTAS LIGADAS SIMPLES*/
//void eliminarPorPosicion(nodo** nodo_inicial, int posicion) {
//	//Creación de nodos auxiliares
//	nodo* anterior = *nodo_inicial;
//	nodo* actual = *nodo_inicial;
//
//	//Revisar si la lista está vacía
//	if (*nodo_inicial == NULL) {
//		cout << "La lista esta vacia" << endl;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si la lista no está vacía  ========== */
//
//	//Revisa si se quiere eliminar el primer elemento
//	if (posicion == 1) 
//		*nodo_inicial = actual->siguiente;
//
//	for (int i = posicion; i > 1; i--) {
//		//Romper el ciclo si llegamos al último elemento para no insertar
//		//en una posición que no existe en la lista
//		if (actual->siguiente == NULL)
//			break;
//
//		anterior = actual;
//		actual = anterior->siguiente;
//	}
//
//	//Recuperar la conexión de los nodos
//	anterior->siguiente = actual->siguiente;
//	actual->siguiente->anterior = anterior;
//
//	//Eliminar el nodo
//	delete actual;
//	actual = NULL;
//}

void eliminarPorPosicion(nodo** nodo_inicial, int posicion) {
	nodo * encontrado = buscarPorPosicion(nodo_inicial, posicion);

	//Revisar que se haya encontrado el nodo
	if (encontrado == NULL) {
		cout << "Nodo no encontrado" << endl;
		return;
	}

	/* ========== Esto solo va a pasar si el elemento fue encontrado  ========== */

	//Si el nodo es el primero de la lista, cambia el valor del NODO_INICIAL
	//Si no es el primero, ligar el elemento anterior con el siguiente al nodo proporcionado
	if (encontrado->anterior == NULL)
		*nodo_inicial = encontrado->siguiente;
	else
		encontrado->anterior->siguiente = encontrado->siguiente;

	//Si no es el último elemento, ligar el elemento siguiente con el anterior
	//al nodo proporcionado
	if (encontrado->siguiente != NULL)
		encontrado->siguiente->anterior = encontrado->anterior;

	delete encontrado;
	encontrado = NULL;
}
void eliminarPorDato(nodo** nodo_inicial, nodo eliminarElemento) {
	//Buscar el nodo proporcionado
	nodo* encontrado = buscarPorDato(nodo_inicial, eliminarElemento);

	//Revisar que se haya encontrado el nodo
	if (encontrado == NULL) {
		cout << "Nodo no encontrado" << endl;
		return;
	}

	/* ========== Esto solo va a pasar si el elemento fue encontrado  ========== */

	//Si el nodo es el primero de la lista, cambia el valor del NODO_INICIAL
	//Si no es el primero, ligar el elemento anterior con el siguiente al nodo proporcionado
	if (encontrado->anterior == NULL) 
		*nodo_inicial = encontrado->siguiente;
	else
		encontrado->anterior->siguiente = encontrado->siguiente;

	//Si no es el último elemento, ligar el elemento siguiente con el anterior
	//al nodo proporcionado
	if (encontrado->siguiente != NULL)
		encontrado->siguiente->anterior = encontrado->anterior;

	delete encontrado;
	encontrado = NULL;	
}

void editarPorPosicion(nodo** nodo_inicial, int posicion, nodo nuevoDato) {
	//Buscar el nodo proporcionado
	nodo* encontrado = buscarPorPosicion(nodo_inicial, posicion);

	//Revisar que se haya encontrado el nodo
	if (encontrado == NULL) {
		cout << "Nodo no encontrado" << endl;
		return;
	}

	/* ========== Esto solo va a pasar si el elemento fue encontrado  ========== */

	//Actualizar los datos que hayan sido especificados
	if (nuevoDato.edad != NULL && nuevoDato.edad > 0) 
		encontrado->edad = nuevoDato.edad;

	if (nuevoDato.nombre != "")
		encontrado->nombre = nuevoDato.nombre;
}
void editarPorDato(nodo** nodo_inicial, nodo busqueda, nodo nuevoDato) {
	//Buscar el nodo proporcionado
	nodo* encontrado = buscarPorDato(nodo_inicial, busqueda);

	//Revisar que se haya encontrado el nodo
	if (encontrado == NULL) {
		cout << "Nodo no encontrado" << endl;
		return;
	}

	/* ========== Esto solo va a pasar si el elemento fue encontrado  ========== */

	//Actualizar los datos que hayan sido especificados
	if (nuevoDato.edad != NULL && nuevoDato.edad > 0)
		encontrado->edad = nuevoDato.edad;

	if (nuevoDato.nombre != "")
		encontrado->nombre = nuevoDato.nombre;
}
#pragma endregion

#pragma region Archivos Binarios

void guardaNodos(nodo** nodo_inicial) {
	//Creamos la variable para escritura
	ofstream archivo(nombreArchivo, ios::out | ios::app | ios::binary);

	//Confirmamos que haya sido abierto correctamente
	if (archivo.is_open()) {
		//Recorremos la lista y guardamos cada nodo
		nodo* indice = *nodo_inicial;
		while (indice != NULL) {
			archivo.write((char*)indice, sizeof(nodo));
			indice = indice->siguiente;
		}

		//Cerramos el archivo
		archivo.close();
	}
}

void leeNodos(nodo** nodo_inicial) {
	//Creamos la variable de lectura
	ifstream archivo(nombreArchivo, ios::in | ios::binary);

	//Variable para almacenar los nodos del archivo
	nodo* nodoLeido;

	//Verificar que el archivo se haya abierto correctamente
	if (archivo.is_open()) {
		//Ubicar el cursor al final del archivo
		archivo.seekg(0, ios::end);
		//Almacenar la posición en que se encuentra el cursor
		//Como lo movimos al final, nos va a decir cuantos caracteres hay en total
		int tam = archivo.tellg();
		//Regresar el cursor al inicio del archivo
		archivo.seekg(0, ios::beg);

		//Ciclo para leer el archivo mientras no lleguemos a su final
		while (archivo.tellg() < tam) {
			//Crear nuevo nodo con la info del archivo
			nodoLeido = new nodo;
			archivo.read((char*)nodoLeido, sizeof(nodo));
			nodoLeido->siguiente = NULL;

			if (*nodo_inicial == NULL) {
				*nodo_inicial = nodoLeido;
				nodoLeido->anterior = NULL;
			}
			else {
				nodo* indice = *nodo_inicial;

				while (indice->siguiente != NULL) {
					indice = indice->siguiente;
				}

				indice->siguiente = nodoLeido;
				nodoLeido->anterior = indice;
			}
		}


		archivo.close();
	}
}

#pragma endregion

#pragma endregion