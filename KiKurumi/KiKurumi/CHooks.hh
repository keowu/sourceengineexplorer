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
#include <cstringt.h>
#include "CInterProcessComunication.hh"
#include "Th3rd/minhook/include/MinHook.h"

#if _WIN64
#pragma comment(lib, "Th3rd/minhook/lib/Debug/libMinHook.x64.lib")
#else
#pragma comment(lib, "Th3rd/minhook/lib/Debug/libMinHook.x86.lib")
#endif

namespace CHooks
{

    namespace CounterStrikeEngine {

        using tpSendPackageNet = struct sockaddr* (__cdecl *)( _In_ void* steamCtx, _In_ SOCKET s, _In_reads_bytes_(len) char* buf, _In_ int len, _In_reads_bytes_(tolen) struct sockaddr* to, _In_ int tolen );
        
        static tpSendPackageNet g_phSendPackageNet = nullptr;
        
        static tpSendPackageNet g_phSendPackageNetRef = reinterpret_cast< tpSendPackageNet >( (uintptr_t)GetModuleHandle(L"engine.dll") + 0x1D1F20 );
        
        auto __cdecl SendPackageNet( void* steamCtx, SOCKET s, char* buf, int len, struct sockaddr* to, int tolen ) -> struct sockaddr*;        

    };


    namespace CValveTierZero {

        using tpdReadServerBuff = int(_cdecl*)( _In_ SOCKET s, _Out_writes_bytes_to_(len, return) __out_data_source(NETWORK) char FAR* buf, _In_ int len, _In_ int flags, _Out_writes_bytes_to_opt_(*fromlen, *fromlen) struct sockaddr FAR* from, _Inout_opt_ int FAR* fromlen );

        static tpdReadServerBuff g_phReadServerBuff = nullptr;
        
        static tpdReadServerBuff g_phReadServerBuffRef = reinterpret_cast< tpdReadServerBuff >( (uintptr_t)GetModuleHandle( L"tier0.dll" ) + 0x10020 );

        auto _cdecl ReadServerBuff( _In_ SOCKET s, _Out_writes_bytes_to_(len, return) __out_data_source(NETWORK) char FAR* buf, _In_ int len, _In_ int flags, _Out_writes_bytes_to_opt_(*fromlen, *fromlen) struct sockaddr FAR* from, _Inout_opt_ int FAR* fromlen ) -> int;

    };


    namespace CSteamClient {


        /*
         SteamClientInstance struc ; (sizeof=0x20D4, align=0x4, copyof_274)
         00000000 gap0            db 8364 dup(?)
         000020AC steamSocketClient dd ?
         000020B0 gap20B0         db 8 dup(?)
         000020B8 byte20B8        db ?
         000020B9                 db ? ; undefined
         000020BA                 db ? ; undefined
         000020BB                 db ? ; undefined
         000020BC byte20BC        db ?
         000020BD gap20BD         db 7 dup(?)
         000020C4 dword20C4       dd ?
         000020C8 gap20C8         db 8 dup(?)
         000020D0 dword20D0       dd ?
         000020D4 SteamClientInstance ends        
        */
        typedef struct SteamClientInstance
        {

            unsigned char gap0[8364];
            SOCKET steamSocketClient;
            unsigned char gap20B0[8];
            unsigned char byte20B8;
            __declspec(align(4)) unsigned char byte20BC;
            unsigned char gap20BD[7];
            DWORD dword20C4;
            unsigned char gap20C8[8];
            DWORD dword20D0;

        };

        /*
        00000000 ProcessIdentification struc ; (sizeof=0x10, align=0x4, copyof_275)
        00000000 dword0          dd ?
        00000004 dword4          dd ?
        00000008 dword8          dd ?
        0000000C createEventHandle dd ? ; offset
        00000010 ProcessIdentification ends
        */
        typedef struct ProcessIdentification {

            DWORD dword0;
            DWORD dword4;
            DWORD dword8;
            HANDLE createEventHandle;

        };


        /*
            Somente algumas declarações não foi necessário obter demais dados ou declarações
        */
        typedef struct sourceEngineIntance
        { 

            unsigned char gap0[16];
            DWORD dword10;
            WORD word14;

        };

        using tpdPreparateSocketSender = int (__thiscall* )( SteamClientInstance* pThis, sourceEngineIntance* pThisSource, unsigned char* steamBufferClient );
        static tpdPreparateSocketSender g_phPreparateSocketSender = nullptr;
        /*
                ISSO DEPENDE DA VERSÃO DA API DA STEAM E PRECISA SER RECONSTRUÍDO QUANDO HAVER UMA ATUALIZAÇÃO
                Busque pelas seguintes instruções:

                                    SteamClient__PreparateSocketSender proc near
	            to              = sockaddr ptr -88h
	            var_8           = dword ptr -8
	            var_4           = byte ptr -4
	            thisSource      = dword ptr  8
	            steamBufferClient= dword ptr  0Ch
	
					push    ebp
					mov     ebp, esp
					sub     esp, 88h
					push    ebx
					mov     ebx, ecx
					push    esi
					push    edi
					mov     edi, ds:?AssertFailed@?$AssertMsgHelper@$00@@SA_NPBDI0@Z ; AssertMsgHelper<1>::AssertFailed(char const *,uint,char const *)
					cmp     byte ptr [ebx+20B8h], 0
					jnz     loc_9B3F90
					cmp     dword ptr [ebx+20ACh], 0FFFFFFFFh
					jz      loc_9B414D
					mov     byte ptr [ebx+20B8h], 1
					call    sub_9B3840
					mov     esi, eax
					cmp     byte ptr [esi+60h], 0
					jnz     loc_9B3F90
					lea     ecx, [esi+24h]
					call    ds:?IsAlive@CThread@@QBE_NXZ ; CThread::IsAlive(void)
					test    al, al
					jnz     short loc_9B3F10
					cmp     dword ptr [esi+5Ch], 0
					jnz     short loc_9B3EF4
					push    offset aMThreadclientM ; "m_ThreadClient.m_pParentSocket"
					push    1D5h
					push    offset aCBuildslaveSte_351 ; "c:\\buildslave\\steam_rel_client_hotfix"...
					mov     eax, edi
					call    eax
					add     esp, 0Ch
					test    al, al
					jnz     short loc_9B3EF4
					int     3               ; Trap to Debugger

                    Consulte o IDB que acompanha o código fonte para maior entendimento a partir de padrões antigos !

        */
        static tpdPreparateSocketSender g_phPreparateSocketSenderRef = reinterpret_cast< tpdPreparateSocketSender >( (uintptr_t)GetModuleHandle(L"steamclient.dll") + 0x9DA650 );
        /*
            edxRegisterShit -> Representa o registrador edx como um parâmetro morto porque em teoria ele não é utilizado visto que o __thiscall não pode ser usado e __thiscall é identico.
            outra forma seria hookar o vtable mais eu não vejo necessidade disso nesse projeto.
            Caso você esteja vendo isso e queira entender mais recomendo as seguintes leituras que explicam as diferenças dessas calling coventions:
                https://en.wikipedia.org/wiki/X86_calling_conventions#Microsoft_fastcall
                https://en.wikipedia.org/wiki/X86_calling_conventions#thiscall
            Os demais foram tratados como struct visto que no assembly classes são basicamente estruturas de dados armazenadas em uma vtable!
        */
        auto __fastcall PreparateSocketSender( SteamClientInstance* pThis, void* edxRegisterShit, sourceEngineIntance* pThisSource, unsigned char* steamBufferClient ) -> int;

        static int g_nakedbuffSize;
        static unsigned char* g_steamBufferClient;
        inline auto __declspec(naked) g_phGetSendBufferSize( ) -> void {

            __asm {

                mov ecx, g_steamBufferClient
                mov eax, [ecx + 0x20]
                mov g_nakedbuffSize, eax
                retn;

            }

        };

        static char* g_nakedbuff;
        inline auto __declspec(naked) g_phGetBuffer( ) -> void {
            
            __asm {

                mov ecx, g_steamBufferClient
                lea eax, [ecx + 0x26]
                mov g_nakedbuff, eax
                retn

            }

        }


    };

    namespace CSsStrikeCHudArmor {

        typedef struct CSsContext {

            unsigned char gap0[132];
            unsigned char byte84;
            __declspec(align(2)) char char86;
            DWORD dword88;
            DWORD dword8C;
            float float90;
            float float94;
            float float98;
            float float9C;

        };

        using tpdPaint = void(__thiscall*) ( CSsContext* pThis );
        static tpdPaint g_phPaint = nullptr;
        static tpdPaint g_phPaintRef = reinterpret_cast< tpdPaint >( (uintptr_t)GetModuleHandle(L"client.dll") + 0x1ECCA0 );
        auto __fastcall dtPaint( CSsContext* pThis, void* ecx ) -> void;

    };

    namespace CsSourceEngineHiddenFeatures {

        typedef struct valvePlayerMessage

        {
            int		index;			// # Linha
            float	time_to_live;	// # Quantidade de segundos para desaparecer completamente. -1 significa que mostrara 1 frame e depois vai sair da tela.
            float	color[3];		// # Cores rgb ( 0.0 -> 1.0 escala ) É simplesmente um caso de dividir seu valor RGB, chame de x por 255: Se x = 95, seu valor é 95/255 = 0,373 (para 3) oniiiichan :)
            bool	fixed_width_font;

        } valvePlayerMessage_t;

        using tpdValveSendPlayerMsg = void (*) ( const valvePlayerMessage_t* info, const char* fmt, ... );
        static tpdValveSendPlayerMsg valveSendPlayerMsg = reinterpret_cast< tpdValveSendPlayerMsg >( (uintptr_t)GetModuleHandle(L"engine.dll") + 0x0DDF00 );

        using tpdGetCSSPlayer = void *(*)(void);
        static tpdGetCSSPlayer getCssPlayer = reinterpret_cast< tpdGetCSSPlayer >( (uintptr_t)GetModuleHandle(L"client.dll") + 0x259D70 );

        using tpdGetArmorValue = int(__thiscall*)(void* pThis);
        static tpdGetArmorValue getPlayerColete = reinterpret_cast< tpdGetArmorValue >( (uintptr_t)GetModuleHandle(L"client.dll") + 0x1D0840 );

        using tpdGetMinFOV = int (*)(void);
        static tpdGetMinFOV phGetMinFOV = nullptr;
        static tpdGetMinFOV phGetMinFOVRef = reinterpret_cast< tpdGetMinFOV >( (uintptr_t)GetModuleHandle(L"client.dll") + 0x187D0 );
        auto dtGetMinFOV() -> int;

        using tpdGetAccount = int(__thiscall*)(void* pThis, void* ecx);
        static tpdGetAccount phGetAccount = nullptr;
        static tpdGetAccount phGetAccountRef = reinterpret_cast< tpdGetAccount >( (uintptr_t)GetModuleHandle(L"client.dll") + 0x001D1C90 );
        auto _fastcall dtGetAccount(void* pThis, void* ecx) -> int;

    };

    namespace CSourceIntermediareDebug {

        /*
            TODAS AS SAÍDAS ABAIXO REPRESENTAM FUNÇÕES DE LOGS EXPORTADAS OU PRIVADAS USADAS PELAS FUNÇÕES E DESENVOLVIMENTO
            OU DEBUG DO JOGO DA VALVE

             A primeira coisa que você, claro se estiver lendo isso deve saber é que na Source engine existem grupos
             Jogadores
             Desenvolvedores da valve
             Network

             Aqui estão todas as declarações desses grupos permitindo que intercepte tudo que for enviado.
        */

        //Tipo pré-definido utilizado
        typedef char tchar;

        //Recriação da classe color a partir da RTTI
        /*
            Atenção:
                É possível reconstruir toda uma classe a partir da RTTI, foi oque eu fiz, isso deu muito trabalho, infelizmente
                algumas chamadas forçam a utilizar, ECX sendo declarado como void  na chamada de __thiscall não resolve pois a valve faz
                conversões explicitas dentro, então essa com certeza não está 100% completa e não era para estar mesmo, eu fiz o suficiente para
                funcionar bem, agora se você quer melhorar fique a vontade, eu não tenho sanidade para analisar toda a RTTI em busca de métodos dessa classe
                ela me demandou tempo, espero que você aprecie e use a vontade, é nois meu friend :)
        */
        class Cor
        {

            private:
                unsigned char cor[ 4 ] { 0 };

            public:
                
                Cor() { *((int*)this) = 0; }
                Cor(int r, int g, int b) { SetCor(r, g, b, 0); }
                Cor(int r, int g, int b, int a) { SetCor(r, g, b, a); }

                //===========================================================================================
                // Os método abaixo são utilizado para definir cores a partir do construtor, após a RTTI quase me deixar louco eu entendi:
                // r - Vermelho (0-255)
                // g - Verde (0-255)
                // b - Azul (0-255)
                // a - Alpha, Ele controla a transparência da cor desejada (0 - é o mais transparente possível, 255 - opaco);
                auto SetCor(int r, int g, int b, int a = 0) -> void {
                    this->cor[0] = (unsigned char)r;
                    this->cor[1] = (unsigned char)g;
                    this->cor[2] = (unsigned char)b;
                    this->cor[3] = (unsigned char)a;
                }

                auto GetCor(int& r, int& g, int& b, int& a) const -> void {
                    r = this->cor[0];
                    g = this->cor[1];
                    b = this->cor[2];
                    a = this->cor[3];
                }

                //===========================================================================================

                auto SetRawCor(int cor32) -> void {
                    *((int*)this) = cor32;
                }

                auto GetRawCor() const -> int {
                    return *((int*)this);
                }

                inline auto r() const -> int { return this->cor[0]; }
                inline auto g() const -> int { return this->cor[1]; }
                inline auto b() const -> int { return this->cor[2]; }
                inline auto a() const -> int { return this->cor[3]; }

                auto operator[](int indice) -> unsigned char& {
                    return this->cor[indice];
                }

                auto operator[](int indice) const -> const unsigned char& {
                    return this->cor[indice];
                }

                auto operator == (const Cor& rhs) const -> bool {
                    return (*((int*)this) == *((int*)&rhs));
                }

                auto operator != (const Cor& rhs) const -> bool {
                    return !(operator==(rhs));
                }

                Cor& operator=(const Cor& rhs) {
                    SetRawCor(rhs.GetRawCor());
                    return *this;
                }
        };


        /* Algumas mensagens dinâmicas supercomuns, aqui por conveniência, haha nha! */
        /* Estes pertencem ao grupo "desenvolvedor" */
        using tpdDevMsg = void (__cdecl *) ( int level, const tchar* pMsg, ... );
        static tpdDevMsg phDevMsg = nullptr;
        static tpdDevMsg phDevMsgRef = reinterpret_cast< tpdDevMsg >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x5100 );
        auto dtDevMsg(int level, const tchar* pMsg, ...) -> void;

        using tpdDevWarning = void (__cdecl *) (int level, const tchar* pMsg, ...);
        static tpdDevWarning phDevWarning = nullptr;
        static tpdDevWarning phDevWarningRef = reinterpret_cast< tpdDevWarning >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x51C0 );
        auto dtDevWarning(int level, const tchar* pMsg, ...) -> void;

        using tpdDevLog = void (__cdecl *) ( int level, const tchar* pMsg, ... );
        static tpdDevLog phDevLog = nullptr;
        static tpdDevLog phDevLogRef = reinterpret_cast< tpdDevLog >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x5040 );
        auto dtDevLog(int level, const tchar* pMsg, ...) -> void;

        /* Versões de nível padrão (nível 1) */
        using tpdDevMsgSobrecarga = void (*) ( const tchar* pMsg, ... );
        static tpdDevMsgSobrecarga phDevMsgSobrecarga = nullptr;
        static tpdDevMsgSobrecarga phDevMsgSobrecargaRef = reinterpret_cast< tpdDevMsgSobrecarga >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4310 );
        auto dtDevMsgSobrecarga(const tchar* pMsg, ...) -> void;

        using tpdDevWarningSobrecarga = void (*) (const tchar* pMsg, ...);
        static tpdDevWarningSobrecarga phDevWarningSobrecarga = nullptr;
        static tpdDevWarningSobrecarga phDevWarningSobrecargaRef = reinterpret_cast< tpdDevWarningSobrecarga >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x43D0 );
        auto dtDevWarningSobrecarga(const tchar* pMsg, ...) -> void;

        using tpdDevLogSobrecarga = void (*) (const tchar* pMsg, ...);
        static tpdDevLogSobrecarga phDevLogSobrecarga = nullptr;
        static tpdDevLogSobrecarga phDevLogSobrecargaRef = reinterpret_cast< tpdDevLogSobrecarga >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4250);
        auto dtDevLogSobrecarga(const tchar* pMsg, ...) -> void;

        /* Estes pertencem para o grupo "console" */
        using tpdConColorMsg = void (*) (int level, const Cor& clr, const tchar* pMsg, ...);
        static tpdConColorMsg phConColorMsg = nullptr;
        static tpdConColorMsg phConColorMsgRef = reinterpret_cast< tpdConColorMsg >((uintptr_t)GetModuleHandle( L"tier0.dll") + 0x4780 );
        auto dtConColorMsg(int level, const Cor& clr, const tchar* pMsg, ...) -> void;

        using tpdConMsg = void (*) (int level, const tchar* pMsg, ...);
        static tpdConMsg phConMsg = nullptr;
        static tpdConMsg phConMsgRef = reinterpret_cast< tpdConMsg >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4C00 );
        auto dtConMsg(int level, const tchar* pMsg, ...) -> void;

        using tpdConWarning = void (*) (int level, const tchar* pMsg, ...);
        static tpdConWarning phConWarning = nullptr;
        static tpdConWarning phConWarningRef = reinterpret_cast< tpdConWarning >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4CC0 );
        auto dtConWarning(int level, const tchar* pMsg, ...) -> void;

        using tpdConLog = void (*) (int level, const tchar* pMsg, ...);
        static tpdConLog phConLog = nullptr;
        static tpdConLog phConLogRef = reinterpret_cast< tpdConLog >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4B40 );
        auto dtConLog(int level, const tchar* pMsg, ...) -> void;

        /* Versão de console padrão (nível 1) */
        using tpdConColorMsgSobrecarga = void (*) (const Cor& clr, const tchar* pMsg, ...);
        static tpdConColorMsgSobrecarga phConColorMsgSobrecarga = nullptr;
        static tpdConColorMsgSobrecarga phConColorMsgSobrecargaRef = reinterpret_cast< tpdConColorMsgSobrecarga >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x3F50 );
        auto dtConColorMsgSobrecarga(const Cor& clr, const tchar* pMsg, ...) -> void;

        using tpdConMsgSobrecarga = void (*) (const tchar* pMsg, ...);
        static tpdConMsgSobrecarga phConMsgSobrecarga = nullptr;
        static tpdConMsgSobrecarga phConMsgSobrecargaRef = reinterpret_cast< tpdConMsgSobrecarga >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x40D0 );
        auto dtConMsgSobrecarga(const tchar* pMsg, ...) -> void;

        using tpdConWarningSobrecarga = void (*) (const tchar* pMsg, ...);
        static tpdConWarningSobrecarga phConWarningSobrecarga = nullptr;
        static tpdConWarningSobrecarga phConWarningSobrecargaRef = reinterpret_cast< tpdConWarningSobrecarga >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4190 );
        auto dtConWarningSobrecarga(const tchar* pMsg, ...) -> void;

        using tpdConLogSobrecarga = void (*) (const tchar* pMsg, ...);
        static tpdConLogSobrecarga phConLogSobrecarga = nullptr;
        static tpdConLogSobrecarga phConLogSobrecargaRef = reinterpret_cast< tpdConLogSobrecarga >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4010 );
        auto dtConLogSobrecarga(const tchar* pMsg, ...) -> void;

        /* Versão do console do desenvolvedor (nível 2) */
        using tpdConDColorMsg = void (*) (const Cor& clr, const tchar* pMsg, ...);
        static tpdConDColorMsg phConDColorMsg = nullptr;
        static tpdConDColorMsg phConDColorMsgRef = reinterpret_cast< tpdConDColorMsg >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4840 );
        auto dtConDColorMsg(const Cor& clr, const tchar* pMsg, ...) -> void;

        using tpdConDMsg = void (*) (const tchar* pMsg, ...);
        static tpdConDMsg phConDMsg = nullptr;
        static tpdConDMsg phConDMsgRef = reinterpret_cast< tpdConDMsg >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x49C0 );
        auto dtConDMsg(const tchar* pMsg, ...) -> void;

        using tpdConDWarning = void (*) (const tchar* pMsg, ...);
        static tpdConDWarning phConDWarning = nullptr;
        static tpdConDWarning phConDWarningRef = reinterpret_cast< tpdConDWarning >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4A80 );
        auto dtConDWarning(const tchar* pMsg, ...) -> void;

        using tpdConDLog = void (*) (const tchar* pMsg, ...);
        static tpdConDLog phConDLog = nullptr;
        static tpdConDLog phConDLogRef = reinterpret_cast< tpdConDLog >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x4900 );
        auto dtConDLog(const tchar* pMsg, ...) -> void;

        /* Estes pertencem ao grupo "rede" */
        using tpdNetMsg = void (*) (int level, const tchar* pMsg, ...);
        static tpdNetMsg phNetMsg = nullptr;
        static tpdNetMsg phNetMsgRef = reinterpret_cast< tpdNetMsg >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x5770 );
        auto dtNetMsg(int level, const tchar* pMsg, ...) -> void;

        using tpdNetWarning = void (*) (int level, const tchar* pMsg, ...);
        static tpdNetWarning phNetWarning = nullptr;
        static tpdNetWarning phNetWarningRef = reinterpret_cast< tpdNetWarning >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x5830 );
        auto dtNetWarning(int level, const tchar* pMsg, ...) -> void;

        using tpdNetLog = void (*) (int level, const tchar* pMsg, ...);
        static tpdNetLog phNetLog = nullptr;
        static tpdNetLog phNetLogRef = reinterpret_cast< tpdNetLog >( (uintptr_t)GetModuleHandle(L"tier0.dll") + 0x56B0 );
        auto dtNetLog(int level, const tchar* pMsg, ...) -> void;

    };

    namespace KurumiMemoryManage {

        auto memoryToString( void* const buff, const size_t len, std::string& out ) -> void;

        auto memoryToString( char* buff, const size_t len, char* out ) -> void;

        auto findProtocolSign( char* buff, char* sign, size_t szSignLen, size_t szBuffLen ) -> bool;

    };

    auto throwHookExcept = [ ] ( const wchar_t* lstrMotivo ) {

            std::wstring wStr(L"Ops, um erro ocorreu ao intervir com as funções do Jogo,"
                "encerrando por segurança.\Motivo do erro: ");

            wStr.append( lstrMotivo );

            MessageBox(

                _In_opt_ NULL,
                _In_opt_ wStr.c_str(),
                _In_opt_ L"Ops, problema!",
                _In_ NULL

            );

            FreeLibraryAndExitThread(

                _In_ NULL,
                _In_ - 1

            );

        };

    auto installHooksInCSS( void ) -> void;

    auto uninstallHookInCSS( void ) -> void;

};

