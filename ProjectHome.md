FlamingTux is a xfire client for linux

Woohoo we finally come to the release of Flamingtux 0.2, with a shedload of new features for people to play with ;)

And you need to have these libs:
Gtk+
gtkmm
glademm
gtkspell

We also now have a standard build system for the GNU lovers and for ease of use.


---

**TO BUILD**:

First compile our modified Xfirelib (Download on right):

1- Extract the .tar.gz (tar -xf filename.tar.gz)

2- ./build.sh

Next, compile flamingtux:

1 - extract the .tar.gz (tar -xf filename.tar.gz)

2 - inside the top-level directory of flamingtux execute:

a - ./autogen.sh

b - ./configure

c - make

d - make install (as root or sudo)

**END BUILD**

---


Below are the changes we made in 0.2:

proper preferences and configuration to enable/disable stuff

optional styles

optional logging of conversations

optional spell checking

optional sounds

proper compiling system (./configure blah blah)

all bugs from 0.1 fixed (if any reported)

option for closing to systray

remove buddy option

Enjoy it and report any bugs!

Again, remember, flamingtux is still far from finished. We are still on version 0.2, not 1.0.

Credits to: **peacey**, **Jeed**, **aMSN** for the default sounds, **Xfire** for creating the protocol
and the **Xfirelib** developers for creating the backend of our client.


---

**Screenshots**

![http://i38.tinypic.com/w8bbpu.png](http://i38.tinypic.com/w8bbpu.png)

`Fullsize: http://i36.tinypic.com/28byxea.png`

![http://i38.tinypic.com/149380k.png](http://i38.tinypic.com/149380k.png)

`Fullsize: http://i35.tinypic.com/w9jgwk.png`

![http://i38.tinypic.com/av2zgn.png](http://i38.tinypic.com/av2zgn.png)

`Fullsize: http://i36.tinypic.com/2i0et53.png`