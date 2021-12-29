// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {

#define PLUGIN_NAME "ZLOADINGS"

  class LoadManager {
  private:
    nlohmann::json jsonFile;
    const string configFileName = "zLoadings.json";
    string lang;
    std::locale locale;

    zCView* quoteView;

    string GetSysPackLanguage();
    void TryAddText();
    void ParseConfig();

  public:
    string text;
    string splash;

    void GetRandom();
    void SetScreen();
    void ClearScreen();
    void Init();
  };

  LoadManager loadManager;
}