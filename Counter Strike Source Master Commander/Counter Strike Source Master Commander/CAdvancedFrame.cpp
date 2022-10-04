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
#include "CAdvancedFrame.hh"


auto CAdvancedFrame::OnCloseAdvancedFunctions( _In_ wxCloseEvent& evt ) -> void {

	evt.Skip(

	); // Propagando evento

}

auto CAdvancedFrame::OnButtonRemoveMemoryListPatchClicked( wxCommandEvent& evt ) -> void {

	auto* instStored = CAdvancedFrameGlobals::g_patchsInstanceList.at( CAdvancedFrameGlobals::g_wxListBoxPatchsList->GetSelection( ) );

	CVacOsSyscall::preparetePatch( instStored->getAddy( ), instStored->getUcPatchStorageOriginal( ), instStored->getSzPatchSz( ) );

	CAdvancedFrameGlobals::g_patchsInstanceList.erase( CAdvancedFrameGlobals::g_patchsInstanceList.begin( ) + CAdvancedFrameGlobals::g_wxListBoxPatchsList->GetSelection( ) );

	delete instStored->getUcPatchStorageOriginal( ); // Memória alocada que atualmente é inútil

	CAdvancedFrameGlobals::g_wxListPatchsView.RemoveAt( CAdvancedFrameGlobals::g_wxListBoxPatchsList->GetSelection( ) );

	CAdvancedFrameGlobals::g_wxListBoxPatchsList->Clear( );

	CAdvancedFrameGlobals::g_wxListBoxPatchsList->InsertItems( CAdvancedFrameGlobals::g_wxListPatchsView, 0 );

	CAdvancedFrameGlobals::g_patchsInstanceList.erase( CAdvancedFrameGlobals::g_patchsInstanceList.begin( ) + CAdvancedFrameGlobals::g_wxListBoxPatchsList->GetSelection( ) );

}

auto CAdvancedFrame::OnButtonAddMemoryListPatchClicked( wxCommandEvent& evt ) -> void {

	wxString wxStrScript( "" );

	auto* cPatch = new CPatchsModel( "", NULL, "", 0, NULL, NULL, false );

	auto* wxTextEntryDlgInfo = new wxTextEntryDialog( );

	wxTextEntryDlgInfo->Create( this, "Entre com o script a ser executado: ", "Adicionar um novo Patch/Shellcode", wxStrScript );

	if ( wxTextEntryDlgInfo->ShowModal( ) == wxID_CANCEL ) {

		wxMessageBox( "Você cancelou o processo de adicionar um novo patch/shellcode !", "Ops, cancelado !", wxICON_INFORMATION );
		
		return;
	}	
	else wxStrScript = wxTextEntryDlgInfo->GetValue( );

	cPatch->setStrScript( wxStrScript.ToStdString( ) );

	auto [ullAddy, strInfoCallback] = CNKurumimLangParser::getParsed( wxStrScript.ToStdString( ) );

	switch ( ullAddy )
	{

	case CNKurumimLangParser::Utils::errorCodes::CNKURUMI_ERROR_IS_INVALID_MODULE:
	{
		CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( "Error[CNKURUMI_ERROR_IS_INVALID_MODULE]: " + strInfoCallback );

		return;
	}

	case CNKurumimLangParser::Utils::errorCodes::CNKURUMI_ERROR_IS_VALID_SCRIPT:
	{
		CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( "Error[CNKURUMI_ERROR_IS_VALID_SCRIPT]: " + strInfoCallback );

		return;
	}

	case CNKurumimLangParser::Utils::errorCodes::CNKURUMI_UKNOWN_ERROR:
	{
		CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( "Error[CNKURUMI_UKNOWN_ERROR]: " + strInfoCallback );

		return;
	}

	default:
		break;
	}

	cPatch->setAddy( ullAddy );

	wxTextEntryDlgInfo->SetValue( "" );

	wxTextEntryDlgInfo->~wxTextEntryDialog( );

	wxTextEntryDlgInfo = new wxTextEntryDialog( );

	wxTextEntryDlgInfo->Create( this, "Entre com o shellcode a ser utilizado, no padrão: AB CD EF FF AA BB", "Informe o shellcode !", wxStrScript );

	if ( wxTextEntryDlgInfo->ShowModal( ) == wxID_CANCEL ) {

		wxMessageBox( "Você cancelou o processo de adicionar um shellcode !", "Ops, cancelado !", wxICON_INFORMATION );

		return;
	} else wxStrScript = wxTextEntryDlgInfo->GetValue( );

	std::string strShellCode( wxStrScript.ToStdString( ) );

	CVacOsSyscall::doStringTrim( &strShellCode );

	CVacOsSyscall::doStringHexClear( &strShellCode );

	auto* shellCodeMemory = new unsigned char[ strShellCode.length( ) / 2 ];

	CVacOsSyscall::StringToMemory( strShellCode, shellCodeMemory );

	cPatch->setStrPatch( strShellCode );

	cPatch->setSzPatchSz( strShellCode.length( ) / 2 );

	cPatch->setUcPatchStorage( shellCodeMemory );

	cPatch->setUcPatchStorageOriginal( CVacOsSyscall::preparetePatch( ullAddy, shellCodeMemory, strShellCode.length( ) / 2) );

	cPatch->setActivated( true );

	CAdvancedFrameGlobals::g_patchsInstanceList.push_back( cPatch );

	CAdvancedFrameGlobals::g_wxListPatchsView.Add( cPatch->getListViewParsed( ) );

	CAdvancedFrameGlobals::g_wxListBoxPatchsList->Clear( );

	CAdvancedFrameGlobals::g_wxListBoxPatchsList->InsertItems ( CAdvancedFrameGlobals::g_wxListPatchsView, 0 );

}

auto WINAPI CAdvancedFrame::diasmThread( PVOID args ) -> DWORD {

	try {
		auto* engineContext = reinterpret_cast<tpdEngineContext*>( args );

		CAdvancedFrameGlobals::g_disassemblerWrapper = new CDisassemblerWrapper( );

		CAdvancedFrameGlobals::g_wxTextDisasm->Clear( );

		CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( "[ ! ] Aguarde enquando a engine trabalha...\n" );

		auto [ infoa, infob ] = CAdvancedFrameGlobals::g_disassemblerWrapper->wrpDisasmBegin( "NULL", ( ULONGLONG )engineContext->ullAddy, ( ULONGLONG )engineContext->ullAddy + 1024 );

		if ( infoa.empty( ) && infob.empty( ) ) {
			CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( "Tente de novo, erro de endereço inválido !!\n" );
			return WN_BAD_POINTER;
		}

		try {
			CAdvancedFrameGlobals::g_wxTextDisasm->Clear( );

			CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( infoa + " " + infob );
		}catch( ... ) { }
	}
	catch ( std::exception& ex ) { }

	return WN_SUCCESS;
}


auto CAdvancedFrame::OnButtonDecompilerClicked( wxCommandEvent& evt ) -> void {

	wxMessageBox(
	
		_In_ "Obrigado pelo interesse em utilizar nosso decompiler baseado em nuvem, esse decompiler é gratuito para uso não comercial, com ele é possível usar a tecnologia proprietaria da minha tecnologia pseudo-código.",
		_In_ "Atenção !"

	);

	wxMessageBox(

		_In_ "Ops, o Decompiler ainda não está disponível, fique de olho no andamento do projeto ;)",
		_In_ "Ops, desculpe !"

	);

}


auto CAdvancedFrame::OnButtonExecuteClick( wxCommandEvent& evt ) -> void {

	auto [ullAddy, strArguments ] = CNKurumimLangParser::getParsed(
																		CAdvancedFrameGlobals::g_wxTextInputEndereco->GetValue( ).ToStdString( )
																   );

	switch ( ullAddy )
	{

		case CNKurumimLangParser::Utils::errorCodes::CNKURUMI_ERROR_IS_INVALID_MODULE:
		{
			CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( "Error[CNKURUMI_ERROR_IS_INVALID_MODULE]: " + strArguments );

			return;
		}

		case CNKurumimLangParser::Utils::errorCodes::CNKURUMI_ERROR_IS_VALID_SCRIPT:
		{
			CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( "Error[CNKURUMI_ERROR_IS_VALID_SCRIPT]: " + strArguments );

			return;
		}

		case CNKurumimLangParser::Utils::errorCodes::CNKURUMI_UKNOWN_ERROR:
		{
			CAdvancedFrameGlobals::g_wxTextDisasm->WriteText( "Error[CNKURUMI_UKNOWN_ERROR]: " + strArguments );

			return;
		}

	default: // A engine do script parser não retornou nenhum erro !
		break;
	}

	auto* engineContext = new tpdEngineContext[ ] {

													ullAddy,
													strArguments

												};

	HANDLE hThread = CreateThread(

		_In_opt_ NULL,
		_In_ NULL,
		_In_ CAdvancedFrame::diasmThread,
		_In_opt_ __drv_aliasesMem (LPVOID)engineContext,
		_In_ NULL,
		_Out_opt_ NULL

	);

	if (!CloseHandle(

		_In_ _Post_ptr_invalid_ hThread

	))
		OutputDebugString( _In_opt_ L"Falhou ao tentar fechar o HANDLE de diasm thread" );

}


CAdvancedFrame::CAdvancedFrame( const wxString& wxStrTitulo ) : wxFrame( 
																		nullptr, wxID_ANY, wxStrTitulo,
																		wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE 
																										      & ~wxMAXIMIZE_BOX
																		) {

	auto* pBase = new wxPanel( 

		_In_ this 

	);

	pBase->SetBackgroundColour(

		_In_ UiExperience::ui_InterfaceBackgroundColorStandard

	);

	CreateStatusBar(

	);

	auto wxStaticInformacao = new wxStaticText(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Digite um endereço:",
		_In_ wxPoint(
			_In_ 10,
			_In_ 13
		)

	);

	wxStaticInformacao->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	CAdvancedFrameGlobals::g_wxTextInputEndereco = new wxTextCtrl(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "IMGBASE(\"hl2.exe\")+RVA(\"0x00000000\")",
		_In_ wxPoint(
			_In_ 120,
			_In_ 10
		),
		_In_ wxSize(
			_In_ 220,
			_In_ -1
		)

	);

	auto* wxStaticOpcoes = new wxStaticText(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Selecione uma opção:",
		_In_ wxPoint(
			_In_ 340,
			_In_ 13
		)

	);

	wxStaticOpcoes->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxArrayString wxStrChoises;

	wxStrChoises.Add( _In_ "Disasm via Endereço" );
	wxStrChoises.Add( _In_ "Disasm via Pattern" );
	wxStrChoises.Add( _In_ "Disasm toda a seção .text" );
	wxStrChoises.Add( _In_ "Disasm todas as seções do binário PE" );

	auto* wxComBoxOpcoes = new wxComboBox(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "",
		_In_ wxPoint(
			_In_ 460,
			_In_ 10
		),
		_In_ wxSize(
			_In_ 200,
			_In_ -1
		),
		_In_ wxStrChoises

	);

	auto* wxButtonExecutar = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Executar",
		_In_ wxPoint(
			_In_ 680,
			_In_ 7
		),
		_In_ wxSize(
			_In_ -1,
			_In_ 30
		)

	);

	wxButtonExecutar->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonExecutar->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonExecutar->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxButtonExecutar->Bind(

		_In_ wxEVT_BUTTON,
		_In_ &CAdvancedFrame::OnButtonExecuteClick,
		_In_ this

	);

	auto* wxStaticDisasmView = new wxStaticText(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Visão do Assembly:",
		_In_ wxPoint(
			_In_ 10,
			_In_ 50
		)

	);

	wxStaticDisasmView->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	CAdvancedFrameGlobals::g_wxTextDisasm = new wxTextCtrl(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "",
		_In_ wxPoint(
			_In_ 10,
			_In_ 70
		),
		_In_ wxSize(
			_In_ 450,
			_In_ 450
		),
		wxBORDER_NONE | wxTE_MULTILINE

	);

	CAdvancedFrameGlobals::g_wxListBoxPatchsList = new wxListBox(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ wxPoint(
			_In_ 470,
			_In_ 130
		),
		_In_ wxSize(
			_In_ 300,
			_In_ 300
		)

	);

	auto* wxStaticMPatch = new wxStaticText(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Minhas modificações no binário:",
		_In_ wxPoint(
			_In_ 470,
			_In_ 50
		)

	);

	wxStaticMPatch->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	auto* wxButtonAddPatch = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "+",
		_In_ wxPoint(
			_In_ 470,
			_In_ 108
		),
		_In_ wxSize(
			_In_ 20,
			_In_ 20
		)

	);

	wxButtonAddPatch->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonAddPatch->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonAddPatch->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxButtonAddPatch->Bind(

		_In_ wxEVT_BUTTON,
		_In_ &CAdvancedFrame::OnButtonAddMemoryListPatchClicked,
		_In_ this

	); 

	auto* wxButtonRemovePatch = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "-",
		_In_ wxPoint(
			_In_ 490,
			_In_ 108
		),
		_In_ wxSize(
			_In_ 20,
			_In_ 20
		)

	);

	wxButtonRemovePatch->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonRemovePatch->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonRemovePatch->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxButtonRemovePatch->Bind(

		_In_ wxEVT_BUTTON,
		_In_ &CAdvancedFrame::OnButtonRemoveMemoryListPatchClicked,
		_In_ this

	);

	auto wxButtonStartPatch = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "OFF",
		_In_ wxPoint(
			_In_ 740,
			_In_ 100
		),
		_In_ wxSize(
			_In_ 30,
			_In_ 30
		)

	);

	wxButtonStartPatch->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonStartPatch->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonStartPatch->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	auto* wxButtonStopPatch = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "ON",
		_In_ wxPoint(
			_In_ 710,
			_In_ 100
		),
		_In_ wxSize(
			_In_ 30,
			_In_ 30
		)

	);

	wxButtonStopPatch->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonStopPatch->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonStopPatch->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	auto* wxButtonDecompiler = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Obter Pseudo-C\n(Servidor na Nuvem)",
		_In_ wxPoint(
			_In_ 470,
			_In_ 450
		),
		_In_ wxSize(
			_In_ 120,
			_In_ 70
		)

	);

	wxButtonDecompiler->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonDecompiler->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonDecompiler->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxButtonDecompiler->Bind(

		wxEVT_BUTTON,
		&CAdvancedFrame::OnButtonDecompilerClicked,
		this

	);

	auto* wxButtonLoadExemplePatchs = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Carregar arquivo(patchs)\n(Exemplos da comunidade)",
		_In_ wxPoint(
			_In_ 650,
			_In_ 450
		),
		_In_ wxSize(
			_In_ 120,
			_In_ 70
		)

	);

	wxButtonLoadExemplePatchs->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonLoadExemplePatchs->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonLoadExemplePatchs->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	this->Bind(

		wxEVT_CLOSE_WINDOW,
		&CAdvancedFrame::OnCloseAdvancedFunctions,
		this

	);

}