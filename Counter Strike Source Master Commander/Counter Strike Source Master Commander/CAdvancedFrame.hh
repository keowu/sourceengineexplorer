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

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include <vector>
#include "CDisassemblerWrapper.hh"
#include "CUiExperience.hh"
#include "CNKurumimLang.hh"
#include "CPatchsModel.hh"

namespace CAdvancedFrameGlobals {

	static wxTextCtrl* g_wxTextDisasm;

	static wxListBox* g_wxListBoxPatchsList;

	static CDisassemblerWrapper* g_disassemblerWrapper;

	static wxTextCtrl* g_wxTextInputEndereco;

	static wxArrayString g_wxListPatchsView;

	static std::vector< CPatchsModel* > g_patchsInstanceList;

};

class CAdvancedFrame : public wxFrame
{

private:

	typedef struct {

		ULONGLONG ullAddy;
		std::string strInfo;

	} tpdEngineContext;

	static auto WINAPI diasmThread( PVOID args ) -> DWORD;

	auto OnButtonExecuteClick( wxCommandEvent& evt ) -> void;

	auto OnButtonDecompilerClicked( wxCommandEvent& evt ) -> void;

	auto OnButtonAddMemoryListPatchClicked( wxCommandEvent& evt ) -> void;

	auto OnButtonRemoveMemoryListPatchClicked( wxCommandEvent& evt ) -> void;

	auto OnCloseAdvancedFunctions( _In_ wxCloseEvent& evt ) -> void;

public:

	CAdvancedFrame( const wxString& wxStrTitulo );

};

