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
#include "CInterProcessServerComm.hh"



auto CInterProcessServerComm::createComm( ) -> void {

	CInterProcessServerComm::g_hSlots = CreateMailslot(

		_In_ CInterProcessServerComm::g_lpcMailName,
		_In_ NULL,
		_In_ MAILSLOT_WAIT_FOREVER,
		_In_ NULL

	);

	if ( CInterProcessServerComm::g_hSlots == INVALID_HANDLE_VALUE ) return;

	CInterProcessServerComm::g_bIsClosed = TRUE;

}

auto CInterProcessServerComm::writeComm( char* buffer, DWORD dwBuffSize ) -> void {

	if ( !CInterProcessServerComm::g_bIsClosed ) return;

	DWORD dwBuffWritten = 0;

	BOOL isWritten = WriteFile(

		CInterProcessServerComm::g_hSlots,
		buffer,
		dwBuffSize,
		&dwBuffWritten,
		NULL

	);

	if ( !isWritten ) return;

}

auto CInterProcessServerComm::readComm( char* buffer, DWORD dwBuffSize ) -> void {

	if ( !CInterProcessServerComm::g_bIsClosed ) return;

	DWORD dwBuffWritten = 0;

	BOOL isWritten = ReadFile(

		_In_ CInterProcessServerComm::g_hSlots,
		_Out_ buffer,
		_In_ dwBuffSize,
		_Out_ &dwBuffWritten,
		_In_ NULL

	);

	if ( !isWritten ) return;

}

auto CInterProcessServerComm::closeComm( ) -> void {

	CInterProcessServerComm::g_bIsClosed = CloseHandle(

		_In_ CInterProcessServerComm::g_hSlots

	);

	if ( !CInterProcessServerComm::g_bIsClosed ) return;

}