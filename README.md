# oeShowTool

This is an openFrameworks Tool. 
Please visit http://openframeworks.cc to download it. In case you don't know it: it is very worth checking it out!
 

The tool will be used for driving shows based on video projection. 
This is an early alpha version, but already features different screens with mapping/warping and a bezier based mask editor.
The screens are connected to input slots (color, image, video), scenes are used to store all settings.

The Tool is based on the following excellent external plugins:
- ofxBezierWarp (https://github.com/gameoverhack/ofxBezierWarp) for mapping/warping
- ofxImGui (https://github.com/jvcleave/ofxImGui) which again is a wrapper for ImGui (https://github.com/ocornut/imgui) fr the GUI
- ofxBezierEditor (https://github.com/acarreras/ofxBezierEditor) for the mask functionality
Thanks a lot to the authors for the incredible work!


# TODO:

a lot, most important 
- descriptions/help
- error catching
- check multi monitor problems on ubuntu
- add windows version (with gstreamer)

# Screenshots
(so that you can see where it goes)

[![screenshot multiple_screens](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen1.png)](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen1.png)
multiple screens (that usuallybe on secon screen/projector probably) and GUI

[![screenshot warping_a_screen](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen2.png)](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen2.png)
warping a screen

[![screenshot building_a_mask](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen3.png)](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen3.png)
building a mask
