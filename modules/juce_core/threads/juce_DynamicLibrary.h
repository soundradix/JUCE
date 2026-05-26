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

/**
    Handles the opening and closing of DLLs.

    This class can be used to open a DLL and get some function pointers from it.
    Since the DLL is freed when this object is deleted, it's handy for managing
    library lifetimes using RAII.

    @tags{Core}
*/
class JUCE_API  DynamicLibrary
{
public:
    /** Creates an unopened DynamicLibrary object.
        Call open() to actually open one.
    */
    DynamicLibrary() = default;

    /** Creates an opened DynamicLibrary object. */
    DynamicLibrary (const String& name)  { open (name); }

    /** Move constructor */
    DynamicLibrary (DynamicLibrary&& other) noexcept
    {
        std::swap (handle, other.handle);
    }

    /** Destructor.
        If a library is currently open, it will be closed when this object is
        destroyed.
    */
    ~DynamicLibrary()   { close(); }

    /** Opens a DLL.

        The name and the method by which it gets found is platform-specific, and
        may or may not include a path, depending on the OS.

        If a library is already open when this method is called, it will first
        close the library before attempting to load the new one.

        @returns true if the library was successfully found and opened.
    */
    bool open (const String& name);

    /** Releases the currently-open DLL, or has no effect if none was open. */
    void close();

    /** Tries to find a named function in the currently-open DLL and returns a
        pointer to it.

        If the library is not currently open, or if the named function cannot be
        found, this method will return a null pointer.
    */
    void* getFunction (const String& functionName) noexcept;

    /** Tries to find a named function in the currently-open DLL and returns a
        pointer to it, cast to the requested function type.

        If the library is not currently open, or if the named function cannot be
        found, this method will return a null pointer.

        The template parameter must be a function type, e.g.

        @code
        auto fn = library.getFunction<void*(const char*)> ("someFunction");
        @endcode

        It is the caller's responsibility to ensure that the requested function
        signature matches the actual function exported by the dynamic library.
        Passing an incorrect signature will result in undefined behaviour.
    */
    template <typename Fn>
    Fn* getFunction (const String& functionName) noexcept
    {
        return reinterpret_cast<Fn*> (getFunction (functionName));
    }

    /** Returns true if the library was successfully found and opened. */
    bool isOpen() const noexcept { return handle != nullptr; }

    /** Returns the platform-specific native library handle.
        You'll need to cast this to whatever is appropriate for the OS that's in
        use.
    */
    void* getNativeHandle() const noexcept { return handle; }

private:
    void* handle = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicLibrary)
};

} // namespace juce
