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
#include <iostream>
#include <wx/wx.h>

class CPatchsModel {


private:
	std::string strScript{ };
	ULONGLONG ullAddy{ };
	std::string strPatch{ };
	std::size_t szPatchSz{ };
	unsigned char* ucPatchStorage{ };
	unsigned char* ucPatchStorageOriginal{ };
	bool isActivated{ };

public:

	CPatchsModel( std::string strScript, unsigned long long ullAddy, std::string strPatch, std::size_t szPatchSz, unsigned char* ucPatchStorage, unsigned char* ucPatchStorageOriginal, bool isActivated )
		: strScript( strScript ), ullAddy( ullAddy ), strPatch( strPatch ), szPatchSz( szPatchSz ), ucPatchStorage( ucPatchStorage ), ucPatchStorageOriginal( ucPatchStorageOriginal ), isActivated( isActivated ) { };

	std::function< void ( std::string ) > setStrScript = [ & ] ( std::string strScript ) -> void {
		this->strScript = strScript;
	};

	std::function< void( unsigned long long )> setAddy = [ & ] ( ULONGLONG&& addy ) -> void {
		this->ullAddy = addy;
	};

	std::function< void ( std::string ) > setStrPatch = [ & ] ( std::string strPatch ) -> void {
		this->strPatch = strPatch;
	};
	
	std::function< void ( std::size_t ) > setSzPatchSz = [ & ] ( std::size_t szPatchSz ) -> void {
		this->szPatchSz = szPatchSz;
	};

	std::function< void ( unsigned char* ) > setUcPatchStorage = [ & ] ( unsigned char* ucPatchStorage ) -> void {
		this->ucPatchStorage = ucPatchStorage;
	};

	std::function< void ( unsigned char* ) > setUcPatchStorageOriginal = [ & ] ( unsigned char* ucPatchStorageOriginal ) -> void {
		this->ucPatchStorageOriginal = ucPatchStorageOriginal;
	};

	std::function< void ( bool ) > setActivated = [ & ] ( bool isActivated ) -> void {
		this->isActivated = isActivated;
	};

	std::function< std::string ( void ) > getStrScript = [ & ] ( void ) -> std::string {
		return this->strScript;
	};

	std::function< ULONGLONG ( void ) > getAddy = [ & ] ( void ) -> ULONGLONG {
		return this->ullAddy;
	};

	std::function< std::string ( void ) > getStrPatch = [ & ] ( void ) -> std::string {
		return this->strPatch;
	};

	std::function< std::size_t ( void ) > getSzPatchSz = [ & ] ( void ) -> std::size_t {
		return this->szPatchSz;
	};

	std::function< unsigned char* ( void ) > getUcPatchStorage = [ & ] ( void ) -> unsigned char* {
		return this->ucPatchStorage;
	};

	std::function< unsigned char* ( void ) > getUcPatchStorageOriginal = [ & ] ( void ) -> unsigned char* {
		return this->ucPatchStorageOriginal;
	};

	std::function< bool ( void ) > getActivated = [ & ] ( void ) -> bool {
		return this->isActivated;
	};

	std::function< wxString ( void ) > getListViewParsed = [ & ] ( void ) -> wxString {
		return wxString::Format( "%s -> 0x%0.2X", this->getStrScript( ), this->getAddy( ) );
	};

};