/*
 _   __ _____ _____  _    _ _   _
| | / /|  ___|  _  || |  | | | | |
| |/ / | |__ | | | || |  | | | | |
|    \ |  __|| | | || |/\| | | | |
| |\  \| |___\ \_/ /\  /\  / |_| |
\_| \_/\____/ \___/  \/  \/ \___/
							2022

Copyright (c) Fluxuss Cyber Tech Desenvolvimento de Software, SLU (FLUXUSS)

Algumas estruturas presentes neste projeto podem ser pertencentes ao c?digo fonte do Windows cujo propriet?rio e detentor se denomina Microsoft(Copyright (c) Microsoft Corporation)
sua utiliza??o ? feita de acordo com atos "legais" de acordo com leis, como a Lei de direito autoral Brasileira e Lei Americana de Direito Autoral.
alguns desses dados foram extra?dos da ntdll.h extra?da do c?digo fonte do React Os(Copyright (c) React Os), e complementadas com o c?digo fonte vazado do Windows XP.

Esse c?digo fonte est? progido de acordo com Copyright Act of 1976 lei aplic?vel a devida parte interessadas Microsoft, Valve(Copyright (c) Valve Corporation).
Esse c?digo se aplica "ao uso justo", mais informa??es https://www.copyright.gov/title17/.

Obrigado Open Source <3

Com todo respeito e nada mais a declarar.


"The Kurumi Source Engine explorer Engine"

THE CAKE IS A LIE!

  ^^
Keowu

*/
#pragma once
#include <wx/wx.h>
#include <iostream>
#include <ppl.h>
#include <fstream>
#include <filesystem>

#define KEOWU_MS_IS_QUIT 0
#define KEOWU_MS_IS_SHIT -1

namespace CGlobalsUtils
{

	typedef struct wx_parallelContext{

		wxTextCtrl* wxTextProtocol;
		wxGauge* wxProgressBar;
		wxTextCtrl* wxLogCtrl;

	};

	static wx_parallelContext g_parallelCtx{ 0 };

	auto doReculculationOfWindowsForm( void ) -> void;

	static HANDLE g_mutex;

	auto WINAPI parallelIntelAmdMixCores( PVOID args ) -> DWORD;

};

