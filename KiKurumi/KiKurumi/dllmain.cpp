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
#include "pch.h"
#include "CInterProcessComunication.hh"
#include "CHooks.hh"


auto readMemory( unsigned int ptrBaseAddress, unsigned int* ptrOffset, unsigned int* uiPtrFinal, int iSize ) -> void {

    unsigned int uiMemPtr = ptrBaseAddress;

    for (auto i = iSize-1; i >= 0; i--)

        std::invoke( [ & ] ( ) {

                uiMemPtr = *( unsigned int* )uiMemPtr;
                uiMemPtr += *( ptrOffset + i );

        } );

    *uiPtrFinal = uiMemPtr;
}

auto copyShellCode( unsigned int ptrBaseAddress, const char *cShellCode, int iSize ) -> void {

    DWORD dwOldPageProtection;

    __try {

        if (VirtualProtect(

            _In_ (LPVOID)ptrBaseAddress,
            _In_ iSize,
            _In_ PAGE_EXECUTE_READWRITE,
            _Out_ &dwOldPageProtection

        ))
            if (memcpy_s(

                _Out_ (void *)ptrBaseAddress,
                _In_ iSize,
                _In_ (void *)cShellCode,
                _In_ iSize

            ) == WN_SUCCESS)
                VirtualProtect(

                    _In_ (LPVOID)ptrBaseAddress,
                    _In_ iSize,
                    _In_ dwOldPageProtection,
                    _Out_ &dwOldPageProtection

                );

    }
    __except ( EXCEPTION_ACCESS_VIOLATION ) {

        VirtualProtect(

            _In_ & ptrBaseAddress,
            _In_ iSize,
            _In_ dwOldPageProtection,
            _Out_ & dwOldPageProtection

        );  return;

    }

}

auto WINAPI MainThread(PVOID args) -> DWORD {

    CInterProcessComunication::createComm( );

    if ( !CInterProcessComunication::g_bIsClosed )
        MessageBox(

            _In_ NULL,
            _In_ L"A comunicação não foi criada !",
            _In_ L"ERRO DE COMUNICAÇÃO !",
            _In_ NULL

        );

    char buff[ 1024 ]{ "\nGANKEI_BOOT(Hello World, from game !!)" };

    CInterProcessComunication::writeComm( buff, sizeof( buff ) );

    CHooks::installHooksInCSS( );

    /*
    * Exemplo de shellcode direto
    copyShellCode((unsigned int)GetModuleHandle(L"server.dll") + 0x122248, "\x90\x90\x90", 3);
    copyShellCode((unsigned int)GetModuleHandle(L"client.dll") + 0x335A4A, "\x90\x90", 2);

    unsigned int offsets[ 3 ]{ 0x7F8, 0x08, 0x1C };
    unsigned int* uiPtrFinal = new unsigned int;
    readMemory( (unsigned int)GetModuleHandle( L"client.dll" ) + 0x004E5C3C, offsets, uiPtrFinal, 3 );
    char teste[ 1024 ]{ 0 };
    sprintf_s( teste, 1024, "\nVALOR LIDO DA MEMÓRIA DO JOGO #TESTE: %d", *(int*)*uiPtrFinal );
    CInterProcessComunication::writeComm( teste, sizeof( teste ) );

    */

    while ( true ) continue;

    CHooks::uninstallHookInCSS( );

    CInterProcessComunication::closeComm( );

    return 0;
}


auto APIENTRY DllMain( _In_ HMODULE hModule,
                       _In_ DWORD  ul_reason_for_call,
                       _In_ LPVOID lpReserved
                     ) -> BOOL
{
    switch ( ul_reason_for_call )
    {

    case DLL_PROCESS_ATTACH:

        CreateThread(

            _In_opt_ NULL,
            _In_ NULL,
            _In_ MainThread,
            _In_opt_ __drv_aliasesMem NULL,
            _In_ NULL,
            _Out_opt_ NULL

        );


    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;

    }

    return TRUE;
}

