/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

class PluginListComponent::TableModel  : public TableListBoxModel
{
public:
    TableModel (PluginListComponent& c, KnownPluginList& l)  : owner (c), list (l) {}

    int getNumRows() override
    {
        return list.getNumTypes() + list.getBlacklistedFiles().size();
    }

    void paintRowBackground (Graphics& g, int /*rowNumber*/, int /*width*/, int /*height*/, bool rowIsSelected) override
    {
        const auto defaultColour = owner.findColour (ListBox::backgroundColourId);
        const auto c = rowIsSelected ? defaultColour.interpolatedWith (owner.findColour (ListBox::textColourId), 0.5f)
                                     : defaultColour;

        g.fillAll (c);
    }

    enum
    {
        nameCol = 1,
        typeCol = 2,
        categoryCol = 3,
        manufacturerCol = 4,
        descCol = 5
    };

    void paintCell (Graphics& g, int row, int columnId, int width, int height, bool /*rowIsSelected*/) override
    {
        String text;
        bool isBlacklisted = row >= list.getNumTypes();

        if (isBlacklisted)
        {
            if (columnId == nameCol)
                text = list.getBlacklistedFiles() [row - list.getNumTypes()];
            else if (columnId == descCol)
                text = TRANS("Deactivated after failing to initialise correctly");
        }
        else
        {
            auto desc = list.getTypes()[row];

            switch (columnId)
            {
                case nameCol:         text = desc.name; break;
                case typeCol:         text = desc.pluginFormatName; break;
                case categoryCol:     text = desc.category.isNotEmpty() ? desc.category : "-"; break;
                case manufacturerCol: text = desc.manufacturerName; break;
                case descCol:         text = getPluginDescription (desc); break;

                default: jassertfalse; break;
            }
        }

        if (text.isNotEmpty())
        {
            const auto defaultTextColour = owner.findColour (ListBox::textColourId);
            g.setColour (isBlacklisted ? Colours::red
                                       : columnId == nameCol ? defaultTextColour
                                                             : defaultTextColour.interpolatedWith (Colours::transparentBlack, 0.3f));
            g.setFont (Font (height * 0.7f, Font::bold));
            g.drawFittedText (text, 4, 0, width - 6, height, Justification::centredLeft, 1, 0.9f);
        }
    }

    void deleteKeyPressed (int) override
    {
        owner.removeSelectedPlugins();
    }

    void sortOrderChanged (int newSortColumnId, bool isForwards) override
    {
        switch (newSortColumnId)
        {
            case nameCol:         list.sort (KnownPluginList::sortAlphabetically, isForwards); break;
            case typeCol:         list.sort (KnownPluginList::sortByFormat, isForwards); break;
            case categoryCol:     list.sort (KnownPluginList::sortByCategory, isForwards); break;
            case manufacturerCol: list.sort (KnownPluginList::sortByManufacturer, isForwards); break;
            case descCol:         break;

            default: jassertfalse; break;
        }
    }

    static String getPluginDescription (const PluginDescription& desc)
    {
        StringArray items;

        if (desc.descriptiveName != desc.name)
            items.add (desc.descriptiveName);

        items.add (desc.version);

        items.removeEmptyStrings();
        return items.joinIntoString (" - ");
    }

    PluginListComponent& owner;
    KnownPluginList& list;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TableModel)
};

//==============================================================================
PluginListComponent::PluginListComponent (AudioPluginFormatManager& manager, KnownPluginList& listToEdit,
                                          const File& deadMansPedal, PropertiesFile* const props,
                                          bool allowPluginsWhichRequireAsynchronousInstantiation)
    : formatManager (manager),
      list (listToEdit),
      deadMansPedalFile (deadMansPedal),
      optionsButton ("Options..."),
      propertiesToUse (props),
      allowAsync (allowPluginsWhichRequireAsynchronousInstantiation),
      numThreads (allowAsync ? 1 : 0)
{
    tableModel.reset (new TableModel (*this, listToEdit));

    TableHeaderComponent& header = table.getHeader();

    header.addColumn (TRANS("Name"),         TableModel::nameCol,         200, 100, 700, TableHeaderComponent::defaultFlags | TableHeaderComponent::sortedForwards);
    header.addColumn (TRANS("Format"),       TableModel::typeCol,         80, 80, 80,    TableHeaderComponent::notResizable);
    header.addColumn (TRANS("Category"),     TableModel::categoryCol,     100, 100, 200);
    header.addColumn (TRANS("Manufacturer"), TableModel::manufacturerCol, 200, 100, 300);
    header.addColumn (TRANS("Description"),  TableModel::descCol,         300, 100, 500, TableHeaderComponent::notSortable);

    table.setHeaderHeight (22);
    table.setRowHeight (20);
    table.setModel (tableModel.get());
    table.setMultipleSelectionEnabled (true);
    addAndMakeVisible (table);

    addAndMakeVisible (optionsButton);
    optionsButton.onClick = [this] { showOptionsMenu(); };
    optionsButton.setTriggeredOnMouseDown (true);

    setSize (400, 600);
    list.addChangeListener (this);
    updateList();
    table.getHeader().reSortTable();

    PluginDirectoryScanner::applyBlacklistingsFromDeadMansPedal (list, deadMansPedalFile);
    deadMansPedalFile.deleteFile();
}

PluginListComponent::~PluginListComponent()
{
    list.removeChangeListener (this);
}

void PluginListComponent::setOptionsButtonText (const String& newText)
{
    optionsButton.setButtonText (newText);
    resized();
}

void PluginListComponent::setScanDialogText (const String& title, const String& content)
{
    dialogTitle = title;
    dialogText = content;
}

void PluginListComponent::setNumberOfThreadsForScanning (int num)
{
    numThreads = num;
}

void PluginListComponent::resized()
{
    auto r = getLocalBounds().reduced (2);

    optionsButton.setBounds (r.removeFromBottom (24));
    optionsButton.changeWidthToFitText (24);

    r.removeFromBottom (3);
    table.setBounds (r);
}

void PluginListComponent::changeListenerCallback (ChangeBroadcaster*)
{
    table.getHeader().reSortTable();
    updateList();
}

void PluginListComponent::updateList()
{
    table.updateContent();
    table.repaint();
}

void PluginListComponent::removeSelectedPlugins()
{
    const SparseSet<int> selected (table.getSelectedRows());

    for (int i = table.getNumRows(); --i >= 0;)
        if (selected.contains (i))
            removePluginItem (i);
}

void PluginListComponent::setTableModel (TableListBoxModel* model)
{
    table.setModel (nullptr);
    tableModel.reset (model);
    table.setModel (tableModel.get());

    table.getHeader().reSortTable();
    table.updateContent();
    table.repaint();
}

bool PluginListComponent::canShowSelectedFolder() const
{
    return File::createFileWithoutCheckingPath (list.getTypes()[table.getSelectedRow()].fileOrIdentifier).exists();
}

void PluginListComponent::showSelectedFolder()
{
    if (canShowSelectedFolder())
        File (list.getTypes()[table.getSelectedRow()].fileOrIdentifier).getParentDirectory().startAsProcess();
}

void PluginListComponent::removeMissingPlugins()
{
    auto types = list.getTypes();

    for (int i = types.size(); --i >= 0;)
    {
        auto type = types.getUnchecked (i);

        if (! formatManager.doesPluginStillExist (type))
            list.removeType (type);
    }
}

void PluginListComponent::removePluginItem (int index)
{
    if (index < list.getNumTypes())
        list.removeType (list.getTypes()[index]);
    else
        list.removeFromBlacklist (list.getBlacklistedFiles() [index - list.getNumTypes()]);
}

void PluginListComponent::optionsMenuStaticCallback (int result, PluginListComponent* pluginList)
{
    if (pluginList != nullptr)
        pluginList->optionsMenuCallback (result);
}

void PluginListComponent::optionsMenuCallback (int result)
{
    switch (result)
    {
        case 0:   break;
        case 1:   list.clear(); break;
        case 2:   removeSelectedPlugins(); break;
        case 3:   showSelectedFolder(); break;
        case 4:   removeMissingPlugins(); break;

        default:
            if (AudioPluginFormat* format = formatManager.getFormat (result - 10))
                scanFor (*format);

            break;
    }
}

void PluginListComponent::showOptionsMenu()
{
    PopupMenu menu;
    menu.addItem (1, TRANS("Clear list"));
    menu.addItem (2, TRANS("Remove selected plug-in from list"), table.getNumSelectedRows() > 0);
    menu.addItem (3, TRANS("Show folder containing selected plug-in"), canShowSelectedFolder());
    menu.addItem (4, TRANS("Remove any plug-ins whose files no longer exist"));
    menu.addSeparator();

    for (int i = 0; i < formatManager.getNumFormats(); ++i)
    {
        auto* format = formatManager.getFormat (i);

        if (format->canScanForPlugins())
            menu.addItem (10 + i, "Scan for new or updated " + format->getName() + " plug-ins");
    }

    menu.showMenuAsync (PopupMenu::Options().withTargetComponent (&optionsButton),
                        ModalCallbackFunction::forComponent (optionsMenuStaticCallback, this));
}

bool PluginListComponent::isInterestedInFileDrag (const StringArray& /*files*/)
{
    return true;
}

void PluginListComponent::filesDropped (const StringArray& files, int, int)
{
    OwnedArray<PluginDescription> typesFound;
    list.scanAndAddDragAndDroppedFiles (formatManager, files, typesFound);
}

FileSearchPath PluginListComponent::getLastSearchPath (PropertiesFile& properties, AudioPluginFormat& format)
{
    auto key = "lastPluginScanPath_" + format.getName();

    if (properties.containsKey (key) && properties.getValue (key, {}).trim().isEmpty())
        properties.removeValue (key);

    return FileSearchPath (properties.getValue (key, format.getDefaultLocationsToSearch().toString()));
}

void PluginListComponent::setLastSearchPath (PropertiesFile& properties, AudioPluginFormat& format,
                                             const FileSearchPath& newPath)
{
    auto key = "lastPluginScanPath_" + format.getName();

    if (newPath.getNumPaths() == 0)
        properties.removeValue (key);
    else
        properties.setValue (key, newPath.toString());
}

//==============================================================================
class PluginListComponent::Scanner    : private Timer
{
public:
    Scanner (PluginListComponent& plc, AudioPluginFormat& format, const StringArray& filesOrIdentifiers,
             PropertiesFile* properties, bool allowPluginsWhichRequireAsynchronousInstantiation, int threads,
             const String& title, const String& text)
        : owner (plc), formatToScan (format), filesOrIdentifiersToScan (filesOrIdentifiers), propertiesToUse (properties),
          pathChooserWindow (TRANS("Select folders to scan..."), String(), AlertWindow::NoIcon),
          progressWindow (title, text, AlertWindow::NoIcon),
          progress (0.0), numThreads (threads), allowAsync (allowPluginsWhichRequireAsynchronousInstantiation),
          finished (false)
    {
        FileSearchPath path (formatToScan.getDefaultLocationsToSearch());

        // You need to use at least one thread when scanning plug-ins asynchronously
        jassert (! allowAsync || (numThreads > 0));

        // If the filesOrIdentifiersToScan argumnent isn't empty, we should only scan these
        // If the path is empty, then paths aren't used for this format.
        if (filesOrIdentifiersToScan.isEmpty() && path.getNumPaths() > 0)
        {
           #if ! JUCE_IOS
            if (propertiesToUse != nullptr)
                path = getLastSearchPath (*propertiesToUse, formatToScan);
           #endif

            pathList.setSize (500, 300);
            pathList.setPath (path);

            pathChooserWindow.addCustomComponent (&pathList);
            pathChooserWindow.addButton (TRANS("Scan"),   1, KeyPress (KeyPress::returnKey));
            pathChooserWindow.addButton (TRANS("Cancel"), 0, KeyPress (KeyPress::escapeKey));

            pathChooserWindow.enterModalState (true,
                                               ModalCallbackFunction::forComponent (startScanCallback,
                                                                                    &pathChooserWindow, this),
                                               false);
        }
        else
        {
            startScan();
        }
    }

    ~Scanner() override
    {
        if (pool != nullptr)
        {
            pool->removeAllJobs (true, 60000);
            pool.reset();
        }
    }

private:
    PluginListComponent& owner;
    AudioPluginFormat& formatToScan;
    StringArray filesOrIdentifiersToScan;
    PropertiesFile* propertiesToUse;
    std::unique_ptr<PluginDirectoryScanner> scanner;
    AlertWindow pathChooserWindow, progressWindow;
    FileSearchPathListComponent pathList;
    String pluginBeingScanned;
    double progress;
    int numThreads;
    bool allowAsync, finished, timerReentrancyCheck = false;
    std::unique_ptr<ThreadPool> pool;

    static void startScanCallback (int result, AlertWindow* alert, Scanner* scanner)
    {
        if (alert != nullptr && scanner != nullptr)
        {
            if (result != 0)
                scanner->warnUserAboutStupidPaths();
            else
                scanner->finishedScan();
        }
    }

    // Try to dissuade people from to scanning their entire C: drive, or other system folders.
    void warnUserAboutStupidPaths()
    {
        for (int i = 0; i < pathList.getPath().getNumPaths(); ++i)
        {
            const File f (pathList.getPath()[i]);

            if (isStupidPath (f))
            {
                AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
                                              TRANS("Plugin Scanning"),
                                              TRANS("If you choose to scan folders that contain non-plugin files, "
                                                    "then scanning may take a long time, and can cause crashes when "
                                                    "attempting to load unsuitable files.")
                                                + newLine
                                                + TRANS ("Are you sure you want to scan the folder \"XYZ\"?")
                                                   .replace ("XYZ", f.getFullPathName()),
                                              TRANS ("Scan"),
                                              String(),
                                              nullptr,
                                              ModalCallbackFunction::create (warnAboutStupidPathsCallback, this));
                return;
            }
        }

        startScan();
    }

    static bool isStupidPath (const File& f)
    {
        Array<File> roots;
        File::findFileSystemRoots (roots);

        if (roots.contains (f))
            return true;

        File::SpecialLocationType pathsThatWouldBeStupidToScan[]
            = { File::globalApplicationsDirectory,
                File::userHomeDirectory,
                File::userDocumentsDirectory,
                File::userDesktopDirectory,
                File::tempDirectory,
                File::userMusicDirectory,
                File::userMoviesDirectory,
                File::userPicturesDirectory };

        for (int i = 0; i < numElementsInArray (pathsThatWouldBeStupidToScan); ++i)
        {
            const File sillyFolder (File::getSpecialLocation (pathsThatWouldBeStupidToScan[i]));

            if (f == sillyFolder || sillyFolder.isAChildOf (f))
                return true;
        }

        return false;
    }

    static void warnAboutStupidPathsCallback (int result, Scanner* scanner)
    {
        if (result != 0)
            scanner->startScan();
        else
            scanner->finishedScan();
    }

    void startScan()
    {
        pathChooserWindow.setVisible (false);

        scanner.reset (new PluginDirectoryScanner (owner.list, formatToScan, pathList.getPath(),
                                                   true, owner.deadMansPedalFile, allowAsync));

        if (! filesOrIdentifiersToScan.isEmpty())
        {
            scanner->setFilesOrIdentifiersToScan (filesOrIdentifiersToScan);
        }
        else if (propertiesToUse != nullptr)
        {
            setLastSearchPath (*propertiesToUse, formatToScan, pathList.getPath());
            propertiesToUse->saveIfNeeded();
        }

        progressWindow.addButton (TRANS("Cancel"), 0, KeyPress (KeyPress::escapeKey));
        progressWindow.addProgressBarComponent (progress);
        progressWindow.enterModalState();

        if (numThreads > 0)
        {
            pool.reset (new ThreadPool (numThreads));

            for (int i = numThreads; --i >= 0;)
                pool->addJob (new ScanJob (*this), true);
        }

        startTimer (20);
    }

    void finishedScan()
    {
        owner.scanFinished (scanner != nullptr ? scanner->getFailedFiles()
                                               : StringArray());
    }

    void timerCallback() override
    {
        if (timerReentrancyCheck)
            return;

        if (pool == nullptr)
        {
            const ScopedValueSetter<bool> setter (timerReentrancyCheck, true);

            if (doNextScan())
                startTimer (20);
        }

        if (! progressWindow.isCurrentlyModal())
            finished = true;

        if (finished)
            finishedScan();
        else
            progressWindow.setMessage (TRANS("Testing") + ":\n\n" + pluginBeingScanned);
    }

    bool doNextScan()
    {
        if (scanner->scanNextFile (true, pluginBeingScanned))
        {
            progress = scanner->getProgress();
            return true;
        }

        finished = true;
        return false;
    }

    struct ScanJob  : public ThreadPoolJob
    {
        ScanJob (Scanner& s)  : ThreadPoolJob ("pluginscan"), scanner (s) {}

        JobStatus runJob()
        {
            while (scanner.doNextScan() && ! shouldExit())
            {}

            return jobHasFinished;
        }

        Scanner& scanner;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScanJob)
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Scanner)
};

void PluginListComponent::scanFor (AudioPluginFormat& format)
{
    scanFor (format, StringArray());
}

void PluginListComponent::scanFor (AudioPluginFormat& format, const StringArray& filesOrIdentifiersToScan)
{
    currentScanner.reset (new Scanner (*this, format, filesOrIdentifiersToScan, propertiesToUse, allowAsync, numThreads,
                                       dialogTitle.isNotEmpty() ? dialogTitle : TRANS("Scanning for plug-ins..."),
                                       dialogText.isNotEmpty()  ? dialogText  : TRANS("Searching for all possible plug-in files...")));
}

bool PluginListComponent::isScanning() const noexcept
{
    return currentScanner != nullptr;
}

void PluginListComponent::scanFinished (const StringArray& failedFiles)
{
    StringArray shortNames;

    for (auto& f : failedFiles)
        shortNames.add (File::createFileWithoutCheckingPath (f).getFileName());

    currentScanner.reset(); // mustn't delete this before using the failed files array

    if (shortNames.size() > 0)
        AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          TRANS("Scan complete"),
                                          TRANS("Note that the following files appeared to be plugin files, but failed to load correctly")
                                            + ":\n\n"
                                            + shortNames.joinIntoString (", "));
}

} // namespace juce
