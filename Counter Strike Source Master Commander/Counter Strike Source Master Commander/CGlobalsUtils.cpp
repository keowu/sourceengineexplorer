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
#include "CGlobalsUtils.hh"

auto CGlobalsUtils::doReculculationOfWindowsForm( ) -> void {

    MSG msgMSG;

    while ( PeekMessage(

        _Out_ &msgMSG,
        _In_opt_ NULL,
        _In_ 0,
        _In_ 0,
        _In_ PM_NOREMOVE

    )
        )
    {
        switch ( GetMessage(

            _Out_ &msgMSG,
            _In_ NULL,
            _In_ 0,
            _In_ 0

        )
            )
        {

        case KEOWU_MS_IS_QUIT: {

            PostQuitMessage(

                _In_ msgMSG.wParam

            );

            break;
        }

        case KEOWU_MS_IS_SHIT: {

            //Throw para um erro n�o esperado por parte da Microsoft.

            break;
        }

        default: {

            TranslateMessage(

                _In_ &msgMSG

            );

            DispatchMessageW(

                _In_ &msgMSG

            );

            break;
        }

        }

    }

}


auto WINAPI CGlobalsUtils::parallelIntelAmdMixCores( PVOID args ) -> DWORD {

    auto* wxParallelContext = reinterpret_cast< wx_parallelContext* >( args );

    CGlobalsUtils::g_mutex = CreateMutex(

        _In_opt_ NULL,
        _In_ FALSE,
        _In_opt_ NULL

    );

    size_t tamanho = wxParallelContext->wxTextProtocol->GetNumberOfLines( );

    wxParallelContext->wxProgressBar->SetRange( wxParallelContext->wxTextProtocol->GetNumberOfLines( ) );

    std::ofstream outfile;
    
    outfile.open( std::filesystem::current_path( ).string( ) + "\\csEmulator.txt" );

    concurrency::cancellation_token_source cts;

    concurrency::run_with_cancellation_token([ tamanho, &outfile, &cts, &wxParallelContext ] ( )

        {
            concurrency::parallel_for( size_t( 0 ), tamanho, [ &, cts ] ( size_t i ) {

                WaitForSingleObject(

                    _In_ CGlobalsUtils::g_mutex,
                    _In_ INFINITE

                );

                if ( i == tamanho )
                    std::invoke([ & ] ( ) {

                            CloseHandle(

                                _In_ CGlobalsUtils::g_mutex

                            );

                            wxParallelContext->wxProgressBar->SetValue( tamanho );

                            OutputDebugStringW(

                                _In_  L"Sucesso ao encerrar o parallel loop, os cores do processador devem normalizar agora.."
                            
                            );

                            outfile.close(

                            );

                            cts.cancel(

                            );

                            return WN_SUCCESS;

                    });
                    

                outfile << wxParallelContext->wxTextProtocol->GetLineText( i ).ToStdString( ) << '\n';

                CGlobalsUtils::doReculculationOfWindowsForm( );

                std::wstring log( L"Informa��es de LOG: " );

                log.append( std::to_wstring( i ) );
                log.append( L"\n" );

                OutputDebugStringW( log.c_str( ) );

                wxParallelContext->wxProgressBar->SetValue( wxParallelContext->wxProgressBar->GetValue( ) + 1 );

                ReleaseMutex(

                    _In_ CGlobalsUtils::g_mutex

                );

            });

        }, cts.get_token(

        ));

    return WN_NOT_SUPPORTED;
}