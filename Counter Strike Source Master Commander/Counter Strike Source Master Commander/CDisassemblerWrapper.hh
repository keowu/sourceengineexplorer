/*
 _   __ _____ _____  _    _ _   _
| | / /|  ___|  _  || |  | | | | |
| |/ / | |__ | | | || |  | | | | |
|    \ |  __|| | | || |/\| | | | |
| |\  \| |___\ \_/ /\  /\  / |_| |
\_| \_/\____/ \___/  \/  \/ \___/
							2022

Copyright (c) Fluxuss Cyber Tech Desenvolvimento de Software, SLU (FLUXUSS)

Algumas estruturas presentes neste projeto podem ser pertencentes ao código fonte do Windows cujo proprietário e detentor se denomina Microsoft(Copyright (c) Microsoft Corporation)
sua utilização é feita de acordo com atos "legais" de acordo com leis, como a Lei de direito autoral Brasileira e Lei Americana de Direito Autoral.
alguns desses dados foram extraídos da ntdll.h extraída do código fonte do React Os(Copyright (c) React Os), e complementadas com o código fonte vazado do Windows XP.

Esse código fonte está progido de acordo com Copyright Act of 1976 lei aplicável a devida parte interessadas Microsoft, Valve(Copyright (c) Valve Corporation).
Esse código se aplica "ao uso justo", mais informações https://www.copyright.gov/title17/.

Obrigado Open Source <3

Com todo respeito e nada mais a declarar.


"The Kurumi Source Engine explorer Engine"

THE CAKE IS A LIE!

  ^^
Keowu

*/
#pragma once
#include <functional>
#include <map>
#include <filesystem>
#include "CAssemblyIterator.hh"
#include "CVacOsSyscall.hh"

class CDisassemblerWrapper {

private:
	CAssemblyIterator* itControll = new CAssemblyIterator( );

	std::function< std::string ( unsigned char*, size_t ) > ukDisasm = [ & ]( unsigned char* bpOpcodes, size_t szpOpcodes ) -> std::string {

		return "";
	};

	std::function < std::pair< size_t, uintptr_t > ( uintptr_t ) > ukPredictFunctionSz = [ & ] ( uintptr_t upInitAdd ) ->  std::pair< size_t, uintptr_t > {

		unsigned char* ucBuffer = new unsigned char[ 1024 ];

		auto hProc = CVacOsSyscall::getProcessHandle( );

		if ( !ReadProcessMemory(

			_In_ hProc,
			_In_ (LPCVOID)upInitAdd,
			_Out_ ucBuffer,
			_In_ 1024,
			_Out_opt_ NULL

		) ) return { -1, -1 };
		
		auto i = 0;

		unsigned char chOpcode = 0;

		uintptr_t upEnderecoFinal = upInitAdd;

		size_t szProc = 0;

		while ( i < 1024 ) {
			
			chOpcode = *( ucBuffer + i );

			if ( chOpcode == 0xC3 ) return { upEnderecoFinal, szProc };

			upEnderecoFinal++, szProc++, i++;

		}
		
		if ( !CloseHandle(

			_In_ hProc

		) ) return { -1, -1 };

		return { -1, -1 };
	};

public:

	std::function< std::pair< std::string, std::string > ( std::string, ULONGLONG, ULONGLONG ) > wrpDisasmBegin = [ & ] ( std::string moduleName, ULONGLONG upEnderecoInicio, ULONGLONG upEnderecoFinal ) -> std::pair< std::string, std::string > {

		this->itControll->setInitAdd( upEnderecoInicio );

		this->itControll->setEndAdd( upEnderecoFinal );

		size_t szRoutina = 0;

		if ( !upEnderecoFinal )
			std::invoke([ & ] ( ) {

			auto [szRoutinax, upEnderecoFinalx ] = ukPredictFunctionSz( upEnderecoInicio );

			upEnderecoFinal = upEnderecoFinalx;

			szRoutina = szRoutinax;

		});

		if ( upEnderecoFinal == -1 || szRoutina == -1 ) return { "Erro", "Erro Diasssembly engine, não é possível determinar o endereço final e seu tamanho !" };

		uint8_t* temp = new uint8_t[ ( upEnderecoFinal - upEnderecoInicio ) ]{ 0 };

		auto hProc = CVacOsSyscall::getProcessHandle( );

		SIZE_T szNumberBytesRead = 0;

		if ( !ReadProcessMemory(

			_In_ hProc,
			_In_ (LPCVOID)upEnderecoInicio,
			_Out_ temp,
			_In_ upEnderecoFinal - upEnderecoInicio,
			_Out_opt_ &szNumberBytesRead

		) )
			return { "Error", "Disassembler Engine - ReadProcessMemory falhou!" };

		std::wstring strPath( std::filesystem::temp_directory_path( ).wstring( ) );

		DeleteFile(
			_In_ std::wstring( strPath ).append( L"indiasm.bin" ).c_str( )
		);

		DeleteFile(
			_In_ std::wstring( strPath ).append( L"outdiasm.bin" ).c_str( )
		);

		HANDLE hFile = CreateFile(

			_In_ std::wstring( strPath ).append( L"indiasm.bin" ).c_str( ),
			_In_ GENERIC_WRITE,
			_In_ 0,
			_In_opt_ NULL,
			_In_ CREATE_NEW,
			_In_ FILE_ATTRIBUTE_NORMAL,
			_In_opt_ NULL

		);

		if ( hFile == INVALID_HANDLE_VALUE ) return { "Error hFile", "INVALID_HANDLE_VALUE para o resultado da engine" };
		

		DWORD dwWritten = 0;

		WriteFile(

			_In_ hFile,
			_In_ temp,
			_In_ upEnderecoFinal - upEnderecoInicio,
			_Out_opt_ &dwWritten,
			_Inout_opt_ NULL

		);

		if ( dwWritten != upEnderecoFinal - upEnderecoInicio ) return { "Error dwWritten e temp", "dwWritten != (upEnderecoFinal - upEnderecoInicio) difere do resultado esperado da engine" };

		CloseHandle(

			_In_ hFile

		);

		STARTUPINFO sInfo{ 0 };

		PROCESS_INFORMATION pInfo{ 0 };

		if ( !CreateProcess(

			_In_opt_ std::wstring( std::filesystem::current_path( ).wstring( ) ).append( L"\\KurumiDisasmEngineCon.exe" ).c_str( ),
			_Inout_opt_ NULL,
			_In_opt_ NULL,
			_In_opt_ NULL,
			_In_ FALSE,
			_In_ NORMAL_PRIORITY_CLASS, //| CREATE_NO_WINDOW
			_In_opt_ NULL,
			_In_opt_ NULL,
			_In_ &sInfo,
			_Out_ &pInfo

		) ) {

			MessageBox( NULL, std::to_wstring( GetLastError( ) ).c_str( ), L"ERROR", NULL );

			return { "Error create process", "Erro ao criar processo da engine de disassembler !" };

		}
		
		//Usando mutex para esperar o HANDLE do processo, ou seja meu mutex espera e "sincroniza" até que o handle seja encerrado pelo sistema !
		WaitForSingleObject(

			_In_ pInfo.hProcess,
			_In_ 16000 //INFINITE

		);

		CloseHandle(

			_In_ pInfo.hProcess

		);

		CloseHandle(

			_In_ pInfo.hThread

		);

		hFile = CreateFile(

			_In_ std::wstring( std::filesystem::temp_directory_path( ).wstring( ) ).append( L"outdiasm.bin" ).c_str( ),
			_In_ GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,
			_In_ FILE_SHARE_READ,
			_In_opt_ NULL,
			_In_ OPEN_EXISTING,
			_In_ FILE_ATTRIBUTE_NORMAL,
			_In_opt_ NULL

		);

		if ( hFile == INVALID_HANDLE_VALUE ) return { "Error hFile", "INVALID_HANDLE_VALUE para hFile difere do esperado pela engine " + std::to_string(GetLastError())};
		

		DWORD dwFileSize = GetFileSize(

			_In_ hFile,
			_Out_opt_ NULL

		);

		if ( dwFileSize == 0 ) return { "Error dwFileSize", "dwFileSize é zero difere do esperado pela engine" };

		char* chMappedFile = new char[ dwFileSize ]{ 0 };

		DWORD dwNumberBytesRead = 0;

		ReadFile(

			_In_ hFile,
			_Out_ chMappedFile,
			_In_ dwFileSize,
			_Out_ &dwNumberBytesRead,
			_Inout_ NULL

		);

		if ( dwNumberBytesRead != dwFileSize ) {
			OutputDebugString( std::to_wstring( GetLastError( ) ).c_str( ) );
			return { "Error dwNumberBytesRead e dwFileSize", "dwNumberBytesRead != dwFileSize diferem do valor esperado" };
		}

		if ( !CloseHandle(

			_In_ hFile

		) ) return { "Error hFile", "Não foi possível fechar o HANDLE para hFile" };

		if ( !CloseHandle(

			_In_ hProc

		) ) return { "Error hProc", "Não foi possível fechar o HANDLE para hProc" };

		std::string strInformations( "" );

		std::string strDisasm( ( char* ) chMappedFile );

		delete chMappedFile;

		return { strDisasm, strInformations };

	};

};