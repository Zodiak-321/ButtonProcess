#include "ButtonProcess.h"

bool BUTTON::API_read_button(void)
{
    return *_buttonIO;
}

uint32_t BUTTON::API_get_tick(void)
{
    return esp_timer_get_time() / 1000;
}

void BUTTON::API_delay(uint32_t ms)
{
    uint32_t current_time = API_get_tick();

    while(API_get_tick() - current_time <= ms);
}

/*=============================================================================================================================*/

BUTTON::BUTTON(bool* buttonIO,
             bool use_doublePress,
             bool one_longPressEventProcess,
             Active active,
             uint32_t keepStateTime,
             uint32_t min_freshTime_ms,
             uint32_t min_longPressTime_ms,
             uint32_t max_doublePressTime_ms,
             uint32_t min_eliminateShockTime_ms)
        :_buttonIO(buttonIO), _use_doublePress(use_doublePress), _one_longPressEventProcess(one_longPressEventProcess), _active(active), 
        _keepStateTime(keepStateTime), _min_freshTime_ms(min_freshTime_ms), _min_longPressTime_ms(min_longPressTime_ms), 
        _max_doublePressTime_ms(max_doublePressTime_ms), _min_eliminateShockTime_ms(min_eliminateShockTime_ms)
{
    stateTypedef = NONE_STATE;
    _shortPress_event_callback.callback = nullptr;
    _shortPress_event_callback.context = nullptr;
    _doublePress_event_callback.callback = nullptr;
    _doublePress_event_callback.context = nullptr;
    _longPress_event_callback.callback = nullptr;
    _longPress_event_callback.context = nullptr;

    _shortPress_enent_rigister = false;
    _doublePress_enent_rigister = false;
    _longPress_enent_rigister = false;
}

bool BUTTON::read_button(void)
{
    if(API_read_button() == true && _active == Active::HIGH_ACTIVE)
        return true;
    else if(API_read_button() == false && _active == Active::LOW_ACTIVE)
        return true;
    else
        return false;
}

void BUTTON::set_shortPress_event(ButtonCallback callback, void* context)
{
    _shortPress_event_callback.callback = callback;
    _shortPress_event_callback.context = context;
    _shortPress_enent_rigister = true;
}

void BUTTON::set_doublePress_event(ButtonCallback callback, void* context)
{
    _doublePress_event_callback.callback = callback;
    _doublePress_event_callback.context = context;
    _doublePress_enent_rigister = true;
}

void BUTTON::set_longPress_event(ButtonCallback callback, void* context)
{
    _longPress_event_callback.callback = callback;
    _longPress_event_callback.context = context;
    _longPress_enent_rigister = true;
}

BUTTON::ButtonUpdateReturn BUTTON::update_button(void)
{
    static uint32_t last_time = 0;
    uint32_t current_time = API_get_tick();

    static uint32_t last_determine_time = 0;
    uint32_t current_determine_time = 0;

    static bool state_change = false;

    if(current_time - last_time < _min_freshTime_ms)
        return ButtonUpdateReturn::FRESH_TIME;

    if(state_change == true && (current_time - last_time > _keepStateTime))
    {
        state_change = false;
        _buttonState = ButtonState::none;
    }

    switch(stateTypedef)
    {
        case NONE_STATE:
            if(read_button() == true)
            {
                stateTypedef = ELIMINATE_SHOCK_STATE;
                last_determine_time = API_get_tick();
            }
            else 
                return ButtonUpdateReturn::NONE;
        case ELIMINATE_SHOCK_STATE:
            if(read_button() == true && current_determine_time - last_determine_time >= _min_eliminateShockTime_ms)
            {
                stateTypedef = DETERMINE_STATE;
                last_determine_time = API_get_tick();
            }
            else 
            {
                stateTypedef = NONE_STATE;
                return ButtonUpdateReturn::SHAKE;
            }
        case DETERMINE_STATE:
            current_determine_time = API_get_tick();
            if(read_button() == false)
                stateTypedef = SHORT_PRESS_STATE;
            else if(current_determine_time - last_determine_time >= _min_longPressTime_ms)
                stateTypedef = LONG_PRESS_STATE;
            break;
        case SHORT_PRESS_STATE:
            if(_use_doublePress == true)
            {
                current_determine_time = API_get_tick();
                if(read_button() == true && current_determine_time - last_determine_time >= _min_freshTime_ms + _min_eliminateShockTime_ms * 2)
                    stateTypedef = DOUBLE_PRESS_STATE;
                else if(current_determine_time - last_determine_time >= _max_doublePressTime_ms)
                {
                    _buttonState = ButtonState::short_press;
                    if(_shortPress_enent_rigister)
                        _shortPress_event_callback.callback(_shortPress_event_callback.context);
                    stateTypedef = END_STATE;
                }
            }
            else
            {
                _buttonState = ButtonState::short_press;
                if(_shortPress_enent_rigister)
                    _shortPress_event_callback.callback(_shortPress_event_callback.context);
                stateTypedef = END_STATE; 
            }
            break;
        case DOUBLE_PRESS_STATE:
            _buttonState = ButtonState::double_press;
            if(_doublePress_enent_rigister)
                _doublePress_event_callback.callback(_doublePress_event_callback.context);
            if(read_button() == false)
                stateTypedef = END_STATE;
            break;
        case LONG_PRESS_STATE:
            _buttonState = ButtonState::long_press;
            if(_longPress_enent_rigister)
                _longPress_event_callback.callback(_longPress_event_callback.context);
            if(read_button() == false || _one_longPressEventProcess == true)
                stateTypedef = END_STATE;
            break;
        case END_STATE:
            if(_one_longPressEventProcess == true)
            {
                if(read_button() == false)
                {
                    stateTypedef = NONE_STATE;
                    state_change = true;
                    last_time = current_time;
                    return ButtonUpdateReturn::OK;
                }
            }
            else
            {
                stateTypedef = NONE_STATE;
                state_change = true;
                last_time = current_time;
                return ButtonUpdateReturn::OK;
            }
        default:
            stateTypedef = ERROR_STATE;
            last_time = current_time;
            return ButtonUpdateReturn::ERROR;
    }

    return ButtonUpdateReturn::ERROR;
}




