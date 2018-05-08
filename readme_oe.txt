
WINDOWS
-------

1) Windows - install gstreamer.

    You get the installers here (https://gstreamer.freedesktop.org/data/pkg/windows/). You will need the base installer (like gstreamer-1.0-x86-1.6.4.msi) and the devel installer (like gstreamer-1.0-devel-x86-1.6.4.msi). For 64bit you need the appropriate ones (x86_64).
    I used Version 1.6.4. as 1.7.1 crashed on my system when decoding h264. But (installer!) it is easy to install and uninstall in case you want to try another version.
    please install the complete package, otherwise it might not work later on with openFrameworks Apps.
    On my system the installer wanted to use the second disk for the installation path. To prevent confusion I suggest to change this: In the installer click “custom” and validate the path. If neccessary change it to default (C:\gstreamer). You can then go back and click “complete”.

You then have installed gstreamer on your system. The installer took care of the path variable. You can try if it works by opening a command prompt and typing sth. like “gst-play-1.0 YOURMOVIE.mp4”.


LINUX
-----

System menus Linux should use gtk2 for openCv, so in case you get errors:

sudo apt-get remove libgtk-3-dev
sudo apt-get install libgtk2.0-dev
