# osu!relax

Emulates the behaviour of the Relax gameplay modifier externally.

Modified by [FilipeSilvens](https://steamcommunity.com/id/FilipeSilvens) to add a more "human" feel to the cheat.

For more details, read the [tutorial](https://aixxe.net/2016/10/osu-game-hacking) here.

## Usage
* Ensure the signature defined in `relax.h` is up-to-date for the active release channel.
    * If you are using _Stable (Fallback)_ be sure to uncomment the correct signature in `relax.h`.
* Make sure you have created a `config.ini` where your executable is with the [following parameters](https://pastebin.com/raw/sFyeeST9)
* Configure the config.ini to the settings of your preference.
    * If you don't use the default <kbd>Z</kbd> and <kbd>X</kbd> for clicking, you can find the hex value for your keys [here.](https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx)
	* Change the window name to something not suspicious.
	* Modify all of the other settings to your likings. Documentation can be found on the pastebin link I have specified above.
* Drag .osu beatmap file onto `relax.exe` while on the song selection screen.
* Start playing the song and if the `ready.wav` is heard, you are ready to go.
    * Note, if you do not hear the `ready.wav`, please restart both `osu!` and the cheat because it may have found the wrong time address. While this is rare, it can sometimes happen.
	
You can close osu!relax at any time by pressing the <kbd>Home</kbd> key.

## Demonstration

[![Video](http://img.youtube.com/vi/EaeepI1-qGU/0.jpg)](https://www.youtube.com/watch?v=EaeepI1-qGU)

## Known issues

If you select the cheat to be the default program to open .osu files, the `config.ini` file doesn't seem to work. Hardcoding a directory for the `config.ini` will fix it. You can do this by going in `configs.h` and changing the `.\\config.ini` to something like `C:\\relax\\config.ini`.