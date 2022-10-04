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
#include "CVacOsSyscall.hh"

auto CVacOsSyscall::EmulatorInterface::initSyscalls( ) -> bool {

	FARPROC pProcAdd = nullptr;

	//NtOpenProcess
	if ( pProcAdd = GetProcAddress( _In_ LoadLibrary( _In_ ntdll ), _In_ "NtOpenProcess" ), pProcAdd == NULL ) return false;

	NtOpenProcess = reinterpret_cast< tpNtOpenProcess >( pProcAdd );

	//NtAllocateVirtualMemory
	if ( pProcAdd = GetProcAddress( _In_ LoadLibrary( _In_ ntdll ), _In_ "NtAllocateVirtualMemory" ), pProcAdd == NULL ) return false;

	NtAllocateVirtualMemory = reinterpret_cast< tpNtAllocateVirtualMemory >( pProcAdd );

	//NtWriteVirtualMemory
	if ( pProcAdd = GetProcAddress( _In_ LoadLibrary( _In_ ntdll ), _In_ "NtWriteVirtualMemory" ), pProcAdd == NULL ) return false;

	NtWriteVirtualMemory = reinterpret_cast< tpNtWriteVirtualMemory >( pProcAdd );

	//NtCreateThreadEx
	if ( pProcAdd = GetProcAddress( _In_ LoadLibrary( _In_ ntdll ), _In_ "NtCreateThreadEx" ), pProcAdd == NULL ) return false;

	NtCreateThreadEx = reinterpret_cast< tpNtCreateThreadEx >( pProcAdd );

	//LoadLibrary
	if ( pProcAdd = GetProcAddress( _In_ GetModuleHandle( _In_ L"kernel32.dll" ), _In_ "LoadLibraryA" ), pProcAdd == NULL ) return false;

	loadLibRef = pProcAdd;
	
	return true;
}

auto CVacOsSyscall::EmulatorInterface::initEmulator( ) -> bool {

	if ( !CVacOsSyscall::EmulatorInterface::initSyscalls( ) ) return false;

	//abrir processo
	DWORD dwPid;

	GetWindowThreadProcessId(

		_In_ CVacOsSyscall::getHwnd(
								_In_ L"Counter-Strike Source"
							  ),
		_Out_ &dwPid

	);

	HANDLE hCss;

	OBJECT_ATTRIBUTES oaOpenProc { 0 };

	InitializeObjectAttributes( 

								_Inout_ &oaOpenProc,
								_In_ NULL,
								_In_ NULL,
								_In_ NULL,
								_In_ NULL

							  );

	CLIENT_ID cidCss { 0 };

	cidCss.UniqueProcess = reinterpret_cast< PVOID >( dwPid );
	cidCss.UniqueThread = NULL;

	__kernel_entry NtOpenProcess(

					_Inout_ &hCss,
					_In_ PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD,
					_Inout_ &oaOpenProc,
					_Inout_ &cidCss

				);

	if ( hCss == INVALID_HANDLE_VALUE ) return false;

	//pegardllpath como const char
	auto strPath( std::filesystem::current_path( ).string( ) );

	strPath.append( "\\KiKurumi.dll" );

	//allocar memoria para o path com o tamanho do path da dll
	PVOID baseAlloc = nullptr;

	SIZE_T kSizeDllPath = sizeof( strPath.c_str( ) );

	__kernel_entry NtAllocateVirtualMemory(

											_In_ hCss,
											_Inout_ &baseAlloc,
											_In_ NULL,
											_Inout_ &kSizeDllPath,
											_In_ MEM_RESERVE | MEM_COMMIT,
											_In_ PAGE_READWRITE

										  );

	if ( !baseAlloc ) return false;

	DWORD dwNumBytesWrite;

	__kernel_entry NtWriteVirtualMemory(

										 _In_ hCss,
										 _In_ baseAlloc,
										 _In_ strPath.c_str( ),
										 _In_ kSizeDllPath,
										 _Inout_ &dwNumBytesWrite

										);

	if (kSizeDllPath != dwNumBytesWrite) return false;

	HANDLE hCssThreadVac = INVALID_HANDLE_VALUE;

	__kernel_entry NtCreateThreadEx(

					 &hCssThreadVac,
					 0x1FFFFF,
					 NULL,
					 hCss,
					 (LPTHREAD_START_ROUTINE)loadLibRef,
					 baseAlloc,
					 FALSE,
					 NULL,
					 NULL,
					 NULL,
					 NULL

				    );

	if ( hCssThreadVac == INVALID_HANDLE_VALUE ) return false;

	CloseHandle(

		_In_ hCssThreadVac

	), CloseHandle(

		_In_ hCss

	);

	return true;
}