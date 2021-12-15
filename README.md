# zLoadings

Plugin made in [Union](https://worldofplayers.ru/threads/40376/) **1.0l** for Gothic 1 and 2.

## Usage

This plugin allows creating random loading screens in a very convenient way. All required files can be packed into one `.mod`/`.vdf` file.

- Plugin file: `zLoadings.dll` has to be put in `\System\Autorun\`.
- Config file: `zLoadings.json` has to be put in `\System\`.
- Text background texture: `ZLOADING_BACKGROUND-C.tex`.
- Textures for loading screens.

### Json config

Json file contains an array of loadings with defined texture name and completely optional `text` object with text that can appear on load.

- Single loading object with text

  ```json
  {
    "splash": "ZLOADING_BEAST_1",
    "text": {
      "en": "Dragon snapper",
      "pl": "Smoczy zębacz",
      "ru": "Драконий снеппер"
    }
  }
  ```

- Optionally, different font can be set for text on loadings.

  ```json
  {
    "font": "FONT_SCREEN",
    "loadings": []
  }
  ```

Example config file

```json
{
  "font": "FONT_SCREEN",
  "loadings": [
    {
      "splash": "ZLOADING_MOE"
    },
    {
      "splash": "ZLOADING_FOREST",
      "text": {
        "en": "Dark Forest"
      }
    },
    {
      "splash": "ZLOADING_BEAST",
      "text": {
        "en": "Dragon snapper",
        "pl": "Smoczy zębacz",
        "ru": "Драконий снеппер"
      }
    },
    {
      "splash": "ZLOADING_RIVER_1"
    },
    {
      "splash": "ZLOADING_RIVER_2"
    }
  ]
}
```

### Language for text

Json file is using UTF-8 encoding and Union is using ANSI, so it will be converted based on symbol from `gothic.ini`.

```ini
[ZLOADINGS]
LanguageSymbol=en
```

- `pl` - `CP1250`
- `ru` - `CP1251`
- any other - `CP1252`

The default symbol will be based on system language one, but it can be changed to any symbol.

- In order for the text to appear, text matching the symbol has to be defined in the config file.
  - At the same time this allows to create multilanguage plugin and easily add translations for other languages.
  - If there is no match, that loading screen will be simply displayed without any text.

```json
{
  "splash": "ZLOADING_BEAST",
  "text": {
    "en": "Dragon snapper",
    "pl": "Smoczy zębacz",
    "ru": "Драконий снеппер"
  }
}
```
