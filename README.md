# LakeShooter-C

![lakeshooter](https://user-images.githubusercontent.com/7093685/139555222-f8c17ef2-c46c-4bea-9aed-d16167cc233c.png)

My goal was to use this to get proficient coding in C and lower level programming. It's heavily inspired by "River Raid", an Atari 2600 game I liked.

Controls buttons are Z and X
You can play at https://jimmycartrette.github.io/LakeShooter-C/

So far I've used bitwise operations and pointer arithmetic to check collision by inspecting the framebuffer drawings of land.
The game generation system is based on a linear feedback shift register, which generates pseduorandom numbers. Because it's always the same random number progression, it can be used to generate island width, edge width, and random fuel and enemy generation, for 50,000 levels, consistently for all players and provide consistent playback when restarting a level.

River raid disassembly here
https://github.com/johnidm/asm-atari-2600/blob/master/RiverRaid.asm

TODO:
Level restart should look the same, but currently it does not the first time.
Add hornets
Add houses/decorations on land
Glide up to bridge like original game
Alter speeds and start possibility of movement a bit earlier for enemies
