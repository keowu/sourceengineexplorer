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
#pragma once

namespace CFluDecompiler {

	/*
			When a binary is sent to the server for decompilation, the content is encrypted with a private key
			shared between the user and the server, the server receives all the data and allocates a position
			in the queue for the user to wait until the binary is decompiled on the server. server and returns
			an index of the queue, this index is checked every period of time until the server indicates a payload
			of the content of the decompiler encrypting with the shared key that only the user has access, thus making
			the decompiled code available.
	*/
	static int g_iPosQueue = 0; //Position in the binary queue on the server

	static unsigned char* g_privateKey = nullptr;
	static unsigned char* g_serverDynamicKey = nullptr;
	static const char* g_decompilerSerialKey = "FLUXUSS-NETWORK-OPEN-SOURCE";


	namespace ServerIPLocation {

		static const char* SELF_HOSTED_BRAZIL = "";
		static const char* AMAZON_UNITED_STATES = "";
		static const char* AMAZON_GERMANY = "";
		static const char* RUSSIA_OFFSHORED = "";
		static const char* ALIBABA_CHINA_PEQUIM = "";
		static const char* IBM_JAPAO = "";

	};

	auto findBestServer = [ & ] ( void ) -> bool {

	};

	auto isUserOnline = [ & ] ( void ) -> bool {

		return false;
	};

	auto sendBinaryToServer = [ & ] ( unsigned char* buff ) -> void {

	};

	auto checkServer = [ & ] ( ) -> void {

	};

};