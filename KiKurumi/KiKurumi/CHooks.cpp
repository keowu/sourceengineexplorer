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
#include "CHooks.hh"


auto CHooks::KurumiMemoryManage::memoryToString( void* const buff, const size_t len, std::string& out ) -> void {

    auto* byteData = reinterpret_cast< unsigned char* >( buff );

    std::stringstream hexStringStream;

    hexStringStream << std::hex << std::setfill( '0' );

    for ( size_t index = 0; index < len; ++index ) hexStringStream << std::uppercase << std::setw( 2 ) << static_cast< int >( byteData[ index ] ) << " ";

    out = hexStringStream.str( ).append( "\0" );

}

auto CHooks::KurumiMemoryManage::memoryToString( char* buff, const size_t len, char* out ) -> void {

    for ( auto i = 0; i < len; i++ ) sprintf_s( &*( out + i ), len*2, "%X ", *( buff + i ) );

}

auto CHooks::KurumiMemoryManage::findProtocolSign(char* buff, char* sign, size_t szSignLen, size_t szBuffLen) -> bool {

    for ( auto i = 0; i < szBuffLen; i++ ) if ( memcmp( &*( buff + i ), sign, szSignLen ) == 0 ) return true;

    return false;
}

auto _cdecl CHooks::CValveTierZero::ReadServerBuff( _In_ SOCKET s, _Out_writes_bytes_to_( len, return ) __out_data_source( NETWORK ) char FAR* buf, _In_ int len, _In_ int flags, _Out_writes_bytes_to_opt_( *fromlen, *fromlen ) struct sockaddr FAR* from, _Inout_opt_ int FAR* fromlen ) -> int {

    char buff[ 1024 ] { 0 };

    if ( len < 0x46510 ) {
        
        sprintf_s( buff, 1024, "\n[RECV]CHK( %0.2X ) | BUFF: ", len );

        CInterProcessComunication::writeComm( buff, sizeof( buff ) );

        std::memset( buff, 0, 1024 );

        std::string str{};

        CHooks::KurumiMemoryManage::memoryToString( ( char* )buf, 100, str );

        memcpy( buff, str.c_str( ), strlen( str.c_str( ) ) );

        CInterProcessComunication::writeComm( buff, sizeof( buff ) );
    }
    else {
        strcpy_s( buff, "\n[RECV] Pacote comprimido com LZSS de dados de voz e serviços da Steam e Hearth Beat." );
        CInterProcessComunication::writeComm( buff, sizeof( buff ) );
    }

    return CHooks::CValveTierZero::g_phReadServerBuff( s, buf, len, flags, from, fromlen );
}


auto __cdecl CHooks::CounterStrikeEngine::SendPackageNet( void* steamCtx, SOCKET s, char* buf, int len, struct sockaddr* to, int tolen ) -> struct sockaddr* {

    char buff[ 1024 ] { 0 };

    if ( KurumiMemoryManage::findProtocolSign( ( char * )buf, ( char * )"\x56\x4D\x6F\x64\x45\x6E\x61\x62\x6C\x65", 10, len ) ) {

        strcpy_s( buff, "\n[SEND] O Cliente está transferindo arquivos necessários para o jogador !" );

        CInterProcessComunication::writeComm( buff, sizeof( buff ) );

        return CHooks::CounterStrikeEngine::g_phSendPackageNet( steamCtx, s, buf, len, to, tolen );
    }

    sprintf_s( buff, "\n\n[SEND]CHK( %0.2X ) | BUFF: ", len );

    CInterProcessComunication::writeComm( buff, sizeof( buff ) );

    std::memset( buff, 0, 1024 );

    //1º enviar comando de buff grande
    strcpy_s( buff, "[INIT_BIG_BUFF]" );

    CInterProcessComunication::writeComm( buff, sizeof( buff ) );

    //2º enviar o tamanho do buff grande
    std::memset( buff, 0, sizeof( buff ) );

    sprintf_s( buff, 1024, "%d", len );

    CInterProcessComunication::writeComm( buff, sizeof( buff ) );

    auto* buffBig = reinterpret_cast< char* >( std::malloc( len * 4 ) );

    std::string str{};

    CHooks::KurumiMemoryManage::memoryToString( ( char* )buf, len, str );

    memcpy( buffBig, str.c_str( ), strlen( str.c_str( ) ) );

    CInterProcessComunication::writeComm( buffBig, len * 2 ); // len*2 porque aumentou-se o tamanho

    std::free( buffBig );

    return CHooks::CounterStrikeEngine::g_phSendPackageNet( steamCtx, s, buf, len, to, tolen );
}

auto __fastcall CHooks::CSteamClient::PreparateSocketSender( SteamClientInstance* pThis, void* edxRegisterShit, sourceEngineIntance* pThisSource, unsigned char* steamBufferClient ) -> int {

    //FF FF FF FF 54 53 6F 75 72 63 65 20 45 6E 67 69 6E 65 20 51 75 65 72 79 00  -> PROTOCOL UPDATE SERVER STATUS
    
    //_________________________SERVER UPDATE
    //FF FF FF FF 55 00 00 00 00
    // FF FF FF FF 55 5D 5A 77 66

    //CONNECT
    //FF FF FF FF 71 6B 69 E0 02 30 30 30 30 3

    /*
    O Cliente está atualmente tentando baixar algo do servidor remoto ?
    arquivos podem levar muito tempo para serem transferidos e são muito grande, por exemplo um mapa do servidor
    no qual o usuário não tenha, então faz extramamente desnecessário baixar pois não se trata de um protocolo e sim dos dados de
    Download em transito CLIENTE -> STEAM -> SERVIDOR
                         CLIENTE <- STEAM <- SERVIDOR
                             CLIENTE <- SERVIDOR 
    */
    /*
        Por padrão quando a valve vai transmitir algo na steam eles instanciam na classe um ponteiro para steamBufferCliente
        quando não recuperado esse indice o mesmo é zero e não pode ser recuperado posteriormente
    */
    if ( !steamBufferClient ) CHooks::CSteamClient::g_steamBufferClient = &pThis->gap0[ 56 ]; //Recupera a instância armazenada na classe de um ponteiro para steamBufferCliente
    else CHooks::CSteamClient::g_steamBufferClient = steamBufferClient; //Do contrario reaproveito a instância que a steam gerou

    CHooks::CSteamClient::g_phGetSendBufferSize( );

    CHooks::CSteamClient::g_phGetBuffer( );

    char buff[ 1024 ]{ 0 };

    std::string str{};

    CHooks::KurumiMemoryManage::memoryToString( CHooks::CSteamClient::g_nakedbuff, CHooks::CSteamClient::g_nakedbuffSize, str );

    sprintf_s( buff, 1024, "\n[SEND_STEAM]CHK( %d ) | HEX( %s ) | ASCII( %s )", CHooks::CSteamClient::g_nakedbuffSize, str.c_str( ), CHooks::CSteamClient::g_nakedbuff );

    CInterProcessComunication::writeComm( buff, sizeof( buff ) );

    return CHooks::CSteamClient::g_phPreparateSocketSender( pThis, pThisSource, steamBufferClient );
}


auto __fastcall CHooks::CSsStrikeCHudArmor::dtPaint( CSsContext* pThis, void* ecx ) -> void {

    CHooks::CsSourceEngineHiddenFeatures::valvePlayerMessage_t s;
    s.time_to_live = 9999999.0;
    s.index = 2;
    s.fixed_width_font = false;
    s.color[ 0 ] = 0.8705882352941176; // 222/255
    s.color[ 1 ] = 0.192156862745098; // 49/255
    s.color[ 2 ] = 0.3882352941176471; // 99/255

    CHooks::CsSourceEngineHiddenFeatures::valveSendPlayerMsg( &s, "MENSAGEM DA VALVE PARA VOCÊ: www.github.com/keowu" );

    void* playerCssClass = CHooks::CsSourceEngineHiddenFeatures::getCssPlayer( );

    s.time_to_live = 9999999.0;
    s.index = 3;
    s.fixed_width_font = false;
    s.color[ 0 ] = 0.8705882352941176; // 222/255
    s.color[ 1 ] = 0.192156862745098; // 49/255
    s.color[ 2 ] = 0.3882352941176471; // 99/255

    CHooks::CsSourceEngineHiddenFeatures::valveSendPlayerMsg( &s, "A player base alocada na memória está em: %X", ( DWORD* )playerCssClass );

    int armour = CHooks::CsSourceEngineHiddenFeatures::getPlayerColete( playerCssClass );

    s.time_to_live = 9999999.0;
    s.index = 4;
    s.fixed_width_font = false;
    s.color[ 0 ] = 0.8705882352941176; // 222/255
    s.color[ 1 ] = 0.192156862745098; // 49/255
    s.color[ 2 ] = 0.3882352941176471; // 99/255

    CHooks::CsSourceEngineHiddenFeatures::valveSendPlayerMsg( &s, "O Valor do colete é %d", armour );

    return CHooks::CSsStrikeCHudArmor::g_phPaint( pThis );
}


auto CHooks::CsSourceEngineHiddenFeatures::dtGetMinFOV( ) -> int {

    return -5;
}

auto _fastcall CHooks::CsSourceEngineHiddenFeatures::dtGetAccount( void* pThis, void* ecx ) -> int {

    return 16000;
}

auto CHooks::CSourceIntermediareDebug::dtDevMsg( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][MSG_DEV]%s", chFormatado );
    
    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtDevWarning( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][WARN_DEV]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtDevLog( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][LOG_DEV]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtDevMsgSobrecarga( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][DEV_MSG1]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtDevWarningSobrecarga( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ]{ 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][DEV_WARN1]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtDevLogSobrecarga( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][DEV_LOG1]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConColorMsg( int level, const Cor& clr, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ]{ 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_MSGCLR]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConMsg( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_MSG]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConWarning( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ]{ 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_WARN]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConLog( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_LOG]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConColorMsgSobrecarga( const Cor& clr, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_MSGCLR]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConMsgSobrecarga( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_MSG1]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConWarningSobrecarga( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_WARN1]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConLogSobrecarga( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_LOG1]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConDColorMsg( const Cor& clr, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_LOGCLR]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConDMsg( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_DMSG]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConDWarning( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_DWARN]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtConDLog( const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][CON_DLOG]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtNetMsg( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][NETMSG]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtNetWarning( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ] { 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][NETWARN]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::CSourceIntermediareDebug::dtNetLog( int level, const tchar* pMsg, ... ) -> void {

    char* vaListParser;

    __crt_va_start( vaListParser, pMsg );

    char chFormatado[ 1024 ] { 0 };

    vsprintf_s( chFormatado, pMsg, vaListParser );

    __crt_va_end( vaListParser );

    char chBuff[ 1024 ]{ 0 };

    sprintf_s( chBuff, 1024, "\n[LOGS_INTER][NETLOG]%s", chFormatado );

    CInterProcessComunication::writeComm( chBuff, sizeof( chBuff ) );

}

auto CHooks::installHooksInCSS( ) -> void {

    if (MH_Initialize() != MH_OK) throwHookExcept(L"A Inicialização do hook falhou");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phNetLogRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtNetLog,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phNetLog))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phNetLog falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phNetWarningRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtNetWarning,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phNetWarning))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phNetWarning falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phNetMsgRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtNetMsg,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phNetMsg))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phNetMsg falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConDLogRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConDLog,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConDLog))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConDLog falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConDWarningRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConDWarning,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConDWarning))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConDWarning falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConDMsgRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConDMsg,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConDMsg))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConDMsg falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConDColorMsgRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConDColorMsg,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConDColorMsg))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConDColorMsg falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConLogSobrecargaRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConLogSobrecarga,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConLogSobrecarga))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConLogSobrecarga falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConWarningSobrecargaRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConWarningSobrecarga,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConWarningSobrecarga))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConWarningSobrecarga falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConMsgSobrecargaRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConMsgSobrecarga,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConMsgSobrecarga))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConMsgSobrecarga falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConColorMsgSobrecargaRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConColorMsgSobrecarga,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConColorMsgSobrecarga))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConColorMsgSobrecarga falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConLogRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConLog,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConLog))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConLog falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConWarningRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConWarning,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConWarning))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConWarning falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConMsgRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConMsg,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConMsg))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConMsg falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phConColorMsgRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtConColorMsg,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phConColorMsg))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phConColorMsg falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phDevLogSobrecargaRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtDevLogSobrecarga,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phDevLogSobrecarga))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phDevLogSobrecarga falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phDevWarningSobrecargaRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtDevWarningSobrecarga,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phDevWarningSobrecarga))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phDevWarningSobrecarga falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phDevMsgSobrecargaRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtDevMsgSobrecarga,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phDevMsgSobrecarga))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phDevMsgSobrecarga falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phDevLogRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtDevLog,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phDevLog))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phDevLog falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phDevWarningRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtDevWarning,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phDevWarning))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phDevWarning falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSourceIntermediareDebug::phDevMsgRef,
        _Out_ &CHooks::CSourceIntermediareDebug::dtDevMsg,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSourceIntermediareDebug::phDevMsg))
        != MH_OK) throwHookExcept(L"CHooks::CSourceIntermediareDebug::phDevMsg falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CsSourceEngineHiddenFeatures::phGetAccountRef,
        _Out_ &CHooks::CsSourceEngineHiddenFeatures::dtGetAccount,
        _Inout_ reinterpret_cast<void**>(&CHooks::CsSourceEngineHiddenFeatures::phGetAccount))
        != MH_OK) throwHookExcept(L"CHooks::CsSourceEngineHiddenFeatures::phGetAccount falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CsSourceEngineHiddenFeatures::phGetMinFOVRef,
        _Out_ &CHooks::CsSourceEngineHiddenFeatures::dtGetMinFOV,
        _Inout_ reinterpret_cast<void**>(&CHooks::CsSourceEngineHiddenFeatures::phGetMinFOV))
        != MH_OK) throwHookExcept(L"CHooks::CsSourceEngineHiddenFeatures::phGetMinFOV falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSsStrikeCHudArmor::g_phPaintRef,
        _Out_ &CHooks::CSsStrikeCHudArmor::dtPaint,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSsStrikeCHudArmor::g_phPaint))
        != MH_OK) throwHookExcept(L"CHooks::CSsStrikeCHudArmor::g_phPaint falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CValveTierZero::g_phReadServerBuffRef,
        _Out_ &CHooks::CValveTierZero::ReadServerBuff,
        _Inout_ reinterpret_cast<void**>(&CHooks::CValveTierZero::g_phReadServerBuff))
        != MH_OK) throwHookExcept(L"CHooks::CValveTierZero::g_phReadServerBuff falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CSteamClient::g_phPreparateSocketSenderRef,
        _Out_ &CHooks::CSteamClient::PreparateSocketSender,
        _Inout_ reinterpret_cast<void**>(&CHooks::CSteamClient::g_phPreparateSocketSender))
        != MH_OK) throwHookExcept(L"CHooks::CSteamClient::g_phPreparateSocketSender falhou.");

    if (MH_CreateHook(
        _In_ CHooks::CounterStrikeEngine::g_phSendPackageNetRef,
        _Out_ & CHooks::CounterStrikeEngine::SendPackageNet,
        reinterpret_cast<void**>(&CHooks::CounterStrikeEngine::g_phSendPackageNet))
        != MH_OK) throwHookExcept(L"CHooks::CounterStrikeEngine::g_phSendPackageNet falhou.");

    MH_EnableHook( MH_ALL_HOOKS );
}

auto CHooks::uninstallHookInCSS( ) -> void {
    
    MH_DisableHook( MH_ALL_HOOKS );

}