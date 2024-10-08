#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>

enum ControlType : uint8_t
{
    // fixed Controls
    Title = 0,

    // updatable Controls
    Pad,
    PadWithCenter,
    Button,
    Label,
    Switcher,
    Slider,
    Number,
    Text,
    Graph,
    GraphPoint,
    Tab,
    Select,
    Option,
    Min,
    Max,
    Step,
    Gauge,
    Accel,
    Separator,
    Time,
    FileDisplay,
    LocalTime,
    TimeFetcher,

    Fragment = 98,
    Password = 99,
    UpdateOffset = 100,
};

enum ControlColor : uint8_t
{
    Turquoise,
    Emerald,
    Peterriver,
    Wetasphalt,
    Sunflower,
    Carrot,
    Alizarin,
    Dark,
    None = 0xFF
};

class Control
{
public:
    ControlType type;
    uint16_t id; // just mirroring the id here for practical reasons
    const char* label;
    std::function<void(Control*, int)> callback;
    String value;
    ControlColor color;
    bool visible;
    bool wide;
    bool vertical;
    bool enabled;
    uint16_t parentControl;
    String panelStyle;
    String elementStyle;
    String inputType;
    Control* next;

    static constexpr uint16_t noParent = 0xffff;

    Control(ControlType type, 
            const char* label, 
            std::function<void(Control*, int)> callback,
            const String& value, 
            ControlColor color, 
            bool visible, 
            uint16_t parentControl);

    Control(const Control& Control);

    void SendCallback(int type);
    void SendCallback(Control* control, int type);
    bool HasCallback() { return (nullptr != callback); }
    bool MarshalControl(ArduinoJson::JsonObject& item, bool refresh, uint32_t DataOffset, uint32_t MaxLength, uint32_t & EstimmatedUsedSpace);
    void MarshalErrorMessage(ArduinoJson::JsonObject& item);
    void DeleteControl();
    bool onWsEvent(String& cmd, String& data);
    inline bool ToBeDeleted() { return _ToBeDeleted; }
    inline bool NeedsSync(uint32_t lastControlChangeID) {return (false == _ToBeDeleted) && (lastControlChangeID < ControlChangeID);}
    void    SetControlChangedId(uint32_t value) {ControlChangeID = value;}

private:
    bool _ToBeDeleted = false;
    uint32_t ControlChangeID = 0;
    String OldValue = emptyString;

    // multiplier for converting a typical controller label or value to a Json object
    #define JsonMarshalingRatio 3
    // Marshaed Control overhead length
    #define JsonMarshaledOverhead 64
};

#define UI_TITLE            ControlType::Title
#define UI_LABEL            ControlType::Label
#define UI_BUTTON           ControlType::Button
#define UI_SWITCHER         ControlType::Switcher
#define UI_PAD              ControlType::Pad
#define UI_CPAD             ControlType::Cpad
#define UI_SLIDER           ControlType::Slider
#define UI_NUMBER           ControlType::Number
#define UI_TEXT_INPUT       ControlType::Text
#define UI_GRAPH            ControlType::Graph
#define UI_ADD_GRAPH_POINT  ControlType::GraphPoint

#define UPDATE_LABEL        ControlType::UpdateLabel
#define UPDATE_SWITCHER     ControlType::UpdateSwitcher
#define UPDATE_SLIDER       ControlType::UpdateSlider
#define UPDATE_NUMBER       ControlType::UpdateNumber
#define UPDATE_TEXT_INPUT   ControlType::UpdateText
#define CLEAR_GRAPH         ControlType::ClearGraph

// Colors
#define COLOR_TURQUOISE     ControlColor::Turquoise
#define COLOR_EMERALD       ControlColor::Emerald
#define COLOR_PETERRIVER    ControlColor::Peterriver
#define COLOR_WETASPHALT    ControlColor::Wetasphalt
#define COLOR_SUNFLOWER     ControlColor::Sunflower
#define COLOR_CARROT        ControlColor::Carrot
#define COLOR_ALIZARIN      ControlColor::Alizarin
#define COLOR_DARK          ControlColor::Dark
#define COLOR_NONE          ControlColor::None
