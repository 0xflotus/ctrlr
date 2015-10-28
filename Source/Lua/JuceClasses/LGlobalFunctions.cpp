#include "stdafx.h"
#include "LGlobalFunctions.h"
#include "Native/CtrlrNative.h"
#include "CtrlrManager/CtrlrManager.h"
#include "CtrlrUtilities.h"

static const String getNativeKeyMapping()
{
    String ret;
    for (int i=0; i<1024; i++)
    {
        if (KeyPress(i).isValid())
        {
            if (! KeyPress(i).getTextDescription().startsWith("#") && KeyPress(i).getTextDescription().isNotEmpty())
                ret << i << ": " << KeyPress(i).getTextDescriptionWithIcons() << "\n";
        }
    }
    return (ret);
}

LGlobalFunctions::LGlobalFunctions()
{
}

double LGlobalFunctions::double_Pi()
{
	return (juce::double_Pi);
}

float LGlobalFunctions::float_Pi()
{
	return (juce::float_Pi);
}

void LGlobalFunctions::debug(const String &argument)
{
    _LUA(argument);
}

void LGlobalFunctions::debug(const std::string &argument)
{
    _LUA(_STR(argument.c_str()));
}

std::string LGlobalFunctions::stringToLua (const String &string)
{
    return (string.toUTF8().getAddress());
}

String LGlobalFunctions::toJuceString (const std::string &string)
{
    return (String(string.c_str()));
}

void LGlobalFunctions::console (const String &arg)
{
    _LUA(removeInvalidChars(arg, true));
}

void LGlobalFunctions::console (const std::string &arg)
{
    _LUA(removeInvalidChars(_STR(arg), true));
}

void LGlobalFunctions::sleep(const int milliseconds)
{
    Thread::sleep(milliseconds);
}



void LGlobalFunctions::wrapForLua (lua_State *L)
{
	using namespace luabind;

	module(L)
    [
        def("getNativeKeyMapping", &getNativeKeyMapping)
        ,
        def("console", (void (*) (const std::string &)) &LGlobalFunctions::console),
        def("console", (void (*) (const String &)) &LGlobalFunctions::console)
        ,
		def("J", (const String (*) (const std::string &)) &LGlobalFunctions::toJuceString),
		def("toJuceString", (const String (*) (const std::string &)) &LGlobalFunctions::toJuceString)
		,
		def("L", &LGlobalFunctions::stringToLua),
		def("toLuaString", &LGlobalFunctions::stringToLua)
        ,
        def("print_debug", (void (*)(const String &))&LGlobalFunctions::debug),
        def("print_d", (void (*)(const String &))&LGlobalFunctions::debug),
        def("_DBG", (void (*)(const String &))&LGlobalFunctions::debug),
        def("_debug", (void (*)(const String &))&LGlobalFunctions::debug)
        ,
        def("sleep", &LGlobalFunctions::sleep)
        ,
		class_<LGlobalFunctions>("juce")
			.def(constructor<>())
			.scope
			[
				def("double_Pi", &LGlobalFunctions::double_Pi),
				def("float_Pi", &LGlobalFunctions::float_Pi)
			]
		,
		class_<CtrlrNative>("CtrlrNative")
            .def("sendKeyPressEvent", (const Result (CtrlrNative::*) (const KeyPress &, const String &)) &CtrlrNative::sendKeyPressEvent)
			.def("sendKeyPressEvent", (const Result (CtrlrNative::*) (const KeyPress &)) &CtrlrNative::sendKeyPressEvent)
            .scope
            [
                def("getNativeObject", &CtrlrNative::getNativeObject)
            ]
		,
		def("jmax", (double (*) (const double, const double))&juce::jmax<double>),
		def("jmax", (double (*) (const double, const double, const double))&juce::jmax<double>),
		def("jmax", (double (*) (const double, const double, const double, const double))&juce::jmax<double>),
		def("jmin", (double (*) (const double, const double))&juce::jmin<double>),
		def("jmin", (double (*) (const double, const double, const double))&juce::jmin<double>),
		def("jmin", (double (*) (const double, const double, const double, const double))&juce::jmin<double>),
		def("jlimit", (double (*) (const double, const double, const double))&juce::jlimit<double>),
		def("isPositiveAndBelow", (bool (*) (const double, const double))&juce::isPositiveAndBelow<double>),
		def("isPositiveAndNotGreaterThan", (bool (*) (const double, const double))&juce::isPositiveAndNotGreaterThan<double>),
        def("roundToInt", &juce::roundToInt<double>),
		def("roundToIntAccurate", &juce::roundToIntAccurate),
		def("roundDoubleToInt", &juce::roundDoubleToInt),
		def("roundFloatToInt", &juce::roundFloatToInt),
		def("isPowerOfTwo", (bool (*) (int))&juce::isPowerOfTwo<int>),
		def("nextPowerOfTwo", &juce::nextPowerOfTwo),

		def("cos", (double (*) (double))&cos),
		def("cosf", (float (*) (float))&cosf),
		def("sin", (double (*) (double))&sin),
		def("sinf", (float (*) (float))&sinf),
		def("tan", (double (*) (double))&tan),
		def("acos", (double (*) (double))&acos),
		def("asin", (double (*) (double))&asin),
		def("atan", (double (*) (double))&atan),
		def("atan2", (double (*) (double, double))&atan2),
		def("cosh", (double (*) (double))&cosh),
		def("sinh", (double (*) (double))&sinh),
		def("tanh", (double (*) (double))&tanh),
		def("exp", (double (*) (double))&exp),
		def("frexp", (double (*) (double, int *))&frexp),
		def("ldexp", (double (*) (double, int))&ldexp),
		def("log", (double (*) (double))&log),
		def("log10", (double (*) (double))&log10),
		def("modf", (double (*) (double, double *))&modf),
		def("pow", (double (*) (double, double))&pow),
		def("powf", (float (*) (float, float))&powf),
		def("sqrt", (double (*) (double))&sqrt),
		def("ceil", (double (*) (double))&ceil),
		def("ceilf", (float (*) (float))&ceilf),
		def("fabs", (double (*) (double))&fabs),
		def("fabsf", (float (*) (float))&fabsf),
		def("floor", (double (*) (double))&floor),
		def("floorf", (float (*) (float))&floorf),
		def("fmod", (double (*) (double, double))&fmod),
		def("fmodf", (float (*) (float, float))&fmodf)
	];
}
