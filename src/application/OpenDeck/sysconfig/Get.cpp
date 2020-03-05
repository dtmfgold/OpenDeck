#include "SysConfig.h"

SysConfig::result_t SysConfig::SysExDataHandler::get(uint8_t block, uint8_t section, size_t index, SysExConf::sysExParameter_t& value)
{
    auto sysExBlock = static_cast<SysConfig::block_t>(block);
    auto result     = SysConfig::result_t::notSupported;

    switch (sysExBlock)
    {
    case SysConfig::block_t::global:
    {
        result = sysConfig.onGetGlobal(static_cast<SysConfig::Section::global_t>(section), index, value);
    }
    break;

    case SysConfig::block_t::buttons:
    {
        result = sysConfig.onGetButtons(static_cast<SysConfig::Section::button_t>(section), index, value);
    }
    break;

    case SysConfig::block_t::encoders:
    {
        result = sysConfig.onGetEncoders(static_cast<SysConfig::Section::encoder_t>(section), index, value);
    }
    break;

    case SysConfig::block_t::analog:
    {
        result = sysConfig.onGetAnalog(static_cast<SysConfig::Section::analog_t>(section), index, value);
    }
    break;

    case SysConfig::block_t::leds:
    {
        result = sysConfig.onGetLEDs(static_cast<SysConfig::Section::leds_t>(section), index, value);
    }
    break;

    case SysConfig::block_t::display:
    {
        result = sysConfig.onGetDisplay(static_cast<SysConfig::Section::display_t>(section), index, value);
    }
    break;

    default:
        break;
    }

#ifdef DISPLAY_SUPPORTED
    sysConfig.display.displayMIDIevent(Interface::Display::eventType_t::in, Interface::Display::event_t::systemExclusive, 0, 0, 0);
#endif

    return result;
}

SysConfig::result_t SysConfig::onGetGlobal(Section::global_t section, size_t index, SysExConf::sysExParameter_t& value)
{
    int32_t readValue = 0;
    auto    result    = SysConfig::result_t::error;

    switch (section)
    {
    case Section::global_t::midiFeature:
    case Section::global_t::midiMerge:
    {
        result = database.read(dbSection(section), index, readValue) ? SysConfig::result_t::ok : SysConfig::result_t::error;
    }
    break;

    case Section::global_t::presets:
    {
        auto setting = static_cast<presetSetting_t>(index);

        switch (setting)
        {
        case presetSetting_t::activePreset:
        {
            readValue = database.getPreset();
            result    = SysConfig::result_t::ok;
        }
        break;

        case presetSetting_t::presetPreserve:
        {
            readValue = database.getPresetPreserveState();
            result    = SysConfig::result_t::ok;
        }
        break;

        default:
            break;
        }
    }
    break;

    default:
        break;
    }

    value = readValue;
    return result;
}

SysConfig::result_t SysConfig::onGetButtons(Section::button_t section, size_t index, SysExConf::sysExParameter_t& value)
{
    int32_t readValue;
    auto    result = database.read(dbSection(section), index, readValue) ? SysConfig::result_t::ok : SysConfig::result_t::error;

    //channels start from 0 in db, start from 1 in sysex
    if ((section == Section::button_t::midiChannel) && (result == SysConfig::result_t::ok))
        readValue++;

    value = readValue;
    return result;
}

SysConfig::result_t SysConfig::onGetEncoders(Section::encoder_t section, size_t index, SysExConf::sysExParameter_t& value)
{
    int32_t              readValue;
    auto                 result = database.read(dbSection(section), index, readValue) ? SysConfig::result_t::ok : SysConfig::result_t::error;
    MIDI::encDec_14bit_t encDec_14bit;

    if (result == SysConfig::result_t::ok)
    {
        if ((section == Section::encoder_t::midiID) || (section == Section::encoder_t::midiID_msb))
        {
            encDec_14bit.value = readValue;
            encDec_14bit.split14bit();

            if (section == Section::encoder_t::midiID)
                readValue = encDec_14bit.low;
            else
                readValue = encDec_14bit.high;
        }
        else if (section == Section::encoder_t::midiChannel)
        {
            //channels start from 0 in db, start from 1 in sysex
            readValue++;
        }
    }

    value = readValue;
    return result;
}

SysConfig::result_t SysConfig::onGetAnalog(Section::analog_t section, size_t index, SysExConf::sysExParameter_t& value)
{
    int32_t              readValue;
    auto                 result = database.read(dbSection(section), index, readValue) ? SysConfig::result_t::ok : SysConfig::result_t::error;
    MIDI::encDec_14bit_t encDec_14bit;

    switch (section)
    {
    case Section::analog_t::midiID:
    case Section::analog_t::midiID_MSB:
    case Section::analog_t::lowerLimit:
    case Section::analog_t::lowerLimit_MSB:
    case Section::analog_t::upperLimit:
    case Section::analog_t::upperLimit_MSB:
    {
        if (result == SysConfig::result_t::ok)
        {
            encDec_14bit.value = readValue;
            encDec_14bit.split14bit();

            switch (section)
            {
            case Section::analog_t::midiID:
            case Section::analog_t::lowerLimit:
            case Section::analog_t::upperLimit:
            {
                readValue = encDec_14bit.low;
            }
            break;

            default:
            {
                readValue = encDec_14bit.high;
            }
            break;
            }
        }
    }
    break;

    case Section::analog_t::midiChannel:
    {
        //channels start from 0 in db, start from 1 in sysex
        if (result == SysConfig::result_t::ok)
            readValue++;
    }
    break;

    default:
        break;
    }

    value = readValue;
    return result;
}

SysConfig::result_t SysConfig::onGetLEDs(Section::leds_t section, size_t index, SysExConf::sysExParameter_t& value)
{
#ifdef LEDS_SUPPORTED
    int32_t readValue;
    auto    result = SysConfig::result_t::ok;

    switch (section)
    {
    case Section::leds_t::testColor:
    {
        readValue = static_cast<int32_t>(leds.getColor(index));
    }
    break;

    case Section::leds_t::testBlink:
    {
        readValue = leds.getBlinkState(index);
    }
    break;

    case Section::leds_t::midiChannel:
    {
        result = database.read(dbSection(section), index, readValue) ? SysConfig::result_t::ok : SysConfig::result_t::error;

        //channels start from 0 in db, start from 1 in sysex
        if (result == SysConfig::result_t::ok)
            readValue++;
    }
    break;

    case Section::leds_t::rgbEnable:
    {
        result = database.read(dbSection(section), Board::io::getRGBID(index), readValue) ? SysConfig::result_t::ok : SysConfig::result_t::error;
    }
    break;

    default:
    {
        result = database.read(dbSection(section), index, readValue) ? SysConfig::result_t::ok : SysConfig::result_t::error;
    }
    break;
    }

    value = readValue;
    return result;
#else
    return SysConfig::result_t::notSupported;
#endif
}

SysConfig::result_t SysConfig::onGetDisplay(Section::display_t section, size_t index, SysExConf::sysExParameter_t& value)
{
#ifdef DISPLAY_SUPPORTED
    int32_t readValue;
    auto    result = database.read(dbSection(section), index, readValue) ? SysConfig::result_t::ok : SysConfig::result_t::error;

    value = readValue;
    return result;
#else
    return SysConfig::result_t::notSupported;
#endif
}