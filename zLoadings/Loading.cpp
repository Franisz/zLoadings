// Supported with union (c) 2020 Union team
// Union SOURCE file

#include <codecvt>

namespace GOTHIC_ENGINE {
  std::string utf8_to_ansi( const std::string& str, const std::locale& loc = std::locale{} ) {
    using wcvt = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>;
    auto wstr = wcvt{}.from_bytes( str );
    std::string result( wstr.size(), '0' );
    std::use_facet<std::ctype<wchar_t>>( loc ).narrow( wstr.data(), wstr.data() + wstr.size(), '?', &result[0] );
    return result;
  }

  void LoadManager::GetRandom() {
    text = "";
    splash = "";

    if ( !jsonFile["loadings"].is_array() )
      return;

    if( jsonFile["loadings"].size() < 1 )
      return;

    int rand = randomizer.Random( 0, jsonFile["loadings"].size() - 1 );

    if ( !jsonFile["loadings"][rand].is_object() )
      return;

    if ( jsonFile["loadings"][rand]["splash"].is_string() )
      splash = utf8_to_ansi( jsonFile["loadings"][rand]["splash"].get<std::string>(), locale ).c_str();

    if ( !jsonFile["loadings"][rand]["text"].is_object() )
      return;

    if ( jsonFile["loadings"][rand]["text"][lang.ToChar()].is_string() )
      text = utf8_to_ansi( jsonFile["loadings"][rand]["text"][lang.ToChar()].get<std::string>(), locale ).c_str();
  }

  void LoadManager::ClearScreen() {
    if ( quoteView && ogame->load_screen ) {
      quoteView->ClrPrintwin();
      ogame->load_screen->RemoveItem( quoteView );
      quoteView = nullptr;
    }
  }

  void LoadManager::TryAddText() {
    if ( !text.Length() )
      return;

    zCFont* oldFont = ogame->load_screen->font;

    // Try font change
    if ( jsonFile["font"].is_string() )
      if ( auto sym = parser->GetSymbol( Z utf8_to_ansi( jsonFile["font"].get<std::string>(), locale ).c_str() ) )
        if ( sym->stringdata && sym->stringdata->Length() )
          ogame->load_screen->SetFont( sym->stringdata );

    // Text view
    if ( !quoteView ) {
      quoteView = new zCView( 0, (8192 * 0.9) - (ogame->load_screen->FontY() * 2), 8192, 8192 * 0.9 );
      quoteView->SetAlphaBlendFunc( zRND_ALPHA_FUNC_BLEND );
      quoteView->SetTransparency( 120 );
      quoteView->SetFont( ogame->load_screen->font );

      if ( zCTexture* tex = tex->Load( Z "ZLOADING_BACKGROUND", true ) ) {
        quoteView->InsertBack( tex );
        tex->Release();
      }

      ogame->load_screen->InsertItem( quoteView );
      ogame->load_screen->SetFont( oldFont );
    }

    // Set and print text
    quoteView->ClrPrintwin();
    quoteView->PrintTimedCXY( text, -2.f, &zCOLOR( 255, 255, 255 ) );
  }

  void LoadManager::SetScreen() {
    TryAddText();

    if ( !splash.Length() )
      return;

    // Set screen texture
    if ( zCTexture* tex = tex->Load( Z splash, true ) ) {
      ogame->load_screen->InsertBack( splash );
      tex->Release();
    }
  }

  void LoadManager::ParseConfig() {
    zoptions->ChangeDir( DIR_SYSTEM );
    zFILE_VDFS* originFile = new zFILE_VDFS( configFileName );

    if ( !originFile->Exists() ) {
      delete originFile;
      return;
    }

    originFile->Open( false );

    zSTRING line, buffer;
    do {
      originFile->Read( line );
      buffer += line;
    } while ( !originFile->Eof() );

    jsonFile = nlohmann::json::parse( buffer.ToChar() );
  }

  void LoadManager::SetLanguage() {
    string sym = "";
    switch ( Union.GetSystemLanguage() )
    {
    case Lang_Rus:
      sym = "ru";
      break;
    case Lang_Ger:
      sym = "de";
      break;
    case Lang_Pol:
      sym = "pl";
      break;
    default:
      sym = "en";
      break;
    }

    lang = A zoptions->ReadString( PLUGIN_NAME, "LanguageSymbol", sym ).Lower();

    if ( lang == "pl" )
      locale = std::locale( ".1250" );
    else if ( lang == "ru" )
      locale = std::locale( ".1251" );
    else
      locale = std::locale( ".1252" );
  }

  void LoadManager::Init() {
    ParseConfig();
    SetLanguage();
  }

  HOOK Hook_oCGame_OpenLoadscreen PATCH( &oCGame::OpenLoadscreen, &oCGame::OpenLoadscreen_Union );
  void oCGame::OpenLoadscreen_Union( bool a1, zSTRING a2 ) {

    // Skip on ZEN change
    if ( a2.Length() ) {
      THISCALL( Hook_oCGame_OpenLoadscreen )(a1, a2);
      return;
    }

    if ( !ogame->load_screen ) {
      THISCALL( Hook_oCGame_OpenLoadscreen )(a1, a2);
      loadManager.GetRandom();
    }

    if ( !loadManager.splash.Length() && !loadManager.text.Length() )
      return;

    loadManager.SetScreen();
  }

  HOOK Hook_oCGame_CloseLoadscreen PATCH( &oCGame::CloseLoadscreen, &oCGame::CloseLoadscreen_Union );
  void oCGame::CloseLoadscreen_Union() {
    loadManager.ClearScreen();
    THISCALL( Hook_oCGame_CloseLoadscreen )();
  }
}