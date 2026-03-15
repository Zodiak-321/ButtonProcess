#ifndef __BUTTONPROCESS_H__
#define __BUTTONPROCESS_H__

#include "esp_timer.h"
#include <Arduino.h>

#include <stdio.h>

typedef void (*ButtonCallback)(void* context);

class BUTTON
{    
    public:
        enum class ButtonState : uint8_t
        {
            none = 0,
            short_press = 1,
            long_press = 2,
            double_press = 3,
        };

        enum class ButtonUpdateReturn : uint8_t
        {
            OK,
            FRESH_TIME,
            NONE,
            SHAKE,
            ERROR,
        };

        enum class Active : uint8_t
        {
            HIGH_ACTIVE,
            LOW_ACTIVE
        };

        BUTTON(bool* buttonIO,
             bool use_doublePress = true,
             bool one_longPressEventProcess = false,
             Active active = Active::HIGH_ACTIVE,
             uint32_t keepStateTime = 40,
             uint32_t min_freshTime_ms = 30,
             uint32_t min_longPressTime_ms = 650,
             uint32_t max_doublePressTime_ms = 350,
             uint32_t min_eliminateShockTime_ms = 30);

        BUTTON::ButtonUpdateReturn update_button(void);
        
        void set_shortPress_event(ButtonCallback callback, void* context = nullptr);
        void set_longPress_event(ButtonCallback callback, void* context = nullptr);
        void set_doublePress_event(ButtonCallback callback, void* context = nullptr);

        inline void set_useDobulePress(bool use_doublePress) { _use_doublePress = use_doublePress; }
        inline void set_oneLongPressEventProcess(bool one_longPressEventProcess) { _one_longPressEventProcess = one_longPressEventProcess; }

        inline ButtonState get_ButtonState(void) { return _buttonState; };

    private:

        typedef enum 
        {
            NONE_STATE,
            ERROR_STATE,
            ELIMINATE_SHOCK_STATE,
            DETERMINE_STATE,
            SHORT_PRESS_STATE,
            LONG_PRESS_STATE,
            DOUBLE_PRESS_STATE,
            END_STATE,
        }StateTypedef;

        typedef struct
        {
            ButtonCallback callback;
            void* context;
        }ButtonEvent;

        bool API_read_button(void);
        uint32_t API_get_tick(void);
        void API_delay(uint32_t ms);

        bool read_button(void);
        void process_shortPress_event(void);
        void process_longPress_event(void);
        void process_doublePress_event(void);

        StateTypedef stateTypedef;
        bool* _buttonIO;
        bool _use_doublePress;
        bool _one_longPressEventProcess;
        Active _active;
        uint32_t _keepStateTime;
        uint32_t _min_freshTime_ms;
        uint32_t _min_longPressTime_ms;
        uint32_t _max_doublePressTime_ms;
        uint32_t _min_eliminateShockTime_ms;

        ButtonEvent _shortPress_event_callback;
        ButtonEvent _longPress_event_callback;
        ButtonEvent _doublePress_event_callback;

        bool _shortPress_enent_rigister;
        bool _doublePress_enent_rigister;
        bool _longPress_enent_rigister;

        ButtonState _buttonState;

};









#endif
