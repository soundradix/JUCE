/*
  ==============================================================================

   This file is part of the JUCE 9 preview.
   Copyright (c) Raw Material Software Limited

   You may use this code under the terms of the AGPLv3
   (see www.gnu.org/licenses).

   For the JUCE 9 preview this file cannot be licensed commercially.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

DynamicObject::DynamicObject() = default;

DynamicObject::DynamicObject (const DynamicObject& other)
   : ReferenceCountedObject(), properties (other.properties)
{
}

bool DynamicObject::hasProperty (const Identifier& propertyName) const
{
    const var* const v = properties.getVarPointer (propertyName);
    return v != nullptr && ! v->isMethod();
}

const var& DynamicObject::getProperty (const Identifier& propertyName) const
{
    return properties [propertyName];
}

void DynamicObject::setProperty (const Identifier& propertyName, const var& newValue)
{
    properties.set (propertyName, newValue);
    didModifyProperty (propertyName, newValue);
}

void DynamicObject::removeProperty (const Identifier& propertyName)
{
    properties.remove (propertyName);
    didModifyProperty (propertyName, std::nullopt);
}

bool DynamicObject::hasMethod (const Identifier& methodName) const
{
    return getProperty (methodName).isMethod();
}

var DynamicObject::invokeMethod (Identifier method, const var::NativeFunctionArgs& args)
{
    if (auto function = properties [method].getNativeFunction())
        return function (args);

    return {};
}

void DynamicObject::setMethod (Identifier name, var::NativeFunction function)
{
    setProperty (name, var (function));
}

void DynamicObject::clear()
{
    auto copy = properties;
    properties.clear();

    for (auto& prop : copy)
        didModifyProperty (prop.name, std::nullopt);
}

void DynamicObject::cloneAllProperties()
{
    for (int i = properties.size(); --i >= 0;)
        if (auto* v = properties.getVarPointerAt (i))
            *v = v->clone();
}

std::unique_ptr<DynamicObject> DynamicObject::clone() const
{
    auto result = std::make_unique<DynamicObject> (*this);
    result->cloneAllProperties();
    return result;
}

void DynamicObject::writeAsJSON (OutputStream& out, const JSON::FormatOptions& format)
{
    out << '{';
    if (format.getSpacing() == JSON::Spacing::multiLine)
        out << newLine;

    const int numValues = properties.size();

    for (int i = 0; i < numValues; ++i)
    {
        if (format.getSpacing() == JSON::Spacing::multiLine)
            JSONFormatter::writeSpaces (out, format.getIndentLevel() + JSONFormatter::indentSize);

        out << '"';
        JSONFormatter::writeString (out, properties.getName (i), format.getEncoding());
        out << "\":";

        if (format.getSpacing() != JSON::Spacing::none)
            out << ' ';

        JSON::writeToStream (out,
                             properties.getValueAt (i),
                             format.withIndentLevel (format.getIndentLevel() + JSONFormatter::indentSize));

        if (i < numValues - 1)
        {
            out << ",";

            switch (format.getSpacing())
            {
                case JSON::Spacing::none: break;
                case JSON::Spacing::singleLine: out << ' '; break;
                case JSON::Spacing::multiLine: out << newLine; break;
            }
        }
        else if (format.getSpacing() == JSON::Spacing::multiLine)
            out << newLine;
    }

    if (format.getSpacing() == JSON::Spacing::multiLine)
        JSONFormatter::writeSpaces (out, format.getIndentLevel());

    out << '}';
}

bool DynamicObject::equals (const DynamicObject& other) const
{
    return properties == other.properties;
}

//==============================================================================
//==============================================================================

#if JUCE_UNIT_TESTS

class DynamicObjectTests : public UnitTest
{
public:
    DynamicObjectTests() : UnitTest { "DynamicObject", UnitTestCategories::containers } {}

    void runTest() override
    {
        struct Action
        {
            Identifier key;
            std::optional<var> value;

            bool operator== (const Action& other) const
            {
                return other.key == key && other.value == value;
            }
        };

        using Actions = std::vector<Action>;

        struct DerivedObject : public DynamicObject
        {
            explicit DerivedObject (Actions& a) : actions (a) {}

            void didModifyProperty (const Identifier& key, const std::optional<var>& value) override
            {
                actions.push_back (Action { key, value });
            }

            Actions& actions;
        };

        Actions actions;
        DerivedObject object { actions };

        beginTest ("didModifyProperty is emitted on setProperty");
        {
            expect (object.getProperties().isEmpty());

            const Identifier key = "foo";
            const var value = 123;
            object.setProperty (key, value);

            expect (actions == Actions { Action { key, value } });
            expect (object.getProperties() == NamedValueSet { { key, value } });
        }

        object.clear();
        actions.clear();

        beginTest ("didModifyProperty is emitted on setMethod");
        {
            expect (object.getProperties().isEmpty());

            const Identifier key = "foo";
            const var::NativeFunction value = [] (const var::NativeFunctionArgs&) { return var{}; };
            object.setMethod (key, value);

            expect (actions.size() == 1);
            expect (actions.back().key == key);

            expect (object.getProperties().size() == 1);
            expect (object.hasMethod (key));
        }

        object.clear();
        actions.clear();

        beginTest ("didModifyProperty is emitted on removeProperty");
        {
            expect (object.getProperties().isEmpty());

            const Identifier key = "bar";
            object.removeProperty (key);

            expect (actions == Actions { Action { key, std::nullopt } });
            expect (object.getProperties().isEmpty());
        }

        object.clear();
        actions.clear();

        beginTest ("didModifyProperty is emitted on clear");
        {
            expect (object.getProperties().isEmpty());

            object.clear();

            expect (actions.empty());

            const Identifier keys[] { "foo", "bar", "baz" };

            for (auto [index, key] : enumerate (keys, int{}))
                object.setProperty (key, index);

            for (auto& key : keys)
                expect (object.hasProperty (key));

            actions.clear();

            object.clear();

            expect (actions.size() == std::size (keys));

            for (auto& key : keys)
            {
                expect (std::find_if (actions.begin(), actions.end(), [&key] (auto& action)
                {
                    return action.key == key && action.value == std::nullopt;
                }) != actions.end());
            }
        }
    }
};

static DynamicObjectTests dynamicObjectTests;

#endif

} // namespace juce
