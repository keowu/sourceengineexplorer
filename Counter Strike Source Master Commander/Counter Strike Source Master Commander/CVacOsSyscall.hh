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
#include <Windows.h>
#include <ntstatus.h>
#include <subauth.h>
#include <iostream>
#include <filesystem>
#include <psapi.h>

namespace CVacOsSyscall
{

	const auto isProcActive = [ ] ( const wchar_t* lpWindowName ) {

		return FindWindow(

			_In_ NULL,
			_In_ lpWindowName

		) != NULL;
	};

	const auto getHwnd = [ ] ( const wchar_t* lpWindowName ) -> HWND {

		return FindWindow(

			_In_ NULL,
			_In_ lpWindowName

			);
	};


	const auto getProcessHandle = [ ] ( void ) -> HANDLE {

		DWORD wdCsPid = 0;

		GetWindowThreadProcessId(

			_Inout_ getHwnd( L"Counter-Strike Source" ),
			_Out_ &wdCsPid

		);

		return OpenProcess(

			_In_ PROCESS_ALL_ACCESS,
			_In_ FALSE,
			_In_ wdCsPid

		);

	};

	const auto getModuleBase = [ ] ( const char* lpModuleName ) -> ULONGLONG {

		HANDLE hProc = getProcessHandle( );
		
		DWORD dwCbSizeNeed = 0;

		EnumProcessModules(

			_In_ hProc,
			_In_ NULL,
			_In_ NULL,
			_Out_ &dwCbSizeNeed

		);

		auto* hModules = new HMODULE[ dwCbSizeNeed ] { 0 };

		EnumProcessModules(

			_In_ hProc,
			_In_ hModules,
			_In_ sizeof( hModules ) * dwCbSizeNeed,
			_Out_ &dwCbSizeNeed

		);

		for ( auto i = 0; i < dwCbSizeNeed; i++ ) {
			
			char chModuleName[ MAX_PATH ] { 0 };

			GetModuleBaseNameA(

				hProc,
				hModules[ i ],
				chModuleName,
				sizeof( chModuleName )

			);
			
			//varrer e procurar nos modulos carregados um modulo com o nome correto!
			if ( strstr( lpModuleName, chModuleName ) ) return reinterpret_cast< ULONGLONG >( hModules[ i ] );

		}

		CloseHandle(

			_In_ hProc

		);

		return 0;
		
	};
	
	const auto doStringTrim = [ ] ( std::string* strIn ) {

		strIn->erase( std::remove( strIn->begin( ), strIn->end( ), ' ' ), strIn->end( ) );

	};

	const auto doStringHexClear = [] ( std::string* strIn ) {

		//Remover possíveis caracteres não ascii presentes
		strIn->erase( remove_if( strIn->begin( ), strIn->end( ), [ ]( char c ) {

			//Vou usar essa lambda para remover se for diferente de A - F e 0 - 9, que seria a referência válida de bytes
			return !( ( c >= 0x41 && c <= 0x46 ) || ( c >= 0x30 && c <= 0x39 ) || ( c == 0x20 ) );

		} ), strIn->end( ) );

	};


	const auto StringToMemory = [ ] ( std::string& strIn, void* const refMemory ) {

		size_t length = strIn.length( );

		unsigned char* byteData = reinterpret_cast< unsigned char* >( refMemory );

		std::stringstream hexStringStream; hexStringStream >> std::hex;

		for ( auto strIndex = 0, dataIndex = 0; strIndex < length; ++dataIndex )
		{
		
			const char tmpStr[ 3 ] = { strIn[ strIndex++ ], strIn[ strIndex++ ], 0 };

			hexStringStream.clear( );
			
			hexStringStream.str( tmpStr );

			int tmpValue = 0;
			
			hexStringStream >> tmpValue;

			byteData[ dataIndex ] = static_cast< unsigned char >( tmpValue );

		}

	};

	const auto preparetePatch = [ ] ( ULONGLONG ullAddy, unsigned char* ucUserPatch, std::size_t szLenght ) -> unsigned char* {

		auto hProc = CVacOsSyscall::getProcessHandle( );

		auto* ucOriginalPatch = new unsigned char[ szLenght ]{ 0 };

		SIZE_T szReturned = 0;

		bool isErro = false;

		ReadProcessMemory(

			hProc,
			(LPCVOID)ullAddy,
			ucOriginalPatch,
			szLenght,
			&szReturned

		);

		if ( szReturned != szLenght ) isErro = true;
		
		WriteProcessMemory(

			hProc,
			(LPVOID)ullAddy,
			ucUserPatch,
			szLenght,
			&szReturned

		);

		if ( szReturned != szLenght ) isErro = true;

		CloseHandle(

			hProc

		);

		if ( isErro ) return nullptr;

		return ucOriginalPatch;
	};


	namespace EmulatorInterface {

		typedef struct _OBJECT_ATTRIBUTES
		{

			ULONG Length;
			HANDLE RootDirectory;
			PUNICODE_STRING ObjectName;
			ULONG Attributes;
			PVOID SecurityDescriptor;
			PVOID SecurityQualityOfService;

		} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

		typedef struct _CLIENT_ID
		{

			HANDLE UniqueProcess;
			HANDLE UniqueThread;

		} CLIENT_ID, * PCLIENT_ID;

		#define InitializeObjectAttributes( p, n, a, r, s ) {				  \
						( p )->Length = sizeof( OBJECT_ATTRIBUTES );          \
						( p )->RootDirectory = r;                             \
						( p )->Attributes = a;                                \
						( p )->ObjectName = n;                                \
						( p )->SecurityDescriptor = s;                        \
						( p )->SecurityQualityOfService = NULL;               \
		}

		using tpNtOpenProcess = NTSYSCALLAPI NTSTATUS( NTAPI * )( _Out_ PHANDLE ProcessHandle, _In_ ACCESS_MASK DesiredAccess, _In_ POBJECT_ATTRIBUTES ObjectAttributes, _In_opt_ PCLIENT_ID ClientId );
		using tpNtAllocateVirtualMemory = NTSYSCALLAPI NTSTATUS( NTAPI * )(	_In_ HANDLE ProcessHandle, _Inout_ PVOID* BaseAddress, _In_ ULONG_PTR ZeroBits,	_Inout_ PSIZE_T RegionSize,	_In_ ULONG AllocationType, _In_ ULONG Protect );
		using tpNtWriteVirtualMemory = NTSYSCALLAPI NTSTATUS( NTAPI *)( _In_ HANDLE ProcessHandle, _In_opt_ PVOID BaseAddress, _In_ CONST VOID* Buffer, _In_ SIZE_T BufferSize, _Out_opt_ PSIZE_T NumberOfBytesWritten );
		using tpNtCreateThreadEx = NTSYSCALLAPI	NTSTATUS( NTAPI* )(	_Out_ PHANDLE ThreadHandle, _In_ ACCESS_MASK DesiredAccess, _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes, _In_ HANDLE ProcessHandle, _In_ LPTHREAD_START_ROUTINE StartRoutine, _In_opt_ PVOID Argument, _In_ ULONG CreateFlags, _In_opt_ ULONG_PTR ZeroBits, _In_opt_ SIZE_T StackSize, _In_opt_ SIZE_T MaximumStackSize, _In_opt_ PVOID AttributeList );

		static const wchar_t* ntdll = L"ntdll.dll";

		static tpNtOpenProcess NtOpenProcess;
		static tpNtAllocateVirtualMemory NtAllocateVirtualMemory;
		static tpNtWriteVirtualMemory NtWriteVirtualMemory;
		static tpNtCreateThreadEx NtCreateThreadEx;
		static PVOID loadLibRef;

		inline auto initSyscalls( void ) -> bool;

		auto initEmulator( void ) -> bool;
	}

};

