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

//==============================================================================
class UIAGridProvider  : public UIAProviderBase,
                         public ComBaseClassHelper<IGridProvider, ITableProvider>
{
public:
    using UIAProviderBase::UIAProviderBase;

    //==============================================================================
    JUCE_COMRESULT GetItem (int row, int column, IRawElementProviderSimple** pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityTableInterface& tableInterface)
        {
            if (! isPositiveAndBelow (row, tableInterface.getNumRows())
                || ! isPositiveAndBelow (column, tableInterface.getNumColumns()))
                return E_INVALIDARG;

            JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")

            if (auto* cellHandler = tableInterface.getCellHandler (row, column))
            {
                cellHandler->getNativeImplementation()->QueryInterface (IID_PPV_ARGS (pRetVal));
                return S_OK;
            }

            if (auto* rowHandler = tableInterface.getRowHandler (row))
            {
                rowHandler->getNativeImplementation()->QueryInterface (IID_PPV_ARGS (pRetVal));
                return S_OK;
            }

            JUCE_END_IGNORE_WARNINGS_GCC_LIKE

            return E_FAIL;
        });
    }

    JUCE_COMRESULT get_RowCount (int* pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityTableInterface& tableInterface)
        {
            *pRetVal = tableInterface.getNumRows();
            return S_OK;
        });
    }

    JUCE_COMRESULT get_ColumnCount (int* pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityTableInterface& tableInterface)
        {
            *pRetVal = tableInterface.getNumColumns();
            return S_OK;
        });
    }

    JUCE_COMRESULT GetRowHeaders (SAFEARRAY**) override
    {
        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    JUCE_COMRESULT GetColumnHeaders (SAFEARRAY** pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityTableInterface& tableInterface)
        {
            auto* header = tableInterface.getHeaderHandler();

            if (header == nullptr)
                return (HRESULT) UIA_E_NOTSUPPORTED;

            const auto children = header->getChildren();

            SafeArrayHandle result { SafeArrayCreateVector (VT_UNKNOWN, 0, (ULONG) children.size()) };

            if (result == nullptr)
                return E_FAIL;

            LONG index = 0;

            for (const auto& child : children)
            {
                ComSmartPtr<IRawElementProviderSimple> provider;

                JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")
                if (child != nullptr)
                    child->getNativeImplementation()->QueryInterface (IID_PPV_ARGS (provider.resetAndGetPointerAddress()));
                JUCE_END_IGNORE_WARNINGS_GCC_LIKE

                if (provider == nullptr)
                    return E_FAIL;

                if (FAILED (SafeArrayPutElement (result.get(), &index, provider)))
                    return E_FAIL;

                ++index;
            }

            *pRetVal = result.release();
            return S_OK;
        });
    }

    JUCE_COMRESULT get_RowOrColumnMajor (RowOrColumnMajor* pRetVal) override
    {
        *pRetVal = RowOrColumnMajor_RowMajor;
        return S_OK;
    }

private:
    template <typename Value, typename Callback>
    JUCE_COMRESULT withTableInterface (Value* pRetVal, Callback&& callback) const
    {
        return withCheckedComArgs (pRetVal, *this, [&]() -> HRESULT
        {
            if (auto* tableHandler = detail::AccessibilityHelpers::getEnclosingHandlerWithInterface (&getHandler(), &AccessibilityHandler::getTableInterface))
                if (auto* tableInterface = tableHandler->getTableInterface())
                    return callback (*tableInterface);

            return (HRESULT) UIA_E_NOTSUPPORTED;
        });
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIAGridProvider)
};

} // namespace juce
