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
#include <functional>
#include <map>
#include <cstring>

class CAssemblyIterator {

private:
	uintptr_t pItPeseudoAssemblyAddrInit = 0; // Endereço inicial de busca do usuário
	uintptr_t pItPeseudoAssemblyAddrEnd = 0; // Endereço final de busca do usuário, ou endereço final da função
	uintptr_t pItPeseudoAssemblyAddrReservaEnd = 0; // Endereço de backup para rollback Endereço final
	uintptr_t pItPeseudoAssemblyAddrReservaInit = 0; // Endereço de backup para rollback Endereço Inicial

public:
	std::function< uintptr_t ( ) > getInitAdd = [ & ] ( ) -> uintptr_t {

		return this->pItPeseudoAssemblyAddrInit;

	};

	std::function< uintptr_t ( ) > getEndAdd = [ & ] ( ) -> uintptr_t {

		return this->pItPeseudoAssemblyAddrEnd;

	};

	std::function< void ( uintptr_t ) > setInitAdd = [ & ] ( uintptr_t dwInitPtr ) -> void {

		if ( this->pItPeseudoAssemblyAddrInit == 0 ) { this->pItPeseudoAssemblyAddrInit = dwInitPtr; return; }

		this->pItPeseudoAssemblyAddrReservaInit = this->pItPeseudoAssemblyAddrInit, this->pItPeseudoAssemblyAddrInit = dwInitPtr;

	};

	std::function< void( uintptr_t ) > setEndAdd = [ & ]( uintptr_t dwEndPtr ) -> void {

		if ( this->pItPeseudoAssemblyAddrEnd == 0 ) { this->pItPeseudoAssemblyAddrEnd = dwEndPtr; return; }

		this->pItPeseudoAssemblyAddrReservaEnd = this->pItPeseudoAssemblyAddrEnd, this->pItPeseudoAssemblyAddrEnd = dwEndPtr;

	};

	std::function< void ( ) > rollBackInit = [ & ] ( ) { this->pItPeseudoAssemblyAddrInit = this->pItPeseudoAssemblyAddrReservaInit; };

	std::function< void ( ) > rollBackEnd = [ & ] ( ) { this->pItPeseudoAssemblyAddrEnd = this->pItPeseudoAssemblyAddrReservaEnd; };

	std::function< void ( ) > fullRollBack = [ & ] ( ) { this->rollBackInit( ), this->rollBackEnd( ); };

};

