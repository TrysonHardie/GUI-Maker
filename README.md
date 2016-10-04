# GUI-Maker

It is a free graphical app, that helps to create GUI in some programming languages (SuperCollider, AutoIt) faster. This code-generation tool written in C++ with the Qt5 framework.
By using GUI maker it's realy easy to create a graphical user interfaces: just drag elements, set parameters and, guess what, you're done.

[Homepage](https://www.trysohard.info/GUI-Maker/)<br>

## Mission:
***
Help people to save their time for making GUIs.


## Detailed Description:
***

How much time it's gets to type this fragment of code:

```SuperCollider
var w;
w = Window.new("gui", Rect(100, 100, 360, 360)).front;

Button(w, Rect(94,28, 128,36))
;
Knob(w, Rect(26,107, 36,36))
;
NumberBox(w, Rect(231,129, 64,32))
;
Slider(w, Rect(131,177, 128,32))
;
PopUpMenu(w, Rect(50,229, 128,32))
;
```

With GUI Maker - just 6 sec. Drag  Button,Knob,NumberBox,Slider,PopUpMenu, and press CopyCode.
![GUI_Maker Screenshot](src/data/screenshots/GUI_Maker_screenshot.png?raw=true)

## Features:
***

* Supported languages:
	-SuperCollider
	-AutoIt
* Color picker
* Font picker
* Array editor (every item it is a row of table)
* Easy window resizing and elements placing
* Code preview with highlight, one-click copying, save as text file function
* Quantized grid


## Disadvanteges
***
* Elements represented by Pixmaps, so you can't see some changes: color, font etc.
* No Undo/Redo
* No binary
* This is not a TextEditor


## License:
***
Copyright (c) 2016 by [Tryson Hardie](https://www.trysohard.info/)<br>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


