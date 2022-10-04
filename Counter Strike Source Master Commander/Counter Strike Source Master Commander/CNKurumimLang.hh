#pragma once
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
#include <functional>
#include <string>
#include "CVacOsSyscall.hh"

namespace CNKurumimLangParser {

	namespace Utils {

		namespace reservedKeys {
			
			static const char* chImageBase = "IMGBASE(\"";
			static const char* chImageBaseEnd = "\")";
			static const char* chImageBaseConcated = "+";

			static const char* chRvA = "RVA(\"";
			static const char* chPattern = "PATTERN(\"";

		};

		namespace errorCodes {

			const int CNKURUMI_ERROR_IS_VALID_SCRIPT = -0X77777778;
			const int CNKURUMI_ERROR_IS_INVALID_MODULE = -0X77777777;
			const int CNKURUMI_UKNOWN_ERROR = -0X77777779;

		};

	};

	static std::function< bool ( std::string ) > isValidScript = [ & ] ( std::string script ) -> bool {

		//IMGBASE("hl2.exe")+RVA("0x00000000")
		//IMGBASE("hl2.exe")+PATTERN("0x00000000")
		//IMGBASE("hl2.exe") p/ .text e all seções
		int iScriptScore = 0;

		if ( script.find( Utils::reservedKeys::chImageBase ) != std::string::npos &&
			script.find( Utils::reservedKeys::chImageBaseEnd ) != std::string::npos ) iScriptScore++;

		if ( script.find( Utils::reservedKeys::chImageBaseConcated ) != std::string::npos ) iScriptScore++;

		if ( script.find( Utils::reservedKeys::chRvA ) != std::string::npos ||
			script.find( Utils::reservedKeys::chPattern ) != std::string::npos ) iScriptScore++;

		return iScriptScore >= 3;
	};


	static std::function< std::pair< ULONGLONG, std::string >( std::string ) > getParsed = [ & ] ( std::string strScript ) -> std::pair< ULONGLONG, std::string > {

		if ( !CNKurumimLangParser::isValidScript( strScript ) ) return { Utils::errorCodes::CNKURUMI_ERROR_IS_VALID_SCRIPT, "Wrong ScriptTable" };

		ULONGLONG ullFinalAdd = 0;

		auto strModuleName = strScript.substr( strScript.find( Utils::reservedKeys::chImageBase ),
													 strScript.find( Utils::reservedKeys::chImageBaseEnd ) );
		
		strModuleName = strModuleName.substr( strlen( Utils::reservedKeys::chImageBase ),
											 strModuleName.length( ) );

		ullFinalAdd = CVacOsSyscall::getModuleBase( strModuleName.c_str( ) );

		if ( !ullFinalAdd ) return { Utils::errorCodes::CNKURUMI_ERROR_IS_INVALID_MODULE, "INVALID MODULE SEARCH" };

		//Usuário usou a keyword + ?
		if ( strScript.find( Utils::reservedKeys::chImageBaseConcated ) != std::string::npos )

			if ( strScript.find( Utils::reservedKeys::chRvA ) != std::string::npos ) { // é um rva ou um pattern ?

				auto strRvA = strScript.substr( strScript.find( Utils::reservedKeys::chRvA ), strScript.length( ) );

				strRvA = strRvA.substr( strlen( Utils::reservedKeys::chRvA ) + strlen( "0x" ), strRvA.length( ) );

				strRvA = strRvA.substr( 0, strRvA.find( Utils::reservedKeys::chImageBaseEnd ) );

				return { ullFinalAdd + std::stoul( strRvA, nullptr, 16 ), "ADDY_OK_NO_PATTERN_AND_VALID_BASE_AND_VALID_RVA" };
			}
			else {
				
				auto strPattern = strScript.substr( strScript.find( Utils::reservedKeys::chPattern ), strScript.length( ) );
				
				strPattern = strPattern.substr( strlen( Utils::reservedKeys::chPattern ), strPattern.length( ) );

				strPattern = strPattern.substr( 0, strPattern.find( Utils::reservedKeys::chImageBaseEnd ) );

				return { ullFinalAdd, strPattern };
			}

		else return { ullFinalAdd , "ADDY_OK_NO_PATTERN_AND_VALID_BASE" }; //Caso não tenha usado uma keyword é só retornar o modulo !
		

		return { Utils::errorCodes::CNKURUMI_UKNOWN_ERROR, "SORRY_THE_CAKE_IS_A_LIE" };

	};

};