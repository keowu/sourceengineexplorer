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
#include "MainFrame.hh"

auto WINAPI MainFrame::CounterStrikeSource::IsCounterStrikeRunning( _In_ PVOID args ) -> DWORD {

	auto cssCtxt = reinterpret_cast< MainFrame::CounterStrikeSource::wx_CSSCtxt * >( args );

	auto iRounded = 0;

	while ( 1 )
		
		if ( CVacOsSyscall::isProcActive( _In_ L"Counter-Strike Source" ) ) {
			
			cssCtxt->ctx_staticLabelStatus->SetLabel( "O cliente está em execução !" );
			
			cssCtxt->ctx_staticLabelStatus->SetForegroundColour(

				wxColour(
					_In_ 10,
					_In_ 164,
					_In_ 41
				)

			);
			cssCtxt->ctx_buttonReconnect->SetLabel( _In_ "Conectar emulador" );

			cssCtxt->ctx_buttonReconnect->Enable( _In_ true );

			break;

		}

	return WN_SUCCESS;
}


auto MainFrame::OnCloseMasterCommander( _In_ wxCloseEvent& evt ) -> void {

	evt.Skip( 

	); // Propagando evento

	std::exit( _In_ -1 );
}

auto MainFrame::OnUserClickSendNetworkCustomPackage( _In_ wxCommandEvent& evt ) -> void {

	wxMessageBox(

		_In_ "Desculpe o recurso para transmissão de um conjunto de bytes esta em desenvolvimento.",
		_In_ "Ops, ainda não!",
		_In_ wxICON_ERROR

	);

}

auto MainFrame::OnUserClickAboutSoftware( _In_ wxCommandEvent& evt ) -> void {

	wxMessageBox(

		_In_ "Este software foi desenvolvido por www.github.com/keowu\nVenha para a nossa comunidade amigável no Discord: EM BREVE\nE-mail: uwu@keowu.anonaddy.me\n",
		_In_ "Sobre o software",
		_In_ wxICON_INFORMATION

	);

}

auto MainFrame::OnUserClickInitEmulator(_In_ wxCommandEvent& evt) -> void {

	if ( !CVacOsSyscall::EmulatorInterface::initEmulator( ) )

		wxMessageBox(

			_In_ "Falhou ao injetar DLL !"

		);

}

auto MainFrame::OnUserClickSaveProtocol( _In_ wxCommandEvent& evt ) -> void {
	
	auto* wxContinuarComParallelSave = new wxMessageDialog(

		_In_ this,
		_In_ "Tem certeza absoluta que quer continuar ? ao salvar TODOS os \"núcleos\" da sua CPU serão utilizados para salvar os pacotes capturados que provavelmente serão grandes, então prossiga com cautela !",
		_In_ "Leia com atenção antes de prosseguir!",
		_In_ wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION

	);
	
	if ( wxContinuarComParallelSave->ShowModal( ) == wxID_NO ) return;
	
	HANDLE tidParallelProc = CreateThread(

		_In_opt_ NULL,
		_In_ NULL,
		_In_ CGlobalsUtils::parallelIntelAmdMixCores,
		_In_opt_ __drv_aliasesMem (void *)&CGlobalsUtils::g_parallelCtx,
		_In_ NULL,
		_Out_opt_ NULL

	);

	if ( tidParallelProc == INVALID_HANDLE_VALUE )
		wxMessageBox(

			_In_ "Problema, não foi possível criar uma thread para o Parallel Loop !",
			_In_ "Problema ao utilizar processamento máximo !"

		);

	if ( !CloseHandle(

		_In_ _Post_ptr_invalid_ tidParallelProc

	) )
		wxMessageBox(

			_In_ "Problema ao fechar a referencia inútil para a thread de parallel loop !",
			_In_ "Problema !"

		);


}

auto MainFrame::OnUserClickLimparProtocol( _In_ wxCommandEvent& evt ) -> void {

	CGlobalsUtils::g_parallelCtx.wxTextProtocol->Clear(

	);
	
	CGlobalsUtils::g_parallelCtx.wxProgressBar->SetValue(

		_In_ 0

	);

	wxMessageBox(

		_In_ "Foi Limpado !"

	);

}


auto MainFrame::OnUserClickAdvancedOption( _In_ wxCommandEvent& evt ) -> void {
	
	//O Cliente está em execução ?
	if ( !CVacOsSyscall::isProcActive( L"Counter-Strike Source" ) ) return;

	auto* cAdvancedFrameParallel = new CAdvancedFrame( "Opções avançadas !" );

	cAdvancedFrameParallel->SetSize(

		_In_ wxSize(

			_In_ 800,
			_In_ 600

		)

	);

	cAdvancedFrameParallel->SetMaxSize(

		_In_ wxSize(

			_In_ 800,
			_In_ 600

		)

	);

	cAdvancedFrameParallel->SetMinSize(

		_In_ wxSize(

			_In_ 800,
			_In_ 600

		)

	);

	cAdvancedFrameParallel->Center(

	);

	cAdvancedFrameParallel->Show(

	);

}


auto WINAPI MainFrame::InterProcessCommThread( _In_ PVOID args ) -> DWORD {

	auto* wxParallelContext = reinterpret_cast< CGlobalsUtils::wx_parallelContext* >( args );

	CInterProcessServerComm::createComm( );

	if( !CInterProcessServerComm::g_bIsClosed ) wxParallelContext->wxTextProtocol->WriteText(

			_In_ "\nERRO AO INICIAR PROTOCOLO INTERPROCESS!"

		);

	char buff[ 1024 ] { 0 };

	while ( true ) {
		
		CInterProcessServerComm::readComm(

			   _In_ buff,
		       _In_ sizeof(buff)

		);

	
	if ( strstr( _In_z_ buff, _In_z_ "\n[LOGS_INTER]" ) ) {

		std::string strFix( buff );

		strFix.erase( strFix.begin() + 1, strFix.begin() + 13 );

		wxParallelContext->wxLogCtrl->WriteText(

			strFix

		);

	}else if ( strcmp(_In_z_ "[INIT_BIG_BUFF]", _In_z_ buff ) == 0 ) { //LOGICA DIFERENCIADA, SE O COMANDO [INIT_BIG_BUFF] for enviado

			//LER O TAMANHO
			CInterProcessServerComm::readComm(

				  _In_ buff,
			  	  _In_ sizeof(buff)

			);

			int szBuff = std::atoi( _In_z_ buff );

			auto* buffCustom = reinterpret_cast< char * >( std::malloc( _In_ szBuff * 2 ) );

			CInterProcessServerComm::readComm(

				_In_ buffCustom,
				_In_ sizeof(buffCustom)

			);

			std::string str(

				_In_ buffCustom

			);

			//Remover possíveis caracteres não ascii presentes
			CVacOsSyscall::doStringHexClear( 

				&str

			);

			wxParallelContext->wxTextProtocol->WriteText(

				_In_ str

			);

			std::free(

				_Pre_maybenull_ _Post_invalid_ buffCustom

			);

	}
	else {
		wxParallelContext->wxTextProtocol->WriteText(

			_In_ buff

		);
	}

		std::memset(

			_Out_writes_bytes_all_(_Size) buff,
			_In_ 0,
			_In_ 1024

		);

	}

	CInterProcessServerComm::closeComm(

	);

	return 0;

}

MainFrame::MainFrame( const wxString& wxsTitulo ) : wxFrame( 
															nullptr, wxID_ANY, wxsTitulo,
															wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE 
																							  & ~wxMAXIMIZE_BOX
															) {

	auto* pBase = new wxPanel(

		_In_ this

	);

	pBase->SetBackgroundColour(

		_In_ UiExperience::ui_InterfaceBackgroundColorStandard

	);

	CGlobalsUtils::g_parallelCtx.wxTextProtocol = new wxTextCtrl(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "",
		wxPoint(
			_In_ 400,
			_In_ 50
		),
		wxSize(
			_In_ 350,
			_In_ 480
		),
		wxBORDER_NONE | wxTE_MULTILINE

	);

	CGlobalsUtils::g_parallelCtx.wxTextProtocol->SetBackgroundColour(

		_In_ UiExperience::ui_TextCtrlColorStandardBack

	);

	CGlobalsUtils::g_parallelCtx.wxTextProtocol->SetForegroundColour(

		_In_ UiExperience::ui_TextCtrlColorStandard

	);

	CGlobalsUtils::g_parallelCtx.wxTextProtocol->SetEditable(

		_In_ false

	);

	CGlobalsUtils::g_parallelCtx.wxTextProtocol->SetEditable(

		_In_ false

	);

	auto* wxButtonClearProtocol = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Limpar Janela de Protocolo",
		wxPoint(
			_In_ 400,
			_In_ 25
		)

	);

	wxButtonClearProtocol->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxButtonClearProtocol->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonClearProtocol->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonClearProtocol->Bind(

		_In_ wxEVT_BUTTON,
		_Inout_ &MainFrame::OnUserClickLimparProtocol,
		_In_ this

	);

	auto* wxButtonSaveProtocol = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Salvar dados de Protocolo",
		wxPoint(
			_In_ 593,
			_In_ 25
		)

	);

	wxButtonSaveProtocol->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxButtonSaveProtocol->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonSaveProtocol->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonSaveProtocol->Bind(

		_In_ wxEVT_BUTTON,
		_Inout_ &MainFrame::OnUserClickSaveProtocol,
		_In_ this

	);

	auto* wxStaticTextInforSendProtocol = new wxStaticText(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Comando: ",
		_In_ wxPoint(
			_In_ 400,
			_In_ 536
		)

	);

	wxStaticTextInforSendProtocol->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	auto* wxTextCtrlSendProtocolCmd = new wxTextCtrl(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "",
		wxPoint(
			_In_ 460,
			_In_ 534
		),
		wxSize(
			_In_ 240,
			_In_ -1
		)

	);

	auto* wxButtonSendProtocolCmd = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Enviar",
		wxPoint(
			_In_ 700,
			_In_ 531
		),
		wxSize(
			_In_ 45,
			_In_ 28
		)

	);

	wxButtonSendProtocolCmd->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxButtonSendProtocolCmd->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonSendProtocolCmd->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonSendProtocolCmd->Bind(

		_In_ wxEVT_BUTTON,
		_In_ &MainFrame::OnUserClickSendNetworkCustomPackage,
		_In_ this

	);

	auto* wxStaticTextGameRunning = new wxStaticText(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "O Cliente não está em execução",
		wxPoint(
			_In_ 20,
			_In_ 25
		),
		wxSize(
			_In_  -1,
			_In_ 35
		)

	);

	wxStaticTextGameRunning->SetForegroundColour(

		wxColour(
			_In_ 255,
			_In_ 0,
			_In_ 0
		)

	);

	auto* wxButtonReconnectToClient = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Tentar Reconectar",
		wxPoint(
			_In_ 190,
			_In_ 21
		)

	);

	wxButtonReconnectToClient->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxButtonReconnectToClient->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxButtonReconnectToClient->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxButtonReconnectToClient->Bind(

		wxEVT_BUTTON,
		&MainFrame::OnUserClickInitEmulator,
		this

	);

	CGlobalsUtils::g_parallelCtx.wxProgressBar = new wxGauge(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ 100,
		_In_ wxPoint(
			_In_ 400,
			_In_ 10
		),
		_In_ wxSize(
			_In_ 350,
			_In_  -1
		)

	);

	auto* wxStaticTextDescriptLogSdk = new wxStaticText(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Logs interceptados da Source Engine, SDK da Valve e Steam: ",
		_In_ wxPoint(
			_In_ 20,
			_In_ 80
		),
		_In_ wxDefaultSize,
		_In_ wxBORDER_NONE | wxTE_MULTILINE

	);

	wxStaticTextDescriptLogSdk->SetForegroundColour(

		_In_ UiExperience::ui_TextCtrlColorStandardBack

	);

	wxStaticTextDescriptLogSdk->SetBackgroundColour(

		_In_ UiExperience::ui_TextCtrlColorStandard

	);

	CGlobalsUtils::g_parallelCtx.wxLogCtrl = new wxTextCtrl(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "",
		_In_ wxPoint(
			_In_ 20,
			_In_ 100
		),
		_In_ wxSize(
			_In_ 350,
			_In_ 400
		),
		_In_ wxBORDER_NONE | wxTE_MULTILINE

	);

	auto* wxAdvancedOptions = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Funções avançadas",
		_In_ wxPoint(
			_In_ 50,
			_In_ 515
		)

	);

	wxAdvancedOptions->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxAdvancedOptions->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxAdvancedOptions->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxAdvancedOptions->Bind(

		wxEVT_BUTTON,
		&MainFrame::OnUserClickAdvancedOption,
		this

	);

	auto* wxAboutButton = new wxButton(

		_In_ pBase,
		_In_ wxID_ANY,
		_In_ "Sobre a Aplicação",
		_In_ wxPoint(
			_In_ 215,
			_In_ 515
		)

	);

	wxAboutButton->SetBackgroundColour(

		_In_ UiExperience::ui_ButtonBackgroundStandard

	);

	wxAboutButton->SetForegroundColour(

		_In_ UiExperience::ui_ButtonFontColorStandard

	);

	wxAboutButton->SetWindowStyle(

		_In_ wxBORDER_NONE

	);

	wxAboutButton->Bind(

		_In_ wxEVT_BUTTON,
		_In_ &MainFrame::OnUserClickAboutSoftware,
		_In_ this

	);
	
	this->Bind(

		_In_ wxEVT_CLOSE_WINDOW,
		_In_ &MainFrame::OnCloseMasterCommander,
		_In_ this

	);

	auto hIpcThread = CreateThread(

		_In_opt_ NULL,
		_In_ NULL,
		_In_ MainFrame::InterProcessCommThread,
		_In_opt_ __drv_aliasesMem(void*)&CGlobalsUtils::g_parallelCtx,
		_In_ NULL,
		_Out_opt_ NULL

	);

	if (hIpcThread == INVALID_HANDLE_VALUE) wxMessageBox(

			_In_ "Problema para criar a thread !",
			_In_ "Erro !"

		);

	auto* th_CSSCtxt = new MainFrame::CounterStrikeSource::wx_CSSCtxt{ 
																		_In_ wxButtonReconnectToClient,
																		_In_ wxStaticTextGameRunning 
																	 };

	auto hCsCtxChecker = CreateThread(

		_In_opt_ NULL,
		_In_ NULL,
		_In_ MainFrame::CounterStrikeSource::IsCounterStrikeRunning,
		_In_opt_ __drv_aliasesMem (void *)th_CSSCtxt,
		_In_ NULL,
		_Out_opt_ NULL

	);

	if (hCsCtxChecker == INVALID_HANDLE_VALUE) wxMessageBox(

			_In_ "Problema ao criar um thread para contexto de verificação do CSS!",
			_In_ "ERRO !"

		);

}