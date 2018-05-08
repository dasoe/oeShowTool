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


# Windows

The windows branch is not stable yet (even less stable then the master branch, which is Linux). 

You can use gstreamer instead of internal windows video player, which is advised (asyncronous loading, more efficient). But additional Installation is needed:

    - You get the installers here (https://gstreamer.freedesktop.org/data/pkg/windows/). You will need the base installer (like gstreamer-1.0-x86-1.6.4.msi) and the devel installer (like gstreamer-1.0-devel-x86-1.6.4.msi). For 64bit you need the appropriate ones (x86_64).
    - I used Version 1.6.4. as 1.7.1 crashed on my system when decoding h264. But (installer!) it is easy to install and uninstall in case you want to try another version.
    please install the complete package, otherwise it might not work later on with openFrameworks Apps.
    - On my system the installer wanted to use the second disk for the installation path. To prevent confusion I suggest to change this: In the installer click “custom” and validate the path. If neccessary change it to default (C:\gstreamer). You can then go back and click “complete”.

You then have installed gstreamer on your system. The installer took care of the path variable. You can try if it works by opening a command prompt and typing sth. like “gst-play-1.0 YOURMOVIE.mp4”.

You switch on/off the use of gstreamer in bin/data/settings.xml


# TODO:

a lot, most important 
- error catching, crash prevention
- descriptions/help
- check multi monitor problems on ubuntu


# Screenshots
(so that you can see where it goes)

[![screenshot multiple_screens](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen1.png)](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen1.png)
multiple screens (that usuallybe on secon screen/projector probably) and GUI


[![screenshot warping_a_screen](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen2.png)](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen2.png)
warping a screen


[![screenshot building_a_mask](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen3.png)](https://raw.githubusercontent.com/dasoe/oeShowTool/master/bin/data/screenShots/oeShowTool_screen3.png)
building a mask with changes seen live on screen
