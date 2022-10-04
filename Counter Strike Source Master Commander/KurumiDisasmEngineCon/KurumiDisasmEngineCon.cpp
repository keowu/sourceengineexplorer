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
#include <iostream>
#include <windows.h>
#include <filesystem>
#include "Th3rd/beaengine-5.3.0/headers/BeaEngine.h"


#pragma comment(lib,"Th3rd/beaengine-5.3.0/dll_x64/BeaEngine.lib")

auto main( ) -> int
{

    HANDLE hFile = CreateFile(

        _In_ std::wstring(std::filesystem::temp_directory_path().wstring()).append(L"\\indiasm.bin").c_str(),
        _In_ GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,
        _In_ FILE_SHARE_READ,
        _In_opt_ NULL,
        _In_ OPEN_EXISTING,
        _In_ FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        _In_ NULL

    );

    if ( hFile == INVALID_HANDLE_VALUE ) return -1;

    DWORD dwFileSize = GetFileSize(

        _In_ hFile,
        _Out_opt_ NULL

    );


    if ( dwFileSize == 0 )
        return -1;


    HANDLE hFileMapping = CreateFileMapping(

        _In_ hFile,
        _In_opt_ NULL,
        _In_ PAGE_READWRITE,
        _In_ NULL,
        _In_ NULL,
        _In_opt_ NULL

    );

    if ( hFileMapping == NULL ) {

        std::cout << GetLastError() << '\n';

        return -1;
    }

    ULONGLONG* u8OpcodeData = reinterpret_cast<ULONGLONG*>(MapViewOfFileEx(

        _In_ hFileMapping,
        _In_ FILE_MAP_ALL_ACCESS,
        _In_ NULL,
        _In_ NULL,
        _In_ 0,
        _In_opt_ 0

    ));

    if ( u8OpcodeData == 0 ) {

        std::cout << GetLastError( ) << '\n';

        return -1;
    }

    if ( !CloseHandle(

        _In_ _Post_ptr_invalid_ hFile

    ) ) return -1;
    
    if (!CloseHandle(

        _In_ _Post_ptr_invalid_ hFileMapping

    )) return -1;

    DISASM infos;

    int len = 0;

    std::memset(

        _Out_ &infos,
        _In_ 0,
        _In_ sizeof(DISASM)

    );

    infos.Archi = 1;

    infos.EIP = reinterpret_cast<UInt64>( u8OpcodeData );
    
    infos.VirtualAddr = 0x00;

    char chBuff[1024] { 0 };
    
    std::string strList;

    while ( ( infos.Error == 0 ) && ( infos.EIP - (UInt64)u8OpcodeData ) < dwFileSize ) {
        
        len = Disasm(

            _Inout_opt_ &infos

        );

        if ( infos.Error != UNKNOWN_OPCODE ) {

            std::memset( chBuff, 0, 1024 );

            sprintf_s( chBuff, 1024, "%llx > %s\n", infos.VirtualAddr, infos.CompleteInstr );

            strList.append( chBuff );

            infos.EIP += len;

            infos.VirtualAddr += len;

        }
    }

    UnmapViewOfFile(

        _In_ u8OpcodeData

    );

    DeleteFile(

        _In_ std::wstring(std::filesystem::temp_directory_path()).append(L"\\indiasm.bin").c_str()

    );

    DeleteFile(

        _In_ std::wstring(std::filesystem::temp_directory_path()).append(L"\\outdiasm.bin").c_str()

    );

    hFile = CreateFile(

        _In_ std::wstring(std::filesystem::temp_directory_path()).append(L"\\outdiasm.bin").c_str(),
        _In_ GENERIC_WRITE,
        _In_ 0,
        _In_opt_ NULL,
        _In_ CREATE_NEW,
        _In_ FILE_ATTRIBUTE_NORMAL,
        _In_opt_ NULL

    );

    if ( hFile == INVALID_HANDLE_VALUE ) return -1;
    
    DWORD dwWritten = 0;

    WriteFile(

        _In_ hFile,
        _In_ strList.c_str(),
        _In_ strlen(strList.c_str()),
        _Out_ &dwWritten,
        _Inout_opt_ NULL

    );

    if ( dwWritten != strlen( strList.c_str( ) ) ) return -1;

    CloseHandle(

        _In_ hFile

    );

    return -1;
}