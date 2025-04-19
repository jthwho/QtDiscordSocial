# Qt bindings for discord social SDK

The discord social SDK can't be distributed with this, so you'll first need to follow the directions
in step 3 of the [Discord Social SDK Guide](https://discord.com/developers/docs/discord-social-sdk/getting-started/using-c++) 
to get the C++ SDK and unpack it in the code root directory.  Once you unzip the zip file in this folder
you should end up with a folder called `discord_social_sdk`.  The CMake build system of this project
will assume that's where it can find the SDK.

From there, assuming you've got Qt6, you should be able to build using the typical CMake instructions:

1. Create a folder in the code root called build
2. Change directory into build
3. Run cmake to build a build system for your platform (e.g. `cmake ..`)
4. Run the resulting build system (e.g. `make`)
5. Bob is now your uncle.


