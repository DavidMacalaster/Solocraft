Description
===========

These are my extensions to the [TrinityCore][4] WoW Server Emulator for WoW
version 3.3.5a that are targetted toward soloing (or with a very small party)
the group content in WoW like dungeons and raids at the level the content was
meant for instead of having to come back and solo when you are 20 levels higher
than the content.

The goal is to automatically apply stat buffs, HP regeneration, procs like
dispelling target regeneration buffs, and other things to the player based on
the instance the player has entered and the size of the party they are in to
make up the non-deal party makeup.

[4]: <https://trinitycore.atlassian.net/wiki/>

Installation
------------

While following the [TrinityCore installation guide][1] after you have used git
to grab the TrinityCore source but before you [run cmake][2] you need to add
this git submodule:

`git submodule add https://github.com/DavidMacalaster/Solocraft.git
src/server/scripts/Custom/Solocraft`

then add Solocraft to [src/server/scripts/Custom/custom_script_loader.cpp][3]:

[3]: <https://github.com/TrinityCore/TrinityCore/blob/3.3.5/src/server/scripts/Custom/custom_script_loader.cpp>

```c++
// This is where scripts' loading functions should be declared:
void AddSC_solocraft();

// The name of this function should match: void Add${NameOfDirectory}Scripts()
void AddCustomScripts()
{
	AddSC_solocraft();
}
```

[1]: <https://trinitycore.atlassian.net/wiki/display/tc/Core+Installation>

[2]: <https://trinitycore.atlassian.net/wiki/display/tc/Linux+Core+Installation#LinuxCoreInstallation-Configuringforcompiling>
