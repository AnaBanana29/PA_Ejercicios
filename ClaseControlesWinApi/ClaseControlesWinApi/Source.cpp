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
};

HINSTANCE hInstanceGlobal = 0;
HWND hHome = 0;

BOOL CALLBACK cHome(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK cAltaProducto(HWND, UINT, WPARAM, LPARAM);

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
	HWND hEdad = GetDlgItem(hWnd, SLD_EDAD);

	switch (msg)
	{
	case WM_INITDIALOG: {
		SendDlgItemMessage(hWnd, SLD_EDAD, TBM_SETRANGE, (WPARAM)FALSE, MAKELPARAM(0, 50));

		wstring especies[3] = { s2ws("Perro"), s2ws("Gato"), s2ws("Ave") };
		SendMessage(hEspecie, CB_INSERTSTRING, 0, (LPARAM)especies[0].c_str());
		SendMessage(hEspecie, CB_INSERTSTRING, 1, (LPARAM)especies[1].c_str());
		SendMessage(hEspecie, CB_INSERTSTRING, 2, (LPARAM)especies[2].c_str());
		break;

		SendMessage(hNombre, WM_SETTEXT, NULL, (LPARAM)"HOLA");
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
				GetDlgItemText(hWnd, TXT_NOMBREMASCOTA, nuevaMascota->nombreMascota, 255);

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
				MessageBox(NULL, L"Aceptar términos y condiciones", L"Aviso", MB_OK | MB_ICONASTERISK);
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