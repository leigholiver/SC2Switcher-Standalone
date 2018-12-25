## SC2Switcher Webhook Standalone

## [Download](https://github.com/leigholiver/SC2Switcher-Standalone/releases/latest/)

## Installation: 
Double click webhook.exe

### Game Webhook 
- When entering or leaving a game, the plugin will send a request to the specified url with information about the game for use in other applications
- For an example of how this could be used you could check out [Ladderbet](https://github.com/leigholiver/ladderbet/), an automated twitch chat betting bot

```
event: 'enter' or 'exit',
displayTime: ~,
players: [
	{
		'name': ~,
		'type': ~,
		'race': 'Terr', 'Zerg' or 'Prot',
		'result': 'Victory' or 'Defeat',
		'isme': 'true' or 'false',
	},
]
scores: {
	'Terr', 'Zerg' or 'Prot': {
		"Victory": ~,
		"Defeat": ~
	},
}
```
- Known issue: Scores may not update until the start of the next game if you change the score manually or come across one of the situations mentioned in the Score Tracker section


### Score Tracker
- The `scores.txt` file will be updated with your session scores (mostly) automatically
- If you play against a random player the plugin will ask you for their race
- If you are neither player or both players (ie barcodes), the plugin will ask which player you were
- There is a small chance that these notifications will take focus over sc2. If this is an issue you can untick 'Popups Enabled' and these will be ignored. You can use the buttons to update the score manually 


## If you use a separate PC to stream: 
Enter the IP address of your SC2 computer in the SC2 PC IP box.
On your SC2 PC, open the Battle.net launcher, click Options, Game Settings, and under SC2, check 'Additional Command Line Arguments', and enter `-clientapi 6119` into the text box. 

You can check that SC2 is configured correctly by going to `http://[Your SC2 PC IP]:6119/ui` in your browser on the streaming PC. It should look something like:
`{"activeScreens":["ScreenBackgroundSC2/ScreenBackgroundSC2","ScreenReplay/ScreenReplay","ScreenNavigationSC2/ScreenNavigationSC2","ScreenForegroundSC2/ScreenForegroundSC2","ScreenBattlenet/ScreenBattlenet"]}`. 


## Build
In cmake set the following variables:
Qt5_DIR [qt directory]/msvc2013_64/lib/cmake/Qt5
OBS_CMAKE_MODULES_PATH [obs-studio source direcrory]\cmake\Modules