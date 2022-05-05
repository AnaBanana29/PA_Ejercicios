#include <Windows.h>
#include <string>
#include "resource.h"
#include "commctrl.h"
using namespace std;

//Handler = Puntero
//HINSTANCE = Handler de Instancia = Puntero de Instancia
//HWND = Handler de Ventana = Puntero de ventana

struct ModeloAdopcion {
	wchar_t nombre[255];
	wchar_t nombreMascota[255];
	int edad;
	wchar_t especie[255];
	int genero;
	wchar_t fecha[255];
	ModeloAdopcion* anterior;
	ModeloAdopcion* siguiente;
};

HINSTANCE hInstanceGlobal = 0;
HWND hHome = 0;

BOOL CALLBACK cHome(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK cAltaProducto(HWND, UINT, WPARAM, LPARAM);

ModeloAdopcion* buscarPorDato(ModeloAdopcion** ModeloAdopcion_inicial, ModeloAdopcion busqueda);
ModeloAdopcion* buscarPorPosicion(ModeloAdopcion** ModeloAdopcion_inicial, int posicion);
bool validarCampos(ModeloAdopcion elemento);
void insertarAlFinal(ModeloAdopcion** ModeloAdopcion_inicial, ModeloAdopcion elementoCreado);
void insertarEnPosicion(ModeloAdopcion** ModeloAdopcion_inicial, int posicion, ModeloAdopcion elementoCreado);
void insertarDespuesDe(ModeloAdopcion* anterior, ModeloAdopcion elementoCreado);
void eliminarPorPosicion(ModeloAdopcion** ModeloAdopcion_inicial, int posicion);
void eliminarPorDato(ModeloAdopcion** ModeloAdopcion_inicial, ModeloAdopcion eliminarElemento);
void editarPorPosicion(ModeloAdopcion** ModeloAdopcion_inicial, int posicion, ModeloAdopcion nuevoDato);
void editarPorDato(ModeloAdopcion** ModeloAdopcion_inicial, ModeloAdopcion busqueda, ModeloAdopcion nuevoDato);
void imprimeModeloAdopcion(ModeloAdopcion* imprimir);

bool cMenu(HWND hWnd, long opcion);
wstring s2ws(const string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	hInstanceGlobal = hInstance;
	
	hHome = CreateDialog(hInstance, MAKEINTRESOURCE(DLG_HOME), 0, cHome);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));


	ShowWindow(hHome, showCmd);
	UpdateWindow(hHome);

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

BOOL CALLBACK cHome(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//Handlers
	HWND hEspecie = GetDlgItem(hWnd, CB_ESPECIE);
	HWND hNombre = GetDlgItem(hWnd, TXT_NOMBRE);
	HWND hNombreMascota = GetDlgItem(hWnd, TXT_NOMBREMASCOTA);
	HWND hEdad = GetDlgItem(hWnd, SLD_EDAD);

	switch (msg)
	{
	case WM_INITDIALOG: {
		SetWindowText(hNombre, (s2ws("HOLA")).c_str());
		SendMessage(hNombreMascota, WM_SETTEXT, 0, (LPARAM)(s2ws("HOLA")).c_str());
		
		SendDlgItemMessage(hWnd, SLD_EDAD, TBM_SETRANGE, (WPARAM)FALSE, MAKELPARAM(0, 50));

		wstring especies[3] = { s2ws("Perro"), s2ws("Gato"), s2ws("Ave") };
		SendMessage(hEspecie, CB_INSERTSTRING, 0, (LPARAM)especies[0].c_str());
		SendMessage(hEspecie, CB_INSERTSTRING, 1, (LPARAM)especies[1].c_str());
		SendMessage(hEspecie, CB_INSERTSTRING, 2, (LPARAM)especies[2].c_str());

		break;


	}

	case WM_COMMAND: {
		long opcion = LOWORD(wParam);
		cMenu(hWnd, opcion);

		switch (LOWORD(wParam))
		{
		case BTN_ACEPTAR: {
			if (IsDlgButtonChecked(hWnd, CHB_TERMINOS) == BST_CHECKED) {
				ModeloAdopcion* nuevaMascota = new ModeloAdopcion;

				GetDlgItemText(hWnd, TXT_NOMBRE, nuevaMascota->nombre, 255);
				
				GetWindowText(hNombreMascota, nuevaMascota->nombreMascota, 255);
				
				int edad = SendMessage(hEdad, TBM_GETPOS, 0, 0);

				SYSTEMTIME fecha;
				SendDlgItemMessage(hWnd, DTP_FECHA, DTM_GETSYSTEMTIME, 0, (LPARAM)&fecha);

				int indiceEspecie = SendDlgItemMessage(hWnd, CB_ESPECIE, CB_GETCURSEL, 0, 0);

				if (IsDlgButtonChecked(hWnd, RAD_MACHO) == BST_CHECKED)
					nuevaMascota->genero = 1;
				if (IsDlgButtonChecked(hWnd, RAD_HEMBRA) == BST_CHECKED)
					nuevaMascota->genero = 2;
				if (IsDlgButtonChecked(hWnd, RAD_OTRO) == BST_CHECKED)
					nuevaMascota->genero = 3;

				SendDlgItemMessage(hWnd, LB_MASCOTASADOPTADAS, LB_ADDSTRING, 0, (LPARAM)nuevaMascota->nombreMascota);
			}else {
				MessageBox(NULL, L"Aceptar t�rminos y condiciones", L"Aviso", MB_OK | MB_ICONASTERISK);
			}

			break;
		}

		case BTN_CANCELAR: {
			DestroyWindow(hWnd);
			break;
		}

		case BTN_FOTO: {
			OPENFILENAME ofn; // common dialog box structur e
			char szFile[260]; // buffer for file name
			HANDLE hf; // file handle
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = (LPWSTR)szFile;
			// Set lpstrFile[0] to 0' so that GetOpenFileName does not
			// use the contents of szFile to initialize itself
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"Bitmap\0 * .bmp";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn) == true) {
				SetWindowText(GetDlgItem(hWnd, TXT_RUTAFOTO), ofn.lpstrFile);
			}
			else {
				MessageBox(0, L"No eligio archivo ", L"Aviso", MB_OK | MB_ICONINFORMATION);
			}

			wchar_t rutaFoto[255];
			GetDlgItemText(hWnd, TXT_RUTAFOTO, rutaFoto, 255);

			static HBITMAP bmp = (HBITMAP)LoadImage(NULL, rutaFoto, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, PC_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

			break;
		}

		default:
			break;
		}
		break;
	}

	case WM_CLOSE: {
		DestroyWindow(hWnd);
		break;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
	}

	default:
		break;
	}

	return false;
}

BOOL CALLBACK cAltaProducto(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		MessageBox(0, L"Bienvenido!", L"Hola", MB_OKCANCEL | MB_ICONHAND);
		break;
	}

	case WM_COMMAND: {
		long opcion = LOWORD(wParam);
		cMenu(hWnd, opcion);

		switch (LOWORD(wParam))
		{
		case BTN_REGRESAR: {
			ShowWindow(hHome, SW_RESTORE);
			break;
		}
		case BTN_CERRAR: {
			DestroyWindow(hWnd);
			break;
		}
		default:
			break;
		}
		break;
	}

	case WM_CLOSE: {
		DestroyWindow(hWnd);
		break;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
	}

	default:
		break;
	}

	return false;
}

bool cMenu(HWND hWnd, long opcion) {
	switch (opcion)
	{
	case ID_ABRIR_ABRIR3: {
		MessageBox(NULL, L"Hola desde abrir 3", L"Hola", MB_OK);
		break;
	}
	case ID_PRODUCTOS_ALTADEPRODUCTO: {
		EndDialog(hWnd, 0);

		HWND hAltaProducto = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_ALTAPRODUCTO), 0, cAltaProducto);

		ShowWindow(hAltaProducto, SW_SHOW);
		UpdateWindow(hAltaProducto);

		break;
	}
	default:
		break;
	}

	return true;
}
//
//ModeloAdopcion* buscarPorNombre(ModeloAdopcion** ModeloAdopcion_inicial, string busqueda) {
//	//Creamos un puntero para usar como �ndice para recorrer la lista
//	ModeloAdopcion* indice = *ModeloAdopcion_inicial;
//
//	//Creamos un puntero en donde almacenaremos el elemento encontrado
//	//Si no encuentra nada, seguir� siendo NULL
//	ModeloAdopcion* encontrado = NULL;
//
//	//Ciclo para recorrer la lista
//	while (indice != NULL) {
//		//Si se especificaron ambos datos del ModeloAdopcion, buscar que ambos coincidan
//		//Si solo se especific� uno de ellos, hace la comparaci�n con el que se haya mandado
//		//Una vez encontrado el ModeloAdopcion, rompe el ciclo
//		wchar_t nombre[255];
//		wchar_t nombreMascota[255];
//		int edad;
//		wchar_t especie[255];
//		int genero;
//		wchar_t fecha[255];
//		if (busqueda != "" && indice->nombre == s2ws(busqueda)) {
//			encontrado = indice;
//			break;
//		}
//		indice = indice->siguiente;
//	}
//
//	return encontrado;
//}
//bool validarCampos(ModeloAdopcion elemento) {
//	if (elemento.nombre == L"" || elemento.nombre == NULL)
//		return false;
//
//	if (elemento.nombreMascota == L"" || elemento.nombre == NULL)
//		return false;
//
//	if (elemento.edad == NULL || elemento.edad < 0)
//		return false;
//
//	if (elemento.especie == L"" || elemento.especie == NULL)
//		return false;
//
//	if (elemento.genero == NULL || elemento.genero < 0)
//		return false;
//
//	if (elemento.fecha == L"" || elemento.fecha == NULL)
//		return false;
//
//	return true;
//}
//
//void insertarAlFinal(ModeloAdopcion** ModeloAdopcion_inicial, ModeloAdopcion elementoCreado) {
//	//Validar campos
//	if (!validarCampos(elementoCreado)) {
//		MessageBox(0, L"Favor de ingresar todos los datos", L"Error", MB_OK | MB_ICONERROR);
//		return;
//	}
//	wchar_t nombre[255];
//	wchar_t nombreMascota[255];
//	int edad;
//	wchar_t especie[255];
//	int genero;
//	wchar_t fecha[255];
//
//
//	/* ========== Esto solo va a pasar si los datos fueron validados  ========== */
//
//	//Crear nuevo ModeloAdopcion
//	ModeloAdopcion* nuevo = new ModeloAdopcion;
//	memccpy(nuevo->nombre, &elementoCreado.nombre, 255);
//	nuevo->nombre = elementoCreado.nombre;
//	nuevo->nombreMascota = elementoCreado.nombreMascota;
//	nuevo->siguiente = NULL;
//
//	//Revisar si la lista est� vac�a
//	if (*ModeloAdopcion_inicial == NULL) {
//		//Si la lista est� vac�a, hace que el puntero ModeloAdopcion_INICIAL
//		//apunte al nuevo ModeloAdopcion, ya que ser� el primero
//		*ModeloAdopcion_inicial = nuevo;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si la lista no est� vac�a  ========== */
//
//	//Creamos un puntero para usar como �ndice para recorrer la lista
//	ModeloAdopcion* indice = *ModeloAdopcion_inicial;
//
//	//Recorremos la lista hasta encontrar el �ltimo elemento
//	while (indice->siguiente != NULL) {
//		indice = indice->siguiente;
//	}
//
//	//Conectamos el �ltimo elemento al ModeloAdopcion nuevo
//	indice->siguiente = nuevo;
//	nuevo->anterior = indice;
//
//
//}
//void insertarEnPosicion(ModeloAdopcion** ModeloAdopcion_inicial, int posicion, ModeloAdopcion elementoCreado) {
//	//Validar campos
//	if (!validarCampos(elementoCreado)) {
//		cout << "ModeloAdopcion invalido" << endl;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si los datos fueron validados  ========== */
//
//	//Crear nuevo ModeloAdopcion
//	ModeloAdopcion* nuevo = new ModeloAdopcion{
//		elementoCreado.nombre,
//		elementoCreado.edad,
//		NULL,
//		NULL
//	};
//
//	//Revisar si la lista est� vac�a
//	if (*ModeloAdopcion_inicial == NULL) {
//		//Si la lista est� vac�a, hace que el puntero ModeloAdopcion_INICIAL
//		//apunte al nuevo ModeloAdopcion, ya que ser� el primero
//		*ModeloAdopcion_inicial = nuevo;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si la lista no est� vac�a  ========== */
//
//	//Revisa si se quiere insertar en la primera posici�n
//	if (posicion == 1) {
//		nuevo->siguiente = *ModeloAdopcion_inicial;
//		*ModeloAdopcion_inicial = nuevo;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si la no se quiere insertar ========== */
//	/* ======================= en la primera posici�n ======================= */
//
//	ModeloAdopcion* indice = *ModeloAdopcion_inicial;
//
//	//Ciclo para llegar a la posici�n deseada
//	//�Por qu� crees que el for se detiene en posicion-2?
//	//Utiliza breakpoints para confirmar tu respuesta
//	for (int i = 0; i < posicion - 2; i++) {
//		//Romper el ciclo si llegamos al �ltimo elemento para no insertar
//		//en una posici�n que no existe en la lista
//		if (indice->siguiente == NULL)
//			break;
//
//		indice = indice->siguiente;
//	}
//
//	//Conectamos el nuevo ModeloAdopcion a los ModeloAdopcions existentes previamente
//	nuevo->siguiente = indice->siguiente;
//	nuevo->anterior = indice;
//	indice->siguiente = nuevo;
//}
//void insertarDespuesDe(ModeloAdopcion* anterior, ModeloAdopcion elementoCreado) {
//	//Validar campos
//	if (!validarCampos(elementoCreado)) {
//		cout << "ModeloAdopcion invalido" << endl;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si los datos fueron validados  ========== */
//
//	//Revisar si el ModeloAdopcion anterior es NULL
//	if (anterior == NULL) {
//		cout << "ModeloAdopcion anterior no puede ser NULL";
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si el ModeloAdopcion anterior no es NULL  ========== */
//
//	//Crear nuevo ModeloAdopcion
//	ModeloAdopcion* nuevo = new ModeloAdopcion{
//		elementoCreado.nombre,
//		elementoCreado.edad,
//		NULL
//	};
//
//	//Insertar nuevo ModeloAdopcion despu�s del anterior que recibimos como par�metro
//	nuevo->siguiente = anterior->siguiente;
//	nuevo->anterior = anterior;
//	anterior->siguiente = nuevo;
//}
//
//void eliminarPorPosicion(ModeloAdopcion** ModeloAdopcion_inicial, int posicion) {
//	ModeloAdopcion* encontrado = buscarPorPosicion(ModeloAdopcion_inicial, posicion);
//
//	//Revisar que se haya encontrado el ModeloAdopcion
//	if (encontrado == NULL) {
//		cout << "ModeloAdopcion no encontrado" << endl;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si el elemento fue encontrado  ========== */
//
//	//Si el ModeloAdopcion es el primero de la lista, cambia el valor del ModeloAdopcion_INICIAL
//	//Si no es el primero, ligar el elemento anterior con el siguiente al ModeloAdopcion proporcionado
//	if (encontrado->anterior == NULL)
//		*ModeloAdopcion_inicial = encontrado->siguiente;
//	else
//		encontrado->anterior->siguiente = encontrado->siguiente;
//
//	//Si no es el �ltimo elemento, ligar el elemento siguiente con el anterior
//	//al ModeloAdopcion proporcionado
//	if (encontrado->siguiente != NULL)
//		encontrado->siguiente->anterior = encontrado->anterior;
//
//	delete encontrado;
//	encontrado = NULL;
//}
//void eliminarPorDato(ModeloAdopcion** ModeloAdopcion_inicial, ModeloAdopcion eliminarElemento) {
//	//Buscar el ModeloAdopcion proporcionado
//	ModeloAdopcion* encontrado = buscarPorDato(ModeloAdopcion_inicial, eliminarElemento);
//
//	//Revisar que se haya encontrado el ModeloAdopcion
//	if (encontrado == NULL) {
//		cout << "ModeloAdopcion no encontrado" << endl;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si el elemento fue encontrado  ========== */
//
//	//Si el ModeloAdopcion es el primero de la lista, cambia el valor del ModeloAdopcion_INICIAL
//	//Si no es el primero, ligar el elemento anterior con el siguiente al ModeloAdopcion proporcionado
//	if (encontrado->anterior == NULL)
//		*ModeloAdopcion_inicial = encontrado->siguiente;
//	else
//		encontrado->anterior->siguiente = encontrado->siguiente;
//
//	//Si no es el �ltimo elemento, ligar el elemento siguiente con el anterior
//	//al ModeloAdopcion proporcionado
//	if (encontrado->siguiente != NULL)
//		encontrado->siguiente->anterior = encontrado->anterior;
//
//	delete encontrado;
//	encontrado = NULL;
//}
//
//void editarPorPosicion(ModeloAdopcion** ModeloAdopcion_inicial, int posicion, ModeloAdopcion nuevoDato) {
//	//Buscar el ModeloAdopcion proporcionado
//	ModeloAdopcion* encontrado = buscarPorPosicion(ModeloAdopcion_inicial, posicion);
//
//	//Revisar que se haya encontrado el ModeloAdopcion
//	if (encontrado == NULL) {
//		cout << "ModeloAdopcion no encontrado" << endl;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si el elemento fue encontrado  ========== */
//
//	//Actualizar los datos que hayan sido especificados
//	if (nuevoDato.edad != NULL && nuevoDato.edad > 0)
//		encontrado->edad = nuevoDato.edad;
//
//	if (nuevoDato.nombre != "")
//		encontrado->nombre = nuevoDato.nombre;
//}
//void editarPorDato(ModeloAdopcion** ModeloAdopcion_inicial, ModeloAdopcion busqueda, ModeloAdopcion nuevoDato) {
//	//Buscar el ModeloAdopcion proporcionado
//	ModeloAdopcion* encontrado = buscarPorDato(ModeloAdopcion_inicial, busqueda);
//
//	//Revisar que se haya encontrado el ModeloAdopcion
//	if (encontrado == NULL) {
//		cout << "ModeloAdopcion no encontrado" << endl;
//		return;
//	}
//
//	/* ========== Esto solo va a pasar si el elemento fue encontrado  ========== */
//
//	//Actualizar los datos que hayan sido especificados
//	if (nuevoDato.edad != NULL && nuevoDato.edad > 0)
//		encontrado->edad = nuevoDato.edad;
//
//	if (nuevoDato.nombre != "")
//		encontrado->nombre = nuevoDato.nombre;
//}
//
//void guardaModeloAdopcions(ModeloAdopcion** ModeloAdopcion_inicial) {
//	//Creamos la variable para escritura
//	ofstream archivo(nombreArchivo, ios::out | ios::app | ios::binary);
//
//	//Confirmamos que haya sido abierto correctamente
//	if (archivo.is_open()) {
//		//Recorremos la lista y guardamos cada ModeloAdopcion
//		ModeloAdopcion* indice = *ModeloAdopcion_inicial;
//		while (indice != NULL) {
//			archivo.write((char*)indice, sizeof(ModeloAdopcion));
//			indice = indice->siguiente;
//		}
//
//		//Cerramos el archivo
//		archivo.close();
//	}
//}
//
//void leeModeloAdopcions(ModeloAdopcion** ModeloAdopcion_inicial) {
//	//Creamos la variable de lectura
//	ifstream archivo(nombreArchivo, ios::in | ios::binary);
//
//	//Variable para almacenar los ModeloAdopcions del archivo
//	ModeloAdopcion* ModeloAdopcionLeido;
//
//	//Verificar que el archivo se haya abierto correctamente
//	if (archivo.is_open()) {
//		//Ubicar el cursor al final del archivo
//		archivo.seekg(0, ios::end);
//		//Almacenar la posici�n en que se encuentra el cursor
//		//Como lo movimos al final, nos va a decir cuantos caracteres hay en total
//		int tam = archivo.tellg();
//		//Regresar el cursor al inicio del archivo
//		archivo.seekg(0, ios::beg);
//
//		//Ciclo para leer el archivo mientras no lleguemos a su final
//		while (archivo.tellg() < tam) {
//			//Crear nuevo ModeloAdopcion con la info del archivo
//			ModeloAdopcionLeido = new ModeloAdopcion;
//			archivo.read((char*)ModeloAdopcionLeido, sizeof(ModeloAdopcion));
//			ModeloAdopcionLeido->siguiente = NULL;
//
//			if (*ModeloAdopcion_inicial == NULL) {
//				*ModeloAdopcion_inicial = ModeloAdopcionLeido;
//				ModeloAdopcionLeido->anterior = NULL;
//			}
//			else {
//				ModeloAdopcion* indice = *ModeloAdopcion_inicial;
//
//				while (indice->siguiente != NULL) {
//					indice = indice->siguiente;
//				}
//
//				indice->siguiente = ModeloAdopcionLeido;
//				ModeloAdopcionLeido->anterior = indice;
//			}
//		}
//
//
//		archivo.close();
//	}
//}