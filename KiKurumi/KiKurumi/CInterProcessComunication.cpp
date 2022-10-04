/*
 _   __ _____ _____  _    _ _   _
| | / /|  ___|  _  || |  | | | | |
| |/ / | |__ | | | || |  | | | | |
|    \ |  __|| | | || |/\| | | | |
| |\  \| |___\ \_/ /\  /\  / |_| |
\_| \_/\____/ \___/  \/  \/ \___/
							2022

Copyright (c) Fluxuss Cyber Tech Desenvolvimento de Software, SLU (FLUXUSS)

Algumas estruturas presentes neste projeto podem ser pertencentes ao c�digo fonte do Windows cujo propriet�rio e detentor se denomina Microsoft(Copyright (c) Microsoft Corporation)
sua utiliza��o � feita de acordo com atos "legais" de acordo com leis, como a Lei de direito autoral Brasileira e Lei Americana de Direito Autoral.
alguns desses dados foram extra�dos da ntdll.h extra�da do c�digo fonte do React Os(Copyright (c) React Os), e complementadas com o c�digo fonte vazado do Windows XP.

Esse c�digo fonte est� progido de acordo com Copyright Act of 1976 lei aplic�vel a devida parte interessadas Microsoft, Valve(Copyright (c) Valve Corporation).
Esse c�digo se aplica "ao uso justo", mais informa��es https://www.copyright.gov/title17/.

Obrigado Open Source <3

Com todo respeito e nada mais a declarar.


"The Kurumi Source Engine explorer Engine"

THE CAKE IS A LIE!

  ^^
Keowu

*/
#include "pch.h"
#include "CInterProcessComunication.hh"


auto CInterProcessComunication::createComm( ) -> void {

	__try {

		CInterProcessComunication::g_hSlots = CreateFile(

			_In_ CInterProcessComunication::g_lpcMailName,
			_In_ GENERIC_READ | GENERIC_WRITE,
			_In_ NULL,
			_In_opt_ NULL,
			_In_ OPEN_EXISTING,
			_In_ FILE_ATTRIBUTE_NORMAL,
			_In_opt_ NULL

		);

		if ( CInterProcessComunication::g_hSlots == INVALID_HANDLE_VALUE ) return;

		CInterProcessComunication::g_bIsClosed = TRUE;

	}__except( 1 ) { }
}

auto CInterProcessComunication::writeComm( char* buffer, DWORD dwBuffSize ) -> void {

	__try {

		if ( !CInterProcessComunication::g_bIsClosed ) return;

		DWORD dwBuffWritten = 0;

		BOOL isWritten = WriteFile(

			_In_ CInterProcessComunication::g_hSlots,
			_In_reads_bytes_opt_(dwBuffSize) buffer,
			_In_ dwBuffSize,
			_Out_opt_ &dwBuffWritten,
			_Inout_opt_ NULL

		);

		if ( !isWritten ) return;

	} __except( 1 ) { }
}

auto CInterProcessComunication::readComm( char* buffer, DWORD dwBuffSize ) -> void {

	__try {

		if ( !CInterProcessComunication::g_bIsClosed ) return;

		DWORD dwBuffWritten = 0;

		BOOL isWritten = ReadFile(

			_In_ CInterProcessComunication::g_hSlots,
			_In_ buffer,
			_In_ dwBuffSize,
			_Inout_opt_ &dwBuffWritten,
			_Inout_opt_ NULL

		);

		if ( !isWritten ) return;

	}__except( 1 ) { }
}

auto CInterProcessComunication::closeComm( ) -> void {

	__try {

		CInterProcessComunication::g_bIsClosed = CloseHandle(

			_Post_ptr_invalid_ CInterProcessComunication::g_hSlots

		);

		if ( !CInterProcessComunication::g_bIsClosed ) return;

	}__except( 1 ){ }

}